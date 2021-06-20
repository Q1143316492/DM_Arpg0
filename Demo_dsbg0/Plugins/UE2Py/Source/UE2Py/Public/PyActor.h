// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "UE2Py.h"
#include "PyUtil.h"
#include "UEPyMgr.h"
#include "UEPyTicker.h"
#include "MainBPLibTools.h"

#include "PyActor.generated.h"

UCLASS()
class UE2PY_API APyActor : public AActor
{
	GENERATED_BODY()
	
public:
	APyActor();
	~APyActor();

	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = "Python", BlueprintReadWrite, meta = (ExposeOnSpawn = true))
		FString PythonModule;

	UPROPERTY(EditAnywhere, Category = "Python", BlueprintReadWrite, meta = (ExposeOnSpawn = true))
		FString PythonClass;
private:
	bool bEnable = false;
	PyObject *PyActorInstance;
	UEPy_PyUObject* PyUObject;
};
