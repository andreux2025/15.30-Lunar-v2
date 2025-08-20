#include "../Public/Abilities.h"

void Abilities::InternalServerTryActiveAbilityHook(UAbilitySystemComponent* AbilitySystemComponent,
    FGameplayAbilitySpecHandle Handle, bool InputPressed, const FPredictionKey& PredictionKey,
    const FGameplayEventData* TriggerEventData)
{
    FGameplayAbilitySpec* Spec = FindAbilitySpecFromHandle(AbilitySystemComponent, Handle);
    if (!Spec)
    {
        printf("InternalServerTryActiveAbility. Rejecting ClientActivation of ability with invalid SpecHandle!");
        AbilitySystemComponent->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
        return;
    }

    const UGameplayAbility* AbilityToActivate = Spec->Ability;

    if (!AbilityToActivate)
        return;

    UGameplayAbility* InstancedAbility = nullptr;
    Spec->InputPressed = true;

    // Attempt to activate the ability (server side) and tell the client if it succeeded or failed.
    if (InternalTryActivateAbility(AbilitySystemComponent, Handle, PredictionKey, &InstancedAbility, nullptr, TriggerEventData))
    {
        // TryActivateAbility handles notifying the client of success
    }
    else
    {
        AbilitySystemComponent->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
        Spec->InputPressed = false;

        AbilitySystemComponent->ActivatableAbilities.MarkItemDirty(*Spec);
    }
}

FGameplayAbilitySpec* Abilities::FindAbilitySpecFromHandle(UAbilitySystemComponent* AbilitySystemComponent, FGameplayAbilitySpecHandle Handle)
{
    for (FGameplayAbilitySpec& Spec : AbilitySystemComponent->ActivatableAbilities.Items)
    {
        if (Spec.Handle.Handle == Handle.Handle)
        {
            return &Spec;
        }
    }

    return nullptr;
}

void Abilities::GiveDefaultAbilitySet(UAbilitySystemComponent* AbilitySystemComponent)
{
    static UFortAbilitySet* GAS_AthenaPlayer = StaticLoadObject<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_AthenaPlayer.GAS_AthenaPlayer");

    if (!GAS_AthenaPlayer)
    {
        printf("Failed to load GAS_AthenaPlayer");
        return;
    }

    for (int i = 0; i < GAS_AthenaPlayer->GameplayAbilities.Num(); ++i)
    {
        UGameplayAbility* AbilityClass = Cast<UGameplayAbility>(GAS_AthenaPlayer->GameplayAbilities[i].Get()->DefaultObject);
        if (!AbilityClass) continue;

        Abilities::GiveAbility(AbilitySystemComponent, AbilityClass);
    }

    for (int i = 0; i < GAS_AthenaPlayer->GrantedGameplayEffects.Num(); ++i)
    {
        UClass* GameplayEffect = GAS_AthenaPlayer->GrantedGameplayEffects[i].GameplayEffect.Get();
        float Level = GAS_AthenaPlayer->GrantedGameplayEffects[i].Level;

        if (!GameplayEffect)
            continue;

        AbilitySystemComponent->BP_ApplyGameplayEffectToSelf(GameplayEffect, Level, FGameplayEffectContextHandle());
    }
}

void Abilities::GiveAbility(UAbilitySystemComponent* AbilitySystemComponent, UGameplayAbility* GameplayAbility)
{
    if (!GameplayAbility)
        return;

    FGameplayAbilitySpec Spec;
    SpecConstructor(&Spec, GameplayAbility, 1, -1, nullptr);
    InternalGiveAbility(AbilitySystemComponent, &Spec.Handle, Spec);
}

void Abilities::GiveAbilityAndActivateOnce(UAbilitySystemComponent* AbilitySystemComponent, UGameplayAbility* GameplayAbility, UObject* SourceObject)
{
    if (!GameplayAbility)
        return;

    FGameplayAbilitySpec Spec;
    SpecConstructor(&Spec, GameplayAbility, 1, -1, SourceObject);
    GiveAbilityAndActivateOnceFn(AbilitySystemComponent, &Spec.Handle, Spec, nullptr);
}

void Abilities::ExecuteGameplayCue(AFortPlayerPawnAthena* Pawn, FGameplayTag GameplayTag)
{
    Pawn->NetMulticast_InvokeGameplayCueAdded(GameplayTag, FPredictionKey(), Pawn->AbilitySystemComponent->MakeEffectContext());
    Pawn->NetMulticast_InvokeGameplayCueExecuted(GameplayTag, FPredictionKey(), Pawn->AbilitySystemComponent->MakeEffectContext());
}

void Abilities::Hooking()
{
    SwapVFTs(UAbilitySystemComponent::StaticClass()->DefaultObject, 0xFE, Abilities::InternalServerTryActiveAbilityHook, nullptr);
    SwapVFTs(UFortAbilitySystemComponent::StaticClass()->DefaultObject, 0xFE, Abilities::InternalServerTryActiveAbilityHook, nullptr);
    SwapVFTs(UFortAbilitySystemComponentAthena::StaticClass()->DefaultObject, 0xFE, Abilities::InternalServerTryActiveAbilityHook, nullptr);
}