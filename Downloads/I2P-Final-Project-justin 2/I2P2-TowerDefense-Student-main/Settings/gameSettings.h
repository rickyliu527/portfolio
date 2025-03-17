//
// Created by GaGa-PC on 2024/5/17.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_GAMESETTINGS_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_GAMESETTINGS_H
#include <string>
#include <random>
extern float WallSpawnSpeed;
extern int Hp;
extern float EnemySpawnSpeed;
extern std::string Music;
extern std::string UIfont;
extern int mode;
extern int points;
extern long long int survival_time;
extern const int gridSize; // 假設為10x10的網格
extern const int numStates; // 狀態由敵人和玩家位置組成
extern const int numActions; // 動作：上(0), 下(1), 左(2), 右(3), 迅速平移(4), 加速(5)
extern const double alpha; // 學習率
extern const double gamma; // 折扣因子
extern const double epsilon; // 探索率
extern const int screenWidth;
extern const int screenHeight;
extern float WallMoveSpeed;
extern std::mt19937 rng;
extern std::random_device rd;
extern double tau;
#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_GAMESETTINGS_H
