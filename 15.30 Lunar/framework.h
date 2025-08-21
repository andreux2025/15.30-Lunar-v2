#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include "Server/SDK/SDK.hpp"
#include "Server/Public/httpclient.h"
#include "Server/Public/uuid.h"
using namespace SDK;
#include "MinHook.h"
//#include "StringConv.h"
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <locale>
#include <codecvt>  // deprecated en C++17, pero funcional
inline inline UFortAthenaAISpawnerDataComponentList* GlobalList = nullptr;
inline std::vector<UAthenaCharacterItemDefinition*> CharacterItemDefs;
inline std::vector<UAthenaBackpackItemDefinition*> BackpackItemDefs;
inline std::vector<UAthenaDanceItemDefinition*> EmoteItemDefs;
inline TArray<APlayerController*> GivenLootPlayers;
namespace Globals
{
	inline FName SpectatingName;
	inline std::string uuid;
	inline int IPlayerNumber = 0;
	inline bool Creative = false;
	inline bool bLateGame = true;
	inline bool Arena = true;
	inline bool bEnableScoringSystem = false;
	inline bool bArensal = false;
}
inline bool operator==(const FGuid& A, const FGuid& B)
{
	return A.A == B.A && A.B == B.B && A.C == B.C && A.D == B.D;
}
static void Hook_Internal(uint64_t Address, PVOID Hook, void** OG = nullptr)
{
	MH_CreateHook(PVOID(Address), Hook, OG);
	MH_EnableHook(PVOID(Address));
}
#define AndreuHook(...) Hook_Internal(__VA_ARGS__);

static void HookVFT(void** Vft, uintptr_t Idx, void* Function, void** Original)
{
	if (Original)
		*Original = Vft[Idx];

	DWORD Protection;
	if (VirtualProtect(&Vft[Idx], sizeof(void*), PAGE_EXECUTE_READWRITE, &Protection))
	{
		Vft[Idx] = Function;
		VirtualProtect(&Vft[Idx], sizeof(void*), Protection, &Protection);
	}
}

template <typename T>
static T* StaticFindObject(std::string ObjectName)
{
	auto Name = std::wstring(ObjectName.begin(), ObjectName.end()).c_str();

	static UObject* (*StaticFindObjectOriginal)(UClass * Class, UObject * Package, const TCHAR * OrigInName, bool ExactClass) = decltype(StaticFindObjectOriginal)(__int64(GetModuleHandleW(0)) + 0x38bfc10);

	return (T*)StaticFindObjectOriginal(T::StaticClass(), nullptr, Name, false);
}

static int ReturnTrue()
{
	return true;
}

static void ReturnHook()
{
	return;
}

static void CollectGravity()
{
	return;
}


inline void (*McpDispatchRequestOG)(__int64, __int64*, int);
inline void McpDispatchRequestHook(__int64 a1, __int64* a2, int a3)
{
	return McpDispatchRequestOG(a1, a2, 3);
}

template <typename T>
static T* Cast(UObject* Object) {
	if (Object && Object->IsA(T::StaticClass()))
	{
		return (T*)Object;
	}

	return nullptr;
}

inline UObject* (*StaticLoadObject_)(UClass* Class, UObject* InOuter, const TCHAR* Name, const TCHAR* Filename, uint32_t LoadFlags, UObject* Sandbox, bool bAllowObjectReconciliation) = decltype(StaticLoadObject_)(InSDKUtils::GetImageBase() + 0x38c1a30);
template <typename T>
inline T* StaticLoadObject(std::string Path, UClass* InClass = T::StaticClass(), UObject* Outer = nullptr)
{
	return (T*)StaticLoadObject_(InClass, Outer, std::wstring(Path.begin(), Path.end()).c_str(), nullptr, 0, nullptr, false);
}


