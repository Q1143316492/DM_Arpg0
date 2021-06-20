# -*- coding: utf-8 -*-
# @author cwl 2021-06-03


import uuid


class IdManagerBase(object):

    def GenID(self):
        raise NotImplemented

    def GenStrID(self):
        raise NotImplemented

    def StrID(self, Uuid):
        raise NotImplemented


class IdManagerImplUuid(IdManagerBase):

    def GenID(self):
        return uuid.uuid1()

    def StrID(self, Uuid):
        return "".join(str(Uuid).split("-"))

    def GenStrID(self):
        return self.StrID(self.GenID())


class IdManagerImplIndex(IdManagerBase):

    def __init__(self):
        self.m_nIndex = 0

    def GenID(self):
        self.m_nIndex += 1
        return self.m_nIndex

    def GenStrID(self):
        return self.StrID(self.GenID())

    def StrID(self, nIndex):
        return str(nIndex)


class IdManagerImplRange(IdManagerBase):

    def __init__(self, nRange):
        self.m_nRange = nRange
        self.m_nIndex = 0

    def GenID(self):
        if self.m_nIndex > self.m_nRange:
            self.m_nIndex = 0
        self.m_nIndex += 1
        return self.m_nIndex

    def GenStrID(self):
        return self.StrID(self.GenID())

    def StrID(self, nIndex):
        return str(nIndex)
