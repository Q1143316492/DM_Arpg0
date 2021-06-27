#include "UEPyMgr.h"

FUEPyMgr* FUEPyMgr::GetInstance()
{
	static FUEPyMgr *Singleton;
	if (!Singleton)
	{
		Singleton = new FUEPyMgr();
		FCoreUObjectDelegates::GetPostGarbageCollect().AddRaw(Singleton, &FUEPyMgr::RunGCDelegate);
	}
	return Singleton;
}

void FUEPyMgr::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObjects(this->m_PythonTrackedObjects);
}

UEPy_PyUObject* FUEPyMgr::GetPyUObject(UObject *Object)
{
	FUEPyUObjectTracker *Tracker = this->m_UObject2PyTracker.Find(Object);
	if (!Tracker)
	{
		return nullptr;
	}

	if (!Tracker->Owner.IsValid(true))
	{
		if (!Tracker->bPyOwned)
		{
			Py_DECREF((PyObject *)Tracker->PyUObject);
		}
		this->UnRegisterPyUObject(Tracker->PyUObject);
		return nullptr;
	}
	
	return Tracker->PyUObject;
}

void FUEPyMgr::RegisterPyUObject(UEPy_PyUObject *PyUObject)
{
	UObject *Object = PyUObject->ue_UObject;
	this->m_UObject2PyTracker.Add(Object, FUEPyUObjectTracker(Object, PyUObject));
}

void FUEPyMgr::UnRegisterPyUObject(UEPy_PyUObject *PyUObject)
{
	FUEPyUObjectTracker *Tracker = this->m_UObject2PyTracker.Find(PyUObject->ue_UObject);
	if (Tracker)
	{
		if (!Tracker->bPyOwned)
		{
			Py_CLEAR(Tracker->PyUObject->py_PyDict);
			Py_CLEAR(Tracker->PyUObject->py_PyObject);
			Py_DECREF(Tracker->PyUObject);
		}
		this->m_UObject2PyTracker.Remove(PyUObject->ue_UObject);
	}
}

int32 FUEPyMgr::RunGC()
{
	int32 Garbaged = PyUObjectsGC();
	return Garbaged;
}

void FUEPyMgr::CheckPyObjectGC()
{
	UE_LOG(LogPython, Warning, TEXT("CheckPyObjectGC ... %d"), this->m_UObject2PyTracker.Num());

}

void FUEPyMgr::RunGCDelegate()
{
	FScopePythonGIL gil;
	RunGC();
}

uint32 FUEPyMgr::PyUObjectsGC()
{
	uint32 Garbaged = 0;
	TArray<UObject *> BrokenList;

	for (auto &UObjectPyItem : this->m_UObject2PyTracker)
	{
		UObject *Object = UObjectPyItem.Key;
		FUEPyUObjectTracker &Tracker = UObjectPyItem.Value;

#if defined(UEPY_MEMORY_DEBUG)
		UE_LOG(LogPython, Warning, TEXT("Checking for UObject at %p"), Object);
#endif

		if (!Tracker.Owner.IsValid(true))
		{

#if defined(UEPY_MEMORY_DEBUG)
			UE_LOG(LogPython, Warning, TEXT("Removing UObject at %p (refcnt: %d)"), Object, Tracker.PyUObject->ob_base.ob_refcnt);
#endif

			BrokenList.Add(Object);
			Garbaged++;
		}
		else
		{
#if defined(UEPY_MEMORY_DEBUG)
			UE_LOG(LogPython, Error, TEXT("UObject at %p %s is in use"), Object, *Object->GetName());
#endif
		}
	}

	for (UObject *Object : BrokenList)
	{
		FUEPyUObjectTracker &Tracker = m_UObject2PyTracker[Object];
		if (!Tracker.bPyOwned)
		{
			Py_DECREF((PyObject *)Tracker.PyUObject);
		}
		this->UnRegisterPyUObject(Tracker.PyUObject);
	}

	return Garbaged;
}
