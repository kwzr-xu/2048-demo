#ifndef _CARD_H_
#define _CARD_H_

#include "cocos2d.h"

USING_NS_CC;

// CardSprite ���ʾ2048��Ϸ�е�һ����Ƭ
class CardSprite : public Sprite {
public:
    // ��̬�������������ڴ�����Ƭ����
    static CardSprite* createCardSprite(int number, int width, int height, float CardSpriteX, float CardSpriteY);

    // ��ʼ������
    virtual bool init();

    // ��ȡ��Ƭ����ʾ������
    int getNumber();

    // ���ÿ�Ƭ����ʾ������
    void setNumber(int num);

private:
    // ��Ƭ����ʾ������
    int number;

    // ��ʼ����Ƭ����
    void enemyInit(int number, int width, int height, float CardSpriteX, float CardSpriteY);

    // ��ʾ���ֵ�Label�ؼ�
    Label* labelCardNumber;

    // ��Ƭ�ı�����
    LayerColor* colorBackground;
};

#endif // _CARD_H_
