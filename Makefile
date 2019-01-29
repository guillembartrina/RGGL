
#VARS, FLAGS
D_INC_M = inc_m
D_INC = inc
D_SRC_M = src_m
D_SRC = src
D_OBJ = obj
D_BIN = bin
F_SRC_M = $(wildcard $(D_SRC_M)/*.cpp)
F_SRC = $(wildcard $(D_SRC)/*.cpp)
F_OBJ_M = $(patsubst $(D_SRC_M)/%.cpp,$(D_OBJ)/%.o,$(F_SRC_M))
F_OBJ = $(patsubst $(D_SRC)/%.cpp,$(D_OBJ)/%.o,$(F_SRC))

CXX_FLAGS = -std=c++11

D_SFML_INC = sfml/include
SFML_FLAGS = -Lsfml/lib -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio

D_BOX2D_INC = box2d/include
BOX2D_FLAGS = -Lbox2d/lib -lBox2D

#COMMANDS
game.exe: $(D_OBJ)/main.o $(F_OBJ_M) $(F_OBJ)
	g++ -o $(D_BIN)/$@ $(D_OBJ)/main.o $(F_OBJ_M) $(F_OBJ) $(SFML_FLAGS) $(BOX2D_FLAGS)

$(D_OBJ)/main.o: main.cpp
	g++ -c $< -o $@ -I$(D_INC_M) -I$(D_INC) -I$(D_SFML_INC) -I$(D_BOX2D_INC) $(CXX_FLAGS)

$(D_OBJ)/%.o: $(D_SRC_M)/%.cpp $(D_INC_M)/%.hpp
	g++ -c $< -o $@ -I$(D_INC_M) -I$(D_INC) -I$(D_SFML_INC) -I$(D_BOX2D_INC) $(CXX_FLAGS)

$(D_OBJ)/%.o: $(D_SRC)/%.cpp $(D_INC)/%.hpp
	g++ -c $< -o $@ -I$(D_INC_M) -I$(D_INC) -I$(D_SFML_INC) -I$(D_BOX2D_INC) $(CXX_FLAGS)

clear:
	rm obj/*.o
	rm bin/*.exe