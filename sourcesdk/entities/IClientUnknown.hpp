//
// Created by sh1rm4n on 5/24/26.
//

#ifndef CENTURY_ICLIENTUNKNOWN_HPP
#define CENTURY_ICLIENTUNKNOWN_HPP

#include "C_BaseEntity.hpp"

class IClientNetworkable;
class IClientRenderable;

class IClientUnknown
{
public:
    virtual void*		        GetCollideable() = 0;
    virtual IClientNetworkable*	GetClientNetworkable() = 0;
    virtual IClientRenderable*	GetClientRenderable() = 0;
    virtual void*		        GetIClientEntity() = 0;
    virtual C_BaseEntity*		GetBaseEntity() = 0;
    virtual void*	            GetClientThinkable() = 0;
};

#endif //CENTURY_ICLIENTUNKNOWN_HPP
