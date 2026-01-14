#include "../Includes.hpp"

#include <Geode/modify/ChallengeNode.hpp>

class $modify(ProChallengeNode, ChallengeNode) {

    struct Fields {
        int m_jamReward = 0;
    };

    void onClaimReward(CCObject*);

    bool init(GJChallengeItem*, ChallengesPage*, bool);

};