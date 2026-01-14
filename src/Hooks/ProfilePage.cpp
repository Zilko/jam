#include "ProfilePage.hpp"

#include "../Other/JamManager.hpp"

void ProProfilePage::loadPageFromUserInfo(GJUserScore* p0) {
    ProfilePage::loadPageFromUserInfo(p0);
    
    auto menu = m_mainLayer->getChildByID("stats-menu");

    if (!menu) {
        return;
    }

    std::string str = GameToolbox::pointsToString(JamManager::get().getJam());

    auto lbl = CCLabelBMFont::create(str.c_str(), "bigFont.fnt");
	lbl->setAnchorPoint({0.f, 0.5f});
	lbl->setPositionY(5);
	lbl->limitLabelWidth(74.25f, 0.625f, 0.0001f);
	
	auto icon = CCSprite::create("jam1.png"_spr);
	icon->setScale(0.29f);
	icon->setAnchorPoint({0.f, 0.5f});
	icon->setPosition({lbl->getScaledContentWidth() + 3.f, 5.f});
	
	auto jamContainer = CCNode::create();
	jamContainer->setID("jam-container"_spr);
	jamContainer->setContentSize({lbl->getScaledContentWidth() + icon->getScaledContentWidth() + 3.f, 10});
    jamContainer->addChild(lbl);
    jamContainer->addChild(icon);

    menu->addChild(jamContainer);
    menu->updateLayout();
}