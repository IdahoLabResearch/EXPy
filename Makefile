CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
SHARED_FLAGS = -fPIC -shared
INCLUDES = -I./extern/libcbv2g/include -I./extern/json/include -I./extern/cxxopts/include -I./include
LDFLAGS = -L./extern/libcbv2g/build/lib/cbv2g
LIBS = -lcbv2g_iso2 -lcbv2g_din -lcbv2g_exi_codec -lcbv2g_tp

BUILD_DIR = build

COMMON_SRCS = src/common.cpp
COMMON_OBJS = $(COMMON_SRCS:%.cpp=%.o)

ISO2_SRCS = src/ISO2Processor.cpp
DIN_SRCS = src/DINProcessor.cpp
APP_SRCS = src/AppHandshakeProcessor.cpp

ISO2_OBJS = $(ISO2_SRCS:%.cpp=%.o) $(COMMON_OBJS)
DIN_OBJS = $(DIN_SRCS:%.cpp=%.o) $(COMMON_OBJS)
APP_OBJS = $(APP_SRCS:%.cpp=%.o) $(COMMON_OBJS)

EXEC = $(BUILD_DIR)/DINProcessor $(BUILD_DIR)/AppHandshakeProcessor $(BUILD_DIR)/ISO2Processor
SHARED = $(BUILD_DIR)/lib-DINProcessor.so $(BUILD_DIR)/lib-AppHandshakeProcessor.so $(BUILD_DIR)/lib-ISO2Processor.so

all: libcbv2g $(EXEC) $(SHARED)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

libcbv2g:
	cd ./extern/libcbv2g && cmake -S . -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=1 && ninja -C build

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SHARED_FLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/ISO2Processor: $(BUILD_DIR) $(ISO2_OBJS)
	$(CXX) $(CXXFLAGS) $(ISO2_OBJS) $(LDFLAGS) $(LIBS) -o $@

$(BUILD_DIR)/lib-ISO2Processor.so: $(BUILD_DIR) $(ISO2_OBJS)
	$(CXX) $(SHARED_FLAGS) $(ISO2_OBJS) $(LDFLAGS) $(LIBS) -o $@

$(BUILD_DIR)/DINProcessor: $(BUILD_DIR) $(DIN_OBJS)
	$(CXX) $(CXXFLAGS) $(DIN_OBJS) $(LDFLAGS) $(LIBS) -o $@

$(BUILD_DIR)/lib-DINProcessor.so: $(BUILD_DIR) $(DIN_OBJS)
	$(CXX) $(SHARED_FLAGS) $(DIN_OBJS) $(LDFLAGS) $(LIBS) -o $@

$(BUILD_DIR)/AppHandshakeProcessor: $(BUILD_DIR) $(APP_OBJS)
	$(CXX) $(CXXFLAGS) $(APP_OBJS) $(LDFLAGS) $(LIBS) -o $@

$(BUILD_DIR)/lib-AppHandshakeProcessor.so: $(BUILD_DIR) $(APP_OBJS)
	$(CXX) $(SHARED_FLAGS) $(APP_OBJS) $(LDFLAGS) $(LIBS) -o $@



clean:
	rm -f $(DIN_OBJS) $(APP_OBJS) $(ISO2_OBJS)
	rm -rf $(BUILD_DIR)
	rm -rf ./extern/libcbv2g/build