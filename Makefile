CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
SHARED_FLAGS = -fPIC -shared
INCLUDES = -I./extern/libcbv2g/include -I./extern/json/include -I./extern/cxxopts/include -I./include
LDFLAGS = -L./extern/libcbv2g/build/lib/cbv2g
# Per-Namespace link sets — ADR-0002 requires each Processor .so to link only
# the libcbv2g codec(s) for its Namespace. libcbv2g does not package appHand
# (the SAP codec) as a standalone library; it is duplicated into each protocol
# library. SAP therefore borrows DIN's archive purely as the smallest carrier
# of the appHand symbols.
LIBS_ISO2 = -lcbv2g_iso2 -lcbv2g_exi_codec -lcbv2g_tp
LIBS_DIN = -lcbv2g_din -lcbv2g_exi_codec -lcbv2g_tp
LIBS_SAP = -lcbv2g_din -lcbv2g_exi_codec -lcbv2g_tp
LIBS_ISO20 = -lcbv2g_iso20 -lcbv2g_exi_codec -lcbv2g_tp

BUILD_DIR = build

COMMON_SRCS = src/common.cpp
COMMON_OBJS = $(COMMON_SRCS:src/%.cpp=$(BUILD_DIR)/%.o)

ISO2_SRCS = src/generated/ISO2Processor.generated.cpp
DIN_SRCS = src/generated/DINProcessor.generated.cpp
SAP_SRCS = src/generated/SupportedAppProtocolProcessor.generated.cpp
EXPY_CLI_SRC = tools/expy_cli.cpp
EXPY_CLI_OBJ = $(BUILD_DIR)/expy_cli.o
ISO20_COMMON_SRCS = src/generated/ISO20CommonProcessor.generated.cpp
ISO20_AC_SRCS = src/generated/ISO20ACProcessor.generated.cpp
ISO20_DC_SRCS = src/generated/ISO20DCProcessor.generated.cpp
ISO20_WPT_SRCS = src/generated/ISO20WPTProcessor.generated.cpp
ISO20_ACDP_SRCS = src/generated/ISO20ACDPProcessor.generated.cpp

DIN_GENERATED = src/generated/DIN_marshalers.generated.cpp
DIN_HEADER = extern/libcbv2g/include/cbv2g/din/din_msgDefDatatypes.h
DIN_PROC_CPP = src/generated/DINProcessor.generated.cpp
DIN_PROC_HPP = src/generated/DINProcessor.generated.hpp
SAP_GENERATED = src/generated/SAP_marshalers.generated.cpp
SAP_HEADER = extern/libcbv2g/include/cbv2g/app_handshake/appHand_Datatypes.h
SAP_PROC_CPP = src/generated/SupportedAppProtocolProcessor.generated.cpp
SAP_PROC_HPP = src/generated/SupportedAppProtocolProcessor.generated.hpp
ISO2_GENERATED = src/generated/ISO2_marshalers.generated.cpp
ISO2_HEADER = extern/libcbv2g/include/cbv2g/iso_2/iso2_msgDefDatatypes.h
ISO2_PROC_CPP = src/generated/ISO2Processor.generated.cpp
ISO2_PROC_HPP = src/generated/ISO2Processor.generated.hpp
ISO20_COMMON_GENERATED = src/generated/ISO20Common_marshalers.generated.cpp
ISO20_COMMON_HEADER = extern/libcbv2g/include/cbv2g/iso_20/iso20_CommonMessages_Datatypes.h
ISO20_COMMON_PROC_CPP = src/generated/ISO20CommonProcessor.generated.cpp
ISO20_COMMON_PROC_HPP = src/generated/ISO20CommonProcessor.generated.hpp
ISO20_AC_GENERATED = src/generated/ISO20AC_marshalers.generated.cpp
ISO20_AC_HEADER = extern/libcbv2g/include/cbv2g/iso_20/iso20_AC_Datatypes.h
ISO20_AC_PROC_CPP = src/generated/ISO20ACProcessor.generated.cpp
ISO20_AC_PROC_HPP = src/generated/ISO20ACProcessor.generated.hpp
ISO20_DC_GENERATED = src/generated/ISO20DC_marshalers.generated.cpp
ISO20_DC_HEADER = extern/libcbv2g/include/cbv2g/iso_20/iso20_DC_Datatypes.h
ISO20_DC_PROC_CPP = src/generated/ISO20DCProcessor.generated.cpp
ISO20_DC_PROC_HPP = src/generated/ISO20DCProcessor.generated.hpp
ISO20_WPT_GENERATED = src/generated/ISO20WPT_marshalers.generated.cpp
ISO20_WPT_HEADER = extern/libcbv2g/include/cbv2g/iso_20/iso20_WPT_Datatypes.h
ISO20_WPT_PROC_CPP = src/generated/ISO20WPTProcessor.generated.cpp
ISO20_WPT_PROC_HPP = src/generated/ISO20WPTProcessor.generated.hpp
ISO20_ACDP_GENERATED = src/generated/ISO20ACDP_marshalers.generated.cpp
ISO20_ACDP_HEADER = extern/libcbv2g/include/cbv2g/iso_20/iso20_ACDP_Datatypes.h
ISO20_ACDP_PROC_CPP = src/generated/ISO20ACDPProcessor.generated.cpp
ISO20_ACDP_PROC_HPP = src/generated/ISO20ACDPProcessor.generated.hpp
CODEGEN_PYTHONPATH = $(CURDIR)/tools

