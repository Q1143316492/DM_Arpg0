// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WndBase.h"

void UWndBase::InitWidge(UWidgetBase *Widget)
{
	this->m_UserWidget = Widget;
}

UWidgetBase* UWndBase::GetWidget()
{
	return this->m_UserWidget;
}

void UWndBase::InitLogic()
{

}