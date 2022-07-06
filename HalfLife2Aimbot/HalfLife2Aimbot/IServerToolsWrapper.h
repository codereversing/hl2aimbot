#pragma once

#include "IBaseInterfaceWrapper.h"
#include "IServerEntityWrapper.h"
#include "VectorWrapper.h"

class IServerTools : public IBaseInterface
{
    using IClientEntity = void;
    using CBaseEntity = void;
    using IEntityFactoryDictionary = void;
    using CBaseAnimating = void;
    using ITempEntsSystem = void;
    using CTakeDamageInfo = void;

public:
    virtual IServerEntity* GetIServerEntity(IClientEntity* pClientEntity) = 0;
    virtual bool SnapPlayerToPosition(const Vector& org, const QAngle& ang, IClientEntity* pClientPlayer = nullptr) = 0;
    virtual bool GetPlayerPosition(Vector& org, QAngle& ang, IClientEntity* pClientPlayer = nullptr) = 0;
    virtual bool SetPlayerFOV(int fov, IClientEntity* pClientPlayer = nullptr) = 0;
    virtual int GetPlayerFOV(IClientEntity* pClientPlayer = nullptr) = 0;
    virtual bool IsInNoClipMode(IClientEntity* pClientPlayer = nullptr) = 0;

    virtual CBaseEntity* FirstEntity(void) = 0;
    virtual CBaseEntity* NextEntity(CBaseEntity* pEntity) = 0;
    virtual CBaseEntity* FindEntityByHammerID(int iHammerID) = 0;

    virtual bool GetKeyValue(CBaseEntity* pEntity, const char* szField, char* szValue, int iMaxLen) = 0;
    virtual bool SetKeyValue(CBaseEntity* pEntity, const char* szField, const char* szValue) = 0;
    virtual bool SetKeyValue(CBaseEntity* pEntity, const char* szField, float flValue) = 0;
    virtual bool SetKeyValue(CBaseEntity* pEntity, const char* szField, const Vector& vecValue) = 0;

    virtual CBaseEntity* CreateEntityByName(const char* szClassName) = 0;
    virtual void DispatchSpawn(CBaseEntity* pEntity) = 0;

    virtual void ReloadParticleDefintions(const char* pFileName, const void* pBufData, int nLen) = 0;

    virtual void AddOriginToPVS(const Vector& org) = 0;
    virtual void MoveEngineViewTo(const Vector& vPos, const QAngle& vAngles) = 0;

    virtual bool DestroyEntityByHammerId(int iHammerID) = 0;
    virtual CBaseEntity* GetBaseEntityByEntIndex(int iEntIndex) = 0;
    virtual void RemoveEntity(CBaseEntity* pEntity) = 0;
    virtual void RemoveEntityImmediate(CBaseEntity* pEntity) = 0;
    virtual IEntityFactoryDictionary* GetEntityFactoryDictionary(void) = 0;

    virtual void SetMoveType(CBaseEntity* pEntity, int val) = 0;
    virtual void SetMoveType(CBaseEntity* pEntity, int val, int moveCollide) = 0;
    virtual void ResetSequence(CBaseAnimating* pEntity, int nSequence) = 0;
    virtual void ResetSequenceInfo(CBaseAnimating* pEntity) = 0;

    virtual void ClearMultiDamage(void) = 0;
    virtual void ApplyMultiDamage(void) = 0;
    virtual void AddMultiDamage(const CTakeDamageInfo* pTakeDamageInfo, CBaseEntity* pEntity) = 0;
    virtual void RadiusDamage(const CTakeDamageInfo* info, const Vector& vecSrc, float flRadius, int iClassIgnore, CBaseEntity* pEntityIgnore) = 0;

    virtual ITempEntsSystem* GetTempEntsSystem(void) = 0;
};