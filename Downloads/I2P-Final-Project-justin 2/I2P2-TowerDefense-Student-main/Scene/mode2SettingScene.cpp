//
// Created by GaGa-PC on 2024/5/16.
//

#include "mode2SettingScene.h"
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

void mode2SettingScene::Initialize() {
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
            std::bind(&mode2SettingScene::WallSpeedSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderWallSpawn);
    AddNewObject(
            new Engine::Label("牆壁移動速度: ", UIfont, 36, 40 + halfW - 160, halfH - 50, 0, 0, 0, 255, 0.5,
                              0.5));
    sliderWallSpawn->SetValue(WallMoveSpeed);


    Slider *sliderHP;
    Hp = 1;
    sliderHP = new Slider(40 + halfW + 100 - 115, halfH - 50 - 2 + 50, 190, 4);
    sliderHP->SetOnValueChangedCallback(
            std::bind(&mode2SettingScene::HpSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderHP);
    AddNewObject(
            new Engine::Label("玩家初始血量: ", UIfont, 36, 40 + halfW - 160, halfH - 50 + 50, 0, 0, 0, 255, 0.5,
                              0.5));
    sliderHP->SetValue(Hp);

    Slider *sliderEnemySpawn;
    sliderEnemySpawn = new Slider(40 + halfW + 100 - 115, halfH - 50 - 2 + 50 + 50, 190, 4);
    sliderEnemySpawn->SetOnValueChangedCallback(
            std::bind(&mode2SettingScene::EnemySpawnSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderEnemySpawn);
    AddNewObject(
            new Engine::Label("敵人生成速度: ", UIfont, 36, 40 + halfW - 160, halfH - 50 + 50 +50, 0, 0, 0, 255, 0.5,
                              0.5));
    sliderEnemySpawn->SetValue(EnemySpawnSpeed);

    btn = new Engine::ImageButton("highlight_0.png", "highlight_1.png", halfW - 200, halfH *1.25- 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&mode2SettingScene::GameOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("開始遊戲", UIfont, 48, halfW, halfH * 1.25, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("highlight_0.png", "highlight_1.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&mode2SettingScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("回上頁", UIfont, 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    bgmInstance = AudioHelper::PlaySample(Music, true, AudioHelper::BGMVolume);
}
void mode2SettingScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void mode2SettingScene::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("play");
}
void mode2SettingScene::GameOnClick() {
    mode=2;
    Engine::GameEngine::GetInstance().ChangeScene("game2");
}
void mode2SettingScene::WallSpeedSlideOnValueChanged(float value) {
    WallMoveSpeed = value;
}

void mode2SettingScene::HpSlideOnValueChanged(float value) {
    Hp = value*10;
    if(Hp<=0) Hp=1;
    std::cout << Hp;
}

void mode2SettingScene::EnemySpawnSlideOnValueChanged(float value) {
    EnemySpawnSpeed = value;
}