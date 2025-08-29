# ==============================================================================
# Usage:
# make              # compile binary
# make debug        # compile debug
# make release      # compile release
# make run          # run debug
# make run-release  # run release
# make test         # run tests
# make test-release # run tests
# make clean        # remove all binaries and objects
# ==============================================================================

# Project name
EXECUTABLE := larry

# Directories
SRC_DIR        := src
BUILD_DIR      := bin
TEST_DIR       := tests
DEBUG_DIR      := $(BUILD_DIR)/debug
RELEASE_DIR    := $(BUILD_DIR)/release
TEST_BUILD_DIR := $(DEBUG_DIR)/test_obj
TEST_BUILD_DIR_RELEASE := $(RELEASE_DIR)/test_obj

DEBUG_OBJ_DIR   := $(DEBUG_DIR)/obj
RELEASE_OBJ_DIR := $(RELEASE_DIR)/obj

INCLUDE_DIRS   := $(shell find ${SRC_DIR} -type d)
CFLAGS_INCLUDE := $(addprefix -I,$(INCLUDE_DIRS))

# Compiler
CC := gcc

# Flags
SDL_CFLAGS  := $(shell pkg-config --cflags sdl3)
SDL_LDFLAGS := $(shell pkg-config --libs sdl3)

CFLAGS_COMMON  := -Wall -Wextra -MMD -MP $(CFLAGS_INCLUDE) $(SDL_CFLAGS)
CFLAGS_DEBUG   := -g -O0 $(CFLAGS_COMMON) -fsanitize=address -fno-omit-frame-pointer
# CFLAGS_DEBUG   := -g -O0 $(CFLAGS_COMMON)
CFLAGS_RELEASE := -O2 $(CFLAGS_COMMON)

# Sources and Objects
SRCS      := $(shell find $(SRC_DIR) -name '*.c')
TEST_SRCS := $(shell find $(TEST_DIR) -name '*.c')

DEBUG_BIN   := $(DEBUG_DIR)/$(EXECUTABLE)
RELEASE_BIN := $(RELEASE_DIR)/$(EXECUTABLE)
TEST_BIN    := $(DEBUG_DIR)/test_runner
TEST_BIN_RELEASE    := $(RELEASE_DIR)/test_runner

DEBUG_OBJS        := $(patsubst $(SRC_DIR)/%.c,$(DEBUG_OBJ_DIR)/%.o,$(SRCS))
RELEASE_OBJS      := $(patsubst $(SRC_DIR)/%.c,$(RELEASE_OBJ_DIR)/%.o,$(SRCS))
TEST_OBJS         := $(patsubst $(TEST_DIR)/%.c,$(TEST_BUILD_DIR)/%.o,$(TEST_SRCS))
TEST_RELEASE_OBJS := $(patsubst $(TEST_DIR)/%.c,$(TEST_BUILD_DIR_RELEASE)/%.o,$(TEST_SRCS))

SRCS_NO_MAIN           := $(filter-out $(SRC_DIR)/main.c,$(SRCS))
TEST_DEPS_OBJS         := $(patsubst $(SRC_DIR)/%.c,$(DEBUG_OBJ_DIR)/%.o,$(SRCS_NO_MAIN))
TEST_RELEASE_DEPS_OBJS := $(patsubst $(SRC_DIR)/%.c,$(RELEASE_OBJ_DIR)/%.o,$(SRCS_NO_MAIN))

DEBUG_DEPS        := $(DEBUG_OBJS:.o=.d)
RELEASE_DEPS      := $(RELEASE_OBJS:.o=.d)
TEST_DEPS         := $(TEST_OBJS:.o=.d)
TEST_RELEASE_DEPS := $(TEST_RELEASE_OBJS:.o=.d)

# Colors
BLUE  := \033[1;36m
GREEN := \033[1;32m
NC    := \033[0m # No Color

# ==============================================================================
# Targets
# ==============================================================================

.PHONY: all debug release run run-release test test-release clean
all: debug

debug: $(DEBUG_BIN)
	@echo "\n${GREEN}Build Successful!${NC}"

release: $(RELEASE_BIN)
	@echo "\n${GREEN}Build Successful!${NC}"

run: $(DEBUG_BIN)
	@echo "\n${GREEN}Running...${NC}"
	./$(DEBUG_BIN)

run-release: $(RELEASE_BIN)
	@echo "\n${GREEN}Running...${NC}"
	./$(RELEASE_BIN)

test: $(TEST_BIN)
	@echo "\n${GREEN}Running Unit Tests...${NC}"
	./$(TEST_BIN)

test-release: $(TEST_BIN_RELEASE)
	@echo "\n${GREEN}Running Unit Tests...${NC}"
	./$(TEST_BIN_RELEASE)

clean:
	@echo "${GREEN}Cleaning up...${NC}"
	rm -rf $(BUILD_DIR)

# ==============================================================================
# Build Rules
# ==============================================================================

# executables
$(DEBUG_BIN): $(DEBUG_OBJS)
	@echo "\n${GREEN}Linking ${BLUE}${@F}${NC}"
	$(CC) $(CFLAGS_DEBUG) $^ -o $@

$(RELEASE_BIN): $(RELEASE_OBJS)
	@echo "\n${GREEN}Linking ${BLUE}${@F}${NC}"
	$(CC) $(CFLAGS_RELEASE) $^ -o $@

$(TEST_BIN): $(TEST_OBJS) $(TEST_DEPS_OBJS)
	@echo "\n${GREEN}Linking Test Runner ${BLUE}${@F}${NC}"
	$(CC) $(CFLAGS_DEBUG) $^ -o $@

$(TEST_BIN_RELEASE): $(TEST_RELEASE_OBJS) $(TEST_RELEASE_DEPS_OBJS)
	@echo "\n${GREEN}Linking Test Runner ${BLUE}${@F}${NC}"
	$(CC) $(CFLAGS_RELEASE) $^ -o $@

# object files
$(DEBUG_OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "${GREEN}Compiling ${BLUE}$<${NC}"
	$(CC) $(CFLAGS_DEBUG) -c $< -o $@

$(RELEASE_OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "${GREEN}Compiling $<${NC}"
	$(CC) $(CFLAGS_RELEASE) -c $< -o $@

$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "${GREEN}Compiling Test ${BLUE}$<${NC}"
	$(CC) $(CFLAGS_DEBUG) -I$(TEST_DIR) -c $< -o $@

$(TEST_BUILD_DIR_RELEASE)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "${GREEN}Compiling Test ${BLUE}$<${NC}"
	$(CC) $(CFLAGS_RELEASE) -I$(TEST_DIR) -c $< -o $@

# ==============================================================================
# Dependency Inclusion
# ==============================================================================

-include $(DEBUG_DEPS)
-include $(RELEASE_DEPS)
-include $(TEST_DEPS)
-include $(TEST_RELEASE_DEPS)
