#include "PlayLayer.hpp"

#include "../Other/JamManager.hpp"

CCPoint ProPlayLayer::getPlayerScreenPos() {
    CCPoint pos = m_player1->convertToWorldSpaceAR({0, 0});
    CCPoint cameraCenter = m_cameraObb2->m_center;

    float angle = CC_DEGREES_TO_RADIANS(-m_gameState.m_cameraAngle);
    float cos = cosf(angle);
    float isn = sinf(angle);
    float offsetX = pos.x - cameraCenter.x;
    float offsetY = pos.y - cameraCenter.y;
    float rotatedX = offsetX * cos - offsetY * isn;
    float rotatedY = offsetX * isn + offsetY * cos;

    pos = ccp(cameraCenter.x + rotatedX, cameraCenter.y + rotatedY);

    return pos;
}

void ProPlayLayer::addRewardLayer() {
    auto rewardLayer = CurrencyRewardLayer::create(
        0,
        0,
        0,
        0,
        CurrencySpriteType::Icon,
        0,
        CurrencySpriteType::Icon,
        0,
        getPlayerScreenPos(),
        CurrencyRewardType::Default,
        0.f,
        0.9f
    );
    rewardLayer->setID("reward-layer"_spr);
    
    addChild(rewardLayer);

    CCLayerColor* overlay = CCLayerColor::create({0, 0, 0, 0});
    overlay->setID("overlay-layer"_spr);
    overlay->runAction(CCSequence::create(
        CCFadeTo::create(0.3f, 100),
        CCDelayTime::create(1.3f + 0.3f),
        CCFadeTo::create(0.4f, 0),
        nullptr
    ));

    addChild(overlay, -1);
}

void ProPlayLayer::modifyJamReward(int jam) {
    auto& jm = JamManager::get();
    auto layer = getChildByType<CurrencyRewardLayer>(0);
    auto children = getChildrenExt<CCNode*>();

    CCNode* newBestContainer = nullptr;

    for (auto i = children.size() - 1; i >= 0; i--) {
        auto child = children[i];

        if (
            child != layer
            && child->isVisible()
            && child->getZOrder() == 100
            && child->getID().empty()
            && static_cast<int>(child->getPositionX()) == static_cast<int>(getContentWidth() / 2.f)
            && child->getChildByType<CCLabelBMFont>(0)
        ) {
            newBestContainer = child;
            break;
        }
    }

    if (!layer) {
        addRewardLayer();
    }

    if (!newBestContainer) {
        return;
    }

    auto referenceLabel = newBestContainer->getChildByType<CCLabelBMFont>(-1);
    
    if (!referenceLabel) {
        return;
    }

    auto yPos = referenceLabel->getPositionY();

    if (referenceLabel->getAnchorPoint().y > 0.5f) {
        yPos -= referenceLabel->getScaledContentHeight() + 20.f;
    } else {
        yPos -= 27.f;
    }

    auto lbl = CCLabelBMFont::create(fmt::format("+{}", jam).c_str(), "bigFont.fnt");
    lbl->setScale(0.6f);
    lbl->setAnchorPoint({1.f, 0.5f});
    lbl->setPositionX(-3);

    auto icon = CCSprite::create("jam1.png"_spr);
    icon->setScale(0.3f);
    icon->setAnchorPoint({0.f, 0.5f});
    icon->setPositionX(3);

    auto container = CCNode::create();
    container->setPosition({0, yPos});
    container->addChild(lbl);
    container->addChild(icon);

    newBestContainer->addChild(container);
}