inline void SwapVFTs(void* Base, uintptr_t Index, void* Detour, void** Original)
{
	auto VTable = (*(void***)Base);
	if (!VTable)
		return;

	if (!VTable[Index])
		return;

	if (Original)
		*Original = VTable[Index];

	DWORD dwOld;
	VirtualProtect(&VTable[Index], 8, PAGE_EXECUTE_READWRITE, &dwOld);
	VTable[Index] = Detour;
	DWORD dwTemp;
	VirtualProtect(&VTable[Index], 8, dwOld, &dwTemp);
}

inline void MiscChecks()
{
	AndreuHook(InSDKUtils::GetImageBase() + 0x52ec180, ReturnTrue);//GetNetMode
	AndreuHook(InSDKUtils::GetImageBase() + 0x4e87860, ReturnTrue);//KickPlayer
	AndreuHook(InSDKUtils::GetImageBase() + 0xa1c160, ReturnTrue);//CanActivateAbility
	AndreuHook(InSDKUtils::GetImageBase() + 0x105d5b0, McpDispatchRequestHook, (void**)&McpDispatchRequestOG);
	AndreuHook(InSDKUtils::GetImageBase() + 0x3747690, CollectGravity);

}

namespace MatchState
{
	static FName InProgress = UKismetStringLibrary::Conv_StringToName(L"InProgress");
	static FName WaitingPostMatch = UKismetStringLibrary::Conv_StringToName(L"WaitingPostMatch");
}

inline FQuat RotatorToQuat(FRotator Rotation)
{
	FQuat Quat;
	const float DEG_TO_RAD = 3.14159f / 180.0f;
	const float DIVIDE_BY_2 = DEG_TO_RAD / 2.0f;

	float SP = sin(Rotation.Pitch * DIVIDE_BY_2);
	float CP = cos(Rotation.Pitch * DIVIDE_BY_2);
	float SY = sin(Rotation.Yaw * DIVIDE_BY_2);
	float CY = cos(Rotation.Yaw * DIVIDE_BY_2);
	float SR = sin(Rotation.Roll * DIVIDE_BY_2);
	float CR = cos(Rotation.Roll * DIVIDE_BY_2);

	Quat.X = CR * SP * SY - SR * CP * CY;
	Quat.Y = -CR * SP * CY - SR * CP * SY;
	Quat.Z = CR * CP * SY - SR * SP * CY;
	Quat.W = CR * CP * CY + SR * SP * SY;

	return Quat;
}

