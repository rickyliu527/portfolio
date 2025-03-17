//
// Created by GaGa-PC on 2024/6/5.
//

#include "Portal.h"
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


Portal::Portal(std::string img, float x, float y) : Engine::Sprite(img, x, y) {

}
void Portal::Update(float deltaTime) {
    Sprite::Update(deltaTime);
}
void Portal::Draw() const {
    Sprite::Draw();
    /*if (DebugMode) {
        // Draw collision radius.
        al_draw_circle(Position.x, Position.y, 5, al_map_rgb(255, 0, 0), 2);
    }*/
}