ISO2_OBJS = $(BUILD_DIR)/ISO2Processor.generated.o $(COMMON_OBJS)
DIN_OBJS = $(BUILD_DIR)/DINProcessor.generated.o $(COMMON_OBJS)
SAP_OBJS = $(BUILD_DIR)/SupportedAppProtocolProcessor.generated.o $(COMMON_OBJS)
ISO20_COMMON_OBJS = $(BUILD_DIR)/ISO20CommonProcessor.generated.o $(COMMON_OBJS)
ISO20_AC_OBJS = $(BUILD_DIR)/ISO20ACProcessor.generated.o $(COMMON_OBJS)
ISO20_DC_OBJS = $(BUILD_DIR)/ISO20DCProcessor.generated.o $(COMMON_OBJS)
ISO20_WPT_OBJS = $(BUILD_DIR)/ISO20WPTProcessor.generated.o $(COMMON_OBJS)
ISO20_ACDP_OBJS = $(BUILD_DIR)/ISO20ACDPProcessor.generated.o $(COMMON_OBJS)

EXEC = $(BUILD_DIR)/DINProcessor $(BUILD_DIR)/SupportedAppProtocolProcessor $(BUILD_DIR)/ISO2Processor $(BUILD_DIR)/ISO20CommonProcessor $(BUILD_DIR)/ISO20ACProcessor $(BUILD_DIR)/ISO20DCProcessor $(BUILD_DIR)/ISO20WPTProcessor $(BUILD_DIR)/ISO20ACDPProcessor
SHARED = $(BUILD_DIR)/lib-DINProcessor.so $(BUILD_DIR)/lib-SupportedAppProtocolProcessor.so $(BUILD_DIR)/lib-ISO2Processor.so $(BUILD_DIR)/lib-ISO20CommonProcessor.so $(BUILD_DIR)/lib-ISO20ACProcessor.so $(BUILD_DIR)/lib-ISO20DCProcessor.so $(BUILD_DIR)/lib-ISO20WPTProcessor.so $(BUILD_DIR)/lib-ISO20ACDPProcessor.so

all: libcbv2g $(EXEC) $(SHARED) $(GENERATED_V2GJSON)

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

