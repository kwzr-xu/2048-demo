/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "MainMenuScene.h"

 // ��������ָ����ھ����Ƿ�ʹ����Ƶ���档
#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#endif

USING_NS_CC;

// Ԥ�������Ʒֱ��ʺ�ʵ����Ļ�ߴ硣
static cocos2d::Size designResolutionSize = cocos2d::Size(1920, 1080);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
#if USE_AUDIO_ENGINE
    AudioEngine::end(); // ������Ƶ����
#endif
}

void AppDelegate::initGLContextAttrs() {
    // ���� OpenGL ����������
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages() {
    return 0; // ����������־
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();

    //��Դ���
    std::vector<std::string> searchPaths;
    searchPaths.push_back("Resources");
    FileUtils::getInstance()->setSearchPaths(searchPaths);

    // ��ʼ�� OpenGL ��ͼ��
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("2048demo", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("2048demo");
#endif
        director->setOpenGLView(glview);
    }

    // ������ʾ FPS ��֡�ʡ�
    director->setDisplayStats(true);
    director->setAnimationInterval(1.0f / 60);

    // ������Ʒֱ��ʡ�
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();

    // ������Ļ�ߴ������Դ���š�
    if (frameSize.height > mediumResolutionSize.height) {
        director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
    }
    else if (frameSize.height > smallResolutionSize.height) {
        director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));
    }
    else {
        director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
    }

    register_all_packages();

    // ���������г�����
    auto scene = MainMenuScene::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll(); // ��ͣ������Ƶ
#endif
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll(); // �ָ�������Ƶ
#endif
}