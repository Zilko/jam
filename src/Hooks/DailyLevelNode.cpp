#include "DailyLevelNode.hpp"

#include "../Other/JamManager.hpp"

ProDailyLevelNode::Fields::~Fields() {
    JamManager::get().m_nextRewardUnlockLayerCanRewardJam = false;
}

void ProDailyLevelNode::onClaimReward(CCObject* sender) {
    auto& jm = JamManager::get();
    auto f = m_fields.self();

    if (f->m_isChest) {
        jm.m_nextRewardUnlockLayerCanRewardJam = true;
    } else {
        if (f->m_jamReward <= 0) {
            return DailyLevelNode::onClaimReward(sender);
        }

        jm.rewardJam(f->m_jamReward);

        jm.m_currencyLayerShouldRewardJam = true;
        jm.m_currencyLayerJamAmount = f->m_jamReward;

        FMODAudioEngine::get()->playEffectAdvanced("lid.mp3"_spr, 0.7f, 1.f, 0.75f, 1.f, false, false, 45, 1000, 0, 0, false, 0, false, false, 0, 0, 0.f, 0);
    }

    DailyLevelNode::onClaimReward(sender);

    jm.m_currencyLayerShouldRewardJam = false;
}

bool ProDailyLevelNode::init(GJGameLevel* p0, DailyLevelPage* p1, bool p2) {
    if (!DailyLevelNode::init(p0, p1, p2)) {
        return false;
    }

    auto bonusLabel = getChildByID("bonus-number-label");

    if (!bonusLabel) {
        m_fields->m_isChest = true;
        return true;
    }

    auto bonusSprite = getChildByID("bonus-sprite");

    if (!bonusSprite) {
        return true;
    }

    auto f = m_fields.self();

    f->m_jamReward = GameStatsManager::get()->getBonusDiamonds(m_level->m_stars.value()) * 6;

    if (f->m_jamReward <= 0) {
        return true;
    }

    bonusLabel->setPosition(bonusLabel->getPosition() + ccp(-3, 7.7f));
    bonusLabel->setScale(bonusLabel->getScale() * 0.75f);
    bonusSprite->setScale(bonusSprite->getScale() * 0.4f);
    bonusSprite->setPosition(bonusSprite->getPosition() + ccp(-9, 7.7f));

    auto lbl = CCLabelBMFont::create(fmt::format("{}x", f->m_jamReward).c_str(), "bigFont.fnt");
    lbl->setID("jam-label"_spr);
    lbl->limitLabelWidth(bonusLabel->getScaledContentWidth() + 2.f, bonusLabel->getScale(), 0.0001f);
    lbl->setAnchorPoint(bonusLabel->getAnchorPoint());
    lbl->setPosition(bonusLabel->getPosition() - ccp(0, 17.4f));

    addChild(lbl);

    auto icon = CCSprite::create("jam1.png"_spr);
    icon->setID("jam-icon"_spr);
    icon->setScale(bonusSprite->getScale() * 0.6f);
    icon->setPosition(bonusSprite->getPosition() - ccp(0, 17.4f));

    addChild(icon);

    return true;
}