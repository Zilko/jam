#include "../Includes.hpp"

#include <Geode/modify/DailyLevelNode.hpp>

class $modify(ProDailyLevelNode, DailyLevelNode) {

    struct Fields {
        int m_jamReward = 0;
        bool m_isChest = false;

        ~Fields();
    };

    void onClaimReward(CCObject*);

    bool init(GJGameLevel*, DailyLevelPage*, bool);

};