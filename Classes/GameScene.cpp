#include "GameScene.h"
#include "Card.h"
#include "MainMenuScene.h"
#include "Leaderboard.h"

USING_NS_CC;

// ������Ϸ����
Scene* GameScene::createScene() {
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}



// ��ʼ������
bool GameScene::init() {
    if (!Layer::init()) {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ���ô����¼�����
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    // ������Ϸ����
    auto layerColorBG = LayerColor::create(Color4B(180, 170, 160, 255));
    this->addChild(layerColorBG);

    // ����������ǩ
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

    // ����4x4�Ŀ�Ƭ
    createCardSprite(visibleSize);

    // ��ʼʱ������������Ϊ2�Ŀ�Ƭ
    createCardNumber();
    createCardNumber();

    return true;
}

void GameScene::menuReturnCallback(Ref* pSender) {
    // ����������
    auto mainMenuScene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, mainMenuScene));
}


// ������Ļ��С������Ƭ
void GameScene::createCardSprite(Size size) {
    int cardSize = static_cast<int>((size.height - 36) / 4);

    // ��ʼ����άvector�Ĵ�С
    cardArr.resize(4, std::vector<CardSprite*>(4, nullptr));

    // ����4x4�ĵ�Ԫ��
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            CardSprite* card = CardSprite::createCardSprite(0, cardSize, cardSize, static_cast<float>(cardSize * i + 80), static_cast<float>(cardSize * j + 20));
            cardArr[i][j] = card;
            addChild(card);
        }
    }
}

// �����¼�����
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

    // �趨һ����ֵ��ֻ�л������볬�������ֵʱ�����ж�Ϊ��Ч����
    const int swipeThreshold = 10; // ���ֵ���Ը�����Ҫ����

    // �ж��Ƿ�����Ч�Ļ�������
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
    // ���û����Ч�Ļ�������ִ���κβ���
}

// ���������Ƭ
void GameScene::createCardNumber() {
    std::vector<std::pair<int, int>> emptyPositions;

    // �������п�λ
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (cardArr[i][j]->getNumber() == 0) {
                emptyPositions.emplace_back(i, j);
            }
        }
    }

    // ���û�п�λ���Ͳ������¿�Ƭ
    if (emptyPositions.empty()) {
        return;
    }

    // ���ѡ��һ����λ
    int randIndex = static_cast<int>(CCRANDOM_0_1() * emptyPositions.size());
    int i = emptyPositions[randIndex].first;
    int j = emptyPositions[randIndex].second;

    cardArr[i][j]->setNumber(CCRANDOM_0_1() * 10 < 1 ? 4 : 2);
}

// �����Ϸ�Ƿ����
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
        std::string playerId = std::to_string(time(nullptr)); // ʹ�ý���ʱ����Ϊ���ID
        leaderboard->addScore(playerId, score);
        Director::getInstance()->replaceScene(TransitionFade::create(1,MainMenuScene::createScene()));
    }
}

// ����������ʵ�� - ���󻬶�
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
    bool isMove = false;  // ����һ���������� isMove�����ڼ�¼�Ƿ��п�Ƭ�ƶ���ϲ�

    for (auto& row : cardArr) {  // ���� cardArr ��ÿһ�У�row �Ƕ� cardArr ��ÿһ�е�����
        for (auto it = row.begin(); it != row.end(); ++it) {  // ����ÿһ���е�Ԫ�أ�it ������Ԫ�صĵ�����
            auto next = std::find_if(it + 1, row.end(), [](CardSprite* card) {
                return card->getNumber() > 0;  // ʹ�� std::find_if ���ҵ�ǰԪ��֮���һ�����ֲ�Ϊ0�Ŀ�Ƭ
                });

            if (next != row.end()) {  // ����ҵ��������Ŀ�Ƭ���� next ��������β��
                if ((*it)->getNumber() == 0) {  // ��鵱ǰ��Ƭ�������Ƿ�Ϊ0
                    (*it)->setNumber((*next)->getNumber());  // ����һ����Ϊ0�Ŀ�Ƭ�����ָ��Ƶ���ǰ��Ƭ
                    (*next)->setNumber(0);  // ���Ǹ���Ƭ����������Ϊ0
                    isMove = true;  // ���� isMove Ϊ true����ʾ�������ƶ�
                    if (it != row.begin()) {  // ��鵱ǰ�������Ƿ�ָ���еĿ�ʼ
                        it--;  // ������ǣ��򽫵���������ƶ�һλ�������¼�鵱ǰλ��
                    }
                }
                else if ((*it)->getNumber() == (*next)->getNumber()) {  // ��鵱ǰ��Ƭ����һ����Ƭ�������Ƿ����
                    auto scaleUp = ScaleTo::create(0.1f, 1.2f);  // ����һ���Ŵ���
                    auto scaleDown = ScaleTo::create(0.1f, 1.0f);  // ����һ����С����
                    auto sequence = Sequence::create(scaleUp, scaleDown, nullptr);  // �������������ϳ�һ������
                    (*it)->runAction(sequence);  // �õ�ǰ��Ƭִ�������������
                    (*it)->setNumber((*it)->getNumber() * 2);  // ����ǰ��Ƭ�����ַ���
                    (*next)->setNumber(0);  // ����һ����Ƭ����������Ϊ0
                    score += (*it)->getNumber();  // ���·���
                    isMove = true;  // ���� isMove Ϊ true����ʾ�������ƶ���ϲ�
                }
            }
        }
    }

    return isMove;  // ���� isMove����ʾ�Ƿ��п�Ƭ�ƶ���ϲ�
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

// ���÷���
void GameScene::setScore(int score) {
    this->score = score;
    cardNumberTTF->setString(std::to_string(score));
}