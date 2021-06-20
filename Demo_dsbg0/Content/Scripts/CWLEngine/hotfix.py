# -*- coding: utf-8 -*-
# @author cwl 2021-04-03

import os
import sys
import importlib
import types
import enum
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

PY_SUFFIX = ".py"

MAX_RELOAD_CLOSURE_DEPTH = 2
MAX_RELOAD_CLASS_DEPTH = 2

g_listReloadPath = []

def InitPath():
    global g_listReloadPath

    szEnginePath = os.path.dirname(os.path.abspath(__file__))
    szScriptsPath = os.path.dirname(szEnginePath)
    g_listReloadPath = [
        os.path.join(szScriptsPath, "CWLEngine"),
        os.path.join(szScriptsPath, "Logic"),
    ]


class Tmp:

    @staticmethod
    def StaticMethod():
        pass

    @classmethod
    def ClassMethod(cls):
        pass


class EReloadAttrType(enum.IntEnum):
    eFunctionType = 1
    eMethodType = 2
    eStaticMethodType = 3
    eClassMethodType = 4
    eClassType = 5


def GetAttrType(AttrObj):
    """
    一个python 模块module下包括
    1, 全局变量 reload的过程中会处理
    2, function 手动替换 { func_code, func_defaults, func_doc }
    3, class {
        method 本质上是绑定了成员对象的function
        class method
        static method
        成员变量
        基类关系（这个没救了）
    }
    """
    dictAttrType = {
        types.FunctionType: EReloadAttrType.eFunctionType,
        types.MethodType: EReloadAttrType.eMethodType,
        type(Tmp.__dict__.get("StaticMethod")): EReloadAttrType.eStaticMethodType,
        type(Tmp.__dict__.get("ClassMethod")): EReloadAttrType.eClassMethodType,
        type: EReloadAttrType.eClassType,
    }
    return dictAttrType.get(type(AttrObj))


def UpdateModule(szModuleName):
    if szModuleName not in sys.modules:
        __import__(szModuleName)
        return

    OldModule = sys.modules.get(szModuleName)
    dictOldAttr = {}

    # [1] 备份旧模块
    for szAttrName, AttrObj in OldModule.__dict__.items():
        dictOldAttr[szAttrName] = AttrObj

    # [2] reload 异常旧还原回去，当什么都没发送
    try:
        NewModule = importlib.reload(OldModule)
    except Exception as e:
        # 报异常了就把模块还原回去
        print("Exception:", e)
        for szAttrName, AttrObj in dictOldAttr.items():
            OldModule.__dict__[szAttrName] = AttrObj
        return

    # [3] 把reload后的module的内容备份，并把旧的写回来
    dictNewAttr = {}
    for szAttrName, AttrObj in NewModule.__dict__.items():
        if szAttrName not in dictOldAttr:
            # 这个属性是新的，reload流程会处理好。不变的module地址下多出了新的属性，不用管
            continue

        if id(AttrObj) == id(dictOldAttr.get(szAttrName)):
            # import 进来的东西id才会一样比如 szAttrName会是比如 os, sys, math, __name__, __doc__, __builtins__ 这些也不用管
            continue

        dictNewAttr[szAttrName] = AttrObj
        OldObj = dictOldAttr.get(szAttrName)
        NewModule.__dict__[szAttrName] = OldObj

        eAttrType = GetAttrType(AttrObj)
        if eAttrType is None:
            continue

        if eAttrType == EReloadAttrType.eFunctionType:
            UpdateFunction(OldObj, AttrObj)
        elif eAttrType == EReloadAttrType.eClassType:
            UpdateClass(OldObj, AttrObj)

    if hasattr(NewModule, "__onreload__"):
        NewModule.__onreload__(dictNewAttr)


