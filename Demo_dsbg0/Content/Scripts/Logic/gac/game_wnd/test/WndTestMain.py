# -*- coding: utf-8 -*-
# @author cwl 2021-06-21


import gac.game_wnd.wnd_base as wnd_base


class WndTestMain(wnd_base.WndBase):

    def __init__(self, szWndName):
        super(WndTestMain, self).__init__(szWndName)

        self.m_btnTest = self.GetChild('btnTest')
        theApp.logger.error(f"debug {self.m_btnTest} {type(self.m_btnTest)} {self.m_btnTest.GetName()}")

if __name__ == '__main__':
    print(issubclass(WndTestMain, wnd_base.WndBase))