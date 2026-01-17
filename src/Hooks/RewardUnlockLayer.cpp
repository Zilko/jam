#include "RewardUnlockLayer.hpp"

#include "../Other/JamManager.hpp"

void ProRewardUnlockLayer::step3() {
    auto& jm = JamManager::get();
    
    if (
        !jm.m_nextRewardUnlockLayerCanRewardJam
        || !m_rewardItem
        || !m_rewardItem->m_rewardObjects
        || m_rewardItem->m_rewardObjects->count() <= 0
        || m_rewardItem->m_rewardObjects->count() >= 7
    ) {
        return RewardUnlockLayer::step3();
    }
    
    jm.m_nextRewardUnlockLayerCanRewardJam = false;

    if (m_rewardItem->m_rewardObjects->count() == 1) {
        auto object = static_cast<GJRewardObject*>(m_rewardItem->m_rewardObjects->firstObject());
        auto unlockType = static_cast<int>(object->m_unlockType);

        if (unlockType > 0 && unlockType <= 15) {
            return RewardUnlockLayer::step3();
        }
    }
    auto jam = jm.getJamChestReward(m_chestType);

    if (jam <= 0) {
        return RewardUnlockLayer::step3();
    }

    jm.rewardJam(jam);

    jm.m_currencyLayerShouldRewardJam = true;
    jm.m_currencyLayerJamAmount = jam;

    RewardUnlockLayer::step3();

    jm.m_currencyLayerShouldRewardJam = false;

    FMODAudioEngine::get()->playEffectAdvanced("lid.mp3"_spr, 0.7f, 1.f, 0.9f, 1.f, false, false, 45, 1000, 0, 0, false, 0, false, false, 0, 0, 0.f, 0);

    auto nodes = m_rewardItem->m_rewardObjects->count();
    auto containers = std::array<CCNode*, 6>{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
    auto delay = nodes * 0.2f + 0.5f;

    for (int i = 1; i <= nodes; i++) {
        containers[i - 1] = m_mainLayer->getChildByIndex(-i);
    }

    auto lbl = CCLabelBMFont::create(fmt::format("+{}", jam).c_str(), "bigFont.fnt");
	lbl->setAnchorPoint({1.f, 0.5f});
	lbl->limitLabelWidth(40.f, 0.625f, 0.0001f);
    lbl->runAction(CCSequence::create(
		CCDelayTime::create(delay),
		CCFadeIn::create(0.3f),
		nullptr
	));
	
	auto icon = CCSprite::create("jam1.png"_spr);
	icon->setScale(0.375f);
	icon->setAnchorPoint({0.f, 0.5f});
	icon->setPositionX(5);
    icon->runAction(CCSequence::create(
		CCDelayTime::create(delay),
		CCFadeIn::create(0.3f),
		nullptr
	));
	
	auto jamContainer = CCNode::create();
	jamContainer->setID("jam-container"_spr);
	jamContainer->addChild(lbl);
	jamContainer->addChild(icon);
    jamContainer->setScale(3.f);
	jamContainer->runAction(CCSequence::create(
		CCHide::create(),
		CCDelayTime::create(delay),
		CCShow::create(),
		CCEaseBounceOut::create(CCScaleTo::create(0.3f, 1.f)),
		nullptr
	));
    
	m_mainLayer->addChild(jamContainer, 10);

    if (containers[0] && containers[1] && containers[2] && containers[3] && containers[4] && containers[5]) {
        jamContainer->setPosition({174, 100});
    } else if (containers[0] && containers[1] && containers[2] && containers[3] && containers[4]) {
        containers[0]->setPosition({174, 153});
        jamContainer->setPosition({393, 153});
    } else if (containers[0] && containers[1] && containers[2] && containers[3]) {
        jamContainer->setPosition({393, 153});
    } else if (containers[0] && containers[1] && containers[2]) {
        containers[2]->setPosition({174, 208});
        containers[1]->setPosition({247, 238});
        containers[0]->setPosition({320, 238});
        jamContainer->setPosition({393, 208});
    } else if (containers[0] && containers[1]) {
        containers[1]->setPosition({208.5f, 218});
        containers[0]->setPosition({283.5f, 238});
        jamContainer->setPosition({358.5f, 218});
    } else if (containers[0]) {
        containers[0]->setPosition({238.5f, 228});
        jamContainer->setPosition({328.5f, 228});
    }
}