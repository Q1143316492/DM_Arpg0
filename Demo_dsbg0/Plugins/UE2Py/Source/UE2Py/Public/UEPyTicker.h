#pragma once

#include "Containers/Ticker.h"
#include "UE2Py.h"
#include "MainBPLibTools.h"
#include "UEPyTicker.generated.h"


UCLASS()
class UEPyTicker : public UObject
{
	GENERATED_BODY()
public:
	static UEPyTicker* GetInstance();

	int32 AddTick(float DelayTimeMS)
	{
		++NextTick;

		FDelegateHandle TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([](float InDelay) {
			FScopePythonGIL gil;
			UMainBPLibTools::CppCallPythonFunction("ue_py.ue_tick", "HandleTick", "i", NextTick);
			return true;
		}), DelayTimeMS / 1000.0f);

		this->m_TickContainer.Emplace(NextTick, TickDelegateHandle);
		return NextTick;
	}

	int32 AddOnceTick(float DelayTimeMS)
	{
		++NextTick;

		FDelegateHandle TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([](float InDelay) {
			FScopePythonGIL gil;
			UMainBPLibTools::CppCallPythonFunction("ue_py.ue_tick", "HandleTick", "i", NextTick);
			return false;
		}), DelayTimeMS / 1000.0f);

		// this->m_TickContainer.Emplace(NextTick, TickDelegateHandle);
		return NextTick;
	}

	bool RemoveTick(int32 TickID)
	{
		FDelegateHandle* TickDelegateHandle = this->m_TickContainer.Find(NextTick);
		if (TickDelegateHandle)
		{
			FTicker::GetCoreTicker().RemoveTicker(*TickDelegateHandle);
			return true;
		}
		return false;
	}

	void Clear()
	{
		NextTick = 0;

		for (auto& Elem : m_TickContainer)
		{
			FTicker::GetCoreTicker().RemoveTicker(Elem.Value);
		}

		m_TickContainer.Empty();
	}
private:
	static int NextTick;
	TMap<int32, FDelegateHandle> m_TickContainer;
};