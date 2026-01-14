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
	
	statMenu->updateLayout();
	
	auto moonZ = 0;
	auto didSetMoons = false;
	auto didSetZ = false;
	auto prevZ = 0;
	
	for (auto o : statMenu->getChildrenExt<CCNode*>()) {
		if (didSetZ && o->getZOrder() <= prevZ) {
			o->setZOrder(prevZ + 1);
		}

		didSetZ = true;
		prevZ = o->getZOrder();
		
		if (!didSetMoons && o->getID() == "moons-container") {
			moonZ = ++prevZ;
		}
	}
	
	auto myStatItem = StatsDisplayAPI::getNewItem(
    	"jam"_spr,
    	CCSprite::create("jam1.png"_spr),
    	JamManager::get().getJam(),
    	0.17f
	);
	myStatItem->setZOrder(moonZ);

	statMenu->addChild(myStatItem);
	statMenu->updateLayout();

	return true;
}