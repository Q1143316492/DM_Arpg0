# -*- coding: utf-8 -*-
# @author cwl 2021-06-19


import id_mgr
import ue_py.ue_tick as ue_tick


class TickMgr(object):
    """
    !!! 请记得注销 def UnitTickMgr
    """

    def __init__(self):
        self.m_nID = id_mgr.IdManagerImplUuid().GenID()
        self.m_dictTickID2TickName = {}
        self.m_dictTickName2TickID = {}
        self.m_dictTickName2TickCallback = {}
        ue_tick.InitTickMgr(self)

    def UnitTickMgr(self):
        ue_tick.UnitTickMgr(self)

    def GetID(self):
        return self.m_nID

    # ===================== 对外用户接口 begin ==================================

    def RegisterOnceTick(self, szTickName, nDelayTimeMs, funTickCallback, *args):
        if szTickName in self.m_dictTickName2TickCallback:
            theApp.logger.error(f"逻辑错误 重复注册Tick {szTickName}")
            return
        nTickID = ue_tick.RegisterOnceTick(self.GetID(), nDelayTimeMs)

        def _TickCallback():
            funTickCallback(*args)

        self.m_dictTickID2TickName[nTickID] = szTickName
        self.m_dictTickName2TickID[szTickName] = nTickID
        self.m_dictTickName2TickCallback[szTickName] = _TickCallback

    def RegisterTick(self, szTickName, nDelayTimeMs, funTickCallback, *args):
        if szTickName in self.m_dictTickName2TickCallback:
            theApp.logger.error(f"逻辑错误 重复注册Tick {szTickName}")
            return
        nTickID = ue_tick.RegisterTick(self.GetID(), nDelayTimeMs)

        def _TickCallback():
            funTickCallback(*args)

        self.m_dictTickID2TickName[nTickID] = szTickName
        self.m_dictTickName2TickID[szTickName] = nTickID
        self.m_dictTickName2TickCallback[szTickName] = _TickCallback

    def UnRegisterTick(self, szTickName):
        nTickID = self.m_dictTickName2TickID.get(szTickName)
        if nTickID is None:
            theApp.logger.info(f"注销Tick [{szTickName}] is None")
            return

        ue_tick.UnRegisterTick(nTickID)
        del self.m_dictTickID2TickName[nTickID]
        del self.m_dictTickName2TickID[szTickName]
        del self.m_dictTickName2TickCallback[szTickName]

    def HasTick(self, szTickName):
        return szTickName in self.m_dictTickName2TickID

    # ===================== 对外用户接口 end ==================================

    def HandleTick(self, nTickID):
        szTickName = self.m_dictTickID2TickName.get(nTickID)
        if not szTickName:
            theApp.logger.error(f"Handle Tick. TickID {nTickID} None")
            return
        funCallback = self.m_dictTickName2TickCallback.get(szTickName)
        if not funCallback:
            theApp.logger.error(f"Tick Callback None. szTickName {szTickName}")
            return
        try:
            funCallback()
        except Exception as e:
            theApp.logger.error("[tick error] {}".format(e))
