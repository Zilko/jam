#include "CurrencyRewardLayer.hpp"

#include "../Other/JamManager.hpp"
#include "../Other/Utils.hpp"

CurrencyRewardLayer* ProCurrencyRewardLayer::create(int orbs, int stars, int moons, int diamonds, CurrencySpriteType demonKey, int keyCount, CurrencySpriteType shardType, int shardsCount, CCPoint position, CurrencyRewardType rewardType, float yOffset, float time) {
    auto& jm = JamManager::get();
    auto isStars = false;

    if (jm.m_currencyLayerShouldRewardJam && jm.m_currencyLayerJamAmount > 0) {
        if (stars > 0 && shardsCount <= 0) {
            shardsCount = jm.m_currencyLayerJamAmount;
            shardType = CurrencySpriteType::FireShard;
        } else if (stars <= 0) {
            isStars = true;
            stars = jm.m_currencyLayerJamAmount;
        } else {
            jm.m_currencyLayerShouldRewardJam = false;
        }
    } else {
        jm.m_currencyLayerShouldRewardJam = false;
    }

    auto ret = CurrencyRewardLayer::create(orbs, stars, moons, diamonds, demonKey, keyCount, shardType, shardsCount, position, rewardType, yOffset, time);

    if (!jm.m_currencyLayerShouldRewardJam) {
        return ret;
    }

    jm.m_currencyLayerShouldRewardJam = false;

    CCSprite* sprite = nullptr;
    CCLabelBMFont* label = nullptr;
    CurrencySpriteType spriteType;
    int jam = jm.getJam() - jm.m_currencyLayerJamAmount;

    if (isStars) {
        sprite = ret->m_starsSprite;
        label = ret->m_starsLabel;
        spriteType = CurrencySpriteType::Star;
        ret->m_stars = jam;
    } else {
        sprite = ret->m_shardsSprite;
        label = ret->m_shardsLabel;
        spriteType = CurrencySpriteType::FireShard;
        ret->m_shards = jam;
    }

    label->stopAllActions();
    label->setColor({255, 255, 255});
    label->setString(std::to_string(jam).c_str());
    
    for (auto particle : ret->m_mainNode->getChildrenExt<CCParticleSystemQuad*>()) {
        if (!typeinfo_cast<CCParticleSystemQuad*>(particle)) {
            continue;
        }
        
        if (ccpDistance(particle->getPosition(), sprite->getPosition()) <= 1.f) {
            particle->setVisible(false);
            break;
        }
    }
    
    auto newParticle = GameToolbox::particleFromString("8a-1a1a0.2a30a0a360a0a10a12a10a0a0a0a0a0a0a2a1a0a0a0.960784a0a0.258824a0a0.258824a0a1a0a1a1a0a0a1a0a0.670588a0a0.670588a0a1a0a0a0a0a0a0a0a0a0a0a0a0a2a1a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0a0", nullptr, false);
    newParticle->setPosition(sprite->getPosition());
    
    ret->m_mainNode->addChild(newParticle);
    
    auto icon = CCSprite::create("jam2.png"_spr);
    
    sprite->setTexture(icon->getTexture());
    sprite->setTextureRect(icon->getTextureRect());
    sprite->setScale(sprite->getScale());
    
    Utils::modifyRewardLayer(ret, spriteType);

    return ret;
}