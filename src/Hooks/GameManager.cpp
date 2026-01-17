#include "GameManager.hpp"

#include "../Other/JamManager.hpp"

bool ProGameManager::isColorUnlocked(int id, UnlockType type) {
    if (!GameManager::isColorUnlocked(id, type)) {
        return JamManager::get().didUnlockItemWithJam(type, id);
    }

    return true;
}

bool ProGameManager::isIconUnlocked(int id, IconType type) {
    if (!GameManager::isIconUnlocked(id, type)) {
        return JamManager::get().didUnlockItemWithJam(iconTypeToUnlockType(type), id);
    }

    return true;
}