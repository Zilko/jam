#include "SecretRewardsLayer.hpp"

#include "../Other/JamManager.hpp"
#include "../UI/JamMarketLayer.hpp"

void ProSecretRewardsLayer::onJamMarket(CCObject*) {
    CCDirector::get()->replaceScene(CCTransitionMoveInT::create(0.5f, JamMarketLayer::scene()));
}

void ProSecretRewardsLayer::onSelectItem(CCObject* sender) {
    JamManager::get().m_nextRewardUnlockLayerCanRewardJam = true;
    SecretRewardsLayer::onSelectItem(sender);
}

bool ProSecretRewardsLayer::init(bool p0) {
    if (!SecretRewardsLayer::init(p0)) {
        return false;
    }

    auto pages = m_mainScrollLayer->getChildByID("pages");

    if (!pages) {
        return true;
    }

    auto page5 = pages->getChildByID("page5");

    if (!page5) {
        return true;
    }

    auto menu = page5->getChildByID("store-menu");

    if (!menu) {
        return true;
    }


    auto spr = CCSprite::create("rope-1.png"_spr);

    for (int i = 0; i < 6; i++) {
        auto subSpr = CCSprite::create("rope-2.png"_spr);
        subSpr->setPosition(ccp(22 - i, 76 + (subSpr->getContentHeight() - 1) * i));

        spr->addChild(subSpr);
    }

    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ProSecretRewardsLayer::onJamMarket));
    btn->setID("jam-market-button"_spr);
    btn->m_fSizeMult = 1.5f;
    btn->m_startPosition = btn->getNormalImage()->getPosition();
    btn->m_animationType = MenuAnimationType::Move;
    btn->m_offset = CCPoint{0, -15};
    btn->m_duration = 0.3f;
    btn->m_unselectedDuration = 0.3f;

    menu->addChild(btn);
    menu->updateLayout();

    return true;
}