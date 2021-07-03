# -*- coding: utf-8 -*-
# @author cwl 2021-06-20


import importlib
import unreal_engine
from decorator import singleton
from gac_gas.enum_def import EWndLayer
from gac.game_wnd.wnd_base import EWidgetVisibility
import gac.game_wnd.wnd_base as wnd_base
import id_mgr

MAX_WND_LIMIT = 10000


def GameWndMgrInitClassInfo(WndMgr):
    WndMgr.RegisterWndClass("wnd_test", "gac.game_wnd.test.WndTestMain", "WndTestMain", EWndLayer.eNormal, 0,
                            "/Game/Res/UMG/Test/TestMainWnd.TestMainWnd_C")


@singleton
class GameWndMgr(object):

    def __init__(self):
        self.m_dictWndClassInfo = {}
        self.m_dictCreatedWnd = {}
        self.m_IdMgr = id_mgr.IdManagerImplRange(MAX_WND_LIMIT)
        self.InitWndClassInfo()

    def RegisterWndClass(self, szWndName, szModuleName, szWndClass, eWndLayer, nZOrder, szBlueprint):
        self.m_dictWndClassInfo[szWndName] = (szModuleName, szWndClass, eWndLayer, nZOrder, szBlueprint)

    def InitWndClassInfo(self):
        GameWndMgrInitClassInfo(self)
        self._RegisterWndInfo2UE()

    def ShowUpWnd(self, szWndName, bShow=True):
        if bShow:
            if szWndName not in self.m_dictCreatedWnd:
                self._CreateNewWnd(szWndName)
            else:
                theApp.logger.error(f"wnd {szWndName} has been create")

    def _CreateNewWnd(self, szWndName):
        if szWndName not in self.m_dictWndClassInfo:
            theApp.logger.error(f"wnd class info not register {szWndName}")
            return
        szModuleName, szWndClass, eWndLayer, nZOrder, szBlueprint = self.m_dictWndClassInfo[szWndName]
        WndModule = importlib.import_module(szModuleName)
        if not WndModule:
            theApp.logger.error(f"wnd config {szWndName} module error {szModuleName}")
            return

        WndClass = getattr(WndModule, szWndClass)
        if not WndClass:
            theApp.logger.error(f"wnd config {szWndName} wnd class error {szModuleName} {szWndClass}")
            return

        Wnd = WndClass(szWndName)
        Wnd.WndMgrInitWnd(szBlueprint)
        Wnd.ShowUpWnd(True)
        self.m_dictCreatedWnd[szWndName] = Wnd


    def _RegisterWndInfo2UE(self):
        for szWndName, tupleWndClassInfo in self.m_dictWndClassInfo.items():
            _, _, eWndLayer, nZOrder, szBlueprint = tupleWndClassInfo
            unreal_engine.AddWndClassInfo(szWndName, eWndLayer.value, nZOrder, szBlueprint)


g_GameWndMain = GameWndMgr()
