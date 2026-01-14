#pragma once

#include "../Includes.hpp"

class Utils {

public:

    static void setHookEnabled(const std::string&, bool);

    static int getRandomInt(int, int);

    static void modifyRewardLayer(CurrencyRewardLayer*, CurrencySpriteType);

    static std::string getJamProgressString(GJGameLevel*);

    static CCLabelBMFont* createJamProgressLabel(GJGameLevel*, bool = false);
    

};