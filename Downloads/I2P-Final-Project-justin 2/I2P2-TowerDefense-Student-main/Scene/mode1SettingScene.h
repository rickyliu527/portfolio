//
// Created by GaGa-PC on 2024/5/16.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_MODE1SETTINGSCENE_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_MODE1SETTINGSCENE_H

#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
class mode1SettingScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    explicit mode1SettingScene() = default;
    void Initialize() override;
    void Terminate() override;
    void BackOnClick();
    void GameOnClick();
    void WallSpawnSlideOnValueChanged(float value);
};


#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_MODE1SETTINGSCENE_H
