#include "../Includes.hpp"

#include <Geode/modify/LevelInfoLayer.hpp>

class $modify(ProLevelInfoLayer, LevelInfoLayer) {

	struct Fields {
		CCLabelBMFont* m_jamLabel = nullptr;
		CCSprite* m_jamIcon = nullptr;
		CCNode* m_jamContainer = nullptr;
		std::unordered_map<CCNode*, float> m_storedScales;
		bool m_firstTime = true;
	};
	
	float getNodeScale(CCNode*);
	
	void updateJam();
	
	bool init(GJGameLevel*, bool);
	
	void levelDownloadFinished(GJGameLevel*);
	
	void levelDownloadFailed(int);
	
	void levelUpdateFinished(GJGameLevel*, UpdateResponse);
	
	void levelUpdateFailed(int);
	
	void levelDeleteFinished(int);
	
	void levelDeleteFailed(int);

	void updateLabelValues();
	
	void updateSideButtons();
	
	void likedItem(LikeItemType, int, bool);
	
};