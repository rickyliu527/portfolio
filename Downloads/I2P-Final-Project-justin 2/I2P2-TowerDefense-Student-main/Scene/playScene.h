//
// Created by GaGa-PC on 2024/5/16.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_PLAYSCENE_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_PLAYSCENE_H
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
class playScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    explicit playScene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick();
    void Mode1OnClick();
    void Mode2OnClick();
    void GameOnClick();
};
#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_PLAYSCENE_H
