#pragma once

#include "IHandleEntityWrapper.h"

class IServerUnknown : public IHandleEntity
{
    using ICollideable = void;
    using IServerNetworkable = void;
    using CBaseEntity = void;

public:
    virtual ICollideable* GetCollideable() = 0;
    virtual IServerNetworkable* GetNetworkable() = 0;
    virtual CBaseEntity* GetBaseEntity() = 0;
};