template<typename T>
inline T* SpawnActor(FVector Loc, FRotator Rot = FRotator(), AActor* Owner = nullptr, SDK::UClass* Class = T::StaticClass(), ESpawnActorCollisionHandlingMethod Handle = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
{
	FTransform Transform{};
	Transform.Scale3D = FVector{ 1,1,1 };
	Transform.Translation = Loc;
	Transform.Rotation = RotatorToQuat(Rot);

	return (T*)UGameplayStatics::FinishSpawningActor(UGameplayStatics::BeginDeferredActorSpawnFromClass(UWorld::GetWorld(), Class, Transform, Handle, Owner), Transform);
}

template<typename T>
inline T* Actors(UClass* Class = T::StaticClass(), FVector Loc = {}, FRotator Rot = {}, AActor* Owner = nullptr)
{
	return SpawnActor<T>(Loc, Rot, Owner, Class);
}
inline std::map<AFortPickup*, float> PickupLifetimes;
inline AFortPickupAthena* SpawnStack(APlayerPawn_Athena_C* Pawn, UFortItemDefinition* Def, int Count, bool giveammo = false, int ammo = 0)
{
	auto Statics = (UGameplayStatics*)UGameplayStatics::StaticClass()->DefaultObject;

	FVector Loc = Pawn->K2_GetActorLocation();
	AFortPickupAthena* Pickup = Actors<AFortPickupAthena>(AFortPickupAthena::StaticClass(), Loc);
	Pickup->bReplicates = true;
	PickupLifetimes[Pickup] = Statics->GetTimeSeconds(UWorld::GetWorld());
	Pickup->PawnWhoDroppedPickup = Pawn;
	Pickup->PrimaryPickupItemEntry.Count = Count;
	Pickup->PrimaryPickupItemEntry.ItemDefinition = Def;
	if (giveammo)
	{
		Pickup->PrimaryPickupItemEntry.LoadedAmmo = ammo;
	}
	Pickup->PrimaryPickupItemEntry.ReplicationKey++;

	Pickup->OnRep_PrimaryPickupItemEntry();
	Pickup->TossPickup(Loc, Pawn, 6, true, true, EFortPickupSourceTypeFlag::Other, EFortPickupSpawnSource::Unset);

	Pickup->MovementComponent = (UProjectileMovementComponent*)Statics->SpawnObject(UProjectileMovementComponent::StaticClass(), Pickup);
	Pickup->MovementComponent->bReplicates = true;
	((UProjectileMovementComponent*)Pickup->MovementComponent)->SetComponentTickEnabled(true);

	return Pickup;
}

inline void sinCos(float* ScalarSin, float* ScalarCos, float Value)
{
	float quotient = (0.31830988618f * 0.5f) * Value;
	if (Value >= 0.0f)
	{
		quotient = (float)((int)(quotient + 0.5f));
	}
	else
	{
		quotient = (float)((int)(quotient - 0.5f));
	}
	float y = Value - (2.0f * 3.1415926535897932f) * quotient;

	float sign;
	if (y > 1.57079632679f)
	{
		y = 3.1415926535897932f - y;
		sign = -1.0f;
	}
	else if (y < -1.57079632679f)
	{
		y = -3.1415926535897932f - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}

	float y2 = y * y;

	*ScalarSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

	float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	*ScalarCos = sign * p;
}

inline FQuat FRotToQuat(FRotator Rot)
{
	const float DEG_TO_RAD = 3.1415926535897932f / (180.f);
	const float DIVIDE_BY_2 = DEG_TO_RAD / 2.f;
	float SP, SY, SR;
	float CP, CY, CR;

	sinCos(&SP, &CP, Rot.Pitch * DIVIDE_BY_2);
	sinCos(&SY, &CY, Rot.Yaw * DIVIDE_BY_2);
	sinCos(&SR, &CR, Rot.Roll * DIVIDE_BY_2);

	FQuat RotationQuat;
	RotationQuat.X = CR * SP * SY - SR * CP * CY;
	RotationQuat.Y = -CR * SP * CY - SR * CP * SY;
	RotationQuat.Z = CR * CP * SY - SR * SP * CY;
	RotationQuat.W = CR * CP * CY + SR * SP * SY;

	return RotationQuat;
}


template<typename T>
inline T* SpawnActor222(FVector Loc, FRotator Rot = FRotator(), AActor* Owner = nullptr, SDK::UClass* Class = T::StaticClass(), ESpawnActorCollisionHandlingMethod Handle = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn)
{
	FTransform Transform{};
	Transform.Scale3D = FVector{ 1,1,1 };
	Transform.Translation = Loc;
	Transform.Rotation = FRotToQuat(Rot);
	return (T*)UGameplayStatics::FinishSpawningActor(UGameplayStatics::BeginDeferredActorSpawnFromClass(UWorld::GetWorld(), Class, Transform, Handle, Owner), Transform);
}


template <class T>
inline T* BuildingSpawnActor(FVector Location, FRotator Rotation = FRotator{ 0, 0, 0 }, UClass* Class = T::StaticClass(), FVector Scale3D = { 1,1,1 })
{
	FTransform Transform{};
	Transform.Rotation = UKismetMathLibrary::Conv_RotatorToTransform(Rotation).Rotation;
	Transform.Scale3D = Scale3D;
	Transform.Translation = Location;

	auto Actor = UGameplayStatics::GetDefaultObj()->BeginSpawningActorFromClass(UWorld::GetWorld(), Class, Transform, false, nullptr);
	if (Actor)
		UGameplayStatics::GetDefaultObj()->FinishSpawningActor(Actor, Transform);
	return (T*)Actor;
}

inline AFortGameStateAthena* GetGameState()
{
	return (AFortGameStateAthena*)UWorld::GetWorld()->GameState;
}

inline UGameplayStatics* GetStatics()
{
	return (UGameplayStatics*)UGameplayStatics::StaticClass()->DefaultObject;
}

inline AActor* SpawnActorManual(UClass* Class, FVector Loc = FVector(), FRotator Rot = FRotator())
{
	FTransform Transform{};
	Transform.Translation = Loc;
	Transform.Scale3D = FVector{ 1,1,1 };
	Transform.Rotation = FRotToQuat(Rot);

	return GetStatics()->FinishSpawningActor(GetStatics()->BeginDeferredActorSpawnFromClass(UWorld::GetWorld(), Class, Transform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, nullptr), Transform);
}


//template<typename T>
//inline vector<T*> GetAllObjectsOfClass(UClass* Class = T::StaticClass())
//{
//	std::vector<T*> Objects{};
//
//	for (int i = 0; i < UObject::GObjects->Num(); ++i)
//	{
//		UObject* Object = UObject::GObjects->GetByIndex(i);
//
//		if (!Object)
//			continue;
//
//		if (Object->GetFullName().contains("Default"))
//			continue;
//
//		if (Object->GetFullName().contains("Test"))
//			continue;
//
//		if (Object->IsA(Class) && !Object->IsDefaultObject())
//		{
//			Objects.push_back((T*)Object);
//		}
//	}
//
//	return Objects;
//}

template<typename T>
inline  T* SpawnActor22(FVector Loc, FRotator Rot = {}, AActor* Owner = nullptr)
{
	static UGameplayStatics* statics = (UGameplayStatics*)UGameplayStatics::StaticClass()->DefaultObject;

	FTransform Transform{};
	Transform.Scale3D = FVector(1, 1, 1);
	Transform.Translation = Loc;

	return (T*)statics->FinishSpawningActor(statics->BeginDeferredActorSpawnFromClass(UWorld::GetWorld(), T::StaticClass(), Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, Owner), Transform);
}

template<typename T>
inline T* SpawnActor22(UClass* Class, FVector Loc, FRotator Rot = {}, AActor* Owner = nullptr)
{
	static UGameplayStatics* statics = (UGameplayStatics*)UGameplayStatics::StaticClass()->DefaultObject;

	FTransform Transform{};
	Transform.Scale3D = FVector(1, 1, 1);
	Transform.Translation = Loc;
	Transform.Rotation = FRotToQuat(Rot);

	return (T*)statics->FinishSpawningActor(statics->BeginDeferredActorSpawnFromClass(UWorld::GetWorld(), Class, Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, Owner), Transform);
}

inline AFortPickupAthena* SpawnPickup222222(FFortItemEntry* ItemEntry, FVector Loc, EFortPickupSourceTypeFlag SourceType, EFortPickupSpawnSource Source, int OverrideCount = -1)
{
	auto SpawnedPickup = SpawnActor22<AFortPickupAthena>(AFortPickupAthena::StaticClass(), Loc);
	SpawnedPickup->bRandomRotation = true;

	auto& PickupEntry = SpawnedPickup->PrimaryPickupItemEntry;
	PickupEntry.ItemDefinition = ItemEntry->ItemDefinition;
	PickupEntry.Count = OverrideCount != -1 ? OverrideCount : ItemEntry->Count;
	PickupEntry.LoadedAmmo = ItemEntry->LoadedAmmo;
	SpawnedPickup->OnRep_PrimaryPickupItemEntry();

	SpawnedPickup->TossPickup(Loc, nullptr, -1, true, false, SourceType, Source);

	if (SourceType == EFortPickupSourceTypeFlag::Container)
	{
		SpawnedPickup->bTossedFromContainer = true;
		SpawnedPickup->OnRep_TossedFromContainer();
	}

	return SpawnedPickup;
}

inline AFortPickupAthena* SpawnPickup222222(FVector Loc, UFortItemDefinition* Def, EFortPickupSourceTypeFlag SourceTypeFlag, EFortPickupSpawnSource SpawnSource, int Count, int LoadedAmmo)
{
	FTransform Transform{};
	Transform.Translation = Loc;
	Transform.Scale3D = FVector{ 1,1,1 };
	AFortPickupAthena* Pickup = Cast<AFortPickupAthena>(UGameplayStatics::FinishSpawningActor(UGameplayStatics::BeginDeferredActorSpawnFromClass(UWorld::GetWorld(), AFortPickupAthena::StaticClass(), (FTransform&)Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn, nullptr), (FTransform&)Transform));
	if (!Pickup)
		return nullptr;
	Pickup->bRandomRotation = true;
	Pickup->PrimaryPickupItemEntry.ItemDefinition = Def;
	Pickup->PrimaryPickupItemEntry.Count = Count;
	Pickup->PrimaryPickupItemEntry.LoadedAmmo = LoadedAmmo;
	Pickup->OnRep_PrimaryPickupItemEntry();
	Pickup->TossPickup(Loc, nullptr, -1, true, false, SourceTypeFlag, SpawnSource);

	if (SourceTypeFlag == EFortPickupSourceTypeFlag::Container)
	{
		Pickup->bTossedFromContainer = true;
		Pickup->OnRep_TossedFromContainer();
	}
	return Pickup;
}
inline AFortPickupAthena* SpawnPickup222222(UFortItemDefinition* ItemDef, int OverrideCount, int LoadedAmmo, FVector Loc, EFortPickupSourceTypeFlag SourceType, EFortPickupSpawnSource Source)
{
	auto SpawnedPickup = SpawnActor22<AFortPickupAthena>(AFortPickupAthena::StaticClass(), Loc);
	SpawnedPickup->bRandomRotation = true;

	auto& PickupEntry = SpawnedPickup->PrimaryPickupItemEntry;
	PickupEntry.ItemDefinition = ItemDef;
	PickupEntry.Count = OverrideCount;
	PickupEntry.LoadedAmmo = LoadedAmmo;
	SpawnedPickup->OnRep_PrimaryPickupItemEntry();

	SpawnedPickup->TossPickup(Loc, nullptr, -1, true, false, SourceType, Source);

	if (SourceType == EFortPickupSourceTypeFlag::Container)
	{
		SpawnedPickup->bTossedFromContainer = true;
		SpawnedPickup->OnRep_TossedFromContainer();
	}

	return SpawnedPickup;
}

#include <iostream>
#include <algorithm>
#include <random>
#include <thread>
#include <intrin.h>

inline UFortPlaylistAthena* CurrentPlaylist()
{
	return ((AFortGameStateAthena*)UWorld::GetWorld()->GameState)->CurrentPlaylistInfo.BasePlaylist;
}

inline std::vector<struct Loadout*> Loadouts{};
template<typename Iter, typename RandomGenerator>
Iter properrandom(Iter start, Iter end, RandomGenerator& g) {
	std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
	std::advance(start, dis(g));
	return start;
}

template<typename Iter>
Iter properrandom(Iter start, Iter end) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return properrandom(start, end, gen);
}

