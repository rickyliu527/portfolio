//
// Created by GaGa-PC on 2024/6/19.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_BULLET_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_BULLET_H

#include "Engine/Sprite.hpp"
class gameScene2;
namespace Engine {
    struct Point;
}  // namespace Engine

class Bullet : public Engine::Sprite {
protected:
    float speed;
    float forward_x, forward_y;
public:
    explicit Bullet(std::string img, float speed, float position_x, float position_y, float forward_x, float forward_y);
    void Update(float deltaTime) override;
    gameScene2* getPlayScene();
};
#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_BULLET_H
