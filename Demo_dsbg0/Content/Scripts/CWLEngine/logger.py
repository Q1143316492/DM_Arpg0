# -*- coding: utf-8 -*-
# @author cwl 2021-06-13


import unreal_engine as ue

class Logger(object):

    def __init__(self):
        pass

    @staticmethod
    def info(szInfo):
        ue.UELog(szInfo)

    @staticmethod
    def error(szInfo):
        ue.UELogError(szInfo)