struct Loadout
{
	std::vector<std::pair<UFortItemDefinition*, std::pair<int, int>>> Items;

	void AddItem(UFortItemDefinition* Def, int Count = 1, int LoadedAmmo = 0)
	{
		Items.push_back({ Def, {Count, LoadedAmmo} });
	}

	static Loadout* GetRandomLoadout()
	{
		std::ranges::shuffle(Loadouts, std::default_random_engine{});
		return *properrandom(Loadouts.begin(), Loadouts.end());
	}
};



static SDK::UObject* (*StaticLoadObjectOG22)(SDK::UClass* Class, SDK::UObject* InOuter, const TCHAR* Name, const TCHAR* Filename, uint32_t LoadFlags, SDK::UObject* Sandbox, bool bAllowObjectReconciliation, void*) = decltype(StaticLoadObjectOG22)(InSDKUtils::GetImageBase() + 0x3776110);
template<typename T>
inline T* WeaponsStaticLoadObject(const TCHAR* name)
{
	auto Ret = (T*)StaticLoadObjectOG22(T::StaticClass(), nullptr, name, nullptr, 0, nullptr, false, nullptr);
	while (!Ret)
		Ret = (T*)StaticLoadObjectOG22(T::StaticClass(), nullptr, name, nullptr, 0, nullptr, false, nullptr);
	return Ret;
}


