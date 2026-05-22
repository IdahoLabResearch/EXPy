cmake_minimum_required(VERSION 3.20)
project(expy LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

# ----------------------------------------------------------------------------
# libcbv2g patches (ADR-0007, 0008, 0009, 0010).
#
# Applied to the vendored submodule's working tree before configuring it.
# Idempotent: `git apply --check --reverse` succeeds when the patch is
# already applied, in which case the apply step is skipped. Mirrors the
# Makefile's `libcbv2g` target.
# ----------------------------------------------------------------------------
file(GLOB LIBCBV2G_PATCHES "${CMAKE_SOURCE_DIR}/patches/libcbv2g/*.patch")
list(SORT LIBCBV2G_PATCHES)
find_program(GIT_EXECUTABLE git REQUIRED)
foreach(patch IN LISTS LIBCBV2G_PATCHES)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} -C "${CMAKE_SOURCE_DIR}/extern/libcbv2g"
                apply --check --reverse "${patch}"
        RESULT_VARIABLE _already_applied
        OUTPUT_QUIET ERROR_QUIET
    )
    if(NOT _already_applied EQUAL 0)
        execute_process(
            COMMAND ${GIT_EXECUTABLE} -C "${CMAKE_SOURCE_DIR}/extern/libcbv2g"
                    apply "${patch}"
            RESULT_VARIABLE _apply_rc
        )
        if(NOT _apply_rc EQUAL 0)
            message(FATAL_ERROR "Failed to apply ${patch}")
        endif()
    endif()
endforeach()

# libcbv2g's CMakeLists.txt requires everest-cmake at ../everest-cmake
# relative to its own source dir — satisfied by extern/everest-cmake.
add_subdirectory(extern/libcbv2g)

# ----------------------------------------------------------------------------
# Per-Namespace codegen + .so build + CLI exe build.
#
# Each entry: NS | header | base | marshaler-stem | cbv2g-archive
#   NS              — Namespace.value (passed to processor_emitter / cli_emitter)
#   header          — libcbv2g _Datatypes.h driving codegen and #includes
#   base            — base filename used by emitters. The .so ships as
#                     ``lib-<base>.so`` and the CLI exe as ``<base>``.
#   marshaler-stem  — leading token of <stem>_marshalers.generated.cpp
#   cbv2g-archive   — per-Namespace static library to link (ADR-0002).
#                     SAP borrows DIN's archive because libcbv2g doesn't
#                     package appHand separately.
# ----------------------------------------------------------------------------
set(_NAMESPACE_SPECS
    "SAP|extern/libcbv2g/include/cbv2g/app_handshake/appHand_Datatypes.h|SupportedAppProtocolProcessor|SAP|cbv2g_din"
    "DIN|extern/libcbv2g/include/cbv2g/din/din_msgDefDatatypes.h|DINProcessor|DIN|cbv2g_din"
    "ISO2|extern/libcbv2g/include/cbv2g/iso_2/iso2_msgDefDatatypes.h|ISO2Processor|ISO2|cbv2g_iso2"
    "ISO20_COMMON|extern/libcbv2g/include/cbv2g/iso_20/iso20_CommonMessages_Datatypes.h|ISO20CommonProcessor|ISO20Common|cbv2g_iso20"
    "ISO20_AC|extern/libcbv2g/include/cbv2g/iso_20/iso20_AC_Datatypes.h|ISO20ACProcessor|ISO20AC|cbv2g_iso20"
    "ISO20_DC|extern/libcbv2g/include/cbv2g/iso_20/iso20_DC_Datatypes.h|ISO20DCProcessor|ISO20DC|cbv2g_iso20"
    "ISO20_WPT|extern/libcbv2g/include/cbv2g/iso_20/iso20_WPT_Datatypes.h|ISO20WPTProcessor|ISO20WPT|cbv2g_iso20"
    "ISO20_ACDP|extern/libcbv2g/include/cbv2g/iso_20/iso20_ACDP_Datatypes.h|ISO20ACDPProcessor|ISO20ACDP|cbv2g_iso20"
)

