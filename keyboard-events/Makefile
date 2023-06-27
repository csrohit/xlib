target=key-press

BUILD_DIR = build
SRC_DIRS=src
SRCS = $(shell find $(SRC_DIRS) -name '*.cpp')
OBJS = $(SRCS:%.cpp=$(BUILD_DIR)/%.o)
LD_FLAGS = -lX11
CPP_FLAGS = -DXK_MISCELLANY

all: $(target)

$(target): $(OBJS)
	g++ -o $@ $^ $(LD_FLAGS) $(CPP_FLAGS) $(CXXFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	g++ $(CPP_FLAGS) $(CXXFLAGS) -o $@ -c $<


clean:
	rm $(OBJS) $(target)

