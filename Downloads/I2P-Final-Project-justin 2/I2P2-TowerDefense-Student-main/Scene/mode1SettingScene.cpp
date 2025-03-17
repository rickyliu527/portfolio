//
// Created by GaGa-PC on 2024/5/16.
//

#include "mode1SettingScene.h"
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

#include <iostream>

void mode1SettingScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    AddNewObject(new Engine::Image("background_3.png", halfW, halfH, 1920, 1080, 0.5, 0.5));
    //AddNewObject(new Engine::Label("Portal to Dodge", UIfont, 100, halfW, halfH / 3 + 50, 10, 255, 255, 255, 0.5, 0.5));

    Slider *sliderWallSpawn;
    sliderWallSpawn = new Slider(40 + halfW + 100 - 115, halfH - 50 - 2, 190, 4);
    sliderWallSpawn->SetOnValueChangedCallback(
            std::bind(&mode1SettingScene::WallSpawnSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderWallSpawn);
    AddNewObject(
            new Engine::Label("牆壁移動速度: ", UIfont, 36, 40 + halfW - 160, halfH - 50, 0, 0, 0, 255, 0.5,
                              0.5));
    sliderWallSpawn->SetValue(WallSpawnSpeed);

    btn = new Engine::ImageButton("highlight_0.png", "highlight_1.png", halfW - 200, halfH *1.25- 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&mode1SettingScene::GameOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("開始遊戲", UIfont, 48, halfW, halfH * 1.25, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("highlight_0.png", "highlight_1.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&mode1SettingScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("回上頁", UIfont, 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    bgmInstance = AudioHelper::PlaySample(Music, true, AudioHelper::BGMVolume);
}

void mode1SettingScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void mode1SettingScene::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("play");
}


void mode1SettingScene::WallSpawnSlideOnValueChanged(float value) {
    WallSpawnSpeed = value;
}

void mode1SettingScene::GameOnClick() {
    mode=1;
    Engine::GameEngine::GetInstance().ChangeScene("game1");
}