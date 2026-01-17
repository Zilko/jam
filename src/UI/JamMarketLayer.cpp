#include "JamMarketLayer.hpp"
#include "JamPurchaseItemPopup.hpp"

#include "../Other/JamManager.hpp"
#include "../Other/Utils.hpp"

CCScene* JamMarketLayer::scene() {
    auto layer = new JamMarketLayer();

    layer->init();
    layer->autorelease();

    auto scene = CCScene::create();
    scene->addChild(layer);

    return scene;
}

void JamMarketLayer::boughtItem(GJStoreItem* item) {
    updateJamCount();

    if (!m_buttonForItem.contains(item)) {
        return;
    }

    auto btn = m_buttonForItem.at(item);
    auto icon = static_cast<GJItemIcon*>(btn->getNormalImage());

    btn->getChildByIndex(-1)->setVisible(false);
    btn->setEnabled(false);
    icon->darkenStoreItem(ShopType::Normal);
}

void JamMarketLayer::boughtChest(int chest) {
    updateJamCount();
    updateChests();
}

void JamMarketLayer::updateJamCount() {
    m_jamLabel->setString(std::to_string(JamManager::get().getJam()).c_str());
}

void JamMarketLayer::updateTimer(float) {
    if (m_willRestock) {
        return;
    }

    auto& jm = JamManager::get();
    auto remaining = jm.getTimeForRestock() - jm.getTimeSinceLastRestock();

    if (remaining <= 0) {
        m_willRestock = true;
        
        m_restockLabel->runAction(CCRepeatForever::create(
            CCSequence::create(
                CCFadeTo::create(0.85f, 120),
                CCFadeTo::create(0.85f, 163),
                nullptr
            )
        ));

        return m_restockLabel->setString("Restocking any time now...");
    }

    m_restockLabel->setString(
        ("Restock in " + Utils::getTimeLeftString(remaining)).c_str()
    );
}

void JamMarketLayer::tooExpensive(CCObject* sender) {
    FMODAudioEngine::get()->playEffectAdvanced("pop.mp3"_spr, 1.f, 1.f, 0.35f, 1.f, true, false, 45, 1000, 0, 0, false, 0, false, false, 0, 0, 0.f, 0);
        
    runAction(CCSequence::create(
        CCDelayTime::create(0.11f),
        CallFuncExt::create([] {
            FMODAudioEngine::get()->playEffectAdvanced("pop.mp3"_spr, 0.6f, 1.f, 0.35f, 1.f, true, false, 45, 1000, 0, 0, false, 0, false, false, 0, 0, 0.f, 0);
        }),
        nullptr
    ));

    static_cast<CCNode*>(sender)->runAction(CCSequence::create(
        CCMoveBy::create(0.04f, ccp(-1.5f, 0)),
        CCMoveBy::create(0.04f, ccp(3, 0)),
        CCMoveBy::create(0.04f, ccp(-3, 0)),
        CCMoveBy::create(0.04f, ccp(1.5f, 0)),
        nullptr
    ));

    m_jamLabel->runAction(CCSequence::create(
        CCMoveBy::create(0.04f, ccp(-2, 0)),
        CCMoveBy::create(0.04f, ccp(4, 0)),
        CCMoveBy::create(0.04f, ccp(-4, 0)),
        CCMoveBy::create(0.04f, ccp(2, 0)),
        nullptr
    ));

    m_jamIcon->runAction(CCSequence::create(
        CCMoveBy::create(0.04f, ccp(-2, 0)),
        CCMoveBy::create(0.04f, ccp(4, 0)),
        CCMoveBy::create(0.04f, ccp(-4, 0)),
        CCMoveBy::create(0.04f, ccp(2, 0)),
        nullptr
    ));

    m_jamParticle->runAction(CCSequence::create(
        CCMoveBy::create(0.04f, ccp(-2, 0)),
        CCMoveBy::create(0.04f, ccp(4, 0)),
        CCMoveBy::create(0.04f, ccp(-4, 0)),
        CCMoveBy::create(0.04f, ccp(2, 0)),
        nullptr
    ));
}

void JamMarketLayer::onPurchase(CCObject* sender) {
    auto& jm = JamManager::get();
    auto item = m_shopItems[static_cast<CCNode*>(sender)->getTag()];

    if (jm.getItemJamPrice(item) > jm.getJam()) {
        return tooExpensive(sender);
    }

    JamPurchaseItemPopup::create(item, this)->show();
}

void JamMarketLayer::onPurchaseChest(CCObject* sender) {
    auto& jm = JamManager::get();
    auto chest = static_cast<CCNode*>(sender)->getTag();

    if (jm.getChestJamPrice(chest) > jm.getJam()) {
        return tooExpensive(sender);
    }

    JamPurchaseItemPopup::create(chest, this)->show();
}

void JamMarketLayer::keyBackClicked() {
    onBack(nullptr);
}

