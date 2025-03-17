// [main.cpp]
// This is the entry point of your game.
// You can register your scenes here, and start the game.
#include "Engine/GameEngine.hpp"
#include "Engine/LOG.hpp"
#include "Scene/startScene.h"
#include "Scene/settingsScene.h"
#include "Scene/playScene.h"
#include "Scene/mode1SettingScene.h"
#include "Scene/mode2SettingScene.h"
#include "Scene/gameScene.h"
#include "Scene/endScene.h"
#include "Scene/scoreboardScene.h"
#include "Scene/gameScene2.h"
#include <iostream>
/*#include <new>

size_t totalAllocated = 0;

void* operator new(size_t size) {
    totalAllocated += size;
    return malloc(size);
}

void operator delete(void* memory, size_t size) noexcept {
    totalAllocated -= size;
    free(memory);
}*/

int main(int argc, char **argv) {
	Engine::LOG::SetConfig(true);
	Engine::GameEngine& game = Engine::GameEngine::GetInstance();
    game.AddNewScene("start", new startScene());
    game.AddNewScene("settings", new settingsScene());
    game.AddNewScene("play", new playScene());
    game.AddNewScene("mode1Setting", new mode1SettingScene());
    game.AddNewScene("mode2Setting", new mode2SettingScene());
    game.AddNewScene("end", new endScene());
    game.AddNewScene("scoreboard", new scoreboardScene());
    game.AddNewScene("game1", new gameScene());
    game.AddNewScene("game2", new gameScene2());
	game.Start("start", 144, 1920, 1080);
    //std::cout << "Total allocated: " << totalAllocated << " bytes" << std::endl;
	return 0;
}
