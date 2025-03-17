
// Created by GaGa-PC on 2024/5/21.
//

#include "Player.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/color.h>
#include <cmath>
#include <random>
#include <string>
#include <vector>
#include "Engine/AudioHelper.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IScene.hpp"
#include "Engine/LOG.hpp"
#include "Scene/gameScene.h"
#include "Scene/gameScene2.h"
/*gameScene* Player::getPlayScene() {
    return dynamic_cast<gameScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}*/
Player::Player(std::string img, float x, float y, float radius, float speed, float hp) :
        Engine::Sprite(img, x, y), speed(speed), hp(hp) {
    CollisionRadius = radius;
    Velocity = Engine::Point(0, 0);
    coolDown = false;
}

void Player::Update(float deltaTime) {
    Sprite::Update(deltaTime);
}
void Player::Draw() const {
    Sprite::Draw();
    /*if (gameScene::DebugMode) {
        // Draw collision radius.
        al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
    }*/
}


gameScene2* Player::getPlayScene() {
    return dynamic_cast<gameScene2*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