def UpdateFunction(funOld, funNew, nClosureDepth=MAX_RELOAD_CLOSURE_DEPTH):
    """
    把funNew的内容写道funOld里面
    """
    nOldClosureDepth = 0
    if funOld.__closure__:
        nOldClosureDepth = len(funOld.__closure__)
    nNewClosureDepth = 0
    if funNew.__closure__:
        nNewClosureDepth = len(funNew.__closure__)
    if nNewClosureDepth != nOldClosureDepth:
        return False
    setattr(funOld, "__code__", funNew.__code__)
    setattr(funOld, "__defaults__", funNew.__defaults__)
    setattr(funOld, "__doc__", funNew.__doc__)
    setattr(funOld, "__dict__", funNew.__dict__)

    if nClosureDepth == 0 or nOldClosureDepth == 0:
        return True
    for nIndex, CellObj in enumerate(funOld.__closure__):
        if GetAttrType(CellObj.cell_contents) == EReloadAttrType.eFunctionType:
            UpdateFunction(CellObj.cell_contents, funNew.__closure__[nIndex].cell_contents)
    return True


def UpdateClass(ClsOld, ClsNew, nMaxDepth=MAX_RELOAD_CLASS_DEPTH):
    """
    把新的ClsNew 内容写道ClsOld里面
    """
    if nMaxDepth == 0:
        return

    setAllowAddAndDelAttrType = {
        EReloadAttrType.eFunctionType,
        EReloadAttrType.eMethodType,
        EReloadAttrType.eClassMethodType,
        EReloadAttrType.eStaticMethodType,
    }
    for szAttrName, AttrObj in ClsOld.__dict__.items():
        if szAttrName in ClsNew.__dict__:
            continue
        if GetAttrType(AttrObj) not in setAllowAddAndDelAttrType:
            continue
        # 新的类中删除了类型
        delattr(ClsOld, szAttrName)

    for szAttrName, AttrObj in ClsNew.__dict__.items():
        # 新的类新增了类型
        if szAttrName not in ClsOld.__dict__:
            setattr(ClsOld, szAttrName, AttrObj)
            continue

        OldAttrObj = ClsOld.__dict__.get(szAttrName)
        eAttrType = GetAttrType(AttrObj)
        if eAttrType == EReloadAttrType.eFunctionType or eAttrType == EReloadAttrType.eMethodType:
            # 这里AttrObj是类，而不是实例，所以理论上应该只会有function而没有method
            UpdateFunction(OldAttrObj, AttrObj)
        elif eAttrType == EReloadAttrType.eStaticMethodType:
            UpdateFunction(OldAttrObj.__get__(object), AttrObj.__get__(object))
        elif eAttrType == EReloadAttrType.eClassMethodType:
            UpdateFunction(OldAttrObj.__get__(object).__func__, AttrObj.__get__(object).__func__)
        elif eAttrType == EReloadAttrType.eClassType:
            UpdateClass(OldAttrObj, AttrObj, nMaxDepth - 1)

    if hasattr(ClsOld, "__onreload__"):
        ClsOld.__onreload__(ClsNew.__dict__)


class HotfixMgr(FileSystemEventHandler):

    def __init__(self):
        self.m_hotfixObserverObj = Observer()
        self.m_setModulePath = set()

        InitPath()
        self.LoadHotfixPath()
        self.m_hotfixObserverObj.start()
        self.m_bStart = True

    def LoadHotfixPath(self):
        global g_listReloadPath
        for szPath in g_listReloadPath:
            self.m_hotfixObserverObj.schedule(self, szPath, recursive=True)

    def Update(self):
        if not self.m_bStart:
            return

        if not self.m_setModulePath:
            return

        for szModulePath in self.m_setModulePath:
            global g_listReloadPath
            for szSourcePath in g_listReloadPath:
                if szModulePath.startswith(szSourcePath):
                    szModuleName = szModulePath[len(szSourcePath) + 1: -len(PY_SUFFIX)]  # del pre and .py
                    szModuleName = szModuleName.replace("\\", ".").replace("/", ".").replace("\\\\", ".")
                    UpdateModule(szModuleName)
                    theApp.logger.info(f"[reload] {szModuleName}.py")

        self.m_setModulePath = set()

    def Unit(self):
        self.m_hotfixObserverObj.join()

    def on_modified(self, event):
        if event.is_directory or not event.src_path.endswith(PY_SUFFIX):
            return
        self.m_setModulePath.add(event.src_path)


if __name__ == '__main__':
    ReloadMgr = HotfixMgr()
    while True:
        import time

        time.sleep(1)
        ReloadMgr.Update()
