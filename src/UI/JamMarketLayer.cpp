#include "JamMarketLayer.hpp"

#include "../Other/Utils.hpp"

CCScene* JamMarketLayer::scene() {
    auto layer = new JamMarketLayer();

    layer->init();
    layer->autorelease();

    auto scene = CCScene::create();
    scene->addChild(layer);

    return scene;
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

bool JamMarketLayer::init() {
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

    auto menu = CCMenu::create();
    menu->setPosition({0, 0});

    addChild(menu);

    auto backBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"),
        this,
        menu_selector(JamMarketLayer::onBack)
    );
    backBtn->setPosition({24, winSize.height - 23});

    menu->addChild(backBtn);

    auto timer = CCLabelBMFont::create("Re-stock in 3h 55min", "bigFont.fnt");
    timer->setScale(0.4f);
    timer->setColor({154, 94, 65});
    timer->setOpacity(173);
    timer->setAnchorPoint({0.f, 0.5f});
    timer->setPosition({winSize.width / 2.f - 198.5f, 183.5f});

    addChild(timer);

    for (int i = 1; i <= 3; ++i) {
        auto chestSpr = CCSprite::createWithSpriteFrameName(fmt::format("chest_0{}_02_001.png", i).c_str());
        chestSpr->setScale(57.6f / chestSpr->getContentWidth());

        auto chestBtn = CCMenuItemSpriteExtra::create(chestSpr, this, nullptr);
        chestBtn->setPosition({winSize.width / 2.f + (i - 2) * 113.5f, 62});

        menu->addChild(chestBtn);

        auto lbl = CCLabelBMFont::create("2,000", "bigFont.fnt");
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
    }

    menu = CCMenu::create();
    menu->setPositionY(0);

    addChild(menu);

    auto gsm = GameStatsManager::get();
    auto unlockableItems = std::vector<GJStoreItem*>{};

    for (const auto& [key, item] : CCDictionaryExt<int, GJStoreItem*>(gsm->m_storeItems)) {
        if (
            item->m_shopType != ShopType::Paths
            && item->m_unlockType != 0xC
            && item->m_unlockType > 0
            && !gsm->isStoreItemUnlocked(key)
        ) {
            unlockableItems.push_back(item);
        }
    }
    
    auto chosenItems = std::vector<GJStoreItem*>{};

    for (int i = 0; i < 4 && !unlockableItems.empty(); ++i) {
        int randomIndex = Utils::getRandomInt(0, unlockableItems.size() - 1);
        chosenItems.push_back(unlockableItems[randomIndex]);
        unlockableItems.erase(unlockableItems.begin() + randomIndex);
    }

    for (int i = 0; i < chosenItems.size(); i++) {
        auto item = chosenItems[i];
        auto icon = GJItemIcon::createStoreItem(
            static_cast<UnlockType>(item->m_unlockType.value()),
            item->m_typeID.value(),
            false,
            GameManager::get()->colorForIdx(item->m_typeID.value())
        );

        auto btn = CCMenuItemSpriteExtra::create(icon, this, nullptr);
        btn->setPosition({-127.5f + 85.f * i, 130.5f});

        menu->addChild(btn);
        
        auto lbl = CCLabelBMFont::create("2,000", "bigFont.fnt");
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

        btn->addChild(container);
    }
    
    return true;
}