#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/EffectGameObject.hpp>

// for the randomised numbers
#include <random>

#define GET_SETTING_BOOL(value, name) bool value = Mod::get()->getSettingValue<bool>(name)
#define GET_SETTING_FLOAT(value, name) float value = static_cast<float>(Mod::get()->getSettingValue<double>(name))

#define DEBUG_OPTIONS false 

// portal object IDs
const int SLOW_SPEED_PORTAL = 200;
const int NORMAL_SPEED_PORTAL = 201;
const int FAST_SPEED_PORTAL = 202;
const int FASTER_SPEED_PORTAL = 203;
const int FASTEST_SPEED_PORTAL = 1334;

// randomised speed logic
float speedRandomiser(float minSpeed, float maxSpeed) {
    static std::default_random_engine e;
    static std::uniform_real_distribution<float> dis(minSpeed, maxSpeed);
    return dis(e);
}

float portalSpeeds(Speed speed, bool isPlayer1) {
    GET_SETTING_BOOL(enabled, "enable-mod");
    GET_SETTING_BOOL(randomSpeed, "random-speed");
    GET_SETTING_BOOL(enabled2p, "enable-mod-2p");

    // Player 1 settings
    GET_SETTING_FLOAT(minSpeedSlowP1, "min-speed-slow-p1");
    GET_SETTING_FLOAT(maxSpeedSlowP1, "max-speed-slow-p1");
    GET_SETTING_FLOAT(minSpeedNormalP1, "min-speed-normal-p1");
    GET_SETTING_FLOAT(maxSpeedNormalP1, "max-speed-normal-p1");
    GET_SETTING_FLOAT(minSpeedFastP1, "min-speed-fast-p1");
    GET_SETTING_FLOAT(maxSpeedFastP1, "max-speed-fast-p1");
    GET_SETTING_FLOAT(minSpeedFasterP1, "min-speed-faster-p1");
    GET_SETTING_FLOAT(maxSpeedFasterP1, "max-speed-faster-p1");
    GET_SETTING_FLOAT(minSpeedFastestP1, "min-speed-fastest-p1");
    GET_SETTING_FLOAT(maxSpeedFastestP1, "max-speed-fastest-p1");
    GET_SETTING_FLOAT(slowSpeedP1, "slow-speed-p1");
    GET_SETTING_FLOAT(fullSpeedP1, "normal-speed-p1");
    GET_SETTING_FLOAT(fastSpeedP1, "fast-speed-p1");
    GET_SETTING_FLOAT(fasterSpeedP1, "faster-speed-p1");
    GET_SETTING_FLOAT(fastestSpeedP1, "fastest-speed-p1");

    // Player 2 settings
    GET_SETTING_FLOAT(minSpeedSlowP2, "min-speed-slow-p2");
    GET_SETTING_FLOAT(maxSpeedSlowP2, "max-speed-slow-p2");
    GET_SETTING_FLOAT(minSpeedNormalP2, "min-speed-normal-p2");
    GET_SETTING_FLOAT(maxSpeedNormalP2, "max-speed-normal-p2");
    GET_SETTING_FLOAT(minSpeedFastP2, "min-speed-fast-p2");
    GET_SETTING_FLOAT(maxSpeedFastP2, "max-speed-fast-p2");
    GET_SETTING_FLOAT(minSpeedFasterP2, "min-speed-faster-p2");
    GET_SETTING_FLOAT(maxSpeedFasterP2, "max-speed-faster-p2");
    GET_SETTING_FLOAT(minSpeedFastestP2, "min-speed-fastest-p2");
    GET_SETTING_FLOAT(maxSpeedFastestP2, "max-speed-fastest-p2");
    GET_SETTING_FLOAT(slowSpeedP2, "slow-speed-p2");
    GET_SETTING_FLOAT(fullSpeedP2, "normal-speed-p2");
    GET_SETTING_FLOAT(fastSpeedP2, "fast-speed-p2");
    GET_SETTING_FLOAT(fasterSpeedP2, "faster-speed-p2");
    GET_SETTING_FLOAT(fastestSpeedP2, "fastest-speed-p2");
    
    if (isPlayer1) {
        switch (speed) {
            case Speed::Slow: return !enabled ? 0.7 :  (randomSpeed ? speedRandomiser(minSpeedSlowP1, maxSpeedSlowP1) : slowSpeedP1);
            case Speed::Normal: return !enabled ? 0.9 :  (randomSpeed ? speedRandomiser(minSpeedNormalP1, maxSpeedNormalP1) : fullSpeedP1);
            case Speed::Fast: return !enabled ? 1.1 :  (randomSpeed ? speedRandomiser(minSpeedFastP1, maxSpeedFastP1) : fastSpeedP1);
            case Speed::Faster: return !enabled ? 1.3 :  (randomSpeed ? speedRandomiser(minSpeedFasterP1, maxSpeedFasterP1) : fasterSpeedP1);
            case Speed::Fastest: return !enabled ? 1.6 :  (randomSpeed ? speedRandomiser(minSpeedFastestP1, maxSpeedFastestP1) : fastestSpeedP1);
        }
    } else {
        switch (speed) {
            case Speed::Slow: return (!enabled && !enabled2p) ? 0.7 :  (randomSpeed ? speedRandomiser(minSpeedSlowP2, maxSpeedSlowP2) : slowSpeedP2);
            case Speed::Normal: return (!enabled && !enabled2p) ? 0.9 :  (randomSpeed ? speedRandomiser(minSpeedNormalP2, maxSpeedNormalP2) : fullSpeedP2);
            case Speed::Fast: return (!enabled && !enabled2p) ? 1.1 :  (randomSpeed ? speedRandomiser(minSpeedFastP2, maxSpeedFastP2) : fastSpeedP2);
            case Speed::Faster: return (!enabled && !enabled2p) ? 1.3 :  (randomSpeed ? speedRandomiser(minSpeedFasterP2, maxSpeedFasterP2) : fasterSpeedP2);
            case Speed::Fastest: return (!enabled && !enabled2p) ? 1.6 :  (randomSpeed ? speedRandomiser(minSpeedFastestP2, maxSpeedFastestP2) : fastestSpeedP2);
        }
    }
}

