#include "LevelCell.hpp"

#include "../Other/JamManager.hpp"
#include "../Other/Utils.hpp"

void ProLevelCell::addNormalCell() {
	auto& jm = JamManager::get();
	auto referenceLabel = m_mainLayer->getChildByID("orbs-label");

	if (!referenceLabel) {
		referenceLabel = m_mainLayer->getChildByID("likes-label");
	}
	
	if (!referenceLabel) {
		return;
	}
	
	auto scale = referenceLabel->getScale() / 0.4f;
	
	auto icon = CCSprite::create("jam1.png"_spr);
	icon->setID("jam-icon"_spr);
	icon->setScale(0.225f * scale);
	icon->setPosition({
		referenceLabel->getPositionX() + referenceLabel->getScaledContentWidth() + 14.f * scale,
		referenceLabel->getPositionY()
	});
	
	auto lbl = Utils::createJamProgressLabel(m_level, true);
	lbl->setID("jam-label"_spr);
	lbl->setScale(referenceLabel->getScale());
	lbl->setAnchorPoint({0.f, 0.5f});
	lbl->setPosition({
		icon->getPositionX() + icon->getScaledContentWidth() / 2.f + 2.f,
		icon->getPositionY()
	});
	
	m_mainLayer->addChild(icon);
	m_mainLayer->addChild(lbl);

	auto mostRight = lbl->getPositionX() + lbl->getScaledContentWidth();

	if (mostRight > 342.67f) {
		auto scale = 342.67f / mostRight / 1.31162f;

		referenceLabel->setScale(referenceLabel->getScale() * scale);
		referenceLabel->setPositionX(referenceLabel->getPositionX() - 3 * scale);

		lbl->setScale(lbl->getScale() * scale);
		lbl->setPositionX(lbl->getPositionX() - 14 * scale);

		icon->setPositionX(icon->getPositionX() - 14 * scale);
	}
}

void ProLevelCell::addDailyCell() {
	auto& jm = JamManager::get();
	auto diffContainer = m_mainLayer->getChildByID("difficulty-container");
	
	if (!diffContainer) {
		return;
	}
	
	auto starsLabel = diffContainer->getChildByID("stars-label");
	auto starsIcon = diffContainer->getChildByID("stars-icon");
	auto diamondsLabel = diffContainer->getChildByID("diamond-label");
	auto diamondsIcon = diffContainer->getChildByID("diamond-icon");
	
	if (!starsLabel || !starsIcon || !diamondsLabel || !diamondsIcon) {
		return;
	}
	
	auto lbl = Utils::createJamProgressLabel(m_level);
	lbl->setAnchorPoint({1.f, 0.5f});
	lbl->setPosition({lbl->getContentWidth(), 10});
	
	auto icon = CCSprite::create("jam1.png"_spr);
	icon->setScale(0.55f);
	icon->setPosition({lbl->getContentWidth() + icon->getScaledContentWidth() / 2.f + 8.5f, 10});
	
	auto container = CCNode::create();
	container->setAnchorPoint({0.5f, 0.5f});
	container->setContentSize({icon->getPositionX() + icon->getScaledContentWidth() / 2.f + 4.5f, 20});
	// container->setScale(27.f / container->getContentWidth());
	container->setScale(0.35f);
	container->addChild(lbl);
	container->addChild(icon);
	
	diffContainer->addChild(container);
	
	auto coinIcon1 = diffContainer->getChildByID("coin-icon-1");
	auto coinIcon2 = diffContainer->getChildByID("coin-icon-2");
	auto coinIcon3 = diffContainer->getChildByID("coin-icon-3");
	auto scale = 1.f;
	
	if (diamondsLabel && !coinIcon1) {
		scale = 0.9f;
	} else if (diamondsLabel && coinIcon1) {
		scale = 0.81f;
	}
	
	auto yDiff = 14.5f * scale;
	auto xDiff = diamondsLabel->getPositionX() - diamondsLabel->getScaledContentWidth();
	
	container->setPosition({
		xDiff + (diamondsIcon->getPositionX() - xDiff + diamondsIcon->getScaledContentWidth() / 2.f) / 2.f,
		starsLabel->getPositionY() - yDiff
	});
	
	icon->setScale(icon->getScale() * scale);
	lbl->setScale(scale);
	container->setPositionY(starsLabel->getPositionY() - yDiff);				

	starsLabel->setScale(starsLabel->getScale() * scale);
	starsIcon->setScale(starsIcon->getScale() * scale);
			
	diamondsLabel->setScale(diamondsLabel->getScale() * scale);
	diamondsLabel->setPositionY(container->getPositionY() - yDiff);
			
	if (auto diamondsIcon = diffContainer->getChildByID("diamond-icon")) {
		diamondsIcon->setScale(diamondsIcon->getScale() * scale);
		diamondsIcon->setPositionY(container->getPositionY() - yDiff);
	}
	
	if (coinIcon1) {
		coinIcon1->setScale(coinIcon1->getScale() * scale);
		coinIcon1->setPositionY(diamondsLabel->getPositionY() - yDiff);
	}
	
	if (coinIcon2) {
		coinIcon2->setScale(coinIcon2->getScale() * scale);
		coinIcon2->setPositionY(diamondsLabel->getPositionY() - yDiff);
	}
			
	if (coinIcon3) {
		coinIcon3->setScale(coinIcon3->getScale() * scale);
		coinIcon3->setPositionY(diamondsLabel->getPositionY() - yDiff);
	}
	
	if (coinIcon2 && !coinIcon3) {
		coinIcon1->setPositionX(coinIcon1->getPositionX() + 1.7f);
		coinIcon2->setPositionX(coinIcon3->getPositionX() - 1.7f);
	}
	else if (coinIcon2 && coinIcon3) {
		coinIcon1->setPositionX(coinIcon1->getPositionX() + 1.7f);
		coinIcon3->setPositionX(coinIcon3->getPositionX() - 1.7f);
	}
}

void ProLevelCell::loadCustomLevelCell() {
	LevelCell::loadCustomLevelCell();

	if (!JamManager::get().canLevelHaveJam(m_level)) {
		return;
	}
	
	if (m_level->m_listPosition != 0) {
		return;
	}
	
	if (m_level->m_dailyID.value() <= 0) {
		addNormalCell();
	} else {
		addDailyCell();
	}
}