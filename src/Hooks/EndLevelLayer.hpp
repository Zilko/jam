#include "../Includes.hpp"

#include <Geode/modify/EndLevelLayer.hpp>

class $modify(ProEndLevelLayer, EndLevelLayer) {
	
	struct Fields {
		int m_jamReward = 0;
		CCNode* m_jamContainer = nullptr;
	};

	void applyScaled(CCNode*, CCNode*, CCNode*, CCNode*, CCNode*, CCNode*, float, float);
	
	void addRewardLayer();

	void showLayer(bool);

	void playEndEffect();
	
};