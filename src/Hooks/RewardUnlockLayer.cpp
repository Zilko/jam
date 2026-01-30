#include "RewardUnlockLayer.hpp"

#include "../Other/JamManager.hpp"
#include "../Other/Utils.hpp"

ProRewardUnlockLayer::Fields::~Fields() {
    JamManager::get().m_currencyLayerShouldRewardJam = false;
}

bool ProRewardUnlockLayer::init(int p0, RewardsPage* p1) {
    if (!RewardUnlockLayer::init(p0, p1)) {
        return false;
    }
    
    if (JamManager::get().m_nextRewardUnlockLayerCanRewardJam) {
        schedule(schedule_selector(ProRewardUnlockLayer::updateLookout), 0.f);
    }

    return true;
}

void ProRewardUnlockLayer::calculateJam() {

    auto& jm = JamManager::get();

    if (
        !m_rewardItem->m_rewardObjects
        || m_rewardItem->m_rewardObjects->count() <= 0
        || m_rewardItem->m_rewardObjects->count() >= 7
    ) {
        return;
    }

    jm.m_nextRewardUnlockLayerCanRewardJam = false;

    if (m_rewardItem->m_rewardObjects->count() == 1) {
        auto object = static_cast<GJRewardObject*>(m_rewardItem->m_rewardObjects->firstObject());
        auto unlockType = static_cast<int>(object->m_unlockType);

        if (unlockType > 0 && unlockType <= 15) {
            return;
        }
    }

    auto jam = jm.getJamChestReward(m_chestType);

    if (jam <= 0) {
        return;
    }

    jm.rewardJam(jam);

    jm.m_currencyLayerShouldRewardJam = true;
    jm.m_currencyLayerJamAmount = jam;
}

void ProRewardUnlockLayer::updateLookout(float) {
    if (!m_rewardItem) {
        return;
    }

    auto& jm = JamManager::get();
    auto f = m_fields.self();
    auto currencyLayer = getChildByType<CurrencyRewardLayer>(0);

    if (!f->m_foundRewardItem) {
        calculateJam();

        if (!jm.m_currencyLayerShouldRewardJam || jm.m_currencyLayerJamAmount <= 0) {
            f->m_doLookout = false;
            unschedule(schedule_selector(ProRewardUnlockLayer::updateLookout));
            return;
        }

        f->m_foundRewardItem = true;
        f->m_doLookout = true;

        if (currencyLayer) {
            currencyLayer->createObjectsFull(
                CurrencySpriteType::Star, 
                jm.m_currencyLayerJamAmount,
                nullptr,
                CCDirector::get()->getWinSize() / 2.f + ccp(0, 9),
                0.f
            );

            Utils::modifyRewardLayer(currencyLayer, CurrencySpriteType::Star);

            return;
        }
    }

    if (f->m_doLookout && currencyLayer) {
        f->m_doLookout = false;
        unschedule(schedule_selector(ProRewardUnlockLayer::updateLookout));
        realStep3();
    }
}

void ProRewardUnlockLayer::realStep3() {
    auto& jm = JamManager::get();

    jm.m_currencyLayerShouldRewardJam = false;

    FMODAudioEngine::get()->playEffectAdvanced("lid.mp3"_spr, 0.7f, 1.f, 0.9f, 1.f, false, false, 45, 1000, 0, 0, false, 0, false, false, 0, 0, 0.f, 0);

    auto nodes = m_rewardItem->m_rewardObjects->count();
    auto containers = std::array<CCNode*, 6>{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
    auto delay = nodes * 0.2f + 0.5f;

    for (int i = 1; i <= nodes; i++) {
        containers[i - 1] = m_mainLayer->getChildByIndex(-i);
    }

    auto lbl = CCLabelBMFont::create(fmt::format("+{}", jm.m_currencyLayerJamAmount).c_str(), "bigFont.fnt");
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
    jamContainer->setVisible(false);
	jamContainer->runAction(CCSequence::create(
		CCDelayTime::create(delay),
		CCShow::create(),
		CCEaseBounceOut::create(CCScaleTo::create(0.3f, 1.f)),
		nullptr
	));
    
	m_mainLayer->addChild(jamContainer, 10);

    auto center = CCDirector::get()->getWinSize() / 2.f;

    // lol
    if (containers[0] && containers[1] && containers[2] && containers[3] && containers[4] && containers[5]) {
        jamContainer->setPosition(ccp(174, 100) - ccp(283.5f, 160) + center);
    } else if (containers[0] && containers[1] && containers[2] && containers[3] && containers[4]) {
        containers[0]->setPosition(ccp(174, 153) - ccp(283.5f, 160) + center);
        jamContainer->setPosition(ccp(393, 153) - ccp(283.5f, 160) + center);
    } else if (containers[0] && containers[1] && containers[2] && containers[3]) {
        jamContainer->setPosition(ccp(393, 153) - ccp(283.5f, 160) + center);
    } else if (containers[0] && containers[1] && containers[2]) {
        containers[2]->setPosition(ccp(174, 208) - ccp(283.5f, 160) + center);
        containers[1]->setPosition(ccp(247, 238) - ccp(283.5f, 160) + center);
        containers[0]->setPosition(ccp(320, 238) - ccp(283.5f, 160) + center);
        jamContainer->setPosition(ccp(393, 208) - ccp(283.5f, 160) + center);
    } else if (containers[0] && containers[1]) {
        containers[1]->setPosition(ccp(208.5f, 218) - ccp(283.5f, 160) + center);
        containers[0]->setPosition(ccp(283.5f, 238) - ccp(283.5f, 160) + center);
        jamContainer->setPosition(ccp(358.5f, 218) - ccp(283.5f, 160) + center);
    } else if (containers[0]) {
        containers[0]->setPosition(ccp(238.5f, 228) - ccp(283.5f, 160) + center);
        jamContainer->setPosition(ccp(328.5f, 228) - ccp(283.5f, 160) + center);
    }
}