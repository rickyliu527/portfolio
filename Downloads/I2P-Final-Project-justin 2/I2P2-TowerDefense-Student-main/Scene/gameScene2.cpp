//
// Created by GaGa-PC on 2024/6/15.
//

#include "gameScene2.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro.h>
#include <functional>
#include <memory>
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Settings/gameSettings.h"
#include "Engine/Collider.hpp"
#include "Wall/Wall.h"
#include <iostream>
#include <allegro5/allegro_primitives.h>
#include "Enemy/Triangle.h"
#include "Enemy/Square.h"
#include "Enemy/Circle.h"
#include "Enemy/Enemy.h"
#include "Bullet/Bullet.h"
#include "Engine/Resources.hpp"
gameScene2::direction curDir = gameScene2::direction::none;
//TODO：畫面問題，portal圖層會在牆上面，牆圖層會在設定上面

void gameScene2::Initialize() {

    firePortal = false;
    isTeleported = false;
    DebugMode = false;
    isJumping = false;
    showSetting = false;
    canTeleport = false;
    pause = false;
    moving = false;
    preDir = none;
    curDir = none;
    isTraining = false;

    float defaultSpwanInterval = 3; //3秒生成一個wall
    //hide mouse cursor
    al_hide_mouse_cursor(Engine::GameEngine::GetInstance().GetDisplay());

    int w = Engine::GameEngine::GetInstance().GetScreenSize().x;
    int h = Engine::GameEngine::GetInstance().GetScreenSize().y;
    halfW = w / 2;
    halfH = h / 2;
    bgmInstance = AudioHelper::PlaySample(Music, true, AudioHelper::BGMVolume);
    AddNewObject(new Engine::Image("./play/bg_1.png", halfW, halfH, 1920, 1080, 0.5, 0.5));
    floor_y = halfH+342;
    //show ground
    //AddNewObject(new Engine::Line(0, halfH+342, w, halfH+342, 255, 0, 0, 2, 0, 0)); //地板是在y=halfH+260的地方
    //AddNewObject(new Engine::Line(0, floor_y - 127/2, halfW*2, floor_y - 127/2, 255, 0, 0, 2, 0, 0)); 傳送門的中心點

    //init enemy
    /*
    implement wall spawn, by default we spawn a wall every 3 seconds,
    and walls whose movingDir property is left or right would arrive the other side in 8 seconds 1600/200=8
    but walls whose movingDir property is up or down would arrive the other side in 4.16 seconds 832/200=4.16
    We have to according to the wallspawnspeed, which is defined in gameSettings.h, to determine the spawnspeed of
    the wall.
    A wall is generated every 3 * 2 * wallspawnspeed(0~1), we randomly choose a movingDir property for the wall.
    maybe implement another feature that we could customly set the speed of walls
     */
    //init player
    AddNewObject(PlayerGroup = new Group());
    PlayerGroup->AddNewObject(player = new Player("pentagon.png", halfW, halfH, 5, 200, Hp));
    player->Acceleration.y = 9.8;

    //init tracing line
    mouse = Engine::GameEngine::GetInstance().GetMousePosition();
    AddNewObject(LineGroup = new Group());
    LineGroup->AddNewObject(tracingLine = new Engine::Line(player->Position.x, player->Position.y, mouse.x, mouse.y, 128, 138, 135, 5, 0, 0));
    LineGroup->AddNewObject(hpBar = new Engine::Line(50, 50, 550, 50, 255, 0, 0, 50, 0, 0));

    //float x1, float y1, float x2, float y2, unsigned char r, unsigned char g, unsigned char b,float width, float anchorX, float anchorY

    spawnInterval = 30*2; //by default
    spawn_timer = al_create_timer(0.1);
    if(!spawn_timer) throw std::runtime_error("failed to create timer");
    al_start_timer(spawn_timer);

    AddNewObject(WallGroup = new Group());

    //init portal
    AddNewObject(PortalGroup = new Group());

    //init enemy
    AddNewObject(EnemyGroup = new Group());
    EnemyGroup->AddNewObject(new Triangle(halfW+200, halfH));
    EnemyGroup->AddNewObject(new Square(halfW+300, halfH));
    EnemyGroup->AddNewObject(new Circle(halfW+400, halfH));


    //init bullet
    AddNewObject(BulletGroup = new Group());

    //init image
    AddNewObject(ImageGroup = new Group());
    AddNewObject(UIGroup = new Group());

    //init setting button
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("./play/setting.png", "./play/setting.png", w-100 , 0, 100, 100);
    btn->SetOnClickCallback(std::bind(&gameScene2::ToggleSetting, this));
    AddNewControlObject(btn);

    portal_animation_timer = al_create_timer(0.1);
    if(!portal_animation_timer) throw std::runtime_error("failed to create timer");

    //init jump timer
    jump_timer = al_create_timer(0.1);
    if(!jump_timer) throw std::runtime_error("failed to create timer");

    survival_timer = al_create_timer(1);
    if(!survival_timer) throw std::runtime_error("failed to create timer");
    al_start_timer(survival_timer);

    cooldown_timer = al_create_timer(1);
    if(!cooldown_timer) throw std::runtime_error("failed to create timer");

    hpBar_animation_timer = al_create_timer(0.01); //0.01秒 +1
    if(!hpBar_animation_timer) throw std::runtime_error("failed to create timer");

    train_timer = al_create_timer(0.01);
    if(!train_timer) throw std::runtime_error("failed to create timer");
    al_start_timer(train_timer);

    srand(time(0));
}
void gameScene2::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();

    if(cooldown_timer){
        al_stop_timer(cooldown_timer);
        al_destroy_timer(cooldown_timer);
        cooldown_timer = nullptr;
    }
    // Destroy timers
    if(spawn_timer){
        al_stop_timer(spawn_timer);
        al_destroy_timer(spawn_timer);
        spawn_timer = nullptr;
    }
    if(portal_animation_timer){
        al_stop_timer(portal_animation_timer);
        al_destroy_timer(portal_animation_timer);
        portal_animation_timer = nullptr;
    }
    if(jump_timer){
        al_stop_timer(jump_timer);
        al_destroy_timer(jump_timer);
        jump_timer = nullptr;
    }
    if(survival_timer){
        al_stop_timer(survival_timer);
        al_destroy_timer(survival_timer);
        survival_timer = nullptr;
    }
    if(hpBar_animation_timer){
        al_stop_timer(hpBar_animation_timer);
        al_destroy_timer(hpBar_animation_timer);
        hpBar_animation_timer = nullptr;
    }
    if(train_timer){
        al_stop_timer(train_timer);
        al_destroy_timer(train_timer);
        train_timer = nullptr;
    }

    if(PlayerGroup) PlayerGroup->Clear();
    if(LineGroup) LineGroup->Clear();
    if(ImageGroup) ImageGroup->Clear();
    if(UIGroup) UIGroup->Clear();
    if(WallGroup) WallGroup->Clear();
    if(PortalGroup) PortalGroup->Clear();
    //correctly delete everything in the scene

    //std::cout << "Total allocated: " << totalAllocated << " bytes" << std::endl;
    IScene::Terminate();
}