set(_CODEGEN_PYTHONPATH "${CMAKE_SOURCE_DIR}/tools")
set(_GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${_GENERATED_DIR}")
file(GLOB _CODEGEN_PY "${CMAKE_SOURCE_DIR}/tools/codegen/*.py")

# common.cpp is shared by every Processor's .so and CLI exe — build it once
# as an OBJECT library to keep link sets honest.
add_library(expy_common OBJECT ${CMAKE_SOURCE_DIR}/src/common.cpp)
target_include_directories(expy_common PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/extern/libcbv2g/include
    ${CMAKE_SOURCE_DIR}/extern/json/include
)

foreach(_spec IN LISTS _NAMESPACE_SPECS)
    string(REPLACE "|" ";" _parts "${_spec}")
    list(GET _parts 0 _ns)
    list(GET _parts 1 _hdr_rel)
    list(GET _parts 2 _base)
    list(GET _parts 3 _marsh_stem)
    list(GET _parts 4 _cbv2g_lib)

    set(_header "${CMAKE_SOURCE_DIR}/${_hdr_rel}")
    set(_proc_cpp "${_GENERATED_DIR}/${_base}.generated.cpp")
    set(_proc_hpp "${_GENERATED_DIR}/${_base}.generated.hpp")
    set(_marsh_cpp "${_GENERATED_DIR}/${_marsh_stem}_marshalers.generated.cpp")
    set(_cli_cpp "${_GENERATED_DIR}/${_base}_cli.generated.cpp")

    add_custom_command(
        OUTPUT "${_marsh_cpp}"
        COMMAND ${CMAKE_COMMAND} -E env
                "PYTHONPATH=${_CODEGEN_PYTHONPATH}"
                ${Python3_EXECUTABLE} -m codegen
                --header "${_header}" --out "${_marsh_cpp}"
        DEPENDS "${_header}" ${_CODEGEN_PY}
        VERBATIM
    )
    add_custom_command(
        OUTPUT "${_proc_cpp}" "${_proc_hpp}"
        COMMAND ${CMAKE_COMMAND} -E env
                "PYTHONPATH=${_CODEGEN_PYTHONPATH}"
                ${Python3_EXECUTABLE} -m codegen.processor_emitter
                --namespace "${_ns}" --header "${_header}"
                --out-cpp "${_proc_cpp}" --out-hpp "${_proc_hpp}"
        DEPENDS "${_header}" ${_CODEGEN_PY}
        VERBATIM
    )
    add_custom_command(
        OUTPUT "${_cli_cpp}"
        COMMAND ${CMAKE_COMMAND} -E env
                "PYTHONPATH=${_CODEGEN_PYTHONPATH}"
                ${Python3_EXECUTABLE} -m codegen.processor_cli_emitter
                --namespace "${_ns}" --header "${_header}" --out-cpp "${_cli_cpp}"
        DEPENDS "${_header}" ${_CODEGEN_PY}
        VERBATIM
    )

    # OBJECT lib for the Processor.cpp TU; reused by both the .so and the CLI
    # exe so we only compile and codegen once per Namespace.
    add_library(_${_base}_obj OBJECT "${_proc_cpp}")
    target_sources(_${_base}_obj PRIVATE "${_marsh_cpp}")
    # Marshaler .cpp is #included by Processor.cpp, not compiled separately.
    # HEADER_FILE_ONLY keeps it out of the TU list while still wiring the
    # custom-command dependency so codegen runs first.
    set_source_files_properties("${_marsh_cpp}" PROPERTIES HEADER_FILE_ONLY TRUE)
    target_include_directories(_${_base}_obj PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/extern/libcbv2g/include
        ${CMAKE_SOURCE_DIR}/extern/json/include
        "${_GENERATED_DIR}"
    )

    # Python-loadable .so — ctypes opens this from expy/v2gjson/.
    add_library(${_base} MODULE)
    target_link_libraries(${_base} PRIVATE
        _${_base}_obj
        expy_common
        ${_cbv2g_lib}
        cbv2g_exi_codec
        cbv2g_tp
    )
    set_target_properties(${_base} PROPERTIES
        PREFIX ""
        OUTPUT_NAME "lib-${_base}"
        SUFFIX ".so"
    )
    install(TARGETS ${_base} LIBRARY DESTINATION expy/v2gjson)

    # CLI exe — bundled under expy/_bin/. expy/_cli.py launchers (mapped via
    # [project.scripts]) os.execv() into these.
    add_executable(${_base}_cli "${_cli_cpp}")
    target_link_libraries(${_base}_cli PRIVATE
        _${_base}_obj
        expy_common
        ${_cbv2g_lib}
        cbv2g_exi_codec
        cbv2g_tp
    )
    target_include_directories(${_base}_cli PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/extern/libcbv2g/include
        ${CMAKE_SOURCE_DIR}/extern/json/include
        ${CMAKE_SOURCE_DIR}/extern/cxxopts/include
        "${_GENERATED_DIR}"
    )
    set_target_properties(${_base}_cli PROPERTIES OUTPUT_NAME "${_base}")
    install(TARGETS ${_base}_cli RUNTIME DESTINATION expy/_bin)
endforeach()
