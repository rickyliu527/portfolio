//
// Created by GaGa-PC on 2024/6/15.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_ENEMY_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_ENEMY_H
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Settings/gameSettings.h"
#include <iostream>
class gameScene2;
class Enemy : public Engine::Sprite {
protected:
    //std::vector<Engine::Point> path;
    float speed;
    float hp;
    float unknockness;
    float damage;
    //gameScene* getPlayScene();
public:

    Enemy(std::string img, float x, float y, float radius, float speed, float hp, float unknockness, float damage);
    void Update(float deltaTime) override;
    void Draw() const override;
    void Hit();
    void explode();
    gameScene2* getPlayScene();
};
#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_ENEMY_H
