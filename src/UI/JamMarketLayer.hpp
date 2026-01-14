#include "../Includes.hpp"

class JamMarketLayer : public CCLayer {

private:

    bool m_closing = false;

    bool init() override;

    void onBack(CCObject*);

    void keyBackClicked() override;

public:

    static CCScene* scene();

};