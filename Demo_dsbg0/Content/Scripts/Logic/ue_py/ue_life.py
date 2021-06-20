# -*- coding: utf-8 -*-
# @author cwl 2021-06-19


import unreal_engine
import ue_py.ue_tick as ue_tick


def GameInstanceShutdown():
    unreal_engine.UELog(f"[UEPy] GameInstanceShutdown")
    ue_tick.ClearAllTick()
