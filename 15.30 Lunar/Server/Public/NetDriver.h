#pragma once
#include "../../framework.h"

namespace NetDriver
{
    inline void (*ServerReplicateActors)(UReplicationDriver*);
    inline void (*SetWorld)(UNetDriver*, UWorld*);
    inline bool (*InitListen)(UNetDriver*, void*, FURL&, bool, FString&);
    inline UNetDriver* (*CreateNetDriver)(UEngine*, int64, FName);

    enum ENetMode
    {
        NM_Standalone,
        NM_DedicatedServer,
        NM_ListenServer,
        NM_Client,
        NM_MAX,
    };

    inline ENetMode GetNetMode() { return ENetMode::NM_DedicatedServer; }

    inline void (*TickFlush)(UNetDriver*);

    void TickFlushHook(UNetDriver* NetDriver);

    void NetDriverHooks();
}