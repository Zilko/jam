#include "ChallengesPage.hpp"

#include "../Other/JamManager.hpp"
#include "../Other/Utils.hpp"

void ProChallengesPage::claimItem(ChallengeNode* p0, GJChallengeItem* p1, CCPoint p2) {
    auto& jm = JamManager::get();

    if (jm.m_challengesPageShouldRewardJam) {
        m_currencyRewardLayer->createObjectsFull(
            CurrencySpriteType::Star, 
            jm.m_challengesPageJamAmount,
            nullptr,
            p2,
            0.f
        );

        Utils::modifyRewardLayer(m_currencyRewardLayer, CurrencySpriteType::Star);
    }

    ChallengesPage::claimItem(p0, p1, p2);

    jm.m_challengesPageShouldRewardJam = false;
    
    FMODAudioEngine::get()->playEffectAdvanced("lid.mp3"_spr, 0.7f, 1.f, 0.75f, 1.f, false, false, 45, 1000, 0, 0, false, 0, false, false, 0, 0, 0.f, 0);
}