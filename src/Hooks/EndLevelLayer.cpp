#include "EndLevelLayer.hpp"
#include "PlayLayer.hpp"

#include "../Other/JamManager.hpp"

void resetScaleAction(CCNode* node, float delay, float scale) {
	node->stopAllActions();
	node->setScale(3.f * scale);
	node->runAction(CCSequence::create(
		CCHide::create(),
		CCDelayTime::create(delay),
		CCShow::create(),
		CCEaseBounceOut::create(CCScaleTo::create(0.3f, scale)),
		nullptr
	));
}

void runFadeInAction(CCNode* node, float delay) {
	node->runAction(CCSequence::create(
		CCDelayTime::create(delay),
		CCFadeIn::create(0.3f),
		nullptr
	));
}

void runContainerAction(CCNode* container, CCNode* lbl, CCNode* icon, float delay, float scale) {
	resetScaleAction(container, delay, scale);
	runFadeInAction(lbl, delay);
	runFadeInAction(icon, delay);
}

void ProEndLevelLayer::addRewardLayer() {
	auto& jm = JamManager::get();
	auto f = m_fields.self();

	if (!f->m_jamContainer) {
		return;
	}

	jm.m_currencyLayerShouldRewardJam = true;
	jm.m_currencyLayerJamAmount = m_fields->m_jamReward;

	auto rewardLayer = CurrencyRewardLayer::create(
		0,
		0,
		0,
		0,
		CurrencySpriteType::Icon,
		0,
		CurrencySpriteType::Icon,
		0,
		f->m_jamContainer->convertToWorldSpaceAR({0, 0}),
		CurrencyRewardType::Default,
		0.f,
		0.9f
	);
	rewardLayer->setID("jam-reward-layer"_spr);

	FMODAudioEngine::get()->playEffect("pop.mp3"_spr, 1.f, 1.f, 0.2f);
	FMODAudioEngine::get()->playEffect("magicExplosion.ogg", 1.15f, 1.f, 0.7f);

	addChild(rewardLayer);

	jm.m_currencyLayerShouldRewardJam = false;
}

void ProEndLevelLayer::showLayer(bool p0) {
	EndLevelLayer::showLayer(p0);
	
	auto f = m_fields.self();

	f->m_jamReward = static_cast<ProPlayLayer*>(m_playLayer)->m_fields->m_jamReward;

	if (f->m_jamReward <= 0) {
		return;
	}
	
	auto starContainer = m_mainLayer->getChildByID("star-container");
	auto orbContainer = m_mainLayer->getChildByID("orb-container");
	auto diamondContainer = m_mainLayer->getChildByID("diamond-container");
		
	auto lbl = CCLabelBMFont::create(fmt::format("+{}", f->m_jamReward).c_str(), "bigFont.fnt");
	lbl->setScale(0.625f);
	lbl->setAnchorPoint({1.f, 0.5f});
	lbl->setPositionX(-5);
	
	auto icon = CCSprite::create("jam1.png"_spr);
	icon->setScale(0.375f);
	icon->setAnchorPoint({0.f, 0.5f});
	icon->setPositionX(5);
	
	f->m_jamContainer = CCNode::create();
	f->m_jamContainer->setID("jam-container"_spr);
	f->m_jamContainer->setPosition({403.5f, 105.f});
	f->m_jamContainer->addChild(lbl);
	f->m_jamContainer->addChild(icon);
	
	m_mainLayer->addChild(f->m_jamContainer, 10);
	
	auto delay = 0.f;
	auto lidDelay = 0.f;
	
	if (m_coinsToAnimate && !GameManager::get()->getGameVariable("0168")) {
		delay = m_coinsToAnimate->count() * 0.35f + 0.7f;
		lidDelay = delay - 0.7f;
	}

	runAction(CCSequence::create(
		CCDelayTime::create(lidDelay),
		CallFuncExt::create([] {
			FMODAudioEngine::get()->playEffect("lid.mp3"_spr, 0.7f, 1.f, 0.4f);
			FMODAudioEngine::get()->playEffect("magicExplosion.ogg", 1.4f, 1.f, 0.24f);
		}),
		nullptr
	));
	
	auto runFinished = [&](CCNode* node, SEL_CallFunc func) {
		node->runAction(CCSequence::create(
			CCDelayTime::create(delay + 0.2f),
			CCCallFunc::create(this, func),
			nullptr
		));
	};

	if (!starContainer && !orbContainer && !diamondContainer) {
		f->m_jamContainer->setPosition({403.5f, 178.f});
		
		runContainerAction(f->m_jamContainer, lbl, icon, delay, 1.f);

		f->m_jamContainer->runAction(CCSequence::create(
			CCDelayTime::create(delay + 0.2f),
			CCCallFunc::create(this, callfunc_selector(ProEndLevelLayer::addRewardLayer)),
			nullptr
		));
	}
	else {
		int count = (starContainer != nullptr) + (orbContainer != nullptr) + (diamondContainer != nullptr);
		float scale = count == 3 ? 0.67f : (count == 2 ? 0.77f : 1.f);

		f->m_jamContainer->setScale(scale);
		if (starContainer) starContainer->setScale(scale);
		if (orbContainer) orbContainer->setScale(scale);
		if (diamondContainer) diamondContainer->setScale(scale);

		auto top = starContainer ? starContainer : (orbContainer ? orbContainer : diamondContainer);

		if (starContainer) {
			f->m_jamContainer->setPosition({
				starContainer->getPositionX(),
				starContainer->getPositionY() - 38.f * scale
			});
		}
		else {
			f->m_jamContainer->setPosition(top->getPosition());
		}

		auto prev = f->m_jamContainer;
		for (auto node : { orbContainer, diamondContainer }) {
			if (node && node != top) {
				node->setPositionY(prev->getPositionY() - 38.f * scale);
				prev = node;
			}
		}

		for (auto node : { starContainer, orbContainer, diamondContainer }) {
			if (node) resetScaleAction(node, delay, scale);
		}

		runContainerAction(f->m_jamContainer, lbl, icon, delay, scale);

		if (starContainer) runFinished(starContainer, callfunc_selector(EndLevelLayer::starEnterFinished));
		if (orbContainer) runFinished(orbContainer, callfunc_selector(EndLevelLayer::currencyEnterFinished));
		if (diamondContainer) runFinished(diamondContainer, callfunc_selector(EndLevelLayer::diamondEnterFinished));
	}
}

void ProEndLevelLayer::playEndEffect() {
	auto f = m_fields.self();

	if (f->m_jamReward <= 0) {
		return EndLevelLayer::playEndEffect();
	}

    auto& jm = JamManager::get();

	jm.m_currencyLayerShouldRewardJam = true;
	jm.m_currencyLayerJamAmount = f->m_jamReward;

	FMODAudioEngine::get()->playEffect("pop.mp3"_spr, 1.f, 1.f, 1.2f);
	FMODAudioEngine::get()->playEffect("magicExplosion.ogg", 1.15f, 1.f, 0.7f);

	EndLevelLayer::playEndEffect();

	jm.m_currencyLayerShouldRewardJam = false;
}