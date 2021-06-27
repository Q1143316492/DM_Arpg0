# -*- coding: utf-8 -*-
# @author cwl 2021-06-13


import os
import sys
import unreal_engine as ue


g_listReloadPath = []


def InitPath():
    global g_listReloadPath

    szCurPath = os.path.dirname(os.path.abspath(__file__))
    g_listReloadPath = [
        os.path.join(szCurPath, "CWLEngine"),
        os.path.join(szCurPath, "Logic"),
    ]
    for szPath in g_listReloadPath:
        sys.path.append(szPath)


def ReImport(szSearchRoot, szRootDir, szFile):
    if not szFile.endswith(".py"):
        return
    szAbsolutePath = os.path.join(szRootDir, szFile)
    szModule = szAbsolutePath[len(szSearchRoot) + 1: -len(".py")].replace("/", ".").replace("\\", ".")
    if szModule in sys.modules:
        sys.modules.pop(szModule)
        __import__(szModule)
    else:
        __import__(szModule)


def InitModule():
    if not ue.CheckWithEditor():
        return
    for szSearchRoot in g_listReloadPath:
        for szRootDir, listDir, listFile in os.walk(szSearchRoot):
            for szFile in listFile:
                ReImport(szSearchRoot, szRootDir, szFile)


def TestFunc():
    import unreal_engine
    unreal_engine.UELog("TestFunc")


class PyActor(object):

    def __init__(self):
        ue.UELog("PyActor init success")

    def BeginPlay(self):
        ue.UELog("[UEPy] PyActor Begin Play")
        InitPath()
        InitModule()
        import gac_main
        gac_main.GacInitMain()

    def Tick(self, fDealTime):
        # ue.UELog(f"UE Tick {fDealTime} {type(fDealTime)}")
        pass
