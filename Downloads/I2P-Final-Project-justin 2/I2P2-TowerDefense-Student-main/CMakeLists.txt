cmake_minimum_required(VERSION 3.26.4)
project(2024_I2P2_TowerDefense_with_answer)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(Resource)

file(COPY Resource/audios DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY allegro_monolith-5.2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(2024_I2P2_TowerDefense_with_answer
        Engine/Allegro5Exception.cpp
        Engine/Allegro5Exception.hpp
        Engine/AudioHelper.cpp
        Engine/AudioHelper.hpp
        Engine/Collider.cpp
        Engine/Collider.hpp
        Engine/GameEngine.cpp
        Engine/GameEngine.hpp
        Engine/Group.cpp
        Engine/Group.hpp
        Engine/IControl.cpp
        Engine/IControl.hpp
        Engine/IObject.cpp
        Engine/IObject.hpp
        Engine/IScene.cpp
        Engine/IScene.hpp
        Engine/LOG.cpp
        Engine/LOG.hpp
        main.cpp
        Engine/Point.cpp
        Engine/Point.hpp
        Engine/Resources.cpp
        Engine/Resources.hpp
        Engine/Sprite.cpp
        Engine/Sprite.hpp
        UI/Component/Image.cpp
        UI/Component/Image.hpp
        UI/Component/ImageButton.cpp
        UI/Component/ImageButton.hpp
        UI/Component/Label.cpp
        UI/Component/Label.hpp
        UI/Component/Image.cpp
        UI/Component/Image.hpp
        UI/Component/Slider.cpp
        UI/Component/Slider.hpp
        Scene/startScene.cpp
        Scene/startScene.h
        Scene/settingsScene.cpp
        Scene/settingsScene.h
        Scene/playScene.cpp
        Scene/playScene.h
        Scene/mode1SettingScene.cpp
        Scene/mode1SettingScene.h
        Scene/mode2SettingScene.cpp
        Scene/mode2SettingScene.h
        Settings/gameSettings.cpp
        Settings/gameSettings.h
        Scene/gameScene.cpp
        Scene/gameScene.h
        Scene/scoreboardScene.cpp
        Scene/scoreboardScene.h
        UI/Component/Line.cpp
        UI/Component/Line.h
        Player/Player.cpp
        Player/Player.h
        Wall/Wall.cpp
        Wall/Wall.h
        Portal/Portal.cpp
        Portal/Portal.h
        Scene/endScene.cpp
        Scene/endScene.h
        Enemy/Enemy.cpp
        Enemy/Enemy.h
        Enemy/Circle.cpp
        Enemy/Circle.h
        Enemy/Triangle.cpp
        Enemy/Triangle.h
        Enemy/Square.cpp
        Enemy/Square.h
        Scene/gameScene2.cpp
        Scene/gameScene2.h
        Bullet/Bullet.cpp
        Bullet/Bullet.h
        Scene/scoreboardScene.cpp
        Scene/scoreboardScene.h
        Enemy/Square.cpp
        Enemy/Square.h
        Enemy/Circle.cpp
        Enemy/Circle.h
)

IF (WIN32)
    target_include_directories(2024_I2P2_TowerDefense_with_answer PUBLIC C:/allegro/include)
    target_link_directories(2024_I2P2_TowerDefense_with_answer PUBLIC C:/allegro/lib)
    target_compile_options(2024_I2P2_TowerDefense_with_answer PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2024_I2P2_TowerDefense_with_answer liballegro_monolith)
ELSE()
    find_package(PkgConfig)
    pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_main-5 allegro_font-5 allegro_dialog-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)
    target_include_directories(2024_I2P2_TowerDefense_with_answer PUBLIC lib)
    target_include_directories(2024_I2P2_TowerDefense_with_answer PUBLIC ${ALLEGRO5_INCLUDE_DIRS})
    target_compile_options(2024_I2P2_TowerDefense_with_answer PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2024_I2P2_TowerDefense_with_answer ${ALLEGRO5_LIBRARIES})
    target_link_directories(2024_I2P2_TowerDefense_with_answer PUBLIC ${ALLEGRO5_LIBRARY_DIRS})
ENDIF()