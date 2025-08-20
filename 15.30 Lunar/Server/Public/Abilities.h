#pragma once

#include "../../Framework.h"

namespace Abilities
{
    static inline bool (*InternalTryActivateAbility)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, FPredictionKey, UGameplayAbility**, void*, const FGameplayEventData*) = decltype(InternalTryActivateAbility)(InSDKUtils::GetImageBase() + 0xa767c0);
    static inline FGameplayAbilitySpecHandle* (*InternalGiveAbility)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec) = decltype(InternalGiveAbility)(InSDKUtils::GetImageBase() + 0xa6faf0);
    static inline __int64 (*SpecConstructor)(FGameplayAbilitySpec*, UObject*, int, int, UObject*) = decltype(SpecConstructor)(InSDKUtils::GetImageBase() + 0xa4c210);
    static inline FGameplayAbilitySpecHandle(*GiveAbilityAndActivateOnceFn)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec, FGameplayEventData*) = decltype(GiveAbilityAndActivateOnceFn)(InSDKUtils::GetImageBase() + 0xa6fc20);

    void InternalServerTryActiveAbilityHook(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle, bool InputPressed, const FPredictionKey& PredictionKey, const FGameplayEventData* TriggerEventData);
    FGameplayAbilitySpec* FindAbilitySpecFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle);

    void GiveDefaultAbilitySet(UAbilitySystemComponent* AbilitySystemComponent);
    void GiveAbility(UAbilitySystemComponent* AbilitySystemComponent, UGameplayAbility* GameplayAbility);
    void GiveAbilityAndActivateOnce(UAbilitySystemComponent* AbilitySystemComponent, UGameplayAbility* GameplayAbility, UObject* SourceObject);

    void ExecuteGameplayCue(AFortPlayerPawnAthena* Pawn, FGameplayTag GameplayTag);

    void Hooking();
}