#include "../Includes.hpp"

#include <Geode/modify/LevelPage.hpp>

class $modify(ProLevelPage, LevelPage) {

    struct Fields {
        CCLabelBMFont* m_jamLabel = nullptr;
        CCSprite* m_jamIcon = nullptr;
		std::unordered_map<CCNode*, CCPoint> m_storedPositions;
    };

    static void onModify(auto& self) {
		(void)!self.setHookPriorityPre("LevelPage::updateDynamicPage", Priority::First);
	}

	CCPoint getNodePosition(CCNode*, const CCPoint&);

    void updateDynamicPage(GJGameLevel*);

};