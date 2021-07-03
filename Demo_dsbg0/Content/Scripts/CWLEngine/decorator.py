# -*- coding: utf-8 -*-
# @author cwl 2021-06-19


import functools


def singleton(cls):
    _dictInstance = {}

    @functools.wraps(cls)
    def _GetInstance(*args, **kwargs):
        if cls not in _dictInstance:
            _dictInstance[cls] = cls(*args, **kwargs)
        return _dictInstance[cls]

    return _GetInstance


def override(func):
    return func