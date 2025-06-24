CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
SHARED_FLAGS = -fPIC -shared
INCLUDES = -I./extern/libcbv2g/include -I./extern/json/include -I./extern/cxxopts/include
LDFLAGS = -L./extern/libcbv2g/build/lib/cbv2g
LIBS = -lcbv2g_din -lcbv2g_exi_codec -lcbv2g_tp

BUILD_DIR = build
# Find all cpp files in src directory
SRCS = $(wildcard src/*.cpp)
# Generate object file names from source files
OBJS = $(SRCS:%.cpp=%.o)
EXEC = $(BUILD_DIR)/DINProcessor
SHARED = $(BUILD_DIR)/lib-DINProcessor.so

all: libcbv2g $(EXEC) $(SHARED)

libcbv2g:
	cd ./extern/libcbv2g && cmake -S . -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=1 && ninja -C build

prog: libcbv2g $(EXEC)

libs: libcbv2g $(SHARED)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile object files (add -fPIC for shared library compatibility)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SHARED_FLAGS) $(INCLUDES) -c $< -o $@

# Link object files to create executable
$(EXEC): $(BUILD_DIR) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

# Link object files to create shared library
$(SHARED): $(BUILD_DIR) $(OBJS)
	$(CXX) $(SHARED_FLAGS) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS)
	rm -rf $(BUILD_DIR)
	rm -rf ./extern/libcbv2g/build