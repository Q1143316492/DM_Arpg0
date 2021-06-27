# -*- coding: utf-8 -*-
# @author cwl 2021-06-13


import builtins
import logger
import hotfix


class GacApp(object):

    def __init__(self):
        builtins.theApp = self
        self.logger = None
        self.m_ReloadMgr = None
        self.m_bInit = False

    def Init(self):
        self.logger = logger.Logger()
        self.m_ReloadMgr = hotfix.HotfixMgr()
        self.m_bInit = True
        self.logger.info("GacInitMain Init() finish")

    def Update(self):
        if not self.m_bInit:
            return
        self.m_ReloadMgr.Update()

    def Unit(self):
        self.m_ReloadMgr.Unit()
        self.m_bInit = False


def GacInitMain():
    app = GacApp()
    app.Init()

    from gac.game_wnd.game_wnd_mgr import g_GameWndMain
    g_GameWndMain.ShowUpWnd("wnd_test", True)