void JamMarketLayer::onBack(CCObject*) {
    if (m_closing) {
        return;
    }

    auto scene = SecretRewardsLayer::scene(true);

    if (!scene) {
        return;
    }

    m_closing = true;

    setKeyboardEnabled(false);
    setKeypadEnabled(false);
    retain();
    removeFromParent();

    CCDirector::get()->replaceScene(scene);
    
    scene->addChild(this, 1000);

    release();
    runAction(CCSequence::create(
        CCEaseIn::create(CCMoveTo::create(0.3f, ccp(0, CCDirector::get()->getWinSize().height)), 2.f),
        CallFuncExt::create([this] {
            removeFromParentAndCleanup(true);
        }),
        nullptr
    ));

    GameManager::get()->fadeInMusic("secretLoop.mp3");
}

void JamMarketLayer::updateChests() {
    auto& jm = JamManager::get();
    auto winSize = CCDirector::get()->getWinSize();

    for (auto chest : m_chests) {
        if (chest) {
            chest->removeFromParent();
        }
    }

    m_chests = { nullptr, nullptr, nullptr };

    for (int i = 1; i <= 3; ++i) {
        if (jm.isChestPurchased(i)) {
            auto chestSpr = CCSprite::createWithSpriteFrameName(fmt::format("chest_0{}_04_back_001.png", i).c_str());
            chestSpr->setPosition({winSize.width / 2.f + (i - 2) * 113.5f, 62});

            m_mainMenu->addChild(chestSpr);

            auto chestSpr2 = CCSprite::createWithSpriteFrameName(fmt::format("chest_0{}_04_001.png", i).c_str());
            chestSpr2->setScale(57.6f / chestSpr2->getContentWidth());
            chestSpr2->setPosition({winSize.width / 2.f + (i - 2) * 113.5f, 62});

            m_mainMenu->addChild(chestSpr2);

            chestSpr->setScale(chestSpr2->getScale());

            chestSpr->setColor({ 67, 67, 67 });
            chestSpr2->setColor({ 67, 67, 67 });

            continue;
        }

        auto chestSpr = CCSprite::createWithSpriteFrameName(fmt::format("chest_0{}_02_001.png", i).c_str());
        chestSpr->setScale(57.6f / chestSpr->getContentWidth());

        auto chestBtn = CCMenuItemSpriteExtra::create(chestSpr, this, menu_selector(JamMarketLayer::onPurchaseChest));
        chestBtn->setTag(i);
        chestBtn->setPosition({winSize.width / 2.f + (i - 2) * 113.5f, 62});

        m_mainMenu->addChild(chestBtn);

        if (jm.isChestPurchased(i)) {
            chestSpr->setOpacity(150);
            chestSpr->setColor({107, 107, 107});
            chestBtn->setEnabled(false);
            continue;
        }

        auto price = jm.getChestJamPrice(i);

        auto lbl = CCLabelBMFont::create(GameToolbox::pointsToString(price).c_str(), "bigFont.fnt");
        lbl->setScale(0.35f);
        lbl->setAnchorPoint({0.f, 0.5f});

        auto jamIcon = CCSprite::create("jam1.png"_spr);
        jamIcon->setScale(0.175f);
        jamIcon->setAnchorPoint({0.f, 0.5f});
        jamIcon->setPositionX(lbl->getScaledContentWidth() + 2);

        auto container = CCNode::create();
        container->setPosition({chestBtn->getContentWidth() / 2.f - (lbl->getScaledContentWidth() + jamIcon->getScaledContentWidth() + 2) / 2.f, -8});
        container->addChild(lbl);
        container->addChild(jamIcon);

        chestBtn->addChild(container);

        m_chests[i - 1] = chestBtn;
    }
}

