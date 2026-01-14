#include "GJPathRewardPopup.hpp"

#include "../Other/JamManager.hpp"

void ProGJPathRewardPopup::onClaim(CCObject* sender) {
    JamManager::get().m_nextRewardUnlockLayerCanRewardJam = true;
    GJPathRewardPopup::onClaim(sender);
}