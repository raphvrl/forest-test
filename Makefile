ifeq ($(OS), Windows_NT)
	ifdef MSYSTEM

	CXX = clang++

	CMAKE = cmake

	PRINT = echo
	RM = rm -rf
	MKDIR = mkdir -p
	CP = cp -r

	else
		$(error "This makefile only supports MSYS2/MinGW64 environment")
	endif
else
	$(info "This makefile do not support Linux yet")
endif

SRC_DIR = src
BIN_DIR = bin
OBJ_DIR = $(BIN_DIR)/src
LIB_DIR = lib

CXXFLAGS = -Wall -Wextra -Werror -O3

SRC = $(shell find $(SRC_DIR) -name "*.cpp")
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
DEP = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.d,$(SRC))

DEPFLAGS = -MMD -MP -MF "$(@:%.o=%.d)"

CXXFLAGS += $(DEPFLAGS)
CXXFLAGS += -I$(SRC_DIR)

GLFW_DIR = $(LIB_DIR)/glfw
GLFW_INC = $(GLFW_DIR)/include
GLFW_BIN = $(BIN_DIR)/glfw
GLFW_LIB_DIR = $(GLFW_BIN)/src
GLFW_LIB = $(GLFW_LIB_DIR)/libglfw3.a

GLFW_FLAGS = -DGLFW_BUILD_EXAMPLES=OFF \
			 -DGLFW_BUILD_TESTS=OFF \
			 -DGLFW_BUILD_DOCS=OFF

CXXFLAGS += -I$(GLFW_INC)
LDFLAGS += -L$(GLFW_LIB_DIR) -lglfw3 -lgdi32

GLM_DIR = $(LIB_DIR)/glm

CXXFLAGS += -I$(GLM_DIR)

TINYGLTF_DIR = $(LIB_DIR)/tinygltf
CXXFLAGS += -I$(TINYGLTF_DIR)

VULKAN_INC = $(VULKAN_SDK)/Include
VULKAN_LIB = $(VULKAN_SDK)/Lib

CXXFLAGS += -I$(VULKAN_INC)
LDFLAGS += -L$(VULKAN_LIB) -lvulkan-1

VMA_DIR = $(LIB_DIR)/vma
VMA_INC = $(VMA_DIR)/include

CXXFLAGS += -I$(VMA_INC)

ifeq ($(OS), Windows_NT)
	GLSLC = $(VULKAN_SDK)/Bin/glslc.exe
else
	$(error "This makefile only supports Windows")
endif

SHADER_DIR = $(SRC_DIR)/shaders
SHADER_BIN = assets/shaders

SHADER_VERT = $(wildcard $(SHADER_DIR)/*.vert)
SHADER_FRAG = $(wildcard $(SHADER_DIR)/*.frag)

SHADER_VERT_SPV = $(patsubst $(SHADER_DIR)/%.vert,$(SHADER_BIN)/%.vert.spv,$(SHADER_VERT))
SHADER_FRAG_SPV = $(patsubst $(SHADER_DIR)/%.frag,$(SHADER_BIN)/%.frag.spv,$(SHADER_FRAG))

SHADERS_OBJ = $(SHADER_VERT_SPV) $(SHADER_FRAG_SPV)

ifeq ($(OS), Windows_NT)
	TARGET = forest.exe
else
	TARGET = forest.out
endif

MODE ?= debug
ifeq ($(MODE), debug)
	CXXFLAGS += -g
else ifeq ($(MODE), release)
	CXXFLAGS += -DNDEBUG
	LDFLAGS += -static
	ifeq ($(OS), Windows_NT)
		LDFLAGS += -mwindows
	endif
else
	$(error "Invalid mode. Use 'debug' or 'release'.")
endif

release:
	@$(PRINT) "Building release"
	@$(MAKE) MODE=release all

all: $(SHADERS_OBJ) $(TARGET)

$(TARGET): $(OBJ)| $(GLFW_LIB)
	@$(PRINT) "Linking $@"
	@$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(PRINT) "Compiling $<"
	@$(MKDIR) $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEP)

$(SHADER_BIN)/%.vert.spv: $(SHADER_DIR)/%.vert
	@$(PRINT) "Compiling shader $<"
	@$(MKDIR) $(dir $@)
	@$(GLSLC) -o $@ $<

$(SHADER_BIN)/%.frag.spv: $(SHADER_DIR)/%.frag
	@$(PRINT) "Compiling shader $<"
	@$(MKDIR) $(dir $@)
	@$(GLSLC) -o $@ $<
	
shaders: $(SHADERS_OBJ)

clean-shaders:
	@$(PRINT) "Cleaning shaders"
	@$(RM) $(SHADER_BIN)/*.spv

glfw: $(GLFW_LIB)

$(GLFW_LIB):
	@$(PRINT) "Building GLFW"
	@$(MKDIR) $(GLFW_BIN)
	@$(CMAKE) -S $(GLFW_DIR) -B $(GLFW_BIN) $(GLFW_FLAGS)
	@$(CMAKE) --build $(GLFW_BIN)

.NOTPARALLEL: $(GLFW_LIB)

clean-glfw:
	@$(PRINT) "Cleaning GLFW"
	@$(RM) $(GLFW_BIN)

clean:
	@$(PRINT) "Cleaning code"
	@$(RM) $(SHADER_BIN)
	@$(RM) $(OBJ_DIR)
	@$(RM) $(TARGET)

clean-all:
	@$(PRINT) "Cleaning all"
	@$(RM) $(BIN_DIR)

.PHONY: all clean clean-all glfw clean-glfw
