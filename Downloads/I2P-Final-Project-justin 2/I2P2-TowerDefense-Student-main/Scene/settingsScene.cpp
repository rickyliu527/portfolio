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



void settingsScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    AddNewObject(new Engine::Image("background_3.png", halfW, halfH, 1920, 1080, 0.5, 0.5));
    //AddNewObject(new Engine::Label("Portal to Dodge", UIfont, 100, halfW, halfH / 3 + 50, 10, 255, 255, 255, 0.5, 0.5));

    Slider *sliderBGM, *sliderSFX;
    sliderBGM = new Slider(40 + halfW + 100 - 115, halfH - 50 - 2, 190, 4);
    sliderBGM->SetOnValueChangedCallback(
            std::bind(&settingsScene::BGMSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderBGM);
    AddNewObject(
            new Engine::Label("背景音樂: ", UIfont, 36, 40 + halfW - 60 - 80, halfH - 55, 0, 0, 0, 255, 0.5,
                              0.5));
    sliderSFX = new Slider(40 + halfW + 100 - 115, halfH + 50 - 2, 190, 4);
    sliderSFX->SetOnValueChangedCallback(
            std::bind(&settingsScene::SFXSlideOnValueChanged, this, std::placeholders::_1));
    AddNewControlObject(sliderSFX);
    AddNewObject(
            new Engine::Label("特效音量: ",UIfont, 36, 40 + halfW - 60 - 80, halfH + 45, 0, 0, 0, 255, 0.5,
                              0.5));

    // Not safe if release resource while playing, however we only free while change scene, so it's fine.
    bgmInstance = AudioHelper::PlaySample(Music, true, AudioHelper::BGMVolume);
    sliderBGM->SetValue(AudioHelper::BGMVolume);
    sliderSFX->SetValue(AudioHelper::SFXVolume);

    btn = new Engine::ImageButton("highlight_0.png", "highlight_1.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&settingsScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("回上頁", UIfont, 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));

    //btn = new Engine::ImageButton("button_bg.png", "button_bg_select.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    //btn->SetOnClickCallback(std::bind(&settingsScene::BackOnClick, this));
    //AddNewControlObject(btn);
    AddNewObject(new Engine::Label("音量調整", UIfont, 48, halfW, halfH* 1.275, 0, 0, 0, 255, 0.5, 0.5));

}
void settingsScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void settingsScene::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("start");
}

void settingsScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}

void settingsScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}