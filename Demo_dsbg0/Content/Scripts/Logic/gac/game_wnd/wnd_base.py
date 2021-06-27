# -*- coding: utf-8 -*-
# @author cwl 2021-06-21


import unreal_engine


class EWidgetVisibility:
    eNone = 0
    eVisible = 1
    eCollapsed = 2
    eHidden = 3
    eNotHitTestableSelfAndAllChildren = 4
    eNotHitTestableSelfOnly = 5


class WndBase(object):

    def __init__(self):
        self.m_nWndID = None
        self.m_szWndName = None
        self.m_szResName = None
        self.m_eVisibility = EWidgetVisibility.eNone

    def WndMgrInitWnd(self, nWndID, szWndName, szResName):
        self.m_nWndID = nWndID
        self.m_szWndName = szWndName
        self.m_szResName = szResName

    def SetVisible(self, eWidgetVisibility):
        theApp.logger.error(f"debug set wnd {eWidgetVisibility}")
        self.m_eVisibility = eWidgetVisibility
        unreal_engine.SetWndVisibility(self.m_szWndName, eWidgetVisibility)
