#include "../Includes.hpp"

#include <Geode/modify/LevelListLayer.hpp>

class $modify(ProLevelListLayer, LevelListLayer) {

    struct Fields {
        int m_jamReward = 0;
    };

    void onClaimReward(CCObject*);

    bool init(GJLevelList*);

};