#include "JamPurchaseItemPopup.hpp"
#include "JamMarketLayer.hpp"

#include "../Other/JamManager.hpp"

JamPurchaseItemPopup* JamPurchaseItemPopup::create(GJStoreItem* item, int chest, JamMarketLayer* layer) {
    auto ret = new JamPurchaseItemPopup(item, chest, layer);

    ret->init();
    ret->autorelease();

    return ret;
}

JamPurchaseItemPopup* JamPurchaseItemPopup::createWithItem(GJStoreItem* item, JamMarketLayer* layer) {
    return create(item, 0, layer);
}

JamPurchaseItemPopup* JamPurchaseItemPopup::createWithChest(int chest, JamMarketLayer* layer) {
    return create(nullptr, chest, layer);
}

void JamPurchaseItemPopup::onBuy(CCObject*) {
    auto& jm = JamManager::get();

    if (m_isChest) {
        jm.purchaseChest(m_chest);
        m_layer->boughtChest(m_chest);
    } else {
        jm.purchaseItem(m_item);
        m_layer->boughtItem(m_item);
    }

    FMODAudioEngine::get()->playEffect("buyItem01.ogg", 1.05f, 1.f, 0.28f);
    FMODAudioEngine::get()->playEffectAdvanced("lid.mp3"_spr, 0.7f, 1.f, 0.55f, 1.f, false, false, 45, 1000, 0, 0, false, 0, false, false, 0, 0, 0.f, 0);

    onClose(nullptr);
}

bool JamPurchaseItemPopup::init() {
    Popup::init(260, 190, "GJ_square02.png");

    auto& jm = JamManager::get();

    setTitle("Buy Item", "bigFont.fnt", 0.6f, 16.f);

    m_closeBtn->setVisible(false);

    std::string itemName;
    int itemPrice;

    if (m_isChest) {
        auto chestSpr = CCSprite::createWithSpriteFrameName(fmt::format("chest_0{}_02_001.png", m_chest).c_str());
        chestSpr->setScale(57.6f / chestSpr->getContentWidth());
        chestSpr->setPosition(m_size / 2.f + ccp(0, 22));

        m_mainLayer->addChild(chestSpr);

        itemName = "Chest";
        itemPrice = jm.getChestJamPrice(m_chest);
    } else {
        auto icon = GJItemIcon::createStoreItem(
            static_cast<UnlockType>(m_item->m_unlockType.value()),
            m_item->m_typeID.value(),
            false,
            GameManager::get()->colorForIdx(m_item->m_typeID.value())
        );
        icon->setScale(1.2f);
        icon->setPosition(m_size / 2.f + ccp(0, 22));

        m_mainLayer->addChild(icon);

        itemName = ItemInfoPopup::nameForUnlockType(m_item->m_typeID.value(), static_cast<UnlockType>(m_item->m_unlockType.value()));
        itemPrice = jm.getItemJamPrice(m_item);
    }

    auto textArea = TextArea::create(
        fmt::format(
            "Do you want to <cg>buy</c> this <co>{}</c> for <cy>{}</c> <cr>Jam</c>?",
            itemName,
            GameToolbox::pointsToString(itemPrice)
        ),
        "bigFont.fnt", 1.f, 577.77f, {0.5f, 1.f}, 35.f, false
    );
    textArea->setScale(0.4f);
    textArea->setPosition(m_size / 2.f - ccp(0, 20));
    
    m_mainLayer->addChild(textArea);

    auto btn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Cancel", 50, 0, 0.6f, true, "bigFont.fnt", "GJ_button_06.png", 30.f),
        this,
        menu_selector(JamPurchaseItemPopup::onClose)
    );
    btn->setPosition({m_size.width / 2.f - 40.5f, 22});

    m_buttonMenu->addChild(btn);

    btn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Buy", 50, 0, 0.6f, true, "bigFont.fnt", "GJ_button_01.png", 30.f),
        this,
        menu_selector(JamPurchaseItemPopup::onBuy)
    );
    btn->setPosition({m_size.width / 2.f + 40.5f, 22});

    m_buttonMenu->addChild(btn);

    return true;
}