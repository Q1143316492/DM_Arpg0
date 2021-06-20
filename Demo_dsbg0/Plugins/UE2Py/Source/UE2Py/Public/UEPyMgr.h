#pragma once

#include "Runtime/CoreUObject/Public/UObject/GCObject.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "UE2Py.h"


class FUEPyMgr : public FGCObject
{
public:
	static FUEPyMgr* GetInstance();

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

	UEPy_PyUObject *GetPyUObject(UObject *Object);
	void RegisterPyUObject(UEPy_PyUObject *PyUObject);
	void UnRegisterPyUObject(UEPy_PyUObject *PyUObject);

	int32 RunGC();

	void CheckPyObjectGC();
private:
	void RunGCDelegate();
	uint32 PyUObjectsGC();
private:
	struct FUEPyUObjectTracker 
	{
		FWeakObjectPtr Owner;
		UEPy_PyUObject *PyUObject;
		bool bPyOwned;

		FUEPyUObjectTracker(UObject *Obj, UEPy_PyUObject *TargetPyUObject)
		{
			Owner = FWeakObjectPtr(Obj);
			PyUObject = TargetPyUObject;
			bPyOwned = false;
		}
	};

	TArray<UObject *> m_PythonTrackedObjects;
	TMap<UObject *, FUEPyUObjectTracker> m_UObject2PyTracker;
};