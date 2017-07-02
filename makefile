LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

CC_FLAGS = -c -O1 -Wall -std=c++11

CXX := g++

all: application

%.o: %.cpp
		$(CXX) $(CC_FLAGS) $< -o $@; mv src/*.o obj

%.o: %.hpp
		$(CXX) $(CC_FLAGS) $< -o $@; mv src/*.o obj

application: src/main.o src/Background.o src/Character.o src/Game.o src/Goomba.o src/GameSound.o src/KeyboardInput.o src/HUD.o src/Tilemap.o src/Koopa.o src/Menu.o
		@echo "Building the game"
		$(CXX) -o application obj/*.o $(LIBS)


clean:
		@echo "Removing object files and executable..."
		rm application obj/*.o