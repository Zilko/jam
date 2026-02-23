#include "../Includes.hpp"

class JamMarketLayer;

class JamPurchaseItemPopup : public Popup {

private:

    GJStoreItem* m_item = nullptr;

    JamMarketLayer* m_layer = nullptr;

    int m_chest = 0;

    bool m_isChest = false;

    JamPurchaseItemPopup(GJStoreItem* item, int chest, JamMarketLayer* layer)
        : m_item(item), m_chest(chest), m_layer(layer), m_isChest(item == nullptr) {}

    void onBuy(CCObject*);

    bool init() override;

public:

    static JamPurchaseItemPopup* create(GJStoreItem*, int, JamMarketLayer*);
    static JamPurchaseItemPopup* createWithItem(GJStoreItem*, JamMarketLayer*);
    static JamPurchaseItemPopup* createWithChest(int, JamMarketLayer*);

};