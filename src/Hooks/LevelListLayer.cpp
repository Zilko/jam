#include "LevelListLayer.hpp"

#include "../Other/JamManager.hpp"

void ProLevelListLayer::onClaimReward(CCObject* sender) {
    auto f = m_fields.self();

    if (f->m_jamReward <= 0) {
        return;
    }

    auto& jm = JamManager::get();

    jm.rewardJam(f->m_jamReward);

    jm.m_currencyLayerShouldRewardJam = true;
    jm.m_currencyLayerJamAmount = f->m_jamReward;

    LevelListLayer::onClaimReward(sender);

    jm.m_challengesPageShouldRewardJam = false;

    FMODAudioEngine::get()->playEffectAdvanced("lid.mp3"_spr, 0.7f, 1.f, 0.75f, 1.f, false, false, 45, 1000, 0, 0, false, 0, false, false, 0, 0, 0.f, 0);
}

bool ProLevelListLayer::init(GJLevelList* p0) {
    if (!LevelListLayer::init(p0)) {
        return false;
    }

    auto f = m_fields.self();

    f->m_jamReward = static_cast<int>(p0->m_diamonds * 1.7f) / 5 * 5;

    if (f->m_jamReward <= 0) {
        return true;
    }

    auto menu = getChildByID("left-side-menu");

    if (!menu) {
        return true;
    }

    auto btn = static_cast<CCMenuItemSpriteExtra*>(menu->getChildByID("claim-button"));

    if (!btn) {
        return true;
    }

    auto container = btn->getNormalImage()->getChildByIndex(0);

    if (!container) {
        return true;
    }

    auto diamondIcon = container->getChildByType<CCSprite>(-1);
    auto diamondLabel = container->getChildByType<CCLabelBMFont>(-1);

    if (!diamondIcon || !diamondLabel) {
        return true;
    }

    diamondLabel->setScale(diamondLabel->getScale() * 0.87);
    diamondLabel->setPositionX(diamondLabel->getPositionX() + 1);
    diamondIcon->setScale(diamondIcon->getScale() * 0.87f);
    diamondIcon->setPositionX(diamondIcon->getPositionX() - 1);

    auto lbl = CCLabelBMFont::create(std::to_string(f->m_jamReward).c_str(), "bigFont.fnt");
    lbl->setID("jam-label"_spr);
    lbl->limitLabelWidth(diamondLabel->getScaledContentWidth() + 3.f, diamondLabel->getScale(), 0.0001f);
    lbl->setAnchorPoint(diamondLabel->getAnchorPoint());
    lbl->setPosition(diamondLabel->getPosition() + ccp(0, -14.3f));

    container->addChild(lbl);

    auto icon = CCSprite::create("jam1.png"_spr);
    icon->setID("jam-icon"_spr);
    icon->setScale(diamondIcon->getScale() * 0.21f);
    icon->setPosition(diamondIcon->getPosition() + ccp(0, -14.3f));

    container->addChild(icon);

    return true;
}