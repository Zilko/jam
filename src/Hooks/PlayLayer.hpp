#include "../Includes.hpp"

#include <Geode/modify/PlayLayer.hpp>

class $modify(ProPlayLayer, PlayLayer) {

    struct Fields {
        bool m_didShowNewBest = false;
        int m_jamReward = 0;
    };
	
	static void onModify(auto& self) {
        (void)self.setHookPriorityPre("PlayLayer::destroyPlayer", Priority::Last + 21903809);
    }

    CCPoint getPlayerScreenPos();

    void addRewardLayer();
    
    void modifyJamReward(int);
    
    void addJamReward(int);

    void showNewBest(bool, int, int, bool, bool, bool);

	void destroyPlayer(PlayerObject*, GameObject*);

    void resetLevel();

    void levelComplete();
	
};