inline AFortGameModeAthena* GetGameMode()
{
	return (AFortGameModeAthena*)UWorld::GetWorld()->AuthorityGameMode;
}

inline void HookVTable(void* Base, int Idx, void* Detour, void** OG)
{
	DWORD oldProtection;

	void** VTable = *(void***)Base;

	if (OG)
	{
		*OG = VTable[Idx];
	}

	VirtualProtect(&VTable[Idx], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtection);

	VTable[Idx] = Detour;

	VirtualProtect(&VTable[Idx], sizeof(void*), oldProtection, NULL);
}

inline UFortKismetLibrary* GetFortKismet()
{
	return (UFortKismetLibrary*)UFortKismetLibrary::StaticClass()->DefaultObject;
}



template <class T>
inline TArray<T*> GetAllActorsOfClass() {
	TArray<T*> ResultActors;

	if (UWorld* World = UWorld::GetWorld()) {
		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(World, T::StaticClass(), &OutActors);

		for (AActor* Actor : OutActors) {
			if (T* CastedActor = Cast<T>(Actor)) {
				ResultActors.Add(CastedActor);
			}
		}
	}
	return ResultActors;
}

inline int GetOffset(UObject* Object, std::string name)
{
	FProperty* Property = nullptr;

	for (UStruct* Cls = Object->Class; Cls; Cls = Cls->Super)
	{
		FField* ChildProperties = Cls->ChildProperties;
		if (ChildProperties)
		{
			Property = (FProperty*)ChildProperties;
			std::string PropStr = ChildProperties->Name.ToString();
			while (Property)
			{
				if (PropStr == name)
					return Property->Offset;

				Property = (FProperty*)Property->Next;
				PropStr = Property ? Property->Name.ToString() : "Invalid Property";
			}
		}
	}
	if (!Property)
		return 0;
	return Property->Offset;
}
#include <type_traits>

