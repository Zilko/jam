#pragma once

#include "../Includes.hpp"

class JamManager {

public:

    bool m_challengesPageShouldRewardJam = false;
    int m_challengesPageJamAmount = 0;

    bool m_currencyLayerShouldRewardJam = false;
    int m_currencyLayerJamAmount = 0;

    bool m_nextRewardUnlockLayerCanRewardJam = false;

    static JamManager& get();

    int getJam();

    void grabJam(int);
    
    void rewardJam(int);

    bool canLevelHaveJam(GJGameLevel*);

    int getTotalLevelJam(GJGameLevel*);

    float getLevelJamProgress(GJGameLevel*);

    void setLevelJamProgress(GJGameLevel*, float);

    int getLevelJamForProgress(GJGameLevel*, float);

    int getJamChestReward(int);

    int getTimeForRestock();

    int getTimeSinceLastRestock();

    void restockJamMarket();

    const std::array<GJStoreItem*, 4> getJamMarketItems();

    const std::array<GJStoreItem*, 4> getFakeJamMarketItems();

    int getItemJamPrice(GJStoreItem*);

    int getChestJamPrice(int);

    void unlockItem(UnlockType, int, int);

    void unlockItem(GJStoreItem*);

    void purchaseItem(GJStoreItem*);

    void purchaseChest(int);

    bool didUnlockItemWithJam(GJStoreItem*);

    bool didUnlockItemWithJam(UnlockType, int);

    bool didUnlockItemWithJam(int);

    bool isChestPurchased(int);

};