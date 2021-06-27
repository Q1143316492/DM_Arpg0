# -*- coding: utf-8 -*-
# @author cwl 2021-06-21


import gac.game_wnd.wnd_base as wnd_base


class WndTestMain(wnd_base.WndBase):

    def __init__(self):
        super(WndTestMain, self).__init__()


if __name__ == '__main__':
    print(issubclass(WndTestMain, wnd_base.WndBase))