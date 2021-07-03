# -*- coding: utf-8 -*-
# @author cwl 2021-06-13


def UELog(*args, **kwargs):
    pass


def UELogError(*args, **kwargs):
    pass


def CheckWithEditor(*args, **kwargs) -> bool:
    pass


def RegisterTick(*args, **kwargs) -> int:
    pass


def RegisterOnceTick(*args, **kwargs) -> int:
    pass


def UnRegisterTick(*args, **kwargs) -> bool:
    pass


def UETestExtendMethod(*args, **kwargs):
    pass


# UI

def AddWndClassInfo(*args, **kwargs) -> None:
    pass


class PyWndWrapper(object):

    def __init__(self, *args, **kwargs):
        pass

    def GetChild(*args, **kwargs):
        pass

    def ShowUpWnd(self, *args, **kwargs):
        pass
