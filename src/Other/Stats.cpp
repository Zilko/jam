#include <capeling.garage-stats-menu/include/stats_api.hpp>
#include "JamManager.hpp"

using namespace stats_api;

$execute {
    registerStatItem(
        "jam"_spr,
        []() {
            return cocos2d::CCSprite::create("jam1.png"_spr);
        },
        JamManager::get().getJam(),
        0.17f
    );
}