#include "GameScene.h"
#include "Card.h"
#include "MainMenuScene.h"
#include "Leaderboard.h"

USING_NS_CC;

// 创建游戏场景
Scene* GameScene::createScene() {
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}



// 初始化场景
bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 设置触摸事件监听
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    // 加入游戏背景
    auto layerColorBG = LayerColor::create(Color4B(180, 170, 160, 255));
    this->addChild(layerColorBG);

    // 创建分数标签
    auto scoreTextLabel = Label::createWithSystemFont("sorce :", "fonts/Marker Felt.ttf", 70);
    scoreTextLabel->setPosition(Vec2(visibleSize.width / 2 + 340 ,
        visibleSize.height / 2 + 300));
    this->addChild(scoreTextLabel);

    cardNumberTTF = Label::createWithSystemFont("0", "Arial", 70);
    cardNumberTTF->setPosition(Vec2(visibleSize.width / 2 + 340, visibleSize.height / 2 + 130));
    addChild(cardNumberTTF);


    auto returnLabel = Label::createWithTTF("Return", "fonts/Marker Felt.ttf", 24);
    auto returnItem = MenuItemLabel::create(
        returnLabel,
        CC_CALLBACK_1(GameScene::menuReturnCallback, this));
    returnItem->setPosition(Vec2(origin.x + returnItem->getContentSize().width,
        origin.y + visibleSize.height - returnItem->getContentSize().height));

    auto menu = Menu::create(returnItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // 创建4x4的卡片
    createCardSprite(visibleSize);

    // 初始时生成两个数字为2的卡片
    createCardNumber();
    createCardNumber();

    return true;
}

void GameScene::menuReturnCallback(Ref* pSender) {
    // 返回主界面
    auto mainMenuScene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, mainMenuScene));
}


// 根据屏幕大小创建卡片
void GameScene::createCardSprite(Size size) {
    int cardSize = static_cast<int>((size.height - 36) / 4);

    // 初始化二维vector的大小
    cardArr.resize(4, std::vector<CardSprite*>(4, nullptr));

    // 绘制4x4的单元格
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            CardSprite* card = CardSprite::createCardSprite(0, cardSize, cardSize, static_cast<float>(cardSize * i + 80), static_cast<float>(cardSize * j + 20));
            cardArr[i][j] = card;
            addChild(card);
        }
    }
}

// 触摸事件处理
bool GameScene::onTouchBegan(Touch* touch, Event* event) {
    Point beginTouch = touch->getLocation();
    beginX = static_cast<int>(beginTouch.x);
    beginY = static_cast<int>(beginTouch.y);
    return true;
}

void GameScene::onTouchEnded(Touch* touch, Event* event) {
    Point endTouch = touch->getLocation();
    endX = static_cast<int>(endTouch.x - beginX);
    endY = static_cast<int>(endTouch.y - beginY);

    // 设定一个阈值，只有滑动距离超过这个阈值时，才判定为有效滑动
    const int swipeThreshold = 10; // 这个值可以根据需要调整

    // 判断是否有有效的滑动发生
    if (abs(endX) > swipeThreshold || abs(endY) > swipeThreshold) {
        if (abs(endX) > abs(endY)) {
            if (endX > 0) {
                
                doRight();
            }
            else {
                doLeft();
            }
        }
        else {
            if (endY > 0) {
                doUp();
            }
            else {
               
                doDown();
            }
        }
        createCardNumber();
        doCheck();
        setScore(score);
    }
    // 如果没有有效的滑动，不执行任何操作
}

// 创建随机卡片
void GameScene::createCardNumber() {
    std::vector<std::pair<int, int>> emptyPositions;

    // 查找所有空位
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (cardArr[i][j]->getNumber() == 0) {
                emptyPositions.emplace_back(i, j);
            }
        }
    }

    // 如果没有空位，就不创建新卡片
    if (emptyPositions.empty()) {
        return;
    }

    // 随机选择一个空位
    int randIndex = static_cast<int>(CCRANDOM_0_1() * emptyPositions.size());
    int i = emptyPositions[randIndex].first;
    int j = emptyPositions[randIndex].second;

    cardArr[i][j]->setNumber(CCRANDOM_0_1() * 10 < 1 ? 4 : 2);
}

// 检测游戏是否结束
void GameScene::doCheck() {
    bool isGameOver = true;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (cardArr[x][y]->getNumber() == 0 ||
                (x < 3 && cardArr[x][y]->getNumber() == cardArr[x + 1][y]->getNumber()) ||
                (x > 0 && cardArr[x][y]->getNumber() == cardArr[x - 1][y]->getNumber()) ||
                (y < 3 && cardArr[x][y]->getNumber() == cardArr[x][y + 1]->getNumber()) ||
                (y > 0 && cardArr[x][y]->getNumber() == cardArr[x][y - 1]->getNumber())) {
                isGameOver = false;
            }
        }
    }
    if (isGameOver) {
        auto leaderboard = new Leaderboard("scores.txt");
        std::string playerId = std::to_string(time(nullptr)); // 使用结束时间作为玩家ID
        leaderboard->addScore(playerId, score);
        Director::getInstance()->replaceScene(TransitionFade::create(1,MainMenuScene::createScene()));
    }
}

