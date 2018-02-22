// Fill out your copyright notice in the Description page of Project Settings.

#include "PC_ReplaySpectator.h"
//#include "TP_StrategyWithSteam.h"
#include "Engine/DemoNetDriver.h"


APC_ReplaySpectator::APC_ReplaySpectator(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	PrimaryActorTick.bTickEvenWhenPaused = true;
	bShouldPerformFullTickWhenPaused = true;
}

bool APC_ReplaySpectator::SetCurrentReplayPausedState(bool bDoPause)
{
	AWorldSettings* WorldSettings = GetWorldSettings();

	// Set MotionBlur off and Anti Aliasing to FXAA in order to bypass the pause-bug of both
	static const auto CVarAA = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DefaultFeature.AntiAliasing"));

	static const auto CVarMB = IConsoleManager::Get().FindConsoleVariable(TEXT("r.DefaultFeature.MotionBlur"));

	if (bDoPause)
	{
		PreviousAASetting = CVarAA->GetInt();
		PreviousMBSetting = CVarMB->GetInt();

		// Set MotionBlur to OFF, Anti-Aliasing to FXAA
		CVarAA->Set(1);
		CVarMB->Set(0);

		WorldSettings->Pauser = PlayerState;
		return true;
	}
	// Rest MotionBlur and AA
	CVarAA->Set(PreviousAASetting);
	CVarMB->Set(PreviousMBSetting);

	WorldSettings->Pauser = NULL;
	return false;
}

int32 APC_ReplaySpectator::GetCurrentReplayTotalTimeInSeconds() const
{
	if (GetWorld())
	{
		if (GetWorld()->DemoNetDriver)
		{
			return GetWorld()->DemoNetDriver->DemoTotalTime;
		}
	}

	return 0.f;
}

int32 APC_ReplaySpectator::GetCurrentReplayCurrentTimeInSeconds() const
{
	if (GetWorld())
	{
		if (GetWorld()->DemoNetDriver)
		{
			return GetWorld()->DemoNetDriver->DemoCurrentTime;
		}
	}

	return 0.f;
}

void APC_ReplaySpectator::SetCurrentReplayTimeToSeconds(int32 Seconds)
{
	if (GetWorld())
	{
		if (GetWorld()->DemoNetDriver)
		{
			GetWorld()->DemoNetDriver->GotoTimeInSeconds(Seconds);
		}
	}
}


void APC_ReplaySpectator::SetCurrentReplayPlayRate(float PlayRate)
{
	if (GetWorld())
	{
		if (GetWorld()->DemoNetDriver)
		{
			GetWorld()->GetWorldSettings()->DemoPlayTimeDilation = PlayRate;
		}
	}
}