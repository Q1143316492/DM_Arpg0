#include "UEPyTicker.h"

UEPyTicker* UEPyTicker::GetInstance()
{
	static UEPyTicker* Instance = nullptr;
	if (Instance == nullptr)
	{
		Instance = NewObject<UEPyTicker>();
	}
	return Instance;
}

int UEPyTicker::NextTick = 0;

