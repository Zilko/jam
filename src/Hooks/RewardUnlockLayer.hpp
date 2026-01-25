#include "../Includes.hpp"

#include <Geode/modify/RewardUnlockLayer.hpp>

class $modify(ProRewardUnlockLayer, RewardUnlockLayer) {

    struct Fields {
        ~Fields();
    };

    void realStep3();

    void step2();

};