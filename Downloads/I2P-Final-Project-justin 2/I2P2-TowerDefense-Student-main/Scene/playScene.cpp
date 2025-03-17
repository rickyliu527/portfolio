//
// Created by GaGa-PC on 2024/5/16.
//

#include "startScene.h"
#include <allegro5/allegro_audio.h>
#include <functional>
#include <memory>
#include <string>
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "Engine/Point.hpp"
#include "Engine/Resources.hpp"
#include "UI/Component/Slider.hpp"
#include "settingsScene.h"
#include "playScene.h"
#include "Settings/gameSettings.h"
#include "playScene.h"
void playScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    AddNewObject(new Engine::Image("background_3.png", halfW, halfH, 1920, 1080, 0.5, 0.5));
    //AddNewObject(new Engine::Label("Portal to Dodge",UIfont, 100, halfW, halfH / 3 + 50, 10, 255, 255, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("highlight_0.png", "highlight_1.png", halfW - 200, halfH * 0.75 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&playScene::Mode1OnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("模式一", UIfont, 48, halfW, halfH * 0.75, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("highlight_0.png", "highlight_1.png", halfW - 200, halfH *1- 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&playScene::Mode2OnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("模式二", UIfont, 48, halfW, halfH * 1, 0, 0, 0, 255, 0.5, 0.5));

    //btn = new Engine::ImageButton("highlight_0.png", "highlight_1.png", halfW - 200, halfH *1.25- 50, 400, 100);
    //btn->SetOnClickCallback(std::bind(&playScene::GameOnClick, this));
    //AddNewControlObject(btn);
    //AddNewObject(new Engine::Label("開始遊戲", UIfont, 48, halfW, halfH * 1.25, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("highlight_0.png", "highlight_1.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&playScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("回上頁", UIfont, 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    bgmInstance = AudioHelper::PlaySample(Music, true, AudioHelper::BGMVolume); //可到AudioHelper.cpp更改音量
}
void playScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void playScene::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}
void playScene::Mode1OnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("mode1Setting");
}
void playScene::Mode2OnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("mode2Setting");
}

//TODO:暫時設成game2，之後就不需要start，直接按mode1就可以進去game1的遊戲
void playScene::GameOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("game2");
}