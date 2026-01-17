#pragma once

#include "../Includes.hpp"

class Utils {

public:

    static int getRandomInt(int, int);

    static void modifyRewardLayer(CurrencyRewardLayer*, CurrencySpriteType);

    static std::string getJamProgressString(GJGameLevel*);

    static CCLabelBMFont* createJamProgressLabel(GJGameLevel*, bool = false);
    
    static std::string getTimeLeftString(int);

};