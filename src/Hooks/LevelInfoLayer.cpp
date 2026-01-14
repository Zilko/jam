#include "LevelInfoLayer.hpp"

#include "../Other/JamManager.hpp"
#include "../Other/Utils.hpp"

float ProLevelInfoLayer::getNodeScale(CCNode* node) {
	auto f = m_fields.self();
	
	if (f->m_storedScales.contains(node)) {
		return f->m_storedScales.at(node);
	}
	
	auto scale = node->getScale() * 0.923f;
	
	f->m_storedScales[node] = scale;
	
	return scale;
}

void ProLevelInfoLayer::updateJam() {
	auto& jm = JamManager::get();
	
	if (!jm.canLevelHaveJam(m_level)) {
		return;
	}

	auto starsLabel = getChildByID("stars-label");
	auto starsIcon = getChildByID("stars-icon");
	
	if (!starsLabel || !starsIcon || !starsLabel->isVisible()) {
		return;
	}

	auto f = m_fields.self();
	auto diamondsLabel = getChildByID("diamond-label");
	auto diamondsIcon = getChildByID("diamond-icon");
	auto coinIcon1 = getChildByID("coin-icon-1");
	auto x = starsLabel->getPositionX() - starsLabel->getScaledContentWidth();
	
	if (!f->m_jamLabel) {
		f->m_jamLabel = Utils::createJamProgressLabel(m_level);
		f->m_jamLabel->setAnchorPoint({1.f, 0.5f});
		f->m_jamLabel->setPosition({f->m_jamLabel->getContentWidth(), 10});

		f->m_jamIcon = CCSprite::create("jam1.png"_spr);
		f->m_jamIcon->setScale(0.55f);
		f->m_jamIcon->setPosition({f->m_jamLabel->getContentWidth() + f->m_jamIcon->getScaledContentWidth() / 2.f + 8.5f, 10});
		
		f->m_jamContainer = CCNode::create();
		f->m_jamContainer->setID("jam-container"_spr);
		f->m_jamContainer->setAnchorPoint({0.5f, 0.5f});
		f->m_jamContainer->setContentSize({f->m_jamIcon->getPositionX() + f->m_jamIcon->getScaledContentWidth() / 2.f + 4.5f, 20});
		f->m_jamContainer->setScale(0.35f);
		f->m_jamContainer->addChild(f->m_jamLabel);
		f->m_jamContainer->addChild(f->m_jamIcon);
		f->m_jamContainer->setPosition({
			x + (starsIcon->getPositionX() - x + starsIcon->getScaledContentWidth() / 2.f) / 2.f + 1.f,
			starsLabel->getPositionY() - 14.5f
		});
		
		addChild(f->m_jamContainer);
	}
	
	if (diamondsLabel && diamondsIcon && f->m_firstTime) {
		x = diamondsLabel->getPositionX() - diamondsLabel->getScaledContentWidth();
		f->m_jamContainer->setPositionX(x + (diamondsIcon->getPositionX() - x + diamondsIcon->getScaledContentWidth() / 2.f) / 2.f);
	}
	
	if (diamondsLabel && !coinIcon1) {
		diamondsLabel->setPositionY(f->m_jamContainer->getPositionY() - 14.5f);
				
		if (diamondsIcon) {
			diamondsIcon->setPositionY(f->m_jamContainer->getPositionY() - 14.5f);
		}
	}
	else if (diamondsLabel && coinIcon1) {
		f->m_jamIcon->setScale(getNodeScale(f->m_jamIcon));
		f->m_jamLabel->setScale(getNodeScale(f->m_jamLabel));
		
		f->m_jamContainer->setPositionY(starsLabel->getPositionY() - 13.38f);				

		starsLabel->setScale(getNodeScale(starsLabel));
		starsIcon->setScale(getNodeScale(starsIcon));
				
		diamondsLabel->setScale(getNodeScale(diamondsLabel));
		diamondsLabel->setPositionY(f->m_jamContainer->getPositionY() - 13.38f);
						
		if (auto diamondsIcon = getChildByID("diamond-icon")) {
			diamondsIcon->setScale(getNodeScale(diamondsIcon));
			diamondsIcon->setPositionY(f->m_jamContainer->getPositionY() - 13.38f);
		}
				
		coinIcon1->setScale(getNodeScale(coinIcon1));
		coinIcon1->setPositionY(diamondsLabel->getPositionY() - 13.38f);
				
		if (auto coinIcon2 = getChildByID("coin-icon-2")) {
			coinIcon2->setScale(getNodeScale(coinIcon2));
			coinIcon2->setPositionY(diamondsLabel->getPositionY() - 13.38f);
		}
				
		if (auto coinIcon3 = getChildByID("coin-icon-3")) {
			coinIcon3->setScale(getNodeScale(coinIcon3));
			coinIcon3->setPositionY(diamondsLabel->getPositionY() - 13.38f);
		}
	}
	else if (coinIcon1) {
		coinIcon1->setPositionY(f->m_jamContainer->getPositionY() - 14.5f);
							
		if (auto coinIcon2 = getChildByID("coin-icon-2")) {
			coinIcon2->setPositionY(f->m_jamContainer->getPositionY() - 14.5f);
		}
		
		if (auto coinIcon3 = getChildByID("coin-icon-3")) {
			coinIcon3->setPositionY(f->m_jamContainer->getPositionY() - 14.5f);
		}
	}
	
	f->m_firstTime = false;
}

bool ProLevelInfoLayer::init(GJGameLevel* level, bool challenge) {
	if (!LevelInfoLayer::init(level, challenge)) {
		return false;
	}
	
	updateJam();
	
	return true;
}

void ProLevelInfoLayer::levelDownloadFinished(GJGameLevel* p0) {
	LevelInfoLayer::levelDownloadFinished(p0);
	updateJam();
}
	
void ProLevelInfoLayer::levelDownloadFailed(int p0){
	LevelInfoLayer::levelDownloadFailed(p0);
	updateJam();
}

void ProLevelInfoLayer::levelUpdateFinished(GJGameLevel* p0, UpdateResponse p1){
	LevelInfoLayer::levelUpdateFinished(p0, p1);
	updateJam();
}

void ProLevelInfoLayer::levelUpdateFailed(int p0){
	LevelInfoLayer::levelUpdateFailed(p0);
	updateJam();
}

void ProLevelInfoLayer::levelDeleteFinished(int p0){
	LevelInfoLayer::levelDeleteFinished(p0);
	updateJam();
}

void ProLevelInfoLayer::levelDeleteFailed(int p0){
	LevelInfoLayer::levelDeleteFailed(p0);
	updateJam();
}

void ProLevelInfoLayer::updateLabelValues() {
	LevelInfoLayer::updateLabelValues();
	updateJam();
}

void ProLevelInfoLayer::updateSideButtons() {
	LevelInfoLayer::updateSideButtons();
	updateJam();
}

void ProLevelInfoLayer::likedItem(LikeItemType p0, int p1, bool p2) {
	LevelInfoLayer::likedItem(p0, p1, p2);
	updateJam();
}