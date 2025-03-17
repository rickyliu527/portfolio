//
// Created by GaGa-PC on 2024/5/21.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_PLAYER_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_PLAYER_H
#include "Engine/Point.hpp"
#include "Engine/Sprite.hpp"
class gameScene;
class gameScene2;
class Player : public Engine::Sprite {
protected:
    //std::vector<Engine::Point> path;
    float speed;
    //gameScene* getPlayScene();
public:
    float hp;
    bool coolDown;
    Player(std::string img, float x, float y, float radius, float speed, float hp);
    void Update(float deltaTime) override;
    void Draw() const override;
    void Hit();
    gameScene2* getPlayScene();
};
#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_PLAYER_H
