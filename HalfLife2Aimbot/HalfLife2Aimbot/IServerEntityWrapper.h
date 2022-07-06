#pragma once

#include "IServerUnknownWrapper.h"
#include "string_t.h"

class IServerEntity : public IServerUnknown
{

public:
    virtual					~IServerEntity() {}

    virtual int				GetModelIndex(void) const = 0;
    virtual string_t		GetModelName(void) const = 0;

    virtual void			SetModelIndex(int index) = 0;
};