void gameScene2::ToggleSetting() {
    showSetting = !showSetting;
}

void gameScene2::Update(float deltaTime) {
    IScene::Update(deltaTime);
    if(!pause){
        for (auto &c : EnemyGroup->GetObjects()) {
            if (auto triangle = dynamic_cast<Triangle*>(c)) {
                if (triangle->currentState.enemy_x == -1) {
                    triangle->currentState.enemy_x = triangle->Position.x;
                    triangle->currentState.enemy_y = triangle->Position.y;
                }
                triangle->currentState.player_x = player->Position.x;
                triangle->currentState.player_y = player->Position.y;
                Action action = triangle->chooseActionSoftmax(triangle->currentState, rng, tau);
                State new_state = triangle->currentState;
                switch (action) {
                    case Action::UP:
                        if (new_state.enemy_y > 0) {
                            new_state.enemy_y -= 10;
                            triangle->prevAction = 0;
                            triangle->Position.y -= 10;
                        }
                        break;
                    case Action::DOWN:
                        if (new_state.enemy_y < halfH * 2) {
                            new_state.enemy_y += 10;
                            triangle->prevAction = 1;
                            triangle->Position.y += 10;
                        }
                        break;
                    case Action::LEFT:
                        if (new_state.enemy_x > 0) {
                            new_state.enemy_x -= 10;
                            triangle->prevAction = 2;
                            triangle->Position.x -= 10;
                        }
                        break;
                    case Action::RIGHT:
                        if (new_state.enemy_x < halfW * 2) {
                            new_state.enemy_x += 10;
                            triangle->prevAction = 3;
                            triangle->Position.x += 10;
                        }
                        break;
                }
                double reward = triangle->getReward(triangle->currentState, new_state);
                triangle->updateQ(triangle->currentState, action, reward, new_state);
                triangle->currentState = new_state;
            } else if (auto square = dynamic_cast<Square*>(c)) {
                if (square->currentState.enemy_x == -1) {
                    square->currentState.enemy_x = square->Position.x;
                    square->currentState.enemy_y = square->Position.y;
                }
                square->currentState.player_x = player->Position.x;
                square->currentState.player_y = player->Position.y;
                Action action = square->chooseActionSoftmax(square->currentState, rng, tau);
                State new_state = square->currentState;
                switch (action) {
                    case Action::UP:
                        if (new_state.enemy_y > 0) {
                            new_state.enemy_y -= 10;
                            square->prevAction = 0;
                            square->Position.y -= 10;
                        }
                        break;
                    case Action::DOWN:
                        if (new_state.enemy_y < halfH * 2) {
                            new_state.enemy_y += 10;
                            square->prevAction = 1;
                            square->Position.y += 10;
                        }
                        break;
                    case Action::LEFT:
                        if (new_state.enemy_x > 0) {
                            new_state.enemy_x -= 10;
                            square->prevAction = 2;
                            square->Position.x -= 10;
                        }
                        break;
                    case Action::RIGHT:
                        if (new_state.enemy_x < halfW * 2) {
                            new_state.enemy_x += 10;
                            square->prevAction = 3;
                            square->Position.x += 10;
                        }
                        break;
                }
                double reward = square->getReward(square->currentState, new_state);
                square->updateQ(square->currentState, action, reward, new_state);
                square->currentState = new_state;
            } else if(auto circle = dynamic_cast<Circle*>(c)){
                    if (circle->currentState.enemy_x == -1) {
                        circle->currentState.enemy_x = circle->Position.x;
                        circle->currentState.enemy_y = circle->Position.y;
                    }
                    circle->currentState.player_x = player->Position.x;
                    circle->currentState.player_y = player->Position.y;
                    Action action = circle->chooseActionSoftmax(circle->currentState, rng, tau);
                    State new_state = circle->currentState;
                    switch (action) {
                        case Action::UP:
                            if (new_state.enemy_y > 0) {
                                new_state.enemy_y -= 10;
                                circle->prevAction = 0;
                                circle->Position.y -= 10;
                            }
                            break;
                        case Action::DOWN:
                            if (new_state.enemy_y < halfH * 2) {
                                new_state.enemy_y += 10;
                                circle->prevAction = 1;
                                circle->Position.y += 10;
                            }
                            break;
                        case Action::LEFT:
                            if (new_state.enemy_x > 0) {
                                new_state.enemy_x -= 10;
                                circle->prevAction = 2;
                                circle->Position.x -= 10;
                            }
                            break;
                        case Action::RIGHT:
                            if (new_state.enemy_x < halfW * 2) {
                                new_state.enemy_x += 10;
                                circle->prevAction = 3;
                                circle->Position.x += 10;
                            }
                            break;
                    }
                    double reward = circle->getReward(circle->currentState, new_state);
                    circle->updateQ(circle->currentState, action, reward, new_state);
                    circle->currentState = new_state;
            }
        }

        if(al_get_timer_count(hpBar_animation_timer) != 0 && al_get_timer_count(hpBar_animation_timer) < 100){
            hpBar->setX2(50+(player->hp+1)*(500/Hp)-(500/Hp)*al_get_timer_count(hpBar_animation_timer)/100); //500 is the length of the hp bar, 10 is the max hp
        }else if(al_get_timer_count(hpBar_animation_timer) >= 100){
            hpBar->setX2(50+(player->hp)*(500/Hp));
            if(player->hp == 0){
                survival_time = al_get_timer_count(survival_timer);
                Engine::GameEngine::GetInstance().ChangeScene("end");
            }
            al_stop_timer(hpBar_animation_timer);
            al_set_timer_count(hpBar_animation_timer, 0);
        }
        //update tracing line
        tracingLine->setX1(player->Position.x);
        tracingLine->setY1(player->Position.y);
        mouse = Engine::GameEngine::GetInstance().GetMousePosition();
        double tracing_line_x=player->Position.x,tracing_line_y=player->Position.y;
        while(tracing_line_x<halfW*2 && tracing_line_x>0 && tracing_line_y>0 && tracing_line_y<halfH*2){
            double mousedir_x = (mouse.x - player->Position.x) / sqrt(pow(mouse.x - player->Position.x, 2) + pow(mouse.y - player->Position.y, 2));
            double mousedir_y = (mouse.y - player->Position.y) / sqrt(pow(mouse.x - player->Position.x, 2) + pow(mouse.y - player->Position.y, 2));
            tracing_line_x+=mousedir_x*20;
            tracing_line_y+=mousedir_y*20;
        }
        tracingLine->setX2(tracing_line_x);
        tracingLine->setY2(tracing_line_y);

        //As literally, implement physical gravity
        gravityEngine(deltaTime);

        if(moving){
            player->Position.x += player->Velocity.x;
        }
        if(isJumping&&al_get_timer_count(jump_timer)>0 && al_get_timer_count(jump_timer)<2){
            player->Position.y += player->Velocity.y;
        }else if(isJumping&&al_get_timer_count(jump_timer)>=2){
            al_stop_timer(jump_timer);
            al_set_timer_count(jump_timer,0);
            player->Velocity.y = 0;
            isJumping = false;
        }

        //adjust player position
        if(player->Position.x < player->Size.x/2) player->Position.x = player->Size.x/2;
        if(player->Position.x > halfW*2-player->Size.x/2) player->Position.x = halfW*2-player->Size.x/2;
        if(player->Position.y < player->Size.y/2) player->Position.y = player->Size.y/2;
        if(player->Position.y > halfH*2-player->Size.y/2) player->Position.y = halfH*2-player->Size.y/2;

        //Randomly generate walls
        if(al_get_timer_count(spawn_timer)>=spawnInterval){ //如果牆超出範圍要刪掉
            std::srand(std::time(0));
            int random = std::rand() % 3;

            Wall *wall;
            switch(random){
                case 0:
                    wall = new Wall("play/wall_horizontal.png", halfW, 0, Wall::MovingDirection::DOWN);
                    break;
                case 1:
                    wall = new Wall("play/wall_vertical.png", halfW*2, halfH, Wall::MovingDirection::LEFT);
                    break;
                case 2:
                    wall = new Wall("play/wall_vertical.png", 0, halfH, Wall::MovingDirection::RIGHT);
                    break;
            }

            WallGroup->AddNewObject(wall);
            //walls.push_back(wall);
            al_set_timer_count(spawn_timer,0);
        }
        for(auto &c : WallGroup->GetObjects()){
            if(dynamic_cast<Wall*>(c)->Position.x < 0 || dynamic_cast<Wall*>(c)->Position.x > halfW*2 || dynamic_cast<Wall*>(c)->Position.y < 0 || dynamic_cast<Wall*>(c)->Position.y > halfH*2){
                WallGroup->RemoveObject(c->GetObjectIterator());
                //walls.erase(walls.begin());
            }
        }
        if(player->coolDown && al_get_timer_count(cooldown_timer) >= 3){
            player->coolDown = false;
            al_stop_timer(cooldown_timer);
            al_set_timer_count(cooldown_timer, 0);
            player->Tint = al_map_rgba(255, 255, 255, 255);
            //player->bmp = Engine::Resources::GetInstance().GetBitmap("pentagon.png");
        }
        for(auto &c :  WallGroup->GetObjects()){
            if(!player->coolDown && Engine::Collider::IsPointInBitmap(Engine::Point((player->Position.x - dynamic_cast<Wall*>(c)->Position.x) * dynamic_cast<Wall*>(c)->GetBitmapWidth() / dynamic_cast<Wall*>(c)->Size.x + dynamic_cast<Wall*>(c)->Anchor.x * dynamic_cast<Wall*>(c)->GetBitmapWidth(), (player->Position.y - c->Position.y) * dynamic_cast<Wall*>(c)->GetBitmapHeight() / dynamic_cast<Wall*>(c)->Size.y + dynamic_cast<Wall*>(c)->Anchor.y * dynamic_cast<Wall*>(c)->GetBitmapHeight()), dynamic_cast<Wall*>(c)->bmp)){
                player->hp--;
                al_start_timer(hpBar_animation_timer);
                std::cout << player->hp << std::endl;
                player->coolDown = true;
                al_start_timer(cooldown_timer);
                AudioHelper::PlayAudio("injured.mp3");
                //player->bmp = Engine::Resources::GetInstance().GetBitmap("square.png");
                player->Tint = al_map_rgba(255, 255, 255, 150);
                //walls.erase(walls.begin());
            }
            switch(dynamic_cast<Wall*>(c)->movingDir){
                case Wall::MovingDirection::LEFT:
                    dynamic_cast<Wall*>(c)->Position.x -= 300*WallMoveSpeed*2*deltaTime;
                    break;
                case Wall::MovingDirection::RIGHT:
                    dynamic_cast<Wall*>(c)->Position.x += 300*WallMoveSpeed*2*deltaTime;
                    break;
                case Wall::MovingDirection::UP:
                    dynamic_cast<Wall*>(c)->Position.y -= 300*WallMoveSpeed*2*deltaTime;
                    break;
                case Wall::MovingDirection::DOWN:
                    dynamic_cast<Wall*>(c)->Position.y += 300*WallMoveSpeed*2*deltaTime;
                    break;
            }
        }
        for(auto &c : EnemyGroup->GetObjects()){
            if(!player->coolDown && Engine::Collider::IsPointInBitmap(Engine::Point((player->Position.x - dynamic_cast<Enemy*>(c)->Position.x) * dynamic_cast<Enemy*>(c)->GetBitmapWidth() / dynamic_cast<Enemy*>(c)->Size.x + dynamic_cast<Enemy*>(c)->Anchor.x * dynamic_cast<Enemy*>(c)->GetBitmapWidth(), (player->Position.y - c->Position.y) * dynamic_cast<Enemy*>(c)->GetBitmapHeight() / dynamic_cast<Enemy*>(c)->Size.y + dynamic_cast<Enemy*>(c)->Anchor.y * dynamic_cast<Enemy*>(c)->GetBitmapHeight()), dynamic_cast<Enemy*>(c)->bmp)){
                player->hp--;
                if(player->hp==0) AudioHelper::PlayAudio("explosion.wav");
                al_start_timer(hpBar_animation_timer);
                std::cout << player->hp << std::endl;
                player->coolDown = true;
                al_start_timer(cooldown_timer);
                AudioHelper::PlayAudio("injured.mp3");
                //player->bmp = Engine::Resources::GetInstance().GetBitmap("square.png");
                player->Tint = al_map_rgba(255, 255, 255, 150);
                BulletGroup->RemoveObject(c->GetObjectIterator());
                //walls.erase(walls.begin());
            }
        }

        /*for(auto &c: walls){
            if(c->Position.x < 0 || c->Position.x > halfW*2 || c->Position.y < 0 || c->Position.y > halfH*2){
                WallGroup->RemoveObject(c->GetObjectIterator());
                walls.erase(walls.begin());
               // walls.erase(std::remove(walls.begin(), walls.end(), c), walls.end());
                std::cout << "wall removed" << std::endl;
            }
        }*/
        /*
        for(auto &c : walls){
            if( Engine::Collider::IsPointInBitmap(Engine::Point((player->Position.x - c->Position.x) * c->GetBitmapWidth() / c->Size.x + c->Anchor.x * c->GetBitmapWidth(), (player->Position.y - c->Position.y) * c->GetBitmapHeight() / c->Size.y + c->Anchor.y * c->GetBitmapHeight()), c->bmp)){
                walls.clear();
                survival_time = al_get_timer_count(survival_timer);
                Engine::GameEngine::GetInstance().ChangeScene("end");
            }
            switch(c->movingDir){
                case Wall::MovingDirection::LEFT:
                    c->Position.x -= 300*deltaTime;
                    break;
                case Wall::MovingDirection::RIGHT:
                    c->Position.x += 300*deltaTime;
                    break;
                case Wall::MovingDirection::UP:
                    c->Position.y -= 300*deltaTime;
                    break;
                case Wall::MovingDirection::DOWN:
                    c->Position.y += 300*deltaTime;
                    break;
            }
        }*///8秒到達左邊
        //什麼是deltaTime 這個參數
        // deltaTime是每一個frame所花的時間，單位是秒
        // 這個參數是用來讓遊戲在不同的FPS下，物體的移動速度不會改變
        // 例如，如果我們希望物體每秒移動100 pixel，那麼我們就可以寫成 Position.x += 100 * deltaTime;
        // 這樣無論FPS是多少，物體每秒都會移動100 pixel
        ticks += deltaTime;
        /*PlayerGroup->Update(deltaTime);
        WallGroup->Update(deltaTime);*/

        //Fire Portal
        if(firePortal){
            double tmp_x = player->Position.x;
            double tmp_y = player->Position.y;
            if(curDir==up) {
                portal_entrance->Position.y = player->Position.y - 50;
                tmp_y = floor_y - portal_entrance->Size.y/2;
            }
            else if(curDir==left){
                portal_entrance->Position.x = player->Position.x - 100;
                tmp_x = portal_entrance->Size.x/2;
            }else if(curDir == right){
                portal_entrance->Position.x = player->Position.x + 100;
                tmp_x = halfW*2-portal_entrance->Size.x/2;
            }else if(curDir==none){
                if(player->Position.y+100 > floor_y-portal_entrance->Size.y/2){
                    portal_entrance->Position.y = floor_y-portal_entrance->Size.y/2;
                }else{
                    portal_entrance->Position.y = player->Position.y+100;
                }
                tmp_y = portal_entrance->Size.y/2;
            }
            PortalGroup->AddNewObject(portal_exit = new Portal("play/portal_1.png", tmp_x, tmp_y));
            canTeleport = true;
            firePortal = false;
        }
        //Implement the collision detection between the player and the portal
        if(canTeleport){
            isTeleported = Engine::Collider::IsPointInBitmap(Engine::Point((player->Position.x - portal_entrance->Position.x) * portal_entrance->GetBitmapWidth() / portal_entrance->Size.x + portal_entrance->Anchor.x * portal_entrance->GetBitmapWidth(), (player->Position.y - portal_entrance->Position.y) * portal_entrance->GetBitmapHeight() / portal_entrance->Size.y + portal_entrance->Anchor.y * portal_entrance->GetBitmapHeight()), portal_entrance->bmp);
            if(isTeleported) {
                player->Position.x = portal_exit->Position.x;
                player->Position.y = portal_exit->Position.y;
                canTeleport = false;
                PortalGroup->RemoveObject(portal_exit->GetObjectIterator());
                portal_exit = nullptr;
                PortalGroup->RemoveObject(portal_entrance->GetObjectIterator());
                portal_entrance = nullptr;
            }
        }
        for(auto&c : BulletGroup->GetObjects()){
            dynamic_cast<Bullet*>(c)->Update(deltaTime);
        }
    }

    //Pause the game when we show the setting panel.
    //setting
    if(showSetting){
        pause = true;
        al_show_mouse_cursor(Engine::GameEngine::GetInstance().GetDisplay());
        if(!setting_bg) ImageGroup->AddNewObject(setting_bg = new Engine::Image("./play/floor.png", Engine::GameEngine::GetInstance().GetScreenSize().x/2, Engine::GameEngine::GetInstance().GetScreenSize().y/2, 400, 400, 0.5, 0.5));
        if(!setting_label) UIGroup->AddNewObject(setting_label = new Engine::Label("設定", UIfont, 36, Engine::GameEngine::GetInstance().GetScreenSize().x/2, Engine::GameEngine::GetInstance().GetScreenSize().y/2-150, 255, 255, 255, 255, 0.5, 0.5));
        if(!sliderBGM){
            Engine::Label *label;
            UIGroup->AddNewObject(label = new Engine::Label("背景音量: ", UIfont, 28, 40 + halfW - 60 - 95, halfH - 55, 255, 255, 255, 255, 0.5,
                                                            0.5));
            sliderBGM = new Slider(40 + halfW - 95, halfH - 50 - 2, 190, 4);
            sliderBGM->SetOnValueChangedCallback(std::bind(&gameScene2::BGMSlideOnValueChanged, this, std::placeholders::_1));
            AddNewControlObject(sliderBGM);
        }
        else{
            sliderBGM->SetValue(AudioHelper::BGMVolume);
        }
        if(!sliderSFX) {
            Engine::Label *label;
            UIGroup->AddNewObject(label = new Engine::Label("特效音量: ", UIfont, 28, 40 + halfW - 60 - 95, halfH + 45, 255, 255, 255, 255, 0.5,
                                                            0.5));
            sliderSFX = new Slider(40 + halfW - 95, halfH + 50 - 2, 190, 4);
            sliderSFX->SetOnValueChangedCallback(std::bind(&gameScene2::SFXSlideOnValueChanged, this, std::placeholders::_1));
            AddNewControlObject(sliderSFX);
        }
        else{
            sliderSFX->SetValue(AudioHelper::SFXVolume);
        }
    }
    else{
        if(sliderBGM) RemoveControlObject(sliderBGM->GetControlIterator(), sliderBGM->GetObjectIterator());
        if(sliderSFX) RemoveControlObject(sliderSFX->GetControlIterator(), sliderSFX->GetObjectIterator());
        ImageGroup->Clear();
        UIGroup->Clear();
        if(setting_bg) setting_bg = nullptr;
        if(setting_label) setting_label = nullptr;
        if(sliderBGM) sliderBGM = nullptr;
        if(sliderSFX) sliderSFX = nullptr;
        al_hide_mouse_cursor(Engine::GameEngine::GetInstance().GetDisplay());
        //delete control object sliderBGM
        pause = false;
    }
    for(auto &c :  EnemyGroup->GetObjects()){
        c->Update(deltaTime);
    }

}
void gameScene2::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}

