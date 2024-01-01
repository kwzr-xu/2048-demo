#ifndef _GAME_SCENE_H
#define _GAME_SCENE_H

#include "cocos2d.h"
#include "Card.h"
#include <vector> 
#include "MainMenuScene.h"

// GameScene 类，表示2048游戏的主场景
class GameScene : public cocos2d::Layer {
public:
    // 创建游戏场景的静态方法
    static cocos2d::Scene* createScene();

    // 初始化场景的方法
    virtual bool init();

    // 触摸事件监听回调函数
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    // 暂停、恢复和返回主菜单的回调函数声明
    void menuReturnCallback(cocos2d::Ref* pSender);

    cocos2d::EventListenerTouchOneByOne* touchListener; 

    // 实现CREATE_FUNC宏以便快速创建和初始化对象
    CREATE_FUNC(GameScene);

private:

    
    std::vector<std::vector<CardSprite*>> cardArr; // 二维向量来存储卡片
    // 根据屏幕大小创建卡片
    void createCardSprite(cocos2d::Size size);

    // 触摸开始和结束时的坐标
    int beginX, beginY, endX, endY;

    // 滑动操作
    bool doLeft();
    bool doRight();
    bool doUp();
    bool doDown();
    // 当前分数
    int score;

    // 显示分数的控件
    cocos2d::Label* cardNumberTTF;

    // 创建生成随机卡片
    void createCardNumber();

    // 检测游戏是否结束
    void doCheck();

    // 设置分数
    void setScore(int score);
};

#endif // _GAME_SCENE_H