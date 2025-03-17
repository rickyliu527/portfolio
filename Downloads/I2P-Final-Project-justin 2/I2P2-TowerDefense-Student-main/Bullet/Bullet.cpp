//
// Created by GaGa-PC on 2024/6/19.
//

#include "Bullet.h"
#include "Engine/Collider.hpp"
#include "Engine/GameEngine.hpp"
#include "Engine/Group.hpp"
#include "Engine/IObject.hpp"
#include "Engine/IScene.hpp"
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
#include "Scene/gameScene2.h"
#include "Enemy/Enemy.h"
#include <iostream>
Bullet::Bullet(std::string img, float speed, float position_x, float position_y, float forward_x, float forward_y) :
        Sprite(img, position_x, position_y), speed(speed), forward_x(forward_x), forward_y(forward_y){
    CollisionRadius = 4;
    //get the angle of the bullet and the player
    float theta = acos((0*forward_x + -1*forward_y) / (sqrt(0*0 + 1*1) * sqrt(forward_x*forward_x + forward_y*forward_y)));

    if(forward_x < 0)
        theta = -theta;
    Rotation = (theta + ALLEGRO_PI / 4);
}
void Bullet::Update(float deltaTime) {
    Sprite::Update(deltaTime);
    Position.x += speed * forward_x;
    Position.y += speed * forward_y;
    gameScene2* scene = getPlayScene();
    for (auto& it : scene->EnemyGroup->GetObjects()) {
        Enemy* enemy = dynamic_cast<Enemy*>(it);
        if (Engine::Collider::IsCircleOverlap(Position, CollisionRadius, enemy->Position, enemy->CollisionRadius)) {
            enemy->Hit();
            getPlayScene()->BulletGroup->RemoveObject(objectIterator);
            return;
        }
    }
    // Check if out of boundary.
    if(Position.x < 0 || Position.x >= getPlayScene()->halfW*2 || Position.y < 0 || Position.y >= getPlayScene()->halfH*2) {
        getPlayScene()->BulletGroup->RemoveObject(objectIterator);
        return;
    }

}

gameScene2* Bullet::getPlayScene() {
    return dynamic_cast<gameScene2*>(Engine::GameEngine::GetInstance().GetActiveScene());
}