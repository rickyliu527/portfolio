//
// Created by GaGa-PC on 2024/5/17.
//

#include "gameSettings.h"
#include <string>
#include <random>
float WallSpawnSpeed = 1.0;
float WallMoveSpeed = 1.0;
int Hp = 1;
float EnemySpawnSpeed = 1.0;
long long int survival_time = 0;
int mode=1;
int points=0;
const double alpha = 0.1; // 學習率
const double gamma = 0.9; // 折扣因子
const double epsilon = 0.1; // 探索率
const int screenWidth = 1920;
const int screenHeight = 1080;
std::random_device rd;
std::mt19937 rng(rd());
double tau = 0.5; // 调整 tau 的值来控制探索性

std::string Music = "happy.ogg";
std::string UIfont = "kidsfontW4.ttf";