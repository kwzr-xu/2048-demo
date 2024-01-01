#ifndef _CARD_H_
#define _CARD_H_

#include "cocos2d.h"

USING_NS_CC;

// CardSprite 类表示2048游戏中的一个卡片
class CardSprite : public Sprite {
public:
    // 静态创建方法，用于创建卡片精灵
    static CardSprite* createCardSprite(int number, int width, int height, float CardSpriteX, float CardSpriteY);

    // 初始化方法
    virtual bool init();

    // 获取卡片上显示的数字
    int getNumber();

    // 设置卡片上显示的数字
    void setNumber(int num);

private:
    // 卡片上显示的数字
    int number;

    // 初始化卡片精灵
    void enemyInit(int number, int width, int height, float CardSpriteX, float CardSpriteY);

    // 显示数字的Label控件
    Label* labelCardNumber;

    // 卡片的背景层
    LayerColor* colorBackground;
};

#endif // _CARD_H_
