//
// Created by GaGa-PC on 2024/6/15.
//

#include "Enemy.h"
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
#include "Scene/gameScene2.h"
#include "Settings/gameSettings.h"
/*gameScene* Player::getPlayScene() {
    return dynamic_cast<gameScene*>(Engine::GameEngine::GetInstance().GetActiveScene());
}*/
Enemy::Enemy(std::string img, float x, float y, float radius, float speed, float hp, float unknockness, float damage) :
        Engine::Sprite(img, x, y), speed(speed), hp(hp) {
    CollisionRadius = radius;
    Velocity = Engine::Point(0, 0);
}

void Enemy::Update(float deltaTime) {
    Sprite::Update(deltaTime);
}

void Enemy::Draw() const {
    Sprite::Draw();
    /*if (gameScene::DebugMode) {
        // Draw collision radius.
        al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
    }*/
}
void Enemy::Hit() {
    hp -= 1;
    if (hp <= 0) {
        getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
        AudioHelper::PlayAudio("explosion.wav");
    }else{
        AudioHelper::PlayAudio("damaged.mp3");

    }
}
void Enemy::explode(){
    getPlayScene()->EnemyGroup->RemoveObject(objectIterator);
    AudioHelper::PlayAudio("explosion.wav");
}

gameScene2* Enemy::getPlayScene() {
    return dynamic_cast<gameScene2*>(Engine::GameEngine::GetInstance().GetActiveScene());
}
