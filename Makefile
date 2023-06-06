SRC_DIR := source
OBJ_DIR := intermediate
TARGET_DIR := bin
TARGET_NAME := main.exe

VAR := $(SRC_DIR)/*.cpp $(SRC_DIR)/imgui/*.cpp $(SRC_DIR)/render/*.cpp $(SRC_DIR)/ui/*.cpp $(SRC_DIR)/shading/*.cpp
SRCS := $(wildcard $(VAR))
OBJS := $(addprefix $(OBJ_DIR)/, $(subst source/,,$(SRCS:.cpp=.o)))
TARGET := $(TARGET_DIR)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_DIR)/$(TARGET_NAME)

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O0 -ggdb3
INCLUDE := -I./include -I./library -I./library/imgui -I"C:\opencv\install\include"

LDFLAGS := 
LIBDIR := -L"./library" -L"C:\opencv\lib"
LIBS := -lglfw3 -lopengl32 -lgdi32 -lglew32 -lopencv_core470 -lopencv_highgui470 -lopencv_imgproc470 -lopencv_imgcodecs470

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $(OBJS) $(LDFLAGS) $(LIBDIR) $(LIBS)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c -o $@ $<

CLEAN_DIR := $(OBJ_DIR)/*.o $(OBJ_DIR)/render/*.o $(OBJ_DIR)/ui/*o $(OBJ_DIR)/shading/*.o
CLEANS := $(wildcard $(CLEAN_DIR))
clean:
	del $(subst /,\,$(CLEANS))

CLEAN_ALL_DIR := $(OBJ_DIR)/*.o $(OBJ_DIR)/render/*.o $(OBJ_DIR)/ui/*o $(OBJ_DIR)/shading/*.o $(OBJ_DIR)/imgui/*.o
CLEAN_ALLS := $(wildcard $(CLEAN_ALL_DIR))

clean-all:
	del $(subst /,\,$(CLEAN_ALLS))

test:
	echo $(OBJS)