// speed logic:
// player
class $modify(SpeedPlayer, PlayerObject) {
    void updateTimeMod(Speed speed, bool p1, bool isPlayer1) {
        float playerSpeed = portalSpeeds(speed, isPlayer1);
#if DEBUG_OPTIONS
        log::debug("Player speed: {}", playerSpeed);
#endif
        PlayerObject::updateTimeMod(playerSpeed, p1);
        this->m_playerSpeed = playerSpeed;
    }
};

// assigning speeds to portals when starting level (p0->m_startSpeed)
class $modify(SpeedGJBGL, GJBaseGameLayer) {
    void updateTimeMod(Speed speed, bool p1, bool isPlayer1) {
        this->m_gameState.m_timeModRelated  = 0;
        this->m_gameState.m_timeModRelated2 = 0;
        static_cast<SpeedPlayer *>(this->m_player1)->updateTimeMod(speed, p1, isPlayer1); 
        if (this->m_gameState.m_isDualMode)
            static_cast<SpeedPlayer *>(this->m_player2)->updateTimeMod(speed, p1, !isPlayer1);
    }

    void setupLevelStart(LevelSettingsObject *p0) {
        GJBaseGameLayer::setupLevelStart(p0);

        bool isPlayer1 = !Mod::get()->getSettingValue<bool>("enable-mod-2p") ? true : (this->m_gameState.m_isDualMode ? false : true);
        SpeedPlayer *player = isPlayer1 ? static_cast<SpeedPlayer *>(this->m_player1) : static_cast<SpeedPlayer *>(this->m_player2);
        
        switch (p0->m_startSpeed) {
            case Speed::Slow:    player->updateTimeMod(Speed::Slow, 0, isPlayer1);    break;
            case Speed::Normal:  player->updateTimeMod(Speed::Normal, 0, isPlayer1);  break;
            case Speed::Fast:    player->updateTimeMod(Speed::Fast, 0, isPlayer1);    break;
            case Speed::Faster:  player->updateTimeMod(Speed::Faster, 0, isPlayer1);  break;
            case Speed::Fastest: player->updateTimeMod(Speed::Fastest, 0, isPlayer1); break;
        }
    }
};

class $modify(SpeedEffectGameObject, EffectGameObject) {
    void triggerObject(GJBaseGameLayer *p0, int p1, gd::vector<int> const *p2) {
        EffectGameObject::triggerObject(p0, p1, p2);

        bool isPlayer1 = !Mod::get()->getSettingValue<bool>("enable-mod-2p") ? true : (this->m_activatedByPlayer1 ? true : ((p0->m_gameState.m_isDualMode && this->m_activatedByPlayer2) ? false : true));
        SpeedGJBGL *gameLayer = static_cast<SpeedGJBGL *>(p0);

#if DEBUG_OPTIONS
        log::debug("isPlayer1: {}", isPlayer1);
#endif

        if (this->m_objectID == SLOW_SPEED_PORTAL)
            gameLayer->updateTimeMod(Speed::Slow, this->m_hasNoEffects, isPlayer1);
        else if (this->m_objectID == NORMAL_SPEED_PORTAL)
            gameLayer->updateTimeMod(Speed::Normal, this->m_hasNoEffects, isPlayer1);
        else if (this->m_objectID == FAST_SPEED_PORTAL)
            gameLayer->updateTimeMod(Speed::Fast, this->m_hasNoEffects, isPlayer1);
        else if (this->m_objectID == FASTER_SPEED_PORTAL)
            gameLayer->updateTimeMod(Speed::Faster, this->m_hasNoEffects, isPlayer1);
        else if (this->m_objectID == FASTEST_SPEED_PORTAL)
            gameLayer->updateTimeMod(Speed::Fastest, this->m_hasNoEffects, isPlayer1);
    }
};
