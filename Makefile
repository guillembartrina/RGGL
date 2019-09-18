
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

CXX_FLAGS = -std=c++17 -O2

D_SFML_INC = sfml/include
SFML_FLAGS = -Lsfml/lib -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio

D_IMGUI_INC = imgui/include
IMGUI_FLAGS = -Limgui/lib -limgui

D_IMGUI = imgui
F_IMGUI = $(wildcard $(D_IMGUI)/*.cpp)
O_IMGUI = $(patsubst $(D_IMGUI)/%.cpp,$(D_OBJ)/%.o,$(F_IMGUI))

GL_FLAGS = -lopengl32 -lglu32


#COMMANDS
rggl.exe: $(D_OBJ)/main.o $(F_OBJ_M) $(F_OBJ) $(O_IMGUI)
	g++ -o $(D_BIN)/$@ $(D_OBJ)/main.o $(F_OBJ_M) $(F_OBJ) $(O_IMGUI) $(IMGUI_FLAGS) $(SFML_FLAGS) $(GL_FLAGS)

$(D_OBJ)/main.o: main.cpp
	g++ -c $< -o $@ -I$(D_INC_M) -I$(D_INC) -I$(D_IMGUI) -I$(D_IMGUI_INC) -I$(D_SFML_INC) $(CXX_FLAGS) $(GL_FLAGS)

$(D_OBJ)/%.o: $(D_SRC_M)/%.cpp $(D_INC_M)/%.hpp
	g++ -c $< -o $@ -I$(D_INC_M) -I$(D_INC) -I$(D_IMGUI) -I$(D_IMGUI_INC) -I$(D_SFML_INC) $(CXX_FLAGS) $(GL_FLAGS)

$(D_OBJ)/%.o: $(D_SRC)/%.cpp $(D_INC)/%.hpp
	g++ -c $< -o $@ -I$(D_INC_M) -I$(D_INC) -I$(D_IMGUI) -I$(D_IMGUI_INC) -I$(D_SFML_INC) $(CXX_FLAGS) $(GL_FLAGS)

$(D_OBJ)/%.o: $(D_IMGUI)/%.cpp
	g++ -c $< -o $@ -I$(D_INC_M) -I$(D_INC) -I$(D_IMGUI) -I$(D_IMGUI_INC) -I$(D_SFML_INC) $(CXX_FLAGS) $(GL_FLAGS)

cleanall:
	rm $(F_OBJ)
	rm $(F_OBJ_M)
	rm $(D_BIN)/*.exe

clean:
	rm $(F_OBJ)
	rm $(D_BIN)/*.exe