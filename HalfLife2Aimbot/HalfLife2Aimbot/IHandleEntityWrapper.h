#pragma once

class IHandleEntity
{
    using CBaseHandle = void;
public:
    virtual ~IHandleEntity() {}
    virtual void SetRefEHandle(const CBaseHandle* handle) = 0;
    virtual const CBaseHandle* GetRefEHandle() const = 0;
};