bool JamMarketLayer::init() {
    auto& jm = JamManager::get();

    if (!Mod::get()->hasSavedValue("next-restock")) {
        jm.restockJamMarket();
    }

    setKeyboardEnabled(true);
    setKeypadEnabled(true);

    GameManager::get()->fadeInMusic("jam-market.mp3"_spr);

    CCSpriteFrameCache::get()->addSpriteFramesWithFile("GJ_ShopSheet01.plist");

    auto winSize = CCDirector::get()->getWinSize();

    auto bg = CCSprite::createWithSpriteFrameName("shopBG_001.png");
    bg->setScaleX((winSize.width + 5) / bg->getContentWidth());
    bg->setScaleY((winSize.height + 5) / bg->getContentHeight());
    bg->setColor({255, 157, 157});
    bg->setAnchorPoint({0, 0});
    bg->setPosition({-2.5f, -2.5f});
    
    addChild(bg);

    auto sign = CCSprite::create("jam-market-sign.png"_spr);
    sign->setPosition({winSize.width * 0.35f, winSize.height - 38});

    addChild(sign);

    auto desk = CCSprite::createWithSpriteFrameName("storeDesk_001.png");
    desk->setColor({200, 158, 158});
    desk->setScaleX(0.96f);
    desk->setPosition({winSize.width / 2.f, 95.f});

    addChild(desk);

    m_mainMenu = CCMenu::create();
    m_mainMenu->setPosition({0, 0});

    addChild(m_mainMenu);

    auto wantedPoster = CCSprite::create("JamMaster.png"_spr);
    wantedPoster->setPosition({
        winSize.width / 2.f + 110.5f,
        winSize.height - (winSize.height - (desk->getPositionY() + desk->getContentHeight() / 2.f)) / 2.f - 5.f
    });
    wantedPoster->setScale(0.675f);
    wantedPoster->setOpacity(203);
    wantedPoster->setColor({106, 66, 66});

    addChild(wantedPoster);

    m_jamLabel = CCLabelBMFont::create("2130", "bigFont.fnt");
    m_jamLabel->setScale(0.6f);
    m_jamLabel->setAnchorPoint({1.f, 0.5f});
    m_jamLabel->setPosition(winSize - ccp(35, 15));

    addChild(m_jamLabel);

    m_jamIcon = CCSprite::create("jam1.png"_spr);
    m_jamIcon->setScale(0.25f);
    m_jamIcon->setPosition(winSize - ccp(22, 15));

    addChild(m_jamIcon);

    m_jamParticle = GameToolbox::particleFromString("8a-1a1a0.2a30a0a360a0a10a12a10a0a0a0a0a0a0a2a1a0a0a0.960784a0a0.258824a0a0.258824a0a1a0a1a1a0a0a1a0a0.670588a0a0.670588a0a1a0a0a0a0a0a0a0a0a0a0a0a0a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0", nullptr, false);
    m_jamParticle->setScale(0.625f);
    m_jamParticle->setPosition(m_jamIcon->getPosition());

    addChild(m_jamParticle);

    auto backBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"),
        this,
        menu_selector(JamMarketLayer::onBack)
    );
    backBtn->setPosition({24, winSize.height - 23});

    m_mainMenu->addChild(backBtn);

    m_restockLabel = CCLabelBMFont::create("Restock in 3h 55min", "bigFont.fnt");
    m_restockLabel->setScale(0.4f);
    m_restockLabel->setColor({154, 94, 65});
    m_restockLabel->setOpacity(173);
    m_restockLabel->setAnchorPoint({0.f, 0.5f});
    m_restockLabel->setPosition({winSize.width / 2.f - 198.5f, 183.5f});

    addChild(m_restockLabel);

    auto menu = CCMenu::create();
    menu->setPositionY(0);

    addChild(menu);

    if (jm.getTimeSinceLastRestock() >= jm.getTimeForRestock()) {
        jm.restockJamMarket();
    }

    auto fakeItems = jm.getFakeJamMarketItems();
    m_shopItems = jm.getJamMarketItems();

    for (int i = 0; i < 4; i++) {
        auto item = m_shopItems[i];
        auto locked = false;
        
        if (
            !item
            || GameStatsManager::get()->isStoreItemUnlocked(item->m_index.value())
        ) {
            locked = true;
        }

        if (!item) {
            item = fakeItems[i];
        }

        auto icon = GJItemIcon::createStoreItem(
            static_cast<UnlockType>(item->m_unlockType.value()),
            item->m_typeID.value(),
            false,
            GameManager::get()->colorForIdx(item->m_typeID.value())
        );

        auto btn = CCMenuItemSpriteExtra::create(icon, this, menu_selector(JamMarketLayer::onPurchase));
        btn->setTag(i);
        btn->setPosition({-127.5f + 85.f * i, 130.5f});

        menu->addChild(btn);

        if (locked) {
            icon->darkenStoreItem(ShopType::Normal);
            btn->setEnabled(false);
            continue;
        }
        
        auto lbl = CCLabelBMFont::create(GameToolbox::pointsToString(jm.getItemJamPrice(item)).c_str(), "bigFont.fnt");
        lbl->setScale(0.35f);
        lbl->setAnchorPoint({0.f, 0.5f});

        auto jamIcon = CCSprite::create("jam1.png"_spr);
        jamIcon->setScale(0.175f);
        jamIcon->setAnchorPoint({0.f, 0.5f});
        jamIcon->setPositionX(lbl->getScaledContentWidth() + 2);

        auto container = CCNode::create();
        container->setPosition({btn->getContentWidth() / 2.f - (lbl->getScaledContentWidth() + jamIcon->getScaledContentWidth() + 2) / 2.f, -8});
        container->addChild(lbl);
        container->addChild(jamIcon);

        btn->addChild(container, 100);

        m_buttonForItem[item] = btn;
    }

    updateChests();
    updateJamCount();
    updateTimer(0.f);
    schedule(schedule_selector(JamMarketLayer::updateTimer), 1.f, kCCRepeatForever, 1.f);
    
    return true;
}