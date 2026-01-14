#include "ChallengeNode.hpp"

#include "../Other/JamManager.hpp"

void ProChallengeNode::onClaimReward(CCObject* sender) {
    auto& jm = JamManager::get();
    auto f = m_fields.self();

    jm.rewardJam(f->m_jamReward);

    if (m_challengesPage->m_currencyRewardLayer) {
        jm.m_challengesPageShouldRewardJam = true;
        jm.m_challengesPageJamAmount = f->m_jamReward;
    } else {
        jm.m_currencyLayerShouldRewardJam = true;
        jm.m_currencyLayerJamAmount = f->m_jamReward;
    }

    ChallengeNode::onClaimReward(sender);

    jm.m_challengesPageShouldRewardJam = false;
    jm.m_currencyLayerShouldRewardJam = false;
}

bool ProChallengeNode::init(GJChallengeItem* p0, ChallengesPage* p1, bool p2) {
    if (!ChallengeNode::init(p0, p1, p2)) {
        return false;
    }

    auto f = m_fields.self();

    if (p0) {
        f->m_jamReward = p0->m_reward.value() / 5.f * 2;
    }

    auto rewardLabel = getChildByID("reward-label");
    auto rewardSprite = getChildByID("reward-sprite");

    if (!rewardLabel || !rewardSprite) {
        return true;
    }

    rewardLabel->setPosition(rewardLabel->getPosition() + ccp(5, 12));
    rewardLabel->setScale(rewardLabel->getScale() * 0.75f);
    rewardSprite->setScale(rewardSprite->getScale() * 0.63f);
    rewardSprite->setPositionY(rewardSprite->getPositionY() + 12);

    auto lbl = CCLabelBMFont::create(fmt::format("{}x", f->m_jamReward).c_str(), "bigFont.fnt");
    lbl->setScale(rewardLabel->getScale());
    lbl->setAnchorPoint(rewardLabel->getAnchorPoint());
    lbl->setPosition(rewardLabel->getPosition() - ccp(0, 24));

    addChild(lbl);

    auto icon = CCSprite::create("jam1.png"_spr);
    icon->setScale(rewardSprite->getScale() * 0.5f);
    icon->setPosition(rewardSprite->getPosition() - ccp(0, 24));

    addChild(icon);

    return true;
}