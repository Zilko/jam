#include "GameStatsManager.hpp"

#include "../Other/JamManager.hpp"

bool ProGameStatsManager::isItemUnlocked(UnlockType type, int id) {
    if (!GameStatsManager::isItemUnlocked(type, id)) {
        return JamManager::get().didUnlockItemWithJam(type, id);
    }

    return true;
}

bool ProGameStatsManager::isStoreItemUnlocked(int index) {
    if (!GameStatsManager::isStoreItemUnlocked(index)) {
        return JamManager::get().didUnlockItemWithJam(index);
    }

    return true;
}