#include "MenuGameLayer.hpp"

#include "../Other/JamManager.hpp"
#include "../Other/Utils.hpp"

void ProMenuGameLayer::Fields::currencyWillExit(CurrencyRewardLayer*) {
    m_rewardLayer = nullptr;
}

void ProMenuGameLayer::destroyPlayer() {
    if (!m_playerObject) {
        return MenuGameLayer::destroyPlayer();
    }

    auto pos = m_playerObject->convertToWorldSpaceAR({0, 0});

    MenuGameLayer::destroyPlayer();

    if (!MenuLayer::get()) {
        return;
    }

    auto& jm = JamManager::get();
    auto f = m_fields.self();

    jm.rewardJam(1);

    if (f->m_rewardLayer) {
        f->m_rewardLayer->createObjectsFull(
            CurrencySpriteType::Star, 
            1,
            nullptr,
            pos,
            0.f
        );

        Utils::modifyRewardLayer(f->m_rewardLayer, CurrencySpriteType::Star);

        return;
    }

    jm.m_currencyLayerShouldRewardJam = true;
	jm.m_currencyLayerJamAmount = 1;

    f->m_rewardLayer = CurrencyRewardLayer::create(
		0,
		0,
		0,
		0,
		CurrencySpriteType::Icon,
		0,
		CurrencySpriteType::Icon,
		0,
		pos,
		CurrencyRewardType::Default,
		0.f,
		2.f
	);
	f->m_rewardLayer->setID("jam-reward-layer"_spr);
	f->m_rewardLayer->m_delegate = f;

	MenuLayer::get()->addChild(f->m_rewardLayer, 100);

	jm.m_currencyLayerShouldRewardJam = false;
}