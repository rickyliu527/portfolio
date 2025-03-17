//
// Created by GaGa-PC on 2024/5/28.
//

#include "Wall.h"
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


Wall::Wall(std::string img, float x, float y, MovingDirection movingDir, float speed) :
        Engine::Sprite(img, x, y), speed(speed), movingDir(movingDir){
}

void Wall::Update(float deltaTime) {
    Sprite::Update(deltaTime);
}
void Wall::Draw() const {
    Sprite::Draw();
}