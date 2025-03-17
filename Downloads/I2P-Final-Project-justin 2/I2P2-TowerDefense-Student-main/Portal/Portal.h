//
// Created by GaGa-PC on 2024/6/5.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_PORTAL_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_PORTAL_H
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
class gameScene;
class Portal : public Engine::Sprite {
protected:
public:
    Portal(std::string img, float x, float y);
    void Update(float deltaTime) override;
    void Draw() const override;
};
#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_PORTAL_H
