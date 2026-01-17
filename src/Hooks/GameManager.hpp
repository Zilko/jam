#include "../Includes.hpp"

#include <Geode/modify/GameManager.hpp>

class $modify(ProGameManager, GameManager) {

    bool isColorUnlocked(int, UnlockType);

    bool isIconUnlocked(int, IconType);

};