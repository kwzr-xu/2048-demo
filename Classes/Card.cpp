#include "Card.h"

USING_NS_CC;

// 创建一个卡片精灵的静态方法
CardSprite* CardSprite::createCardSprite(int number, int width, int height, float CardSpriteX, float CardSpriteY) {
	CardSprite* card = new CardSprite();
	if (card && card->init()) {
		card->autorelease();
		card->enemyInit(number, width, height, CardSpriteX, CardSpriteY);
		return card;
	}
	CC_SAFE_DELETE(card);
	return nullptr;
}

// 卡片精灵的初始化方法
bool CardSprite::init() {
	if (!Sprite::init()) {
		return false;
	}
	return true;
}

// 获取卡片上显示的数字
int CardSprite::getNumber() {
	return number;
}

// 设置卡片上显示的数字，并根据数字调整字体大小和颜色
void CardSprite::setNumber(int num) {
	number = num;

	// 更新显示的数字
	if (num > 0) {
		labelCardNumber->setString(std::to_string(number));
	}
	else {
		labelCardNumber->setString("");
	}

	// 根据数字大小设置字体大小
	if (num >= 0) {
		labelCardNumber->setSystemFontSize(100);
	}
	if (num >= 16) {
		labelCardNumber->setSystemFontSize(90);
	}
	if (num >= 128) {
		labelCardNumber->setSystemFontSize(60);
	}
	if (num >= 1024) {
		labelCardNumber->setSystemFontSize(40);
	}

	// 根据数字大小调整背景颜色
	colorBackground->setColor(Color3B(110+number*3, 60, 200));
}

// 卡片初始化的方法
void CardSprite::enemyInit(int number, int width, int height, float CardSpriteX, float CardSpriteY) {
	// 初始化数字
	this->number = number;

	// 添加卡片背景颜色
	colorBackground = LayerColor::create(Color4B(200, 190, 180, 255),
		static_cast<float>(width - 15),
		static_cast<float>(height - 15));
	colorBackground->setPosition(Point(CardSpriteX, CardSpriteY));


	// 添加显示数字的Label
	labelCardNumber = Label::createWithSystemFont("", "Arial", 100);
	labelCardNumber->setPosition(Point(colorBackground->getContentSize().width / 2, colorBackground->getContentSize().height / 2));
	colorBackground->addChild(labelCardNumber);

	this->addChild(colorBackground);

	// 设置卡片上的数字
	this->setNumber(number);
}

