
#pragma once
#include "../../framework.h"

class XP_Accolades
{
public:
    static void GiveAccolade(
        AFortPlayerControllerAthena* PC,
        UFortAccoladeItemDefinition* AccoladeDef,
        UFortQuestItemDefinition* QuestDef,
        EXPEventPriorityType Priority = EXPEventPriorityType::XPBarOnly
    );
};

class XP_Challanges
{
public:
    static void UpdateChallange(
        UFortQuestManager* QuestManager,
        UFortQuestItemDefinition* QuestItem,
        FName BackendName,
        int Count
    );

    static void SendComplexCustomStatEvent(
        UFortQuestManager* ManagerComp,
        UObject* TargetObject,
        FGameplayTagContainer& AdditionalSourceTags,
        FGameplayTagContainer& TargetTags,
        bool* QuestActive,
        bool* QuestCompleted,
        int32 Count
    );

    static void SendStatEvent(
        UFortQuestManager* ManagerComp,
        UObject* TargetObject,
        FGameplayTagContainer& AdditionalSourceTags,
        FGameplayTagContainer& TargetTags,
        bool* QuestActive,
        bool* QuestCompleted,
        int32 Count,
        EFortQuestObjectiveStatEvent StatEvent
    );

    static bool SendDistanceUpdate(UGameplayAbility* Ability);
    static void InitXPHooks();
};
