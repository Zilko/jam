#include "GJGarageLayer.hpp"

#include "../Other/JamManager.hpp"

#include <capeling.garage-stats-menu/include/StatsDisplayAPI.h>

bool ProGJGarageLayer::init() {
	if (!GJGarageLayer::init()) {
		return false;
	}
	
	auto statMenu = this->getChildByID("capeling.garage-stats-menu/stats-menu");
	
	if (!statMenu) {
	    return true;
	}
	
	auto myStatItem = StatsDisplayAPI::getNewItem(
    	"jam"_spr,
    	CCSprite::create("jam1.png"_spr),
    	JamManager::get().getJam(),
    	0.17f
	);

	statMenu->addChild(myStatItem);
	statMenu->updateLayout();

	return true;
}