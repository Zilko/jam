#include "../Includes.hpp"

#include <Geode/modify/MenuGameLayer.hpp>

class $modify(ProMenuGameLayer, MenuGameLayer) {

    struct Fields : CurrencyRewardDelegate {

        CurrencyRewardLayer* m_rewardLayer = nullptr;

        void currencyWillExit(CurrencyRewardLayer*) override;

    };

    void destroyPlayer();

};