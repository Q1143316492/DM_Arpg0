#include "UMG/PyWnd/WndPyBase.h"

void UWndPyBase::InitLogic()
{
	UE_LOG(LogTemp, Warning, TEXT("[WND] PyWndCreate"));
}

void UWndPyBase::SetWndVisibility(int32 eSlateVisibility)
{
	switch (eSlateVisibility)
	{
		case 1:
			this->m_UserWidget->SetVisibility(ESlateVisibility::Visible);
		case 2:
			this->m_UserWidget->SetVisibility(ESlateVisibility::Collapsed);
		case 3:
			this->m_UserWidget->SetVisibility(ESlateVisibility::Hidden);
		case 4:
			this->m_UserWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		case 5:
			this->m_UserWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UWndPyBase::SetWndVisibility(ESlateVisibility InVisibility)
{
	this->m_UserWidget->SetVisibility(InVisibility);
}