static inline void* _NpFH = nullptr;
template <typename _Ot = void*>
inline __forceinline static void ExecHook(const char* _Name, void* _Detour, _Ot& _Orig = _NpFH)
{
	auto _Fn = StaticFindObject<UFunction>(_Name);
	if (!_Fn)
		return;
	if (!std::is_same_v<_Ot, void*>)
		_Orig = (_Ot)_Fn->ExecFunction;

	_Fn->ExecFunction = reinterpret_cast<UFunction::FNativeFuncPtr>(_Detour);
}
//Step: 48 8B 41 20 4C 8B D2 48 8B D1 44 0F B6 08 48 FF
//StepExplicitProperty: 41 8B 40 ? 4D 8B C8

class alignas(0x8) FOutputDevice
{
public:
	bool bSuppressEventTag;
	bool bAutoEmitLineTerminator;
};

struct FOutParmRec
{
	UProperty* Property;
	uint8* PropAddr;
	FOutParmRec* NextOutParm;
};


class FFrame : public FOutputDevice
{
public:
	void** VTable;
	UFunction* Node;
	UObject* Object;
	uint8* Code;
	uint8* Locals;
	FProperty* MostRecentProperty;
	uint8_t* MostRecentPropertyAddress;
	TArray<void*> FlowStack;
	FFrame* PreviousFrame;
	FOutParmRec* OutParms;
	uint8_t _Padding1[0x20]; // wtf else do they store here
	FField* PropertyChainForCompiledIn;
	UFunction* CurrentNativeFunction;
	bool bArrayContextFailed;

public:
	inline void StepCompiledIn(void* const Result = nullptr, bool ForceExplicitProp = false);

