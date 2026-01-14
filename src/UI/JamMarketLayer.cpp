#include "JamMarketLayer.hpp"

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

    auto spr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");

    auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(JamMarketLayer::onBack));
    btn->setPosition({24, winSize.height - 23});

    menu->addChild(btn);

    GameManager::get()->fadeInMusic("jam-market.mp3"_spr);

    return true;
}