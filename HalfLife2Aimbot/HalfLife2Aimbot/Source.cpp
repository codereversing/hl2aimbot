#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <set>
#include <string>
#include <thread>

#include "IServerToolsWrapper.h"
#include "VectorWrapper.h"

#undef max

using CBasePlayer = void;
using CBaseEntity = void;

using GetServerToolsFnc = IServerTools* (__stdcall*)();
using GetLocalPlayerFnc = CBasePlayer* (__stdcall*)();
using GetEyePositionFnc = Vector (__thiscall*)(CBaseEntity* entity);
using SnapEyeAnglesFnc = void (__thiscall*)(CBasePlayer* player, const QAngle& angles);

template <typename T>
T GetFunctionPointer(const std::string moduleName, const DWORD_PTR offset) {

    auto moduleBaseAddress{ GetModuleHandleA(moduleName.c_str()) };
    if (moduleBaseAddress == nullptr) {
        std::cerr << "Could not get base address of " << moduleName
            << std::endl;
        std::abort();
    }
    return reinterpret_cast<T>(
        reinterpret_cast<DWORD_PTR>(moduleBaseAddress) + offset);
}

template <typename T>
T GetClassMember(void* classPtr, const unsigned int offset)
{
    return *(reinterpret_cast<T*>(
        reinterpret_cast<char*>(
            classPtr) + offset));
}

IServerTools* GetServerTools() {

    constexpr auto globalServerToolsOffset{ 0x3FC400 };
    static GetServerToolsFnc getServerToolsFnc{ GetFunctionPointer<GetServerToolsFnc>(
        "server.dll", globalServerToolsOffset) };

    return getServerToolsFnc();
}

CBasePlayer* GetLocalPlayer() {

    constexpr auto globalGetLocalPlayerOffset{ 0x26D5F0 };
    static GetLocalPlayerFnc getLocalPlayerFnc{ GetFunctionPointer<GetLocalPlayerFnc>(
        "server.dll", globalGetLocalPlayerOffset) };

    return getLocalPlayerFnc();
}

Vector GetEyePosition(CBaseEntity* entity) {
    
    constexpr auto globalGetEyePositionOffset{ 0x119D00 };
    static GetEyePositionFnc getEyePositionFnc{ GetFunctionPointer<GetEyePositionFnc>(
        "server.dll", globalGetEyePositionOffset) };

    return getEyePositionFnc(entity);
}

void SnapEyeAngles(CBasePlayer* player, const QAngle& angles)
{
    constexpr auto globalSnapEyeAnglesOffset{ 0x1FD320 };
    static SnapEyeAnglesFnc snapEyeAnglesFnc{ GetFunctionPointer<SnapEyeAnglesFnc>(
        "server.dll", globalSnapEyeAnglesOffset) };

    return snapEyeAnglesFnc(player, angles);
}

const char* GetEntityName(IServerEntity* serverEntity)
{
    return GetClassMember<const char*>(serverEntity, 0x5C);
}

bool IsEntityEnemy(const std::string& entityName) {

    static std::set<std::string> enemyNPCs{
    "npc_antlion", "npc_antlion_grub", "npc_antlionguard",
    "npc_barnacle", "npc_fastzombie", "npc_fastzombie_torso",
    "npc_headcrab", "npc_headcrab_black", "npc_headcrab_fast",
    "npc_hunter", "npc_ichthyosaur", "npc_metropolice",
    "npc_poisonzombie", "npc_rollermine", "npc_sniper",
    "npc_stalker", "npc_strider", "npc_turret_ceiling",
    "npc_turret_floor", "npc_turret_ground", "npc_vortigaunt",
    "npc_zombie", "npc_zombie_torso", "npc_zombine"
    };

    return enemyNPCs.contains(entityName);
}

void SetPlayerEyeAnglesToPosition(const Vector& enemyEyePosition) {

    Vector eyePosition{};
    QAngle eyeAngles{};
    GetServerTools()->GetPlayerPosition(eyePosition, eyeAngles);

    Vector forwardVector{ enemyEyePosition.x - eyePosition.x,
        enemyEyePosition.y - eyePosition.y,
        enemyEyePosition.z - eyePosition.z
    };

    VectorNormalize(forwardVector);

    QAngle newEyeAngles{};
    VectorAngles(forwardVector, newEyeAngles);

    SnapEyeAngles(GetLocalPlayer(), newEyeAngles);
}

Vector GetFurthestVector() {

    return Vector {
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max(),
        std::numeric_limits<float>::max()
    };
}

__declspec(dllexport) BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID reserved)
{
    if (dwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        if (AllocConsole()) {
            (void)freopen("CONOUT$", "w", stdout);
            (void)freopen("CONOUT$", "w", stderr);
            SetConsoleTitle(L"Console");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            std::cerr << "DLL Loaded." << std::endl;
        }

        std::thread([] {
            auto closestEnemyVector{ GetFurthestVector() };
            auto closestEnemyDistance{ std::numeric_limits<float>::max() };
            auto forceSnap{ true };

            while (true) {

                // Press B to lock/unlock view
                if (GetAsyncKeyState(0x42)) {
                    //std::cerr << "Changing force snap state"
                    //	<< std::endl;
                    forceSnap = !forceSnap;
                }

                auto* serverEntity{ reinterpret_cast<IServerEntity*>(
                    GetServerTools()->FirstEntity()) };

                if (serverEntity != nullptr) {
                    do {
                        if (serverEntity == GetServerTools()->FirstEntity()) {
                            //std::cerr << "Setting new vector: " << closestEnemyVector.x
                            //	<< " " << closestEnemyVector.y << " " << closestEnemyVector.z
                            //	<< std::endl;

                            if (forceSnap) {
                                SetPlayerEyeAnglesToPosition(closestEnemyVector);
                            }

                            closestEnemyDistance = std::numeric_limits<float>::max();
                            closestEnemyVector = GetFurthestVector();
                        }

                        auto* modelName{ serverEntity->GetModelName().ToCStr() };
                        if (modelName != nullptr) {
                            auto entityName{ std::string{GetEntityName(serverEntity)} };
                            //std::cerr << entityName
                            //	<< std::endl;

                            if (IsEntityEnemy(entityName)) {
                                Vector eyePosition{};
                                QAngle eyeAngles{};

                                GetServerTools()->GetPlayerPosition(eyePosition, eyeAngles);

                                auto enemyEyePosition{ GetEyePosition(serverEntity) };
                                //std::cerr << "Enemy eye position "
                                //    << enemyEyePosition.x << " "
                                //    << enemyEyePosition.y << " "
                                //    << enemyEyePosition.z
                                //    << std::endl;

                                auto distance{ VectorDistance(enemyEyePosition, eyePosition) };
                                if (distance <= closestEnemyDistance) {
                                    //std::cerr << "New closest distance is " << distance
                                    //	<< std::endl;
                                    closestEnemyDistance = distance;
                                    closestEnemyVector = enemyEyePosition;
                                }
                            }
                        }

                        serverEntity = reinterpret_cast<IServerEntity*>(
                            GetServerTools()->NextEntity(serverEntity));

                    } while (serverEntity != nullptr);
                }

                Sleep(10);
            }
        }).detach();
    }

    return TRUE;
}
