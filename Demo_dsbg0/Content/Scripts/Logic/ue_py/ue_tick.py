# -*- coding: utf-8 -*-
# @author cwl 2021-06-19


import unreal_engine

g_dictTickMgr = {}
g_dictTickID2TickMgrID = {}


def InitTickMgr(TickMgr):
    nTickMgrID = TickMgr.GetID()
    assert nTickMgrID not in g_dictTickMgr, "逻辑错误 重复注册 TickMgr"
    g_dictTickMgr[TickMgr.GetID()] = TickMgr


def UnitTickMgr(TickMgr):
    nTickMgrID = TickMgr.GetID()
    assert nTickMgrID not in g_dictTickMgr, "逻辑错误 TickMgr 已经删除"
    del g_dictTickMgr[nTickMgrID]


def HandleTick(nTickID):
    nTickMgrID = g_dictTickID2TickMgrID.get(nTickID)
    TickMgr = g_dictTickMgr.get(nTickMgrID)
    if TickMgr:
        TickMgr.HandleTick(nTickID)


def RegisterTick(TickMgrID, szDelayTimeMs):
    nTickID = unreal_engine.RegisterTick(szDelayTimeMs)
    g_dictTickID2TickMgrID[nTickID] = TickMgrID
    return nTickID


def RegisterOnceTick(TickMgrID, szDelayTimeMs):
    nTickID = unreal_engine.RegisterOnceTick(szDelayTimeMs)
    g_dictTickID2TickMgrID[nTickID] = TickMgrID
    return nTickID


def UnRegisterTick(nTickID):
    return unreal_engine.UnRegisterTick(nTickID)


def ClearAllTick():
    global g_dictTickMgr
    global g_dictTickID2TickMgrID
    for nTickID, _ in g_dictTickID2TickMgrID.items():
        UnRegisterTick(nTickID)
    g_dictTickMgr = {}
    g_dictTickID2TickMgrID = {}
