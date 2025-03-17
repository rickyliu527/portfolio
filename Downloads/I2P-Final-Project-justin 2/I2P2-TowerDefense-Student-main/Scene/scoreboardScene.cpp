//
// Created by justi on 2024/6/21.
//

#include "scoreboardScene.h"
#include <functional>
#include <memory>
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Settings/gameSettings.h"
#include "Wall/Wall.h"
#include <string>
#include "UI/Component/ImageButton.hpp"
#include "UI/Component/Label.hpp"
#include "Settings/gameSettings.h"
#include <fstream>
#include <algorithm>
#include <iostream>

void scoreboardScene::Initialize() {
    al_show_mouse_cursor(Engine::GameEngine::GetInstance().GetDisplay());
    int w = Engine::GameEngine::GetInstance().GetScreenSize().x; //1600
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y; //832
    int halfW = w / 2;
    int halfH = h / 2;
    Engine::ImageButton* btn;
    AddNewObject(new Engine::Image("background_3.png", halfW, halfH, 1920, 1080, 0.5, 0.5));
    //AddNewObject(new Engine::Label("Portal to Dodge", UIfont, 100, halfW, halfH / 3 + 50, 0, 127, 186, 255, 0.5, 0.5));

    btn = new Engine::ImageButton("highlight_0.png", "highlight_1.png", halfW - 200, halfH * 3 / 2 - 50, 400, 100);
    btn->SetOnClickCallback(std::bind(&scoreboardScene::BackOnClick, this));
    AddNewControlObject(btn);
    AddNewObject(new Engine::Label("回上頁", UIfont, 48, halfW, halfH * 3 / 2, 0, 0, 0, 255, 0.5, 0.5));
//    AddNewObject(new Engine::Label("存活時間: " + std::to_string(survival_time), UIfont, 48, halfW, halfH, 0, 0, 0, 255, 0.5, 0.5));
    bgmInstance = AudioHelper::PlaySample(Music, true, AudioHelper::BGMVolume); //可到AudioHelper.cpp更改音量

    ReadScoreboard();
    if(mode==1){
        Scoreboard.push_back(survival_time);
    }else if(mode==2){
        Scoreboard.push_back(points);
    }
    sort(Scoreboard.begin(), Scoreboard.end());
    reverse(Scoreboard.begin(), Scoreboard.end());
    AddNewObject(new Engine::Label("排行榜", UIfont, 48, halfW, 260, 0, 0, 0, 255, 0.5, 0.5));
    int tmp = Scoreboard.size();
    int size = std::min(10,tmp);
    for(int i=0 ; i<size ; i++) {
        AddNewObject(new Engine::Label(std::to_string(i+1) + ". " , UIfont, 36, halfW-60, 310+45*i, 0, 0, 0, 255, 0, 0.5));
        AddNewObject(new Engine::Label(std::to_string(Scoreboard[i]), UIfont, 36, halfW+60, 310+45*i, 0, 0, 0, 255, 1, 0.5));
    }

    std::string path;

    if(mode==1){
        path = std::string("../Resource/scoreboard_1") + ".txt";
    }else if(mode==2){
        path = std::string("../Resource/scoreboard_2") + ".txt";
    }
    if(mode != 0){
        std::ofstream file(path, std::ios::trunc);
        for(auto& c : Scoreboard){
            file << std::to_string(c) << std::endl;
        }
        file.close();
    }


}
void scoreboardScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();
    IScene::Terminate();
}

void scoreboardScene::BackOnClick() {
    Engine::GameEngine::GetInstance().ChangeScene("end");
}
void scoreboardScene::ReadScoreboard() {
    std::string filename;
    if(mode == 1){
        filename = std::string("../Resource/scoreboard_1") + ".txt";
    }else if(mode == 2) {
        filename = std::string("../Resource/scoreboard_2") + ".txt";
    }
    std::string name;
    Scoreboard.clear();
    int score;
    std::ifstream fin(filename);
    while (fin >> score) {
        Scoreboard.push_back(score);
    }
    fin.close();
}