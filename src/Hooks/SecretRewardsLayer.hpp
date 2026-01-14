#include "../Includes.hpp"

#include <Geode/modify/SecretRewardsLayer.hpp>

class $modify(ProSecretRewardsLayer, SecretRewardsLayer) {

    void onJamMarket(CCObject*);

    void onSelectItem(CCObject*);

    bool init(bool);

};