void gameScene2::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}

void gameScene2::gravityEngine(float deltaTime){
    if(!isJumping){
        if(player->Position.y < floor_y-player->Size.y/2){
            player->Velocity.y += player->Acceleration.y * deltaTime;
            player->Position.y += player->Velocity.y;
        }
        else if(player->Position.y >= floor_y-player->Size.y/2){
            player->Position.y = floor_y - player->Size.y/2;
            player->Velocity.y = 0;
            if(preDir == up && player->Velocity.x > 0 ){
                OnKeyUp(ALLEGRO_KEY_D);
                preDir = none;
            }
            else if(preDir==up && player->Velocity.x < 0) {
                OnKeyUp(ALLEGRO_KEY_A);
                preDir = none;
            }
        }
    }
}

void gameScene2::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    if(keyCode == ALLEGRO_KEY_ESCAPE){
        //if(showSetting) al_hide_mouse_cursor(Engine::GameEngine::GetInstance().GetDisplay());
        showSetting = !showSetting;
    }
    if (keyCode == ALLEGRO_KEY_TAB) {
        DebugMode = !DebugMode;
    }
    if (keyCode == ALLEGRO_KEY_A) {
        curDir = left;
        player->Velocity.x = -2;
        moving = true;

    } else if (keyCode == ALLEGRO_KEY_D) {
        curDir = right;
        player->Velocity.x = 2;
        moving = true;
    } else if (player->Position.y == floor_y-player->Size.y/2 && keyCode == ALLEGRO_KEY_W) {
        curDir = up;
        //若按D、W，在D還沒放開按W會有問題，因為W起來的時候
        al_start_timer(jump_timer);
        player->Velocity.y = -8;
        isJumping = true;
    }
}
void gameScene2::OnKeyUp(int keyCode) {
    IScene::OnKeyUp(keyCode);
    if(keyCode == ALLEGRO_KEY_A){
        player->Velocity.x = 0;
        moving = false;
        curDir = none;
    }
    else if(keyCode == ALLEGRO_KEY_D){
        player->Velocity.x = 0;
        moving = false;
        curDir = none;
    }
}
void gameScene2::OnMouseDown(int button, int mx, int my) {
    IScene::OnMouseDown(button, mx, my);
    if(!pause){
        if(button==1){ //Click left mouse button
            if(!portal_entrance){
                if(player->Position.y >= floor_y - player->Size.y){
                    PortalGroup->AddNewObject(portal_entrance = new Portal("play/portal_0.png", player->Position.x, floor_y-182/2-0.5));
                }
                else{
                    PortalGroup->AddNewObject(portal_entrance = new Portal("play/portal_0.png", player->Position.x, player->Position.y));
                }
            }else{
                //Delete the old portal entrance
                if(portal_exit){
                    PortalGroup->RemoveObject(portal_exit->GetObjectIterator());
                    portal_exit = nullptr;
                }
                firePortal = false;
                PortalGroup->RemoveObject(portal_entrance->GetObjectIterator());
                portal_entrance = nullptr;
                canTeleport = false;
                return;
            }
            if(curDir == up){
                unit_vector_x = 0;
                unit_vector_y = -1;
            }else if(curDir==left){
                unit_vector_x = -1;
                unit_vector_y = 0;
            }else if(curDir==right){
                unit_vector_x = 1;
                unit_vector_y = 0;
            }
            //Create a portal exit at the window edge in the fire direction
            firePortal = true;
        }else if(button==2){ //eject bullet
            double mousedir_x = (mouse.x - player->Position.x) / sqrt(pow(mouse.x - player->Position.x, 2) + pow(mouse.y - player->Position.y, 2));
            double mousedir_y = (mouse.y - player->Position.y) / sqrt(pow(mouse.x - player->Position.x, 2) + pow(mouse.y - player->Position.y, 2));
            BulletGroup->AddNewObject(new Bullet("play/bullet_0.png", 5, player->Position.x, player->Position.y, mousedir_x, mousedir_y));
            AudioHelper::PlayAudio("bulletHit.wav");
        }
    }
}