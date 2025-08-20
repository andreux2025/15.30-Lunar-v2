#pragma once
#include "../../framework.h"



namespace GameMode
{
	void HandleNewSafeZonePhaseHook(AFortGameModeAthena* GameMode, int32 ZoneIndex);
	inline void (*HandleNewSafeZonePhase)(AFortGameModeAthena*, int32);

	bool ReadyToStartMatchHook(AFortGameModeAthena* GameMode);
	APawn* SpawnDefaultPawnForHook(AGameMode* GameMode, AController* NewPlayer, AActor* StartSpot);


	void GameModeHooks();
}