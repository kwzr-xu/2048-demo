#include "Card.h"

USING_NS_CC;

// ����һ����Ƭ����ľ�̬����
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

// ��Ƭ����ĳ�ʼ������
bool CardSprite::init() {
	if (!Sprite::init()) {
		return false;
	}
	return true;
}

// ��ȡ��Ƭ����ʾ������
int CardSprite::getNumber() {
	return number;
}

// ���ÿ�Ƭ����ʾ�����֣����������ֵ��������С����ɫ
void CardSprite::setNumber(int num) {
	number = num;

	// ������ʾ������
	if (num > 0) {
		labelCardNumber->setString(std::to_string(number));
	}
	else {
		labelCardNumber->setString("");
	}

	// �������ִ�С���������С
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

	// �������ִ�С����������ɫ
	colorBackground->setColor(Color3B(110+number*3, 60, 200));
}

// ��Ƭ��ʼ���ķ���
void CardSprite::enemyInit(int number, int width, int height, float CardSpriteX, float CardSpriteY) {
	// ��ʼ������
	this->number = number;

	// ��ӿ�Ƭ������ɫ
	colorBackground = LayerColor::create(Color4B(200, 190, 180, 255),
		static_cast<float>(width - 15),
		static_cast<float>(height - 15));
	colorBackground->setPosition(Point(CardSpriteX, CardSpriteY));


	// �����ʾ���ֵ�Label
	labelCardNumber = Label::createWithSystemFont("", "Arial", 100);
	labelCardNumber->setPosition(Point(colorBackground->getContentSize().width / 2, colorBackground->getContentSize().height / 2));
	colorBackground->addChild(labelCardNumber);

	this->addChild(colorBackground);

	// ���ÿ�Ƭ�ϵ�����
	this->setNumber(number);
}

