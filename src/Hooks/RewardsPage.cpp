#include "RewardsPage.hpp"

#include "../Other/JamManager.hpp"

ProRewardsPage::Fields::~Fields() {
    JamManager::get().m_nextRewardUnlockLayerCanRewardJam = false;
}

void ProRewardsPage::onReward(CCObject* sender) {
    m_fields.self();
    JamManager::get().m_nextRewardUnlockLayerCanRewardJam = true;
    RewardsPage::onReward(sender);
}