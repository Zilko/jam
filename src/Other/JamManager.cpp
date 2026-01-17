#include "JamManager.hpp"
#include "Utils.hpp"

JamManager& JamManager::get() {
    static JamManager instance;
    return instance;
}

int JamManager::getJam() {
    return Mod::get()->getSavedValue<int>("jam");
}

void JamManager::grabJam(int jam) {
    Mod::get()->setSavedValue("jam", getJam() - jam);
}

void JamManager::rewardJam(int jam) {
    if (jam <= 0) {
        return;
    }

    Mod::get()->setSavedValue("jam", getJam() + jam);
}

bool JamManager::canLevelHaveJam(GJGameLevel* level) {
    return level->m_levelID.value() > 0
        && level->m_stars.value() > 0;
}

int JamManager::getTotalLevelJam(GJGameLevel* level) {
    if (!canLevelHaveJam(level)) {
        return 0;
    }

    auto stars = level->m_stars.value();

    if (stars <= 0) {
        return 0;
    }

    if (stars <= 3) {
        return 20;
    }

    if (stars <= 6) {
        return 40;
    }

    if (stars <= 9) {
        return 60;
    }

    if (stars >= 10) {
        return 80;
    }

    return 0;
}

float JamManager::getLevelJamProgress(GJGameLevel* level) {
    auto obj = Mod::get()->getSavedValue<matjson::Value>("jam-level-progress");
    auto id = std::to_string(level->m_levelID);

    if (obj.contains(id)) {
        return obj[id].asDouble().unwrapOr(0.f);
    }

    return 0.f;
}

void JamManager::setLevelJamProgress(GJGameLevel* level, float progress) {
    auto obj = Mod::get()->getSavedValue<matjson::Value>("jam-level-progress");

    obj[std::to_string(level->m_levelID)] = progress;

    Mod::get()->setSavedValue("jam-level-progress", obj);
}

int JamManager::getLevelJamForProgress(GJGameLevel* level, float progress) {
    if (progress >= 1.f) {
        return getTotalLevelJam(level);
    }

    return getTotalLevelJam(level) / 2.f * progress;
}

int JamManager::getJamChestReward(int chestType) {
    if (chestType <= 0 || chestType >= 9) {
        return 0;
    }

    if (
        chestType <= 3
        && Utils::getRandomInt(0, 100) < 36
    ) {
        return 0;
    }
    
    if (
        chestType >= 4
        && chestType <= 6
        && Utils::getRandomInt(0, 100) < 17
    ) {
        return 0;
    }

    switch (chestType) {
        case 1: return Utils::getRandomInt(1, 3) * 5;
        case 2:
        case 3: return Utils::getRandomInt(1, 6) * 5 + 15;
        case 4: return Utils::getRandomInt(1, 15) * 5 + 45;
        case 5: return Utils::getRandomInt(1, 15) * 5 + 120;
        case 6: return Utils::getRandomInt(1, 15) * 5 + 195;
        case 7: return Utils::getRandomInt(1, 5) * 5 + 270;
        case 8: return Utils::getRandomInt(1, 5) * 15 + 295;
        default: return 0;
    }
}

int JamManager::getTimeForRestock() {
    return 14400;
}

int JamManager::getTimeSinceLastRestock() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    if (!Mod::get()->hasSavedValue("next-restock")) {
        restockJamMarket();
    }

    auto next = Mod::get()->getSavedValue<int64_t>("next-restock");

    return getTimeForRestock() - (next - time);
}

void JamManager::restockJamMarket() {
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

    auto chosenFakeItems = std::set<GJStoreItem*>{};

    for (int i = 0; i < 4; i++) {
        GJStoreItem* item = nullptr;

        do {
            auto index = Utils::getRandomInt(0, gsm->m_storeItems->count() - 1);
            auto i = 0;

            for (const auto& [key, _item] : CCDictionaryExt<int, GJStoreItem*>(gsm->m_storeItems)) {
                if (i++ == index) {
                    item = _item;
                    break;
                }
            }
        } while (chosenFakeItems.contains(item));

        chosenFakeItems.insert(item);
    }

    auto array = matjson::Value::array();

    for (auto item : chosenFakeItems) {
        array.push(item->m_index.value());
    }

    Mod::get()->setSavedValue("fake-store-items", array);

    auto chosenItems = std::array<GJStoreItem*, 4>{ nullptr, nullptr, nullptr, nullptr };
    auto added = 0;

    for (int i = 0; i < 4 && !unlockableItems.empty(); ++i) {
        int randomIndex = Utils::getRandomInt(0, unlockableItems.size() - 1);
        chosenItems[added++] = unlockableItems[randomIndex];
        unlockableItems.erase(unlockableItems.begin() + randomIndex);
    }

    array = matjson::Value::array();

    for (auto item : chosenItems) {
        array.push(item->m_index.value());
    }

    Mod::get()->setSavedValue("store-items", array);

    for (int i = 1; i <= 3; i++) {
        Mod::get()->setSavedValue(fmt::format("chest-{}", i), false);
    }

    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto next = Mod::get()->getSavedValue<int64_t>("next-restock");

    if (time < next || !Mod::get()->hasSavedValue("next-restock")) {
        next = time;
    }

    Mod::get()->setSavedValue("next-restock", time + getTimeForRestock() - ((time - next) % getTimeForRestock()));
}

