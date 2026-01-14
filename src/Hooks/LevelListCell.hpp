#include "../Includes.hpp"

#include <Geode/modify/LevelListCell.hpp>

class $modify(ProLevelListCell, LevelListCell) {

    struct Fields {
        int m_jamReward = 0;
    };

    void loadFromList(GJLevelList*);

};