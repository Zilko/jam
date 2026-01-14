#include "LevelListCell.hpp"

void ProLevelListCell::loadFromList(GJLevelList* p0) {
    LevelListCell::loadFromList(p0);
    
    auto diamondLabel = m_mainLayer->getChildByID("diamond-label");
    auto diamondIcon = m_mainLayer->getChildByID("diamond-icon");

    if (!diamondIcon || !diamondLabel) {
        return;
    }

    auto f = m_fields.self();

    f->m_jamReward = static_cast<int>(p0->m_diamonds * 1.7f) / 5 * 5;

    if (f->m_jamReward <= 0) {
        return;
    }

    diamondLabel->setScale(diamondLabel->getScale() * 0.75f);
    diamondLabel->setPositionX(diamondLabel->getPositionX() + 1);
    diamondIcon->setScale(diamondIcon->getScale() * 0.75f);
    diamondIcon->setPositionX(diamondIcon->getPositionX() - 1);

    auto lbl = CCLabelBMFont::create(std::to_string(f->m_jamReward).c_str(), "bigFont.fnt");
    lbl->setID("jam-label"_spr);
    lbl->limitLabelWidth(diamondLabel->getScaledContentWidth() + 3.f, diamondLabel->getScale(), 0.0001f);
    lbl->setAnchorPoint(diamondLabel->getAnchorPoint());
    lbl->setPosition(diamondLabel->getPosition() + ccp(0, -12.3f));

    m_mainLayer->addChild(lbl);

    auto icon = CCSprite::create("jam1.png"_spr);
    icon->setID("jam-icon"_spr);
    icon->setScale(diamondIcon->getScale() * 0.182f);
    icon->setPosition(diamondIcon->getPosition() + ccp(0, -12.3f));

    m_mainLayer->addChild(icon);
}