const std::array<GJStoreItem*, 4> JamManager::getJamMarketItems() {
    if (!Mod::get()->hasSavedValue("store-items")) {
        restockJamMarket();
    }

    auto gsm = GameStatsManager::get();
    auto ret = std::array<GJStoreItem*, 4>{ nullptr, nullptr, nullptr, nullptr };
    auto count = 0;

    for (const auto& v : Mod::get()->getSavedValue<matjson::Value>("store-items")) {
        auto index = v.asInt().unwrapOr(0);

        if (index <= 0) {
            continue;
        }

        ret[count++] = gsm->getStoreItem(index);
    }

    return ret;
}

const std::array<GJStoreItem*, 4> JamManager::getFakeJamMarketItems() {
    if (!Mod::get()->hasSavedValue("fake-store-items")) {
        restockJamMarket();
    }

    auto gsm = GameStatsManager::get();
    auto ret = std::array<GJStoreItem*, 4>{ nullptr, nullptr, nullptr, nullptr };
    auto count = 0;

    for (const auto& v : Mod::get()->getSavedValue<matjson::Value>("fake-store-items")) {
        auto index = v.asInt().unwrapOr(0);

        if (index <= 0) {
            continue;
        }

        ret[count++] = gsm->getStoreItem(index);
    }

    return ret;
}

int JamManager::getItemJamPrice(GJStoreItem* item) {
    auto normalPrice = item->m_price.value();
    auto mult = item->m_shopType == ShopType::Diamond ? 17.3f : 1.3f;

    return static_cast<int>(normalPrice * mult / 5) * 5;
}

int JamManager::getChestJamPrice(int chest) {
    return 2000 + 1000 * chest;
}

void JamManager::unlockItem(UnlockType type, int id, int index) {
    if (didUnlockItemWithJam(type, id)) {
        return;
    }

    auto saveObj = Mod::get()->getSavedValue<matjson::Value>("unlocked-items");

    if (!saveObj.isArray()) {
        saveObj = matjson::Value::array();
    }

    auto obj = matjson::Value{};

    obj["index"] = index;
    obj["id"] = id;
    obj["unlock-type"] = static_cast<int>(type);

    saveObj.push(obj);

    Mod::get()->setSavedValue("unlocked-items", saveObj);
}

void JamManager::unlockItem(GJStoreItem* item) {
    unlockItem(
        static_cast<UnlockType>(item->m_unlockType.value()),
        item->m_typeID.value(),
        item->m_index.value()
    );
}

void JamManager::purchaseItem(GJStoreItem* item) { 
    if (didUnlockItemWithJam(item)) {
        return;
    }

    auto price = getItemJamPrice(item);

    if (price > getJam()) {
        return;
    }

    grabJam(price);
    unlockItem(item);
}

void JamManager::purchaseChest(int chest) {
    if (isChestPurchased(chest)) {
        return;
    }

    auto price = getChestJamPrice(chest);

    if (price > getJam()) {
        return;
    }

    grabJam(price);

    Mod::get()->setSavedValue(fmt::format("chest-{}", chest), true);

    auto items = std::vector<std::pair<int, UnlockType>>{};
    auto gm = GameManager::get();

    for (int i = 0; i < chest; i++) {
        std::pair<int, UnlockType> pair;

        do {
            auto type = Utils::getRandomInt(1, 0xf);
            
            pair = std::make_pair(
                Utils::getRandomInt(1, gm->countForType(gm->unlockTypeToIconType(type))),
                static_cast<UnlockType>(type)
            );
        } while (
            pair.second == UnlockType::GJItem
            || pair.second == UnlockType::Col1
            || pair.second == UnlockType::Col2
            || std::find(items.begin(), items.end(), pair) != items.end()
        );

        items.push_back(pair);
    }

    auto array = CCArray::create();

    for (const auto& [id, type] : items) {
        array->addObject(GJRewardObject::createItemUnlock(type, id));

        auto item = GameStatsManager::get()->getStoreItem(id, static_cast<int>(type));

        unlockItem(type, id, item ? item->m_index : 0);
    }

    auto rewardItem = Ref(GJRewardItem::createWithObjects(
        static_cast<GJRewardType>(chest),
        array
    ));

    auto layer = RewardUnlockLayer::create(chest, nullptr);
    layer->m_noElasticity = true;
    layer->show();
    layer->runAction(CCSequence::create(
        CCDelayTime::create(1.1f),
        CCCallFunc::create(layer, callfunc_selector(RewardUnlockLayer::step2)),
        CCDelayTime::create(0.31f),
        CallFuncExt::create([layer, rewardItem] {
            layer->stopAllActions();
            layer->showCollectReward(rewardItem);
        }),
        nullptr
    ));
}

bool JamManager::didUnlockItemWithJam(GJStoreItem* item) {
    return didUnlockItemWithJam(item->m_index.value());
}

bool JamManager::didUnlockItemWithJam(UnlockType unlockType, int id) {
    auto saveObj = Mod::get()->getSavedValue<matjson::Value>("unlocked-items");
    
    for (const auto& obj : saveObj) {
        if (
            obj["id"].asInt().unwrapOr(0) == id
            && obj["unlock-type"].asInt().unwrapOr(0) == static_cast<int>(unlockType)
        ) {
            return true;
        }
    }

    return false;
}

bool JamManager::didUnlockItemWithJam(int index) {
    auto saveObj = Mod::get()->getSavedValue<matjson::Value>("unlocked-items");
    
    for (const auto& obj : saveObj) {
        if (
            obj["index"].asInt().unwrapOr(0) == index
        ) {
            return true;
        }
    }

    return false;
}

bool JamManager::isChestPurchased(int chest) {
    return Mod::get()->getSavedValue<bool>(fmt::format("chest-{}", chest));
}