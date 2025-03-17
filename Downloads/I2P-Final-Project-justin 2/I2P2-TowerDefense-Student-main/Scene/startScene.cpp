//
// Created by GaGa-PC on 2024/5/15.
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
void startScene::Initialize() {
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x; //1600
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y; //832
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    AddNewObject(new Engine::Image("background_1.png", halfW, halfH, 1920, 1080, 0.5, 0.5));

    btn = new Engine::ImageButton("highlight_0.png", "highlight_1.png", halfW / 2 - 150, halfH / 2 + 350 , 400, 100);
    btn->SetOnClickCallback(std::bind(&startScene::PlayOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("開始", UIfont, 48, halfW / 2 + 50, halfH / 2 + 400, 0, 0, 0, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("highlight_0.png", "highlight_1.png", halfW / 2 - 150, halfH / 2 + 550, 400, 100);
    btn->SetOnClickCallback(std::bind(&startScene::SettingsOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("設定", UIfont, 48, halfW / 2 + 50, halfH / 2 + 600, 0, 0, 0, 255, 0.5, 0.5));
    bgmInstance = AudioHelper::PlaySample(Music, true, AudioHelper::BGMVolume); //可到AudioHelper.cpp更改音量
}
void startScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}
void startScene::PlayOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("play");
}
void startScene::SettingsOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("settings");//改BGM、SFX音量、可回主畫面、可改音樂
}