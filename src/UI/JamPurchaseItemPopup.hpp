#include "../Includes.hpp"

class JamMarketLayer;

class JamPurchaseItemPopup : public Popup<> {

private:

    GJStoreItem* m_item = nullptr;

    JamMarketLayer* m_layer = nullptr;

    int m_chest = 0;

    bool m_isChest = false;

    JamPurchaseItemPopup(GJStoreItem* item, JamMarketLayer* layer)
        : m_item(item), m_layer(layer) {}

    JamPurchaseItemPopup(int chest, JamMarketLayer* layer)
        : m_isChest(true), m_chest(chest), m_layer(layer) {}

    void onBuy(CCObject*);

    bool setup() override;

public:

    static JamPurchaseItemPopup* create(GJStoreItem*, JamMarketLayer*);

    static JamPurchaseItemPopup* create(int, JamMarketLayer*);

};