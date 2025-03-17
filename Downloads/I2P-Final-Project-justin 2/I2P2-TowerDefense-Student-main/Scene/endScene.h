//
// Created by GaGa-PC on 2024/6/11.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_ENDSCENE_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_ENDSCENE_H
#include <allegro5/allegro_audio.h>
#include <memory>
#include "Engine/IScene.hpp"
class endScene final : public Engine::IScene {
private:
    std::shared_ptr<ALLEGRO_SAMPLE_INSTANCE> bgmInstance;
public:
    explicit endScene() = default;
    void Initialize() override;
    void Terminate() override;
//    void ReadScoreboard();
//    std::vector<int> Scoreboard;
    void BackOnClick();
    void ScoreboardOnClick();
};
#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_ENDSCENE_H