	template <typename T>
	__forceinline T& StepCompiledInRef() {
		T TempVal{};
		MostRecentPropertyAddress = nullptr;

		if (Code)
		{
			Step(this, Object, &TempVal);
		}
		else
		{
			FField* _Prop = PropertyChainForCompiledIn;
			PropertyChainForCompiledIn = _Prop->Next;
			StepExplicitProperty(this, &TempVal, _Prop);
		}

		return MostRecentPropertyAddress ? *(T*)MostRecentPropertyAddress : TempVal;
	}

	void IncrementCode();
};
inline auto Step = (void (*)(FFrame*, SDK::UObject*, void* const)) (InSDKUtils::GetImageBase() + 0x3897380); // upd s15
inline auto StepExplicitProperty = (void (*)(FFrame*, void* const, SDK::FField*)) (InSDKUtils::GetImageBase() + 0x38973B0);// upd s15
inline void FFrame::StepCompiledIn(void* const Result, bool ForceExplicitProp)
{
	if (Code && !ForceExplicitProp)
	{
		Step(this, Object, Result);
	}
	else
	{
		FField* _Prop = PropertyChainForCompiledIn;
		PropertyChainForCompiledIn = _Prop->Next;
		StepExplicitProperty(this, Result, _Prop);
	}
}


inline void FFrame::IncrementCode() {
	Code = (uint8_t*)(__int64(Code) + (bool)Code);
}

static_assert(offsetof(FFrame, Object) == 0x18, "FFrame::Object offset is wrong!");
static_assert(offsetof(FFrame, Code) == 0x20, "FFrame::Code offset is wrong!");
static_assert(offsetof(FFrame, MostRecentPropertyAddress) == 0x38, "FFrame::MostRecentPropertyAddress offset is wrong!");
static_assert(offsetof(FFrame, PropertyChainForCompiledIn) == 0x80, "FFrame::PropertyChainForCompiledIn offset is wrong!");



static float EvaluateScalableFloat(FScalableFloat& Float)
{
	if (!Float.Curve.CurveTable)
		return Float.Value;

	float Out;
	UDataTableFunctionLibrary::EvaluateCurveTableRow(Float.Curve.CurveTable, Float.Curve.RowName, (float)0, nullptr, &Out, FString());
	return Out;
}


inline void ShowFoundation(ABuildingFoundation* BuildingFoundation) {
	if (!BuildingFoundation)
		return;

	BuildingFoundation->bServerStreamedInLevel = true;
	BuildingFoundation->DynamicFoundationType = EDynamicFoundationType::Static;
	BuildingFoundation->OnRep_ServerStreamedInLevel();

	BuildingFoundation->FoundationEnabledState = EDynamicFoundationEnabledState::Enabled;
	BuildingFoundation->DynamicFoundationRepData.EnabledState = EDynamicFoundationEnabledState::Enabled;
	BuildingFoundation->DynamicFoundationTransform = BuildingFoundation->GetTransform();
	BuildingFoundation->OnRep_DynamicFoundationRepData();
}


inline UFortPlaylistAthena* GetPlaylist() {
	auto Playlist = Cast<AFortGameStateAthena>(UWorld::GetWorld()->GameState)->CurrentPlaylistInfo.BasePlaylist;

	if (Playlist)
		return Playlist;

	return nullptr;
}

inline UKismetMathLibrary* GetMath()
{
	return (UKismetMathLibrary*)UKismetMathLibrary::StaticClass()->DefaultObject;
}

static double precision(double f, double places)
{
	double n = pow(10., places);
	return round(f * n) / n;
}


template<typename T>
inline std::vector<T*> GetAllObjectsOfClass(UClass* Class = T::StaticClass())
{
	std::vector<T*> Objects{};

	for (int i = 0; i < UObject::GObjects->Num(); ++i)
	{
		UObject* Object = UObject::GObjects->GetByIndex(i);

		if (!Object)
			continue;

		if (Object->GetFullName().contains("Default"))
			continue;

		if (Object->GetFullName().contains("Test"))
			continue;

		if (Object->IsA(Class) && !Object->IsDefaultObject())
		{
			Objects.push_back((T*)Object);
		}
	}

	return Objects;
}

