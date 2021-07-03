// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WndBase.h"


void UWndBase::InitWidge(UWidgetBase *Widget, FString WndName)
{
	this->m_UserWidget = Widget;
	this->m_WndName = WndName;
}

UWidgetBase* UWndBase::GetWidget()
{
	return this->m_UserWidget;
}

void UWndBase::InitLogic()
{

}
