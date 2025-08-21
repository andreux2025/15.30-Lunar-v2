#pragma once
#include "../../framework.h"

namespace Player
{
	void ServerLoadingScreenDroppedHook(AFortPlayerControllerAthena* PlayerController);
	inline void (*ServerLoadingScreenDropped)(const AFortPlayerControllerAthena*);
	void ServerAcknowledgePossessionHook(AFortPlayerControllerAthena* PlayerController, APawn* P);
	void ServerAttemptAircraftJumpHook(const UFortControllerComponent_Aircraft* ControllerComponent, const FRotator& ClientRotation);
	void ServerPlayEmoteItemHook(AFortPlayerController* PlayerController, UFortItemDefinition* EmoteAsset, float RandomEmoteNumber);
	void ServerExecuteInventoryItem(AFortPlayerController* PC, FGuid Guid);
	void GetPlayerViewPoint(APlayerController* PlayerController, FVector& outLocation, FRotator& outRotation);
	inline void (*GetPlayerViewPointOG)(APlayerController* PlayerController, FVector outLocation, FRotator outRotation);
	inline __int64 (*OnDamageServerOG)(ABuildingSMActor* Actor, float Damage, FGameplayTagContainer DamageTags, FVector Momentum, FHitResult HitInfo, AFortPlayerControllerAthena* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle EffectContext);
	__int64 OnDamageServer(ABuildingSMActor* Actor, float Damage, FGameplayTagContainer DamageTags, FVector Momentum, FHitResult HitInfo, AFortPlayerControllerAthena* InstigatedBy, AActor* DamageCauser, FGameplayEffectContextHandle EffectContext);
	inline void (*ServerCreateBuildingActorOG)(AFortPlayerControllerAthena* PlayerController, FCreateBuildingActorData& CreateBuildingData);
	void ServerCreateBuildingActor(AFortPlayerControllerAthena* PlayerController, FCreateBuildingActorData& CreateBuildingData);
	inline __int64 (*CantBuild)(UObject*, UObject*, FVector, FRotator, char, TArray<ABuildingSMActor*>*, char*);
	void ServerAttemptInteract(UFortControllerComponent_Interaction* ControllerComp, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, ETInteractionType InteractType, UObject* OptionalObjectData, EInteractionBeingAttempted InteractionBeingAttempted, int32 RequestId);
	inline void (*ServerAttemptInteractOG)(UFortControllerComponent_Interaction* ControllerComp, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, ETInteractionType InteractType, UObject* OptionalObjectData, EInteractionBeingAttempted InteractionBeingAttempted, int32 RequestId);
	void ServerEndEditingBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* BuildingActorToEdit);
	void ServerEditBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, uint8 RotationIterations, bool bMirrored);
	void ServerBeginEditingBuildingActor(AFortPlayerControllerAthena* PC, ABuildingSMActor* BuildingActorToEdit);
	void ServerHandlePickup(AFortPlayerPawnAthena* Pawn, AFortPickup* Pickup, float InFlyTime, const FVector& InStartDirection, bool bPlayPickupSound);
	inline void (*ServerHandlePickupOG)(AFortPlayerPawn* Pawn, AFortPickup* Pickup, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound);
	void ServerAttemptInventoryDrop(AFortPlayerControllerAthena* PC, FGuid ItemGuid, int Count, bool bTrash);
	void OnPawnAISpawnedHook(AActor* Controller, AFortPlayerPawnAthena* Pawn);
	inline void (*OnPawnAISpawnedHookOG)(AActor* Controller, AFortPlayerPawnAthena* Pawn);
	void OnPossesedPawnDiedHook(AFortAthenaAIBotController* Controller, AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser, FVector& HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector& Momentum);
	inline void (*OnPossesedPawnDiedOG)(AFortAthenaAIBotController* Controller, AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser, FVector& HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector& Momentum);
	inline void (*ClientOnPawnDiedOG)(AFortPlayerControllerZone* PlayerControllerZone, const FFortPlayerDeathReport& DeathReport);
	inline void ClientOnPawnDied(AFortPlayerControllerZone* PlayerControllerZone, const FFortPlayerDeathReport& DeathReport);
	inline void(*OrginalServerSetInAircraft)(AFortPlayerStateAthena* PlayerState, bool bNewInAircraft);
	void ServerSetInAircraft(AFortPlayerStateAthena* PlayerState, bool bNewInAircraft);
	 void ServerSendZiplineState(AFortPlayerPawn* Pawn, FZiplinePawnState InZiplineState);
	void PlayerHooks();
}