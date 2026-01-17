#include "../Includes.hpp"

class JamMarketLayer : public CCLayer {

private:

    CCParticleSystemQuad* m_jamParticle = nullptr;

    CCSprite* m_jamIcon = nullptr;

    CCLabelBMFont* m_restockLabel = nullptr;
    CCLabelBMFont* m_jamLabel = nullptr;

    CCMenu* m_mainMenu = nullptr;

    std::array<GJStoreItem*, 4> m_shopItems = { nullptr, nullptr, nullptr, nullptr };
    std::array<CCMenuItemSpriteExtra*, 3> m_chests = { nullptr, nullptr, nullptr };
    std::unordered_map<GJStoreItem*, CCMenuItemSpriteExtra*> m_buttonForItem;

    bool m_closing = false;
    bool m_willRestock = false;

    bool init() override;

    void updateChests();

    void updateJamCount();

    void updateTimer(float);

    void tooExpensive(CCObject*);

    void onPurchase(CCObject*);

    void onPurchaseChest(CCObject*);

    void onBack(CCObject*);

    void keyBackClicked() override;

public:

    static CCScene* scene();

    void boughtItem(GJStoreItem*);

    void boughtChest(int);

};