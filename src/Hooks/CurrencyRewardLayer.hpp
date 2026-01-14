#include "../Includes.hpp"

#include <Geode/modify/CurrencyRewardLayer.hpp>

class $modify(ProCurrencyRewardLayer, CurrencyRewardLayer) {
  
    static CurrencyRewardLayer* create(int, int, int, int, CurrencySpriteType, int, CurrencySpriteType, int, CCPoint, CurrencyRewardType, float, float);

};