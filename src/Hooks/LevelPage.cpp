#include "LevelPage.hpp"

#include "../Other/JamManager.hpp"
#include "../Other/Utils.hpp"

CCPoint ProLevelPage::getNodePosition(CCNode* node, const CCPoint& offset) {
    auto f = m_fields.self();
	
	if (f->m_storedPositions.contains(node)) {
		return f->m_storedPositions.at(node);
	}
	
	auto pos = node->getPosition() + offset;
	
	f->m_storedPositions[node] = pos;
	
	return pos;
}

void ProLevelPage::updateDynamicPage(GJGameLevel* level) {
    LevelPage::updateDynamicPage(level);

    auto& jm = JamManager::get();

    if (
        !jm.canLevelHaveJam(level)
        || level->m_levelID.value() > 22
    ) {
        return;
    }
    
    auto orbsLabel = m_levelDisplay->getChildByID("orbs-label");
    auto orbsIcon = m_levelDisplay->getChildByID("orbs-icon");

    if (!orbsLabel || !orbsIcon) {
        return;
    }

    auto f = m_fields.self();
    auto scale = 0.75f;
    auto opa = false;

    if (auto mod = Loader::get()->getLoadedMod("firee.overchargedlevels")) {
        if (mod->getSettingValue<bool>("enabled")) {
            opa = true;
            scale = 0.82f;
        }
    }

    if (f->m_jamLabel) {
        f->m_jamLabel->removeFromParent();
        f->m_jamIcon->removeFromParent();
    }

    f->m_jamLabel = CCLabelBMFont::create(Utils::getJamProgressString(level).c_str(), "bigFont.fnt");
    f->m_jamLabel->setID("jam-label"_spr);
    f->m_jamLabel->setAnchorPoint({0.f, 0.5f});
    f->m_jamLabel->setScale(0.4f * scale);

    f->m_jamIcon = CCSprite::create("jam1.png"_spr);
    f->m_jamIcon->setID("jam-icon"_spr);
    f->m_jamIcon->setScale(0.213f * scale);

    m_levelDisplay->addChild(f->m_jamLabel);
    m_levelDisplay->addChild(f->m_jamIcon);

    if (opa) {
        f->m_jamLabel->setPosition({8, orbsLabel->getPositionY()});
    }

    orbsLabel->setScale(0.4f * scale);
    orbsIcon->setScale(0.7f * scale);

    orbsLabel->setPosition(getNodePosition(orbsLabel, {opa ? 4.f : 0.f, -4.f}));
    orbsIcon->setPosition({
        orbsLabel->getPositionX() + orbsLabel->getScaledContentWidth() * (1 - orbsLabel->getAnchorPoint().x) + 7.5f,
        getNodePosition(orbsIcon, ccp(0, -4)).y
    });

    if (!opa) {
        f->m_jamLabel->setPosition(orbsLabel->getPosition() + ccp(0, 15.f * scale));
    }

    f->m_jamIcon->setPosition(f->m_jamLabel->getPosition() + ccp(f->m_jamLabel->getScaledContentWidth() + 7, 0));
}