#include "Utils.hpp"
#include "JamManager.hpp"

#include <random>

void Utils::setHookEnabled(const std::string& name, bool enabled) {
    for (Hook* hook : Mod::get()->getHooks()) {
        if (hook->getDisplayName() == name) {
            (void)(enabled ? hook->enable() : hook->disable());
            break;
        }
    }
}

int Utils::getRandomInt(int min, int max) {
    static std::mt19937 gen(std::random_device{}());

    std::uniform_int_distribution<> dis(min, max);
    
    return dis(gen);
}

void Utils::modifyRewardLayer(CurrencyRewardLayer* rewardLayer, CurrencySpriteType spriteType) {
    class FakeSpriteBatchNode : public CCSpriteBatchNode {

    public:

        void draw() override {
            CCNode::draw();
        }

        void visit() override {
            CCNode::visit();
        }
        
    };

    auto didPatch = false;
    
    for (auto obj : CCArrayExt<CurrencySprite*>(rewardLayer->m_objects)) {
        if (obj->m_spriteType != spriteType) {
            continue;
        }
        
        if (!didPatch) {            
            static void* vtable = []() -> void* {
                FakeSpriteBatchNode temp;
                return *(void**)&temp;
            }();    
            
            *(void**)obj->getParent() = vtable;
            
            didPatch = true;
        }
        
        auto icon = CCSprite::create("jam1.png"_spr);
        icon->setScale(0.47f);
        icon->setPosition(obj->getContentSize() / 2.f);
        
        obj->addChild(icon, -1);
        obj->setOpacity(0);
        
        if (obj->m_particleSystem) {
            obj->m_particleSystem->setStartColor(ccc4FFromccc4B({ 255, 0, 0, 255 }));
            obj->m_particleSystem->setEndColor(ccc4FFromccc4B({ 255, 96, 96, 255 }));
        }
        
        if (auto glow = obj->getChildByType<CCSprite>(0)) {
            glow->stopAllActions();
            glow->setOpacity(0);
            
            icon = CCSprite::create("jam1-glow.png"_spr);
            icon->setScale(0.47f);
            icon->setPosition(glow->getContentSize() / 2.f);
            icon->runAction(CCFadeOut::create(0.55f));
        
            glow->addChild(icon, -1);
        }
        
        if (!obj->m_burstSprite) {
            continue;
        }
        
        obj->m_burstSprite->setOpacity(0);
                    
        if (auto subSpr = obj->m_burstSprite->getChildByType<CCSprite>(0)) {
            subSpr->setColor({ 255, 58, 58 });
                        
            if (auto subSpr2 = subSpr->getChildByType<CCSprite>(0)) {
                subSpr2->setColor({ 255, 71, 71 });
            }
        }
    }
}

std::string Utils::getJamProgressString(GJGameLevel* level) {
    auto& jm = JamManager::get();

    return fmt::format(
        "{}/{}",
        jm.getLevelJamForProgress(level, jm.getLevelJamProgress(level)),
        jm.getTotalLevelJam(level)
    );
}

CCLabelBMFont* Utils::createJamProgressLabel(GJGameLevel* level, bool abbreviate) {
    auto& jm = JamManager::get();
    auto ret = CCLabelBMFont::create(getJamProgressString(level).c_str(), "bigFont.fnt");

    if (jm.getLevelJamProgress(level) >= 1.f) {
        ret->setColor({ 255, 216, 237 });

        if (abbreviate) {
            ret->setString(std::to_string(jm.getTotalLevelJam(level)).c_str());
        }
    }

    return ret;
}