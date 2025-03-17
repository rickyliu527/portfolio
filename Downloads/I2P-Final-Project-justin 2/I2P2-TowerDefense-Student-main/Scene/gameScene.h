//
// Created by GaGa-PC on 2024/5/17.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_GAMESCENE_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_GAMESCENE_H

#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
#include "Player/Player.h"
#include "Portal/Portal.h"
#include "UI/Component/Line.h"
#include "UI/Component/Label.hpp"
#include "UI/Component/Slider.hpp"
#include "Wall/Wall.h"
#include <vector>
class gameScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    explicit gameScene() = default;
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void OnKeyDown(int keyCode) override;
    void OnKeyUp(int keyCode) override;
    void OnMouseDown(int button, int mx, int my) override;
    void ToggleSetting();
    void gravityEngine(float deltaTime);
    bool showSetting;
    bool DebugMode;
    bool firePortal;
    bool isTeleported;
    bool pause;
    bool canTeleport;
    bool moving;
    bool isJumping;
    float floor_y;
    //for tracing line
    //for left click
    double unit_vector_x;
    double unit_vector_y;
    //for right click
    double unit_vector_x2;
    double unit_vector_y2;
    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
    float halfW;
    float halfH;
    ALLEGRO_TIMER * spawn_timer;
    ALLEGRO_TIMER * portal_animation_timer;
    ALLEGRO_TIMER * jump_timer;
    ALLEGRO_TIMER * survival_timer;
    std::vector<Wall*> walls;
    Group* PlayerGroup;
    Group* LineGroup;
    Group* ImageGroup;
    Group* UIGroup;
    Group* WallGroup;
    Group* PortalGroup;
    Portal* portal_entrance;
    Portal* portal_exit;
    Slider *sliderBGM, *sliderSFX;
    Engine::Image *setting_bg;
    Engine::Label *setting_label;
    Engine::Point mouse;
    Player* player;
    Engine::Line* tracingLine;
    float ticks;
    float spawnInterval;
    enum direction{
        up,
        down,
        left,
        right,
        none,
    };
    direction preDir;
};
#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_GAMESCENE_H
