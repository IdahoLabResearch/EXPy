CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
SHARED_FLAGS = -fPIC -shared
INCLUDES = -I./extern/libcbv2g/include -I./extern/json/include -I./extern/cxxopts/include -I./include
LDFLAGS = -L./extern/libcbv2g/build/lib/cbv2g
LIBS = -lcbv2g_iso2 -lcbv2g_din -lcbv2g_exi_codec -lcbv2g_tp

BUILD_DIR = build

COMMON_SRCS = src/common.cpp
COMMON_OBJS = $(COMMON_SRCS:src/%.cpp=$(BUILD_DIR)/%.o)

ISO2_SRCS = src/ISO2Processor.cpp
DIN_SRCS = src/DINProcessor.cpp
SAP_SRCS = src/SupportedAppProtocolProcessor.cpp

DIN_GENERATED = src/generated/DIN_marshalers.generated.cpp
DIN_HEADER = extern/libcbv2g/include/cbv2g/din/din_msgDefDatatypes.h
SAP_GENERATED = src/generated/SAP_marshalers.generated.cpp
SAP_HEADER = extern/libcbv2g/include/cbv2g/app_handshake/appHand_Datatypes.h
ISO2_GENERATED = src/generated/ISO2_marshalers.generated.cpp
ISO2_HEADER = extern/libcbv2g/include/cbv2g/iso_2/iso2_msgDefDatatypes.h
CODEGEN_PYTHONPATH = $(CURDIR)/tools

ISO2_OBJS = $(ISO2_SRCS:src/%.cpp=$(BUILD_DIR)/%.o) $(COMMON_OBJS)
DIN_OBJS = $(DIN_SRCS:src/%.cpp=$(BUILD_DIR)/%.o) $(COMMON_OBJS)
SAP_OBJS = $(SAP_SRCS:src/%.cpp=$(BUILD_DIR)/%.o) $(COMMON_OBJS)

EXEC = $(BUILD_DIR)/DINProcessor $(BUILD_DIR)/SupportedAppProtocolProcessor $(BUILD_DIR)/ISO2Processor
SHARED = $(BUILD_DIR)/lib-DINProcessor.so $(BUILD_DIR)/lib-SupportedAppProtocolProcessor.so $(BUILD_DIR)/lib-ISO2Processor.so

all: libcbv2g $(EXEC) $(SHARED)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

LIBCBV2G_PATCHES = $(sort $(wildcard patches/libcbv2g/*.patch))

# Apply local patches against the pristine submodule before configuring cmake.
# `git apply --check` is idempotent: if the patch is already applied, the
# check fails and we skip the apply step, keeping the build re-runnable.
# See ADR-0007 for what the patches do and why they live here instead of
# being committed against the upstream submodule.
libcbv2g:
	for p in $(LIBCBV2G_PATCHES); do \
	  if git -C ./extern/libcbv2g apply --check --reverse "../../$$p" >/dev/null 2>&1; then \
	    echo "skip $$p (already applied)"; \
	  else \
	    git -C ./extern/libcbv2g apply "../../$$p"; \
	    echo "applied $$p"; \
	  fi; \
	done
	cd ./extern/libcbv2g && cmake -S . -B build -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=1 && ninja -C build

$(BUILD_DIR)/%.o: src/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SHARED_FLAGS) $(INCLUDES) -c $< -o $@

$(DIN_GENERATED): $(DIN_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $@)
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen --header $(DIN_HEADER) --out $@

$(SAP_GENERATED): $(SAP_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $@)
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen --header $(SAP_HEADER) --out $@

$(ISO2_GENERATED): $(ISO2_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $@)
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen --header $(ISO2_HEADER) --out $@

$(BUILD_DIR)/DINProcessor.o: $(DIN_GENERATED)
$(BUILD_DIR)/SupportedAppProtocolProcessor.o: $(SAP_GENERATED)
$(BUILD_DIR)/ISO2Processor.o: $(ISO2_GENERATED)

$(BUILD_DIR)/ISO2Processor: $(BUILD_DIR) $(ISO2_OBJS)
	$(CXX) $(CXXFLAGS) $(ISO2_OBJS) $(LDFLAGS) $(LIBS) -o $@

$(BUILD_DIR)/lib-ISO2Processor.so: $(BUILD_DIR) $(ISO2_OBJS)
	$(CXX) $(SHARED_FLAGS) $(ISO2_OBJS) $(LDFLAGS) $(LIBS) -o $@

$(BUILD_DIR)/DINProcessor: $(BUILD_DIR) $(DIN_OBJS)
	$(CXX) $(CXXFLAGS) $(DIN_OBJS) $(LDFLAGS) $(LIBS) -o $@

$(BUILD_DIR)/lib-DINProcessor.so: $(BUILD_DIR) $(DIN_OBJS)
	$(CXX) $(SHARED_FLAGS) $(DIN_OBJS) $(LDFLAGS) $(LIBS) -o $@

$(BUILD_DIR)/SupportedAppProtocolProcessor: $(BUILD_DIR) $(SAP_OBJS)
	$(CXX) $(CXXFLAGS) $(SAP_OBJS) $(LDFLAGS) $(LIBS) -o $@

$(BUILD_DIR)/lib-SupportedAppProtocolProcessor.so: $(BUILD_DIR) $(SAP_OBJS)
	$(CXX) $(SHARED_FLAGS) $(SAP_OBJS) $(LDFLAGS) $(LIBS) -o $@



PYTEST ?= $(shell test -x .venv/bin/pytest && echo .venv/bin/pytest || echo pytest)

test-unit:
	$(PYTEST) tests/codegen

test-integration: all
	$(PYTEST) tests/integration

test: test-unit test-integration

clean:
	rm -rf $(BUILD_DIR)
	rm -rf ./extern/libcbv2g/build

.PHONY: all libcbv2g clean test test-unit test-integration