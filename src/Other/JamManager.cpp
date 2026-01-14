#include "JamManager.hpp"
#include "Utils.hpp"

JamManager& JamManager::get() {
    static JamManager instance;
    return instance;
}

int JamManager::getJam() {
    return Mod::get()->getSavedValue<int>("jam");
}

void JamManager::rewardJam(int jam) {
    if (jam <= 0) {
        return;
    }

    Mod::get()->setSavedValue("jam", getJam() + jam);
}

bool JamManager::canLevelHaveJam(GJGameLevel* level) {
    return level->m_levelID.value() > 0
        && level->m_stars.value() > 0;
}

int JamManager::getTotalLevelJam(GJGameLevel* level) {
    if (!canLevelHaveJam(level)) {
        return 0;
    }

    auto stars = level->m_stars.value();

    if (stars <= 0) {
        return 0;
    }

    if (stars <= 3) {
        return 20;
    }

    if (stars <= 6) {
        return 40;
    }

    if (stars <= 9) {
        return 60;
    }

    if (stars >= 10) {
        return 80;
    }

    return 0;
}

float JamManager::getLevelJamProgress(GJGameLevel* level) {
    auto obj = Mod::get()->getSavedValue<matjson::Value>("jam-level-progress");
    auto id = std::to_string(level->m_levelID);

    if (obj.contains(id)) {
        return obj[id].asDouble().unwrapOr(0.f);
    }

    return 0.f;
}

void JamManager::setLevelJamProgress(GJGameLevel* level, float progress) {
    auto obj = Mod::get()->getSavedValue<matjson::Value>("jam-level-progress");

    obj[std::to_string(level->m_levelID)] = progress;

    Mod::get()->setSavedValue("jam-level-progress", obj);
}

int JamManager::getLevelJamForProgress(GJGameLevel* level, float progress) {
    if (progress >= 1.f) {
        return getTotalLevelJam(level);
    }

    return getTotalLevelJam(level) / 2.f * progress;
}

int JamManager::getJamChestReward(int chestType) {
    if (chestType <= 0 || chestType >= 9) {
        return 0;
    }

    if (
        chestType <= 3
        && Utils::getRandomInt(0, 100) < 36
    ) {
        return 0;
    }
    
    if (
        chestType >= 4
        && chestType <= 6
        && Utils::getRandomInt(0, 100) < 17
    ) {
        return 0;
    }

    switch (chestType) {
        case 1: return Utils::getRandomInt(1, 3) * 5;
        case 2:
        case 3: return Utils::getRandomInt(1, 6) * 5 + 15;
        case 4: return Utils::getRandomInt(1, 15) * 5 + 45;
        case 5: return Utils::getRandomInt(1, 15) * 5 + 120;
        case 6: return Utils::getRandomInt(1, 15) * 5 + 195;
        case 7: return Utils::getRandomInt(1, 5) * 5 + 270;
        case 8: return Utils::getRandomInt(1, 5) * 15 + 295;
        default: return 0;
    }
}