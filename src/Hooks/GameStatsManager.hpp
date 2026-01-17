#include "../Includes.hpp"

#include <Geode/modify/GameStatsManager.hpp>

class $modify(ProGameStatsManager, GameStatsManager) {

    bool isItemUnlocked(UnlockType, int);

    bool isStoreItemUnlocked(int);

};