#include "gameScene.h"
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
#include "Enemy/Enemy.h"
//TODO：畫面問題，portal圖層會在牆上面，牆圖層會在設定上面



/*
#include <new>
size_t totalAllocated = 0;
void* operator new(size_t size) {
    totalAllocated += size;
    return malloc(size);
}
void operator delete(void* memory, size_t size) noexcept {
    totalAllocated -= size;
    free(memory);
}*/

void gameScene::Initialize() {
    firePortal = false;
    isTeleported = false;
    DebugMode = false;
    isJumping = false;
    showSetting = false;
    canTeleport = false;
    pause = false;
    moving = false;
    preDir = none;

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
    PlayerGroup->AddNewObject(player = new Player("pentagon.png", halfW, halfH, 5, 200, 10));
    player->Acceleration.y = 9.8;

    //init tracing line
    mouse = Engine::GameEngine::GetInstance().GetMousePosition();
    AddNewObject(LineGroup = new Group());
    LineGroup->AddNewObject(tracingLine = new Engine::Line(player->Position.x, player->Position.y, mouse.x, mouse.y, 128, 138, 135, 5, 0, 0));

    spawnInterval = 30*2*WallSpawnSpeed; //by default
    spawn_timer = al_create_timer(0.1);
    if(!spawn_timer) throw std::runtime_error("failed to create timer");
    al_start_timer(spawn_timer);

    AddNewObject(WallGroup = new Group());

    //init portal
    AddNewObject(PortalGroup = new Group());

    portal_animation_timer = al_create_timer(0.1);
    if(!portal_animation_timer) throw std::runtime_error("failed to create timer");

    //init image
    AddNewObject(ImageGroup = new Group());
    AddNewObject(UIGroup = new Group());

    //init setting button
    Engine::ImageButton *btn;
    btn = new Engine::ImageButton("./play/setting.png", "./play/setting.png", w-100 , 0, 100, 100);
    btn->SetOnClickCallback(std::bind(&gameScene::ToggleSetting, this));
    AddNewControlObject(btn);

    //init jump timer
    jump_timer = al_create_timer(0.1);
    if(!jump_timer) throw std::runtime_error("failed to create timer");

    survival_timer = al_create_timer(1);
    if(!survival_timer) throw std::runtime_error("failed to create timer");
    al_start_timer(survival_timer);

}
void gameScene::Terminate() {
    AudioHelper::StopSample(bgmInstance);
    bgmInstance = std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE>();

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

void gameScene::ToggleSetting() {
    showSetting = !showSetting;
}

void gameScene::Update(float deltaTime) {
    IScene::Update(deltaTime);
    if(!pause){
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
        if(al_get_timer_count(spawn_timer)>=spawnInterval){ //如果強超出範圍要刪掉
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
        for(auto &c :  WallGroup->GetObjects()){
            if(Engine::Collider::IsPointInBitmap(Engine::Point((player->Position.x - dynamic_cast<Wall*>(c)->Position.x) * dynamic_cast<Wall*>(c)->GetBitmapWidth() / dynamic_cast<Wall*>(c)->Size.x + dynamic_cast<Wall*>(c)->Anchor.x * dynamic_cast<Wall*>(c)->GetBitmapWidth(), (player->Position.y - c->Position.y) * dynamic_cast<Wall*>(c)->GetBitmapHeight() / dynamic_cast<Wall*>(c)->Size.y + dynamic_cast<Wall*>(c)->Anchor.y * dynamic_cast<Wall*>(c)->GetBitmapHeight()), dynamic_cast<Wall*>(c)->bmp)){
                survival_time = al_get_timer_count(survival_timer);
                Engine::GameEngine::GetInstance().ChangeScene("end");

                //walls.erase(walls.begin());
            }
            switch(dynamic_cast<Wall*>(c)->movingDir){
                case Wall::MovingDirection::LEFT:
                    dynamic_cast<Wall*>(c)->Position.x -= 300*deltaTime;
                    break;
                case Wall::MovingDirection::RIGHT:
                    dynamic_cast<Wall*>(c)->Position.x += 300*deltaTime;
                    break;
                case Wall::MovingDirection::UP:
                    dynamic_cast<Wall*>(c)->Position.y -= 300*deltaTime;
                    break;
                case Wall::MovingDirection::DOWN:
                    dynamic_cast<Wall*>(c)->Position.y += 300*deltaTime;
                    break;
            }
        }
        ///8秒到達左邊
        //什麼是deltaTime 這個參數
        // deltaTime是每一個frame所花的時間，單位是秒
        // 這個參數是用來讓遊戲在不同的FPS下，物體的移動速度不會改變
        // 例如，如果我們希望物體每秒移動100 pixel，那麼我們就可以寫成 Position.x += 100 * deltaTime;
        // 這樣無論FPS是多少，物體每秒都會移動100 pixel
        ticks += deltaTime;

        //Fire Portal
        if(firePortal){
            if(al_get_timer_count(portal_animation_timer)==0){
                al_start_timer(portal_animation_timer);
            }
            if(al_get_timer_count(portal_animation_timer)>=2 || portal_entrance->Position.y > floor_y-portal_entrance->Size.y/2 || portal_entrance->Position.x < portal_entrance->Size.x/2 || portal_entrance->Position.x > halfW*2-portal_entrance->Size.x/2 || portal_entrance->Position.y<portal_entrance->Size.y/2){
                firePortal = false;
                al_stop_timer(portal_animation_timer);
                al_set_timer_count(portal_animation_timer,0);
            }
            portal_entrance->Position.x += unit_vector_x*5;
            portal_entrance->Position.y += unit_vector_y*5;
        }
        if(portal_exit && !firePortal && !canTeleport) { //portal exit exists and portal entrance is fired
            if (portal_exit->Position.x <= halfW * 2 - portal_exit->Size.x/2 && portal_exit->Position.x >= portal_exit->Size.x/2 &&
                portal_exit->Position.y >= portal_exit->Size.y/2 && portal_exit->Position.y <= floor_y - portal_exit->Size.y/2) {
                portal_exit->Position.x += unit_vector_x2 * 20;
                portal_exit->Position.y += unit_vector_y2 * 20;
            } else {
                canTeleport = true;
            }
        }
        //Implement the collision detection between the player and the portal
        if(portal_entrance && portal_exit && canTeleport){
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
            sliderBGM->SetOnValueChangedCallback(std::bind(&gameScene::BGMSlideOnValueChanged, this, std::placeholders::_1));
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
            sliderSFX->SetOnValueChangedCallback(std::bind(&gameScene::SFXSlideOnValueChanged, this, std::placeholders::_1));
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
}
void gameScene::BGMSlideOnValueChanged(float value) {
    AudioHelper::ChangeSampleVolume(bgmInstance, value);
    AudioHelper::BGMVolume = value;
}

void gameScene::SFXSlideOnValueChanged(float value) {
    AudioHelper::SFXVolume = value;
}

void gameScene::gravityEngine(float deltaTime){
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

void gameScene::OnKeyDown(int keyCode) {
    IScene::OnKeyDown(keyCode);
    if(keyCode == ALLEGRO_KEY_ESCAPE){
        //if(showSetting) al_hide_mouse_cursor(Engine::GameEngine::GetInstance().GetDisplay());
        showSetting = !showSetting;
    }
    if (keyCode == ALLEGRO_KEY_TAB) {
        DebugMode = !DebugMode;
    }
    if (keyCode == ALLEGRO_KEY_A) {
        player->Velocity.x = -2;
        moving = true;

    } else if (keyCode == ALLEGRO_KEY_D) {
        player->Velocity.x = 2;
        moving = true;
    } else if (player->Position.y == floor_y-player->Size.y/2 && keyCode == ALLEGRO_KEY_W) {
            //若按D、W，在D還沒放開按W會有問題，因為W起來的時候
        al_start_timer(jump_timer);
        player->Velocity.y = -5;
        isJumping = true;
    }
}
void gameScene::OnKeyUp(int keyCode) {
    IScene::OnKeyUp(keyCode);
    if(keyCode == ALLEGRO_KEY_A){
        player->Velocity.x = 0;
        moving = false;
    }
    else if(keyCode == ALLEGRO_KEY_D){
        player->Velocity.x = 0;
        moving = false;
    }
}
void gameScene::OnMouseDown(int button, int mx, int my) {
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
            unit_vector_x = (mx - player->Position.x) / sqrt(pow(mx - player->Position.x, 2) + pow(my - player->Position.y, 2));
            unit_vector_y = (my - player->Position.y) / sqrt(pow(mx - player->Position.x, 2) + pow(my - player->Position.y, 2));
            //Create a portal exit at the window edge in the fire direction
            firePortal = true;
        }else if(button==2){ //Click right mouse button
            //Fire portal exit
            if(!portal_entrance) return;
            if(!portal_exit){
                if(player->Position.y >= floor_y - player->Size.y){
                    PortalGroup->AddNewObject(portal_exit = new Portal("play/portal_1.png", player->Position.x, floor_y-178/2-0.5));
                }
                else{
                    PortalGroup->AddNewObject(portal_exit = new Portal("play/portal_1.png", player->Position.x, player->Position.y));
                }
                unit_vector_x2 = (mx - player->Position.x) / sqrt(pow(mx - player->Position.x, 2) + pow(my - player->Position.y, 2));
                unit_vector_y2 = (my - player->Position.y) / sqrt(pow(mx - player->Position.x, 2) + pow(my - player->Position.y, 2));
            }
        }
    }
}
