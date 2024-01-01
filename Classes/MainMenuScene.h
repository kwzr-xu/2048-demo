#ifndef __MAIN_MENU_SCENE_H__
#define __MAIN_MENU_SCENE_H__

#include "cocos2d.h"
#include "Leaderboard.h" // �������а���


class MainMenuScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuStartCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuScene);

    void showLeaderboard();

private:
    Leaderboard* leaderboard; // ���а����ָ��
};

#endif // __MAIN_MENU_SCENE_H__