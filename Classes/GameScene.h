#ifndef _GAME_SCENE_H
#define _GAME_SCENE_H

#include "cocos2d.h"
#include "Card.h"
#include <vector> 
#include "MainMenuScene.h"

// GameScene �࣬��ʾ2048��Ϸ��������
class GameScene : public cocos2d::Layer {
public:
    // ������Ϸ�����ľ�̬����
    static cocos2d::Scene* createScene();

    // ��ʼ�������ķ���
    virtual bool init();

    // �����¼������ص�����
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    // ��ͣ���ָ��ͷ������˵��Ļص���������
    void menuReturnCallback(cocos2d::Ref* pSender);

    cocos2d::EventListenerTouchOneByOne* touchListener; 

    // ʵ��CREATE_FUNC���Ա���ٴ����ͳ�ʼ������
    CREATE_FUNC(GameScene);

private:

    
    std::vector<std::vector<CardSprite*>> cardArr; // ��ά�������洢��Ƭ
    // ������Ļ��С������Ƭ
    void createCardSprite(cocos2d::Size size);

    // ������ʼ�ͽ���ʱ������
    int beginX, beginY, endX, endY;

    // ��������
    bool doLeft();
    bool doRight();
    bool doUp();
    bool doDown();
    // ��ǰ����
    int score;

    // ��ʾ�����Ŀؼ�
    cocos2d::Label* cardNumberTTF;

    // �������������Ƭ
    void createCardNumber();

    // �����Ϸ�Ƿ����
    void doCheck();

    // ���÷���
    void setScore(int score);
};

#endif // _GAME_SCENE_H