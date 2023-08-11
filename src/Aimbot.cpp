#pragma once
#include <vector>
#include "LocalPlayer.cpp"
#include "Player.cpp"
#include "Math.cpp"
#include "Level.cpp"
#include "math.h"
#include "X11Utils.cpp"
#include "ConfigLoader.cpp"

class Aimbot
{
private:
    ConfigLoader *m_configLoader;
    Level *m_level;
    LocalPlayer *m_localPlayer;
    std::vector<Player *> *m_players;
    X11Utils *m_x11Utils;

    Player *m_lockedOnPlayer = nullptr;

public:
    Aimbot(ConfigLoader *configLoader,
           Level *level,
           LocalPlayer *localPlayer,
           std::vector<Player *> *players,
           X11Utils *x11Utils)
    {
        m_configLoader = configLoader;
        m_level = level;
        m_localPlayer = localPlayer;
        m_players = players;
        m_x11Utils = x11Utils;
    }
    void update()
    {
        // validations
        if (m_configLoader->getAimbotTrigger() != 0x0000)
        { // our trigger is a button
            if (!m_x11Utils->keyDown(m_configLoader->getAimbotTrigger()))
            {
		m_lockedOnPlayer = nullptr;
                return;
            }
        }
        if (!m_level->isPlayable())
        {
	    m_lockedOnPlayer = nullptr;
            return;
        }
        if (m_localPlayer->isDead() || m_localPlayer->isKnocked())
        {
            m_lockedOnPlayer = nullptr;
            return;
        }
        
        if (m_configLoader->getAimbotTrigger() == 0x0000) { // our trigger is localplayer attacking
            if (!m_localPlayer->isInAttack())
            {
                m_lockedOnPlayer = nullptr;
                return;
            }
	}
        // get desired angle to an enemy
        double desiredViewAngleYaw = 0;
        double desiredViewAnglePitch = 0;
	int smoothingLevel = m_configLoader->getAimbotSmoothing();
	int aimbotFOV = m_configLoader->getAimbotActivationFOV();
	int aimbotRange = m_configLoader->getAimbotMaxRange();
        if (false)
        {
            printf("X:%.6f \t Y: %.6f \t Z:%.6f \n", m_localPlayer->getLocationX(), m_localPlayer->getLocationY(), m_localPlayer->getLocationZ());
            const float dummyX = 31408.732422;
            const float dummyY = -6711.955566;
            const float dummyZ = -29234.839844;
            double distanceToTarget = math::calculateDistanceInMeters(m_localPlayer->getLocationX(), m_localPlayer->getLocationY(), m_localPlayer->getLocationZ(), dummyX, dummyY, dummyZ);
                return;
            desiredViewAngleYaw = math::calculateDesiredYaw(m_localPlayer->getLocationX(), m_localPlayer->getLocationY(), dummyX, dummyY);
            desiredViewAnglePitch = math::calculateDesiredPitch(m_localPlayer->getLocationX(), m_localPlayer->getLocationY(), m_localPlayer->getLocationZ(), dummyX, dummyY, dummyZ);
        }
        else
        {
            if (m_lockedOnPlayer == nullptr || !m_lockedOnPlayer->isVisible())
                m_lockedOnPlayer = findClosestEnemy();
            if (m_lockedOnPlayer == nullptr)
                return;
            double distanceToTarget = math::calculateDistanceInMeters(m_localPlayer->getLocationX(),
                                                                      m_localPlayer->getLocationY(),
                                                                      m_localPlayer->getLocationZ(),
                                                                      m_lockedOnPlayer->getLocationX(),
                                                                      m_lockedOnPlayer->getLocationY(),
                                                                      m_lockedOnPlayer->getLocationZ());
		if(m_localPlayer->isZooming() && distanceToTarget > 20) {smoothingLevel = 7; aimbotFOV=4; aimbotRange=100;}
            if (distanceToTarget > aimbotRange)
                return;
            desiredViewAngleYaw = math::calculateDesiredYaw(m_localPlayer->getLocationX(),
                                                      m_localPlayer->getLocationY(),
                                                      m_lockedOnPlayer->getLocationX(),
                                                      m_lockedOnPlayer->getLocationY());
            desiredViewAnglePitch = math::calculateDesiredPitch(m_localPlayer->getLocationX(),
                                                          m_localPlayer->getLocationY(),
                                                          m_localPlayer->getLocationZ(),
                                                          m_lockedOnPlayer->getLocationX(),
                                                          m_lockedOnPlayer->getLocationY(),
                                                          m_lockedOnPlayer->getLocationZ());
        }

        // Setup Pitch
        const double pitch = m_localPlayer->getPitch();
        const double pitchAngleDelta = math::calculatePitchAngleDelta(pitch, desiredViewAnglePitch);
        const double pitchAngleDeltaAbs = abs(pitchAngleDelta);
        if (pitchAngleDeltaAbs > aimbotFOV / 2)
            return;

        // Setup Yaw
        const double yaw = m_localPlayer->getYaw();
        const double angleDelta = math::calculateAngleDelta(yaw, desiredViewAngleYaw);
        const double angleDeltaAbs = abs(angleDelta);
        if (angleDeltaAbs > aimbotFOV)
            return;
        double newYaw = math::flipYawIfNeeded(yaw + (angleDelta / smoothingLevel));
        m_localPlayer->setYaw(newYaw);
    }
    Player *findClosestEnemy()
    {
        Player *closestPlayerSoFar = nullptr;
        double closestPlayerAngleSoFar;
        for (int i = 0; i < m_players->size(); i++)
        {
            Player *player = m_players->at(i);
            if (!player->isValid())
                continue;
            if (player->isKnocked())
                continue;
	    if(!m_level->isTrainingArea()) if(player->getTeamNumber() == m_localPlayer->getTeamNumber())
                continue;
            if (!player->isVisible())
                continue;
            double desiredViewAngleYaw = math::calculateDesiredYaw(m_localPlayer->getLocationX(),
                                                             m_localPlayer->getLocationY(),
                                                             player->getLocationX(),
                                                             player->getLocationY());
            double angleDelta = math::calculateAngleDelta(m_localPlayer->getYaw(), desiredViewAngleYaw);
            if (closestPlayerSoFar == nullptr)
            {
                closestPlayerSoFar = player;
                closestPlayerAngleSoFar = abs(angleDelta);
            }
            else
            {

                if (abs(angleDelta) < closestPlayerAngleSoFar)
                {
                    closestPlayerSoFar = player;
                    closestPlayerAngleSoFar = abs(angleDelta);
                }
            }
        }
        return closestPlayerSoFar;
    }
};
