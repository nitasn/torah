SRC_DIR := engine
OUT_DIR := build

OBJ_DIR := $(OUT_DIR)/.o
DEP_DIR := $(OUT_DIR)/.d

EXECUTABLE := $(OUT_DIR)/main

__generate_depenencies :=  -MMD -MP -I./$(SRC_DIR)

CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O3 $(__generate_depenencies)
LINKER_FLAGS := -liconv


#################################################################
###                 P H O N Y   T A R G E T S                 ###
#################################################################


PHONY_TARGETS := executable run clean help

.PHONY: $(PHONY_TARGETS)


executable: $(EXECUTABLE)

args := # to be overriden from command line (e.g. make run args="arg1 arg2 arg3")

run: executable
	@echo "running $(EXECUTABLE) $(args)"
	@$(EXECUTABLE) $(args)

clean:
	rm -rf $(OUT_DIR)

help:
	@echo "Available targets:"
	@echo "  make       - build the code"
	@echo "  make run   - build & run the code"
	@echo "  make clean - delete 'build' folder"
	@echo "  make help  - show this menu"


#################################################################
###                  A C T U A L   B U I L D                  ###
#################################################################


# recursively find all .cpp files in the SRC_DIR
SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
DEP_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(DEP_DIR)/%.d,$(SRC_FILES))

-include $(DEP_FILES)

$(EXECUTABLE): $(OBJ_FILES)
	@mkdir -p $(OUT_DIR)
	@echo "linking $@"
	@$(CXX) $(LINKER_FLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@) $(dir $(DEP_DIR)/$*.d)
	@echo "compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@mv $(OBJ_DIR)/$*.d $(DEP_DIR)/$*.d
