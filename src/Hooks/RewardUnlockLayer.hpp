#include "../Includes.hpp"

#include <Geode/modify/RewardUnlockLayer.hpp>

class $modify(ProRewardUnlockLayer, RewardUnlockLayer) {

    struct Fields {
        bool m_doLookout = false;
        bool m_foundRewardItem = false;

        ~Fields();
    };

    void realStep3();

    void calculateJam();

    void updateLookout(float);

    bool init(int, RewardsPage*);

};