void ProPlayLayer::addJamReward(int jam) {
    auto& jm = JamManager::get();
	auto winSize = CCDirector::get()->getWinSize();
	
	auto scaleContainer = CCNode::create();
    scaleContainer->setID("jam-reward-container"_spr);
    scaleContainer->setScale(0.01f);
    scaleContainer->setPosition({winSize.width * 0.5f, winSize.height * 0.5f + 30.f});
    
    addChild(scaleContainer);

    auto lbl = CCLabelBMFont::create("New Reward!", "goldFont.fnt");
    lbl->setScale(1.2f);
    lbl->setAnchorPoint({0.5f, 0.f});
    lbl->setPosition({0.f, 5.f});

    scaleContainer->addChild(lbl);

    lbl = CCLabelBMFont::create(fmt::format("{}%", getCurrentPercentInt()).c_str(), "bigFont.fnt");
    lbl->setID("percent-label");
    lbl->setAnchorPoint({0.5f, 1.f});
    lbl->setPosition({0.f, -4.f});
    lbl->setScale(1.1f);

    scaleContainer->addChild(lbl);

    lbl = CCLabelBMFont::create(fmt::format("+{}", jam).c_str(), "bigFont.fnt");
    lbl->setID("orbs-label");
    lbl->setScale(0.6f);
    scaleContainer->addChild(lbl);

    auto spr = CCSprite::create("jam1.png"_spr);
    spr->setScale(0.3f);
    spr->setID("jam-sprite"_spr);
    spr->setAnchorPoint(ccp(0.f, 0.5f));
    scaleContainer->addChild(spr);

    float offset = -(lbl->getContentWidth() * lbl->getScale() + 5.f + spr->getContentWidth() * spr->getScale()) / 2.f + lbl->getContentWidth() * lbl->getScale() / 2.f;

    lbl->setPosition({offset - 5.f, -62.f});
    spr->setPosition(lbl->getPosition() + ccp(lbl->getScaledContentSize().width * 0.5f + 5.f, -1.f));

    scaleContainer->runAction(CCSequence::create(
        CCEaseElasticOut::create(CCScaleTo::create(0.4f, 1.f), 0.6f),
        CCDelayTime::create(1.3f),
        CCEaseIn::create(CCScaleTo::create(0.2f, 0.01f, 0.01f), 2.f),
        CCRemoveSelf::create(),
        nullptr
    ));

    addRewardLayer();
}

void ProPlayLayer::showNewBest(bool newReward, int orbs, int diamonds, bool demonKey, bool noRetry, bool noTitle) {
    m_fields->m_didShowNewBest = true;

    PlayLayer::showNewBest(newReward, orbs, diamonds, demonKey, noRetry, noTitle);
}

void ProPlayLayer::destroyPlayer(PlayerObject* player, GameObject* object) {
    auto& jm = JamManager::get();

	if (
		(player != m_player1 && player != m_player2)
        || !player
        || player->m_isDead
		|| object == m_anticheatSpike
		|| m_isPracticeMode
		|| m_isTestMode
		|| m_isPlatformer
        || !jm.canLevelHaveJam(m_level)
	) {
		return PlayLayer::destroyPlayer(player, object);
	}
    
    auto progress = getCurrentPercent() / 100.f;
    auto prevProgress = jm.getLevelJamProgress(m_level);

    if (progress <= prevProgress) {
        return PlayLayer::destroyPlayer(player, object);;
    }

    jm.setLevelJamProgress(m_level, progress);

    if (progress < 0.5f) {
        return PlayLayer::destroyPlayer(player, object);;
    }

    auto jam = jm.getLevelJamForProgress(m_level, progress) - jm.getLevelJamForProgress(m_level, prevProgress);

    if (jam <= 0) {
        return PlayLayer::destroyPlayer(player, object);;
    }

    auto f = m_fields.self();

    jm.rewardJam(jam);

    jm.m_currencyLayerShouldRewardJam = true;
    jm.m_currencyLayerJamAmount = jam;

    f->m_didShowNewBest = false;

    PlayLayer::destroyPlayer(player, object);
		
	if (f->m_didShowNewBest) {
		modifyJamReward(jam);
	} else {
		addJamReward(jam);
	}

    f->m_didShowNewBest = false;

    jm.m_currencyLayerShouldRewardJam = false;
}

void ProPlayLayer::resetLevel() {
    PlayLayer::resetLevel();
    m_fields->m_jamReward = 0;
}

void ProPlayLayer::levelComplete() {
    auto& jm = JamManager::get();
	
	if (
        !jm.canLevelHaveJam(m_level)
        || m_isPracticeMode
        || m_isTestMode
    ) {
		return PlayLayer::levelComplete();
	}

	auto f = m_fields.self();

	f->m_jamReward = jm.getTotalLevelJam(m_level) - jm.getLevelJamForProgress(m_level, jm.getLevelJamProgress(m_level));

	jm.rewardJam(f->m_jamReward);
	jm.setLevelJamProgress(m_level, 1.f);
    
    PlayLayer::levelComplete();
}