// 滑动操作的实现 - 向左滑动
bool GameScene::doLeft() {
    bool isMove = false;
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            for (int x1 = x + 1; x1 < 4; x1++) {
                if (cardArr[x1][y]->getNumber() > 0) {
                    if (cardArr[x][y]->getNumber() == 0) {
                        cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
                        cardArr[x1][y]->setNumber(0);
                        x--;
                        isMove = true;
                    }
                    else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()) {
                        auto scaleUp = ScaleTo::create(0.1f, 1.2f);
                        auto scaleDown = ScaleTo::create(0.1f, 1.0f);
                        auto sequence = Sequence::create(scaleUp, scaleDown, nullptr);
                        cardArr[x][y]->runAction(sequence);
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
                        cardArr[x1][y]->setNumber(0);
                        score += cardArr[x][y]->getNumber();
                        isMove = true;
                    }
                    break;
                }
            }
        }
    }
    return isMove;
}


bool GameScene::doRight() {
    bool isMove = false;
    for (int y = 0; y < 4; y++) {
        for (int x = 3; x >= 0; x--) {
            for (int x1 = x - 1; x1 >= 0; x1--) {
                if (cardArr[x1][y]->getNumber() > 0) {
                    if (cardArr[x][y]->getNumber() == 0) {
                        cardArr[x][y]->setNumber(cardArr[x1][y]->getNumber());
                        cardArr[x1][y]->setNumber(0);
                        x++;
                        isMove = true;
                    }
                    else if (cardArr[x][y]->getNumber() == cardArr[x1][y]->getNumber()) {
                        auto scaleUp = ScaleTo::create(0.1f, 1.2f);
                        auto scaleDown = ScaleTo::create(0.1f, 1.0f);
                        auto sequence = Sequence::create(scaleUp, scaleDown, nullptr);
                        cardArr[x][y]->runAction(sequence);
                        cardArr[x][y]->setNumber(cardArr[x][y]->getNumber() * 2);
                        cardArr[x1][y]->setNumber(0);
                        score += cardArr[x][y]->getNumber();
                        isMove = true;
                    }
                    break;
                }
            }
        }
    }
    return isMove;
}

bool GameScene::doDown() {
    bool isMove = false;  // 定义一个布尔变量 isMove，用于记录是否有卡片移动或合并

    for (auto& row : cardArr) {  // 遍历 cardArr 的每一行，row 是对 cardArr 中每一行的引用
        for (auto it = row.begin(); it != row.end(); ++it) {  // 遍历每一行中的元素，it 是行中元素的迭代器
            auto next = std::find_if(it + 1, row.end(), [](CardSprite* card) {
                return card->getNumber() > 0;  // 使用 std::find_if 查找当前元素之后第一个数字不为0的卡片
                });

            if (next != row.end()) {  // 如果找到了这样的卡片（即 next 不等于行尾）
                if ((*it)->getNumber() == 0) {  // 检查当前卡片的数字是否为0
                    (*it)->setNumber((*next)->getNumber());  // 将下一个不为0的卡片的数字复制到当前卡片
                    (*next)->setNumber(0);  // 将那个卡片的数字设置为0
                    isMove = true;  // 设置 isMove 为 true，表示发生了移动
                    if (it != row.begin()) {  // 检查当前迭代器是否不指向行的开始
                        it--;  // 如果不是，则将迭代器向后移动一位，以重新检查当前位置
                    }
                }
                else if ((*it)->getNumber() == (*next)->getNumber()) {  // 检查当前卡片和下一个卡片的数字是否相等
                    auto scaleUp = ScaleTo::create(0.1f, 1.2f);  // 创建一个放大动作
                    auto scaleDown = ScaleTo::create(0.1f, 1.0f);  // 创建一个缩小动作
                    auto sequence = Sequence::create(scaleUp, scaleDown, nullptr);  // 将这两个动作合成一个序列
                    (*it)->runAction(sequence);  // 让当前卡片执行这个动作序列
                    (*it)->setNumber((*it)->getNumber() * 2);  // 将当前卡片的数字翻倍
                    (*next)->setNumber(0);  // 将下一个卡片的数字设置为0
                    score += (*it)->getNumber();  // 更新分数
                    isMove = true;  // 设置 isMove 为 true，表示发生了移动或合并
                }
            }
        }
    }

    return isMove;  // 返回 isMove，表示是否有卡片移动或合并
}


bool GameScene::doUp() {
    bool isMove = false;

    for (auto& row : cardArr) {
        for (auto it = row.rbegin(); it != row.rend(); ++it) {
            auto next = std::find_if(std::next(it), row.rend(), [](CardSprite* card) {
                return card->getNumber() > 0;
                });

            if (next != row.rend()) {
                if ((*it)->getNumber() == 0) {
                    (*it)->setNumber((*next)->getNumber());
                    (*next)->setNumber(0);
                    isMove = true;
                    if (it != row.rbegin()) {
                        it--;
                    }
                }
                else if ((*it)->getNumber() == (*next)->getNumber()) {
                    auto scaleUp = ScaleTo::create(0.1f, 1.2f);
                    auto scaleDown = ScaleTo::create(0.1f, 1.0f);
                    auto sequence = Sequence::create(scaleUp, scaleDown, nullptr);
                    (*it)->runAction(sequence);
                    (*it)->setNumber((*it)->getNumber() * 2);
                    (*next)->setNumber(0);
                    score += (*it)->getNumber();
                    isMove = true;
                }
            }
        }
    }

    return isMove;
}

// 设置分数
void GameScene::setScore(int score) {
    this->score = score;
    cardNumberTTF->setString(std::to_string(score));
}