# -*- coding: utf-8 -*-
# @author cwl 2021-06-20


def TickDoc():
    import tick_mgr
    import unreal_engine
    m_TickMgr = tick_mgr.TickMgr()

    def _TestTick(szName):
        unreal_engine.UELog(f"hello world {szName}")

    m_TickMgr.RegisterTick("TEST_TICK_TAG", 1000, _TestTick, "cwl")
    m_TickMgr.RegisterOnceTick("TEST_TICK_ONCE_TAG", 1000, _TestTick, "cwl")

    m_TickMgr.UnRegisterTick("TEST_TICK_TAG")