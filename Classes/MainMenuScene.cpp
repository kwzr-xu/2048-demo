#include "MainMenuScene.h"
#include "GameScene.h" // 游戏场景头文件
#include "Leaderboard.h" // 游戏场景头文件


USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    scene->addChild(layer);
    return scene;
}

bool MainMenuScene::init()
{
    if (!Layer::init())
    {
        return false;
    }


    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建标题
    auto label = Label::createWithTTF("2048", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 1);

    // 创建开始按钮
    auto startLabel = Label::createWithTTF("Start", "fonts/Marker Felt.ttf", 24);
    auto startItem = MenuItemLabel::create(
        startLabel,
        CC_CALLBACK_1(MainMenuScene::menuStartCallback, this));
    startItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2));

    auto menu = Menu::create(startItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // 创建名字标签
    auto nameLabel = Label::createWithTTF("kwzr xcw", "fonts/Marker Felt.ttf", 24);
    nameLabel->setPosition(Vec2(origin.x + visibleSize.width - nameLabel->getContentSize().width,
        origin.y + nameLabel->getContentSize().height));
    this->addChild(nameLabel, 1);

    // 创建排行榜实例
    leaderboard = new Leaderboard("scores.txt");

    // 显示排行榜
    showLeaderboard();

    return true;
}

void MainMenuScene::menuStartCallback(Ref* pSender)
{
    auto gameScene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, gameScene, Color3B(0, 255, 255)));
}


void MainMenuScene::showLeaderboard() {
    auto topScores = leaderboard->getTopScores(5); // 获取前5名

    // 显示排行榜
    for (size_t i = 0; i < topScores.size(); ++i) {
        auto label = Label::createWithTTF(std::to_string(i + 1) + ". " + topScores[i].playerId + " - " + std::to_string(topScores[i].score),
            "fonts/Marker Felt.ttf", 24);
        label->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height - (i * 30 + 100)));
        this->addChild(label);
    }
}