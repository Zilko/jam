#include "../Includes.hpp"

#include <Geode/modify/GJGarageLayer.hpp>

class $modify(ProGJGarageLayer, GJGarageLayer) {
	
	static void onModify(auto& self) {
		(void)!self.setHookPriorityPre("GJGarageLayer::init", Priority::First);
	}
    
    bool init();
    
};