COMMON_HDRS = $(wildcard include/*.hpp)

$(BUILD_DIR)/%.o: src/%.cpp $(COMMON_HDRS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SHARED_FLAGS) $(INCLUDES) -c $< -o $@

# Generated Processor sources live under src/generated/. Compile them with the
# same flags but a separate pattern rule so % only matches the basename.
$(BUILD_DIR)/%.o: src/generated/%.cpp $(COMMON_HDRS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SHARED_FLAGS) $(INCLUDES) -c $< -o $@

$(DIN_GENERATED): $(DIN_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $@)
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen --header $(DIN_HEADER) --out $@

$(DIN_PROC_CPP) $(DIN_PROC_HPP) &: $(DIN_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $(DIN_PROC_CPP))
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.processor_emitter \
	  --namespace DIN --header $(DIN_HEADER) \
	  --out-cpp $(DIN_PROC_CPP) --out-hpp $(DIN_PROC_HPP)

$(SAP_GENERATED): $(SAP_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $@)
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen --header $(SAP_HEADER) --out $@

$(SAP_PROC_CPP) $(SAP_PROC_HPP) &: $(SAP_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $(SAP_PROC_CPP))
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.processor_emitter \
	  --namespace SAP --header $(SAP_HEADER) \
	  --out-cpp $(SAP_PROC_CPP) --out-hpp $(SAP_PROC_HPP)

# Shared, Namespace-agnostic CLI. Linked into each exec target alongside the
# Namespace's Processor object — see ADR-0002's split between .so (shared lib,
# Python consumers) and exec (dev convenience for hex-stream encode/decode).
$(EXPY_CLI_OBJ): $(EXPY_CLI_SRC) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(ISO2_GENERATED): $(ISO2_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $@)
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen --header $(ISO2_HEADER) --out $@

$(ISO2_PROC_CPP) $(ISO2_PROC_HPP) &: $(ISO2_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $(ISO2_PROC_CPP))
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.processor_emitter \
	  --namespace ISO2 --header $(ISO2_HEADER) \
	  --out-cpp $(ISO2_PROC_CPP) --out-hpp $(ISO2_PROC_HPP)

$(ISO20_COMMON_GENERATED): $(ISO20_COMMON_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $@)
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen --header $(ISO20_COMMON_HEADER) --out $@

$(ISO20_COMMON_PROC_CPP) $(ISO20_COMMON_PROC_HPP) &: $(ISO20_COMMON_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $(ISO20_COMMON_PROC_CPP))
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.processor_emitter \
	  --namespace ISO20_COMMON --header $(ISO20_COMMON_HEADER) \
	  --out-cpp $(ISO20_COMMON_PROC_CPP) --out-hpp $(ISO20_COMMON_PROC_HPP)

$(ISO20_AC_GENERATED): $(ISO20_AC_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $@)
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen --header $(ISO20_AC_HEADER) --out $@

$(ISO20_AC_PROC_CPP) $(ISO20_AC_PROC_HPP) &: $(ISO20_AC_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $(ISO20_AC_PROC_CPP))
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.processor_emitter \
	  --namespace ISO20_AC --header $(ISO20_AC_HEADER) \
	  --out-cpp $(ISO20_AC_PROC_CPP) --out-hpp $(ISO20_AC_PROC_HPP)

$(ISO20_DC_GENERATED): $(ISO20_DC_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $@)
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen --header $(ISO20_DC_HEADER) --out $@

$(ISO20_DC_PROC_CPP) $(ISO20_DC_PROC_HPP) &: $(ISO20_DC_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $(ISO20_DC_PROC_CPP))
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.processor_emitter \
	  --namespace ISO20_DC --header $(ISO20_DC_HEADER) \
	  --out-cpp $(ISO20_DC_PROC_CPP) --out-hpp $(ISO20_DC_PROC_HPP)

$(ISO20_WPT_GENERATED): $(ISO20_WPT_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $@)
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen --header $(ISO20_WPT_HEADER) --out $@

$(ISO20_WPT_PROC_CPP) $(ISO20_WPT_PROC_HPP) &: $(ISO20_WPT_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $(ISO20_WPT_PROC_CPP))
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.processor_emitter \
	  --namespace ISO20_WPT --header $(ISO20_WPT_HEADER) \
	  --out-cpp $(ISO20_WPT_PROC_CPP) --out-hpp $(ISO20_WPT_PROC_HPP)

$(ISO20_ACDP_GENERATED): $(ISO20_ACDP_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $@)
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen --header $(ISO20_ACDP_HEADER) --out $@

# Generate Processor.cpp + Processor.hpp from the libcbv2g header. Single
# emitter invocation writes both outputs; declaring both as targets keeps make
# happy when only one is consumed by a downstream rule.
$(ISO20_ACDP_PROC_CPP) $(ISO20_ACDP_PROC_HPP) &: $(ISO20_ACDP_HEADER) $(wildcard tools/codegen/*.py)
	mkdir -p $(dir $(ISO20_ACDP_PROC_CPP))
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.processor_emitter \
	  --namespace ISO20_ACDP --header $(ISO20_ACDP_HEADER) \
	  --out-cpp $(ISO20_ACDP_PROC_CPP) --out-hpp $(ISO20_ACDP_PROC_HPP)

V2Gjson/sap.py: $(SAP_HEADER) tools/codegen/v2gjson_emitter.py
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.v2gjson_emitter \
	  --header $(SAP_HEADER) --out $@ --namespace-prefix appHand_ \
	  --module-doc "Constructors and enum tables for the SAP (Supported App Protocol) Namespace."

V2Gjson/din.py: $(DIN_HEADER) tools/codegen/v2gjson_emitter.py
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.v2gjson_emitter \
	  --header $(DIN_HEADER) --out $@ --namespace-prefix din_ \
	  --module-doc "Constructors and enum tables for the DIN 70121 Namespace."

V2Gjson/iso2.py: $(ISO2_HEADER) tools/codegen/v2gjson_emitter.py
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.v2gjson_emitter \
	  --header $(ISO2_HEADER) --out $@ --namespace-prefix iso2_ \
	  --module-doc "Constructors and enum tables for the ISO 15118-2 Namespace."

V2Gjson/iso20_common.py: $(ISO20_COMMON_HEADER) tools/codegen/v2gjson_emitter.py
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.v2gjson_emitter \
	  --header $(ISO20_COMMON_HEADER) --out $@ --namespace-prefix iso20_ \
	  --module-doc "Constructors and enum tables for the ISO 15118-20 Common Messages Namespace."

V2Gjson/iso20_ac.py: $(ISO20_AC_HEADER) tools/codegen/v2gjson_emitter.py
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.v2gjson_emitter \
	  --header $(ISO20_AC_HEADER) --out $@ --namespace-prefix iso20_ac_ \
	  --module-doc "Constructors and enum tables for the ISO 15118-20 AC Namespace."

V2Gjson/iso20_dc.py: $(ISO20_DC_HEADER) tools/codegen/v2gjson_emitter.py
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.v2gjson_emitter \
	  --header $(ISO20_DC_HEADER) --out $@ --namespace-prefix iso20_dc_ \
	  --module-doc "Constructors and enum tables for the ISO 15118-20 DC Namespace."

V2Gjson/iso20_wpt.py: $(ISO20_WPT_HEADER) tools/codegen/v2gjson_emitter.py
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.v2gjson_emitter \
	  --header $(ISO20_WPT_HEADER) --out $@ --namespace-prefix iso20_wpt_ \
	  --module-doc "Constructors and enum tables for the ISO 15118-20 WPT Namespace."

V2Gjson/iso20_acdp.py: $(ISO20_ACDP_HEADER) tools/codegen/v2gjson_emitter.py
	PYTHONPATH=$(CODEGEN_PYTHONPATH) python3 -m codegen.v2gjson_emitter \
	  --header $(ISO20_ACDP_HEADER) --out $@ --namespace-prefix iso20_acdp_ \
	  --module-doc "Constructors and enum tables for the ISO 15118-20 ACDP Namespace."

GENERATED_V2GJSON = V2Gjson/sap.py V2Gjson/din.py V2Gjson/iso2.py V2Gjson/iso20_common.py V2Gjson/iso20_ac.py V2Gjson/iso20_dc.py V2Gjson/iso20_wpt.py V2Gjson/iso20_acdp.py

$(BUILD_DIR)/DINProcessor.generated.o: $(DIN_GENERATED) $(DIN_PROC_CPP) $(DIN_PROC_HPP)
$(BUILD_DIR)/SupportedAppProtocolProcessor.generated.o: $(SAP_GENERATED) $(SAP_PROC_CPP) $(SAP_PROC_HPP)
$(BUILD_DIR)/ISO2Processor.generated.o: $(ISO2_GENERATED) $(ISO2_PROC_CPP) $(ISO2_PROC_HPP)
$(BUILD_DIR)/ISO20CommonProcessor.generated.o: $(ISO20_COMMON_GENERATED) $(ISO20_COMMON_PROC_CPP) $(ISO20_COMMON_PROC_HPP)
$(BUILD_DIR)/ISO20ACProcessor.generated.o: $(ISO20_AC_GENERATED) $(ISO20_AC_PROC_CPP) $(ISO20_AC_PROC_HPP)
$(BUILD_DIR)/ISO20DCProcessor.generated.o: $(ISO20_DC_GENERATED) $(ISO20_DC_PROC_CPP) $(ISO20_DC_PROC_HPP)
$(BUILD_DIR)/ISO20WPTProcessor.generated.o: $(ISO20_WPT_GENERATED) $(ISO20_WPT_PROC_CPP) $(ISO20_WPT_PROC_HPP)
$(BUILD_DIR)/ISO20ACDPProcessor.generated.o: $(ISO20_ACDP_GENERATED) $(ISO20_ACDP_PROC_CPP) $(ISO20_ACDP_PROC_HPP)

$(BUILD_DIR)/ISO2Processor: $(BUILD_DIR) $(ISO2_OBJS) $(EXPY_CLI_OBJ)
	$(CXX) $(CXXFLAGS) $(ISO2_OBJS) $(EXPY_CLI_OBJ) $(LDFLAGS) $(LIBS_ISO2) -o $@

$(BUILD_DIR)/lib-ISO2Processor.so: $(BUILD_DIR) $(ISO2_OBJS)
	$(CXX) $(SHARED_FLAGS) $(ISO2_OBJS) $(LDFLAGS) $(LIBS_ISO2) -o $@

$(BUILD_DIR)/DINProcessor: $(BUILD_DIR) $(DIN_OBJS) $(EXPY_CLI_OBJ)
	$(CXX) $(CXXFLAGS) $(DIN_OBJS) $(EXPY_CLI_OBJ) $(LDFLAGS) $(LIBS_DIN) -o $@

$(BUILD_DIR)/lib-DINProcessor.so: $(BUILD_DIR) $(DIN_OBJS)
	$(CXX) $(SHARED_FLAGS) $(DIN_OBJS) $(LDFLAGS) $(LIBS_DIN) -o $@

$(BUILD_DIR)/SupportedAppProtocolProcessor: $(BUILD_DIR) $(SAP_OBJS) $(EXPY_CLI_OBJ)
	$(CXX) $(CXXFLAGS) $(SAP_OBJS) $(EXPY_CLI_OBJ) $(LDFLAGS) $(LIBS_SAP) -o $@

$(BUILD_DIR)/lib-SupportedAppProtocolProcessor.so: $(BUILD_DIR) $(SAP_OBJS)
	$(CXX) $(SHARED_FLAGS) $(SAP_OBJS) $(LDFLAGS) $(LIBS_SAP) -o $@

$(BUILD_DIR)/ISO20CommonProcessor: $(BUILD_DIR) $(ISO20_COMMON_OBJS) $(EXPY_CLI_OBJ)
	$(CXX) $(CXXFLAGS) $(ISO20_COMMON_OBJS) $(EXPY_CLI_OBJ) $(LDFLAGS) $(LIBS_ISO20) -o $@

$(BUILD_DIR)/lib-ISO20CommonProcessor.so: $(BUILD_DIR) $(ISO20_COMMON_OBJS)
	$(CXX) $(SHARED_FLAGS) $(ISO20_COMMON_OBJS) $(LDFLAGS) $(LIBS_ISO20) -o $@

$(BUILD_DIR)/ISO20ACProcessor: $(BUILD_DIR) $(ISO20_AC_OBJS) $(EXPY_CLI_OBJ)
	$(CXX) $(CXXFLAGS) $(ISO20_AC_OBJS) $(EXPY_CLI_OBJ) $(LDFLAGS) $(LIBS_ISO20) -o $@

$(BUILD_DIR)/lib-ISO20ACProcessor.so: $(BUILD_DIR) $(ISO20_AC_OBJS)
	$(CXX) $(SHARED_FLAGS) $(ISO20_AC_OBJS) $(LDFLAGS) $(LIBS_ISO20) -o $@

$(BUILD_DIR)/ISO20DCProcessor: $(BUILD_DIR) $(ISO20_DC_OBJS) $(EXPY_CLI_OBJ)
	$(CXX) $(CXXFLAGS) $(ISO20_DC_OBJS) $(EXPY_CLI_OBJ) $(LDFLAGS) $(LIBS_ISO20) -o $@

$(BUILD_DIR)/lib-ISO20DCProcessor.so: $(BUILD_DIR) $(ISO20_DC_OBJS)
	$(CXX) $(SHARED_FLAGS) $(ISO20_DC_OBJS) $(LDFLAGS) $(LIBS_ISO20) -o $@

$(BUILD_DIR)/ISO20WPTProcessor: $(BUILD_DIR) $(ISO20_WPT_OBJS) $(EXPY_CLI_OBJ)
	$(CXX) $(CXXFLAGS) $(ISO20_WPT_OBJS) $(EXPY_CLI_OBJ) $(LDFLAGS) $(LIBS_ISO20) -o $@

$(BUILD_DIR)/lib-ISO20WPTProcessor.so: $(BUILD_DIR) $(ISO20_WPT_OBJS)
	$(CXX) $(SHARED_FLAGS) $(ISO20_WPT_OBJS) $(LDFLAGS) $(LIBS_ISO20) -o $@

$(BUILD_DIR)/ISO20ACDPProcessor: $(BUILD_DIR) $(ISO20_ACDP_OBJS) $(EXPY_CLI_OBJ)
	$(CXX) $(CXXFLAGS) $(ISO20_ACDP_OBJS) $(EXPY_CLI_OBJ) $(LDFLAGS) $(LIBS_ISO20) -o $@

$(BUILD_DIR)/lib-ISO20ACDPProcessor.so: $(BUILD_DIR) $(ISO20_ACDP_OBJS)
	$(CXX) $(SHARED_FLAGS) $(ISO20_ACDP_OBJS) $(LDFLAGS) $(LIBS_ISO20) -o $@


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