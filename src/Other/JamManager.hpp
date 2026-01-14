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

    void rewardJam(int);

    bool canLevelHaveJam(GJGameLevel*);

    int getTotalLevelJam(GJGameLevel*);

    float getLevelJamProgress(GJGameLevel*);

    void setLevelJamProgress(GJGameLevel*, float);

    int getLevelJamForProgress(GJGameLevel*, float);

    int getJamChestReward(int);

};