# DM_Arpg0



[TOC]

# 1.0 概述



本项目最终目标当然是做一个开放世界Arpg MMO。但是，钱没有，策划未定，美术暂无，框架和逻辑持续开发中。如有雷同，那我也没办法。

基于UE4.26 (过段时间UE5稳定后再考虑切换)。客户端使用C++ 和 Python。已有虚幻的Python插件，UnrealEnginePython作者弃坑，只能运行到UE4.23。官方Python插件在自己扩展上不够灵活。还不能偷公司代码。所以决定手搓一个。放在Plugin/UE2Py目录下。

本项目作为个人学习模板，按自己的认知不断造轮子做游戏ing。



# 2.0 框架说明



## 2.1 基本使用



插件自带蓝图PyActor。Content/Script 自动加入Python模块搜索路径 sys.path

```cpp
	UPROPERTY(EditAnywhere, Category = "Python", BlueprintReadWrite, meta = (ExposeOnSpawn = true))
		FString PythonModule;

	UPROPERTY(EditAnywhere, Category = "Python", BlueprintReadWrite, meta = (ExposeOnSpawn = true))
		FString PythonClass;
```

在PyActor实例设置模块路径和类名 例如Content/Script下main.py。main中有一个类PyActor。填写模块 `main` 和 `PyActor`。蓝图自动绑定Python脚本。并可扩展实现对应函数。

```python
import unreal_engine as ue

class PyActor(object):

    def __init__(self):
        ue.UELog("PyActor init success")

    def BeginPlay(self):
        ue.UELog("[UEPy] PyActor Begin Play")

    def Tick(self, fDealTime):
        # ue.UELog(f"UE Tick {fDealTime} {type(fDealTime)}")
        pass
```

目前已经实现Actor生命周期函数

- PreInitComponents
- PostInitComponents
- BeginPlay
- EndPlay
- Tick

ps: 个人习惯python编码规范不是pep8

实现unreal_engine模块。用于python层与虚幻引擎交互。API 见 doc.py



## 2.1 常用功能介绍



### 2.1.0 Log模块



unreal_engine 模块

```python
    import unreal_engine
    unreal_engine.UELog("log info")
    unreal_engine.UELogError("log error")
```

到C++ 层分别对应虚幻的 UE_LOG

```c++
UE_LOG(LogPython, Log, TEXT("%s"), UTF8_TO_TCHAR(Msg));
UE_LOG(LogPython, Error, TEXT("%s"), UTF8_TO_TCHAR(Msg));
```

不够更推荐的Log方式是。theApp通过嵌入 builtins。不需要import就能拿到。对应GacApp实例，代表客户端。目前只Log到虚幻输出，后续添加存盘

```python
    theApp.logger.info("log info")
    theApp.logger.error("log error")
```



### 2.1.1 定时器



```python
def TickDoc():
    import tick_mgr
    import unreal_engine
    m_TickMgr = tick_mgr.TickMgr()

    def _TestTick(szName):
        unreal_engine.UELog(f"hello world {szName}")

    m_TickMgr.RegisterTick("TEST_TICK_TAG", 1000, _TestTick, "cwl")
    m_TickMgr.RegisterOnceTick("TEST_TICK_ONCE_TAG", 1000, _TestTick, "cwl")

    m_TickMgr.UnRegisterTick("TEST_TICK_TAG")
```



```python
def RegisterOnceTick(self, szTickName, nDelayTimeMs, funTickCallback, *args):
    pass

def RegisterTick(self, szTickName, nDelayTimeMs, funTickCallback, *args):
    pass

def UnRegisterTick(self, szTickName):
    pass
```

每一个tick_mgr 可以通过自己的字符串定义tick。第二个参数单位毫秒，后面是回调函数和参数。到c++层详情见UEPyTicker 对应虚幻的FTicker接口

```c++
FTicker::GetCoreTicker().AddTicker()
```



### 2.1.2 Cpp 和 Python 接口添加



通过 CPython连接Python和C。需要Python扩展和C-API 相关知识。接口定义在 UEPyModule上。

Cpp到Python的调用简单封装了一下

```c++
UMainBPLibTools::CppCallPythonFunction("ue_py.ue_tick", "HandleTick", "i", NextTick);
```

分别是Module Function 参数格式 和参数。参数格式需要参考Python C-API

```python
	template<typename... Args>
	static void CppCallPythonFunction(const char* ModuleName, const char* FunctionName, const char* ParseFmt, Args... PyParams)
	{
		FScopePythonGIL gil;
		PyObject* PyModule = PyImport_ImportModule(ModuleName);
		if (!PyModule)
		{
			UEPyLogPyError();
			return;
		}
		PyObject* PyModuleDict = PyModule_GetDict(PyModule);
		if (!PyModuleDict)
		{
			UEPyLogPyError();
			return;
		}

		PyObject* Func = PyDict_GetItemString(PyModuleDict, FunctionName);
		if (Func && PyCallable_Check(Func))
		{
			PyObject* FuncRet = PyObject_CallFunction(Func, ParseFmt, PyParams...);
			Py_XDECREF(FuncRet);
		}
		Py_DECREF(PyModule);
	}
```



