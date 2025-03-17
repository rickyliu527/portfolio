//
// Created by GaGa-PC on 2024/6/15.
//

#ifndef INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_CIRCLE_H
#define INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_CIRCLE_H

#include "Enemy/Enemy.h"
#include "Engine/Sprite.hpp"
#include "Player/Player.h"
#include "Enemy/Triangle.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include "Settings/gameSettings.h"

//struct State {
//    float enemy_x, enemy_y;
//    float player_x, player_y;
//    bool operator==(const State &other) const {
//        return enemy_x == other.enemy_x && enemy_y == other.enemy_y &&
//               player_x == other.player_x && player_y == other.player_y;
//    }
//};
//
//struct StateHash {
//    std::size_t operator()(const State &s) const {
//        return std::hash<float>()(s.enemy_x) ^ std::hash<float>()(s.enemy_y) ^
//               std::hash<float>()(s.player_x) ^ std::hash<float>()(s.player_y);
//    }
//};
//
//enum Action { UP, DOWN, LEFT, RIGHT };
class gameScene2;

class Circle : public Enemy {
private:
public:
    Circle(int x, int y);

    void Draw() const override;

    void Update(float deltaTime) override;
    std::unordered_map<State, std::vector<double>, StateHash> Q;
    void Hit();
    gameScene2 *getPlayScene();
    bool isRotated;
    float angle;
    float rotateRadian = 2 * ALLEGRO_PI;
    void initializeQ() {
        Q.clear();
    }
    State currentState = {-1,-1,-1,-1};
    Action tmpAction;
    int prevAction;
    Action chooseActionSoftmax(const State &state, std::mt19937 &rng, double tau) {
        if (Q.find(state) == Q.end()) {
            Q[state] = std::vector<double>(4, 0.0); // 初始化为四个动作的 Q 值为 0
        }

        const auto &q_values = Q[state];

        // 计算 softmax 分布的概率
        std::vector<double> probabilities(4);
        double total = 0.0;
        for (size_t i = 0; i < q_values.size(); ++i) {
            probabilities[i] = std::exp(q_values[i] / tau); // 使用 tau 控制 softmax 的温度
            total += probabilities[i];
        }
        // 归一化为概率分布
        for (size_t i = 0; i < probabilities.size(); ++i) {
            probabilities[i] /= total;
        }

        // 根据 softmax 分布选择动作
        std::uniform_real_distribution<double> uniform(0.0, 1.0);
        double rand_value = uniform(rng);
        double cumulative_prob = 0.0;
        for (size_t i = 0; i < probabilities.size(); ++i) {
            cumulative_prob += probabilities[i];
            if (rand_value <= cumulative_prob) {
                return static_cast<Action>(i);
            }
        }
        // 如果由于浮点数精度问题未能正确选择动作，则选择最后一个动作
        return static_cast<Action>(probabilities.size() - 1);
    }

    void updateQ(const State &state, Action action, double reward, const State &new_state) {
        if (Q.find(state) == Q.end()) {
            Q[state] = std::vector<double>(4, 0.0); // 初始化为四个动作的 Q 值为 0
        }
        if (Q.find(new_state) == Q.end()) {
            Q[new_state] = std::vector<double>(4, 0.0); // 初始化为四个动作的 Q 值为 0
        }
        auto &q_values = Q[state];
        double max_future_q = *std::max_element(Q[new_state].begin(), Q[new_state].end());
        q_values[action] = q_values[action] + alpha * (reward + gamma * max_future_q - q_values[action]);
        std::cout << "Updated Q[" << state.enemy_x << "," << state.enemy_y << "][" << action << "] = " << q_values[action] << std::endl;
    }
    double getReward(const State &state, const State &new_state) {
        // 示例奖励函数：接近玩家时正奖励，远离时负奖励
        double old_distance = std::sqrt(std::abs(state.enemy_x - state.player_x)*std::abs(state.enemy_x - state.player_x) + std::abs(state.enemy_y - state.player_y)*std::abs(state.enemy_y - state.player_y));
        double new_distance = std::sqrt(std::abs(new_state.enemy_x - new_state.player_x)*std::abs(new_state.enemy_x - new_state.player_x) + std::abs(new_state.enemy_y - new_state.player_y)*std::abs(new_state.enemy_y - new_state.player_y));
        return old_distance - new_distance;
    }
};
#endif //INC_2024_I2P2_TOWERDEFENSE_WITH_ANSWER_CIRCLE_H


