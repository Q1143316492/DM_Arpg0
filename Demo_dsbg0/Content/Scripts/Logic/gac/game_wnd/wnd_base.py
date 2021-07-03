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

    def __init__(self, szWndName):
        self.m_szWndName = szWndName
        self.m_szResName = None
        self.m_eVisibility = EWidgetVisibility.eNone
        self.m_bIsMainWnd = True
        self.m_PyWndWrapper = unreal_engine.PyWndWrapper(szWndName)

    def WndMgrInitWnd(self, szResName):
        self.m_szResName = szResName

    def ShowUpWnd(self, bShow):
        if bShow:
            self.m_PyWndWrapper.ShowUpWnd(True)
        else:
            self.m_PyWndWrapper.ShowUpWnd(False)

    # def SetVisible(self, eWidgetVisibility):
    #     theApp.logger.error(f"debug set wnd {eWidgetVisibility}")
    #     self.m_eVisibility = eWidgetVisibility
    #     unreal_engine.SetWndVisibility(self.m_szWndName, eWidgetVisibility)

    def GetChild(self, szChildName):
        return self.m_PyWndWrapper.GetChild(szChildName)

    def GetChildWnd(self, szChildWndName):
        pass


class SubWnd(WndBase):

    def __init__(self, Owner):
        super(SubWnd, self).__init__()
        self.m_OwnerWnd = Owner
        self.m_bIsMainWnd = False
