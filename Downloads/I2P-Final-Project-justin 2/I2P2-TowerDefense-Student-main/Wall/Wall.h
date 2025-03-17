//
// Created by GaGa-PC on 2024/5/28.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_WALL_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_WALL_H
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"

class Wall: public Engine::Sprite {


public:
    enum MovingDirection {
        UP, DOWN, LEFT, RIGHT
    };
    enum MovingDirection movingDir;
    Wall(std::string img, float x, float y, MovingDirection movingDir ,float speed=1.0f);
    void Update(float deltaTime) override;
    void Draw() const override;
protected:
    float speed;
};
#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_WALL_H
