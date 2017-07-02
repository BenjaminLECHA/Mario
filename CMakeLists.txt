set(EXECUTABLE_NAME "MarioGame")
add_executable(${EXECUTABLE_NAME} src/main.cpp src/KeyboardInput.hpp
        src/Background.hpp src/Tilemap.hpp src/Character.hpp src/Game.hpp
        src/HUD.hpp src/GameSound.hpp src/Goomba.hpp src/Goomba.cpp
        src/Tilemap.cpp src/Character.cpp src/HUD.cpp src/KeyboardInput.cpp src/Background.cpp src/Game.cpp src/GameSound.cpp src/Koopa.hpp src/Koopa.cpp src/Menu.hpp src/Menu.cpp)
cmake_minimum_required(VERSION 3.3)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Detect and add SFML
set(SFML_DIR "SFML/share/cmake/Modules")
set(CMAKE_MODULE_PATH "/home/minato/Logiciels/SFML-2.4.1/share/SFML/cmake/Modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()
