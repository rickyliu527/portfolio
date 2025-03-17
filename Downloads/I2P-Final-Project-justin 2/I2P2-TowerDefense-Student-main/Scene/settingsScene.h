//
// Created by GaGa-PC on 2024/5/16.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_SETTINGSSCENE_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_SETTINGSSCENE_H
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
class settingsScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    explicit settingsScene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick();
    void BGMSlideOnValueChanged(float value);
    void SFXSlideOnValueChanged(float value);
};
#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_SETTINGSSCENE_H
