

#include "Circle.h"
#include <allegro5/base.h>
#include <random>
#include <string>
#include "Engine/Point.hpp"
#include <iostream>
#include <ctime>
#include "Engine/GameEngine.hpp"
#include "Scene/gameScene2.h"
#include <cmath>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include "Settings/gameSettings.h"
#include "Engine/Collider.hpp"

Circle::Circle(int x, int y) : Enemy("./circle.png", x, y, 1.0, 1.5, 3, 1.0, 1.0){
    isRotated = true;
    CollisionRadius = 20;
}
void Circle::Draw() const {
    Enemy::Draw();
    //al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
}


void Circle::Update(float deltaTime) {
    Enemy::Update(deltaTime);
    gameScene2* scene = getPlayScene();
    Player* player = scene->player;
    /*
    float targetX = scene->player->Position.x;
    float targetY = scene->player->Position.y;

    float deltaX = targetX - Position.x;
    float deltaY = targetY - Position.y;
    float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

    if (distance > 1) {
        std::cout << "distance: " << distance << std::endl;
        float unit_vector_x = deltaX / distance;
        float unit_vector_y = deltaY / distance;
        Position.x += unit_vector_x * speed;
        Position.y += unit_vector_y * speed;

        //TODO: Rotate the triangle to the direction of the player
        Engine::Point originRotation = Engine::Point(cos(Rotation - ALLEGRO_PI / 2), sin(Rotation - ALLEGRO_PI / 2));
        Engine::Point targetRotation = (scene->player->Position - Position).Normalize();
        float maxRotateRadian = rotateRadian * deltaTime;
        float cosTheta = originRotation.Dot(targetRotation);
        // Might have floating-point precision error.
        if (cosTheta > 1) cosTheta = 1;
        else if (cosTheta < -1) cosTheta = -1;
        float radian = acos(cosTheta);
        Engine::Point rotation;
        if (abs(radian) <= maxRotateRadian)
            rotation = targetRotation;
        else
            rotation = ((abs(radian) - maxRotateRadian) * originRotation + maxRotateRadian * targetRotation) / radian;
        // Add 90 degrees (PI/2 radian), since we assume the image is oriented upward.
        Rotation = atan2(rotation.y, rotation.x) + ALLEGRO_PI / 2;
    }*/

}
gameScene2* Circle::getPlayScene() {
    return dynamic_cast<gameScene2*>(Engine::GameEngine::GetInstance().GetActiveScene());
}

