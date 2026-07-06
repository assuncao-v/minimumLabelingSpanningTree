cmake_minimum_required(VERSION 3.16)

project(GrafoMVCA
    VERSION 1.0.0
    DESCRIPTION "Estrutura de grafo em lista de adjacencia com algoritmo MVCA"
    LANGUAGES CXX
)

# ---------------------------------------------------------------------------
# Configurações gerais
# ---------------------------------------------------------------------------

# Padrão C++17, obrigatório e sem extensões específicas de compilador
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Se nenhum tipo de build foi especificado, usa Release por padrão
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Tipo de build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

# Coloca todos os binários gerados em bin/ e libs em lib/, independente da config
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# compile_commands.json para uso com clangd, ccls, etc.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Flags de aviso / compilação
# ---------------------------------------------------------------------------

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Flags específicas por tipo de build
set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

# Opção para habilitar sanitizers em builds de depuração (uso: -DENABLE_SANITIZERS=ON)
option(ENABLE_SANITIZERS "Habilita AddressSanitizer/UBSan em builds Debug" OFF)
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

# ---------------------------------------------------------------------------
# Localização dos fontes
# ---------------------------------------------------------------------------
# Estrutura do projeto:
#   include/Graph.hpp   (header)
#   src/graph/graph.cpp (implementação)
#   src/graph/main.cpp  (executável demo)
#
# A busca abaixo tenta várias pastas comuns e também variações de
# maiúscula/minúscula do header, para não quebrar se ele for renomeado.

set(_PASTAS_CANDIDATAS
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graph
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

function(localizar_fonte NOMES_POSSIVEIS VARIAVEL_SAIDA)
    foreach(_pasta ${_PASTAS_CANDIDATAS})
        foreach(_nome ${NOMES_POSSIVEIS})
            if(EXISTS "${_pasta}/${_nome}")
                set(${VARIAVEL_SAIDA} "${_pasta}/${_nome}" PARENT_SCOPE)
                return()
            endif()
        endforeach()
    endforeach()
    message(FATAL_ERROR
        "Nao encontrei nenhum de: ${NOMES_POSSIVEIS}\n"
        "Procurei em: ${_PASTAS_CANDIDATAS}\n"
        "Ajuste _PASTAS_CANDIDATAS no CMakeLists.txt se necessario."
    )
endfunction()

localizar_fonte("Graph.hpp;graph.hpp" ARQ_GRAPH_HPP)
localizar_fonte("graph.cpp"           ARQ_GRAPH_CPP)
localizar_fonte("main.cpp"            ARQ_MAIN_CPP)

get_filename_component(DIR_INCLUDE ${ARQ_GRAPH_HPP} DIRECTORY)

# ---------------------------------------------------------------------------
# Biblioteca: grafo (No, Graph, MVCA, Kruskal, etc.)
# ---------------------------------------------------------------------------

add_library(grafo
    ${ARQ_GRAPH_CPP}
    ${ARQ_GRAPH_HPP}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/disjoint-set-union/UnionFind.cpp
)

target_include_directories(grafo PUBLIC
    $<BUILD_INTERFACE:${DIR_INCLUDE}>
    $<INSTALL_INTERFACE:include>
)

# ---------------------------------------------------------------------------
# Executável principal (demo do MVCA)
# ---------------------------------------------------------------------------

add_executable(grafo_mvca ${ARQ_MAIN_CPP})
target_link_libraries(grafo_mvca PRIVATE grafo)

# ---------------------------------------------------------------------------
# Gerador de instâncias MLSTP (standalone, sem dependência da lib grafo)
# ---------------------------------------------------------------------------

add_executable(gerador_mlstp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/generator/generator.cpp
)

# ---------------------------------------------------------------------------
# Testes (opcional, habilitado por padrão; requer CTest)
# ---------------------------------------------------------------------------

option(BUILD_TESTS "Compila os testes do projeto" ON)

if(BUILD_TESTS)
    enable_testing()

    # Testa se o binário principal executa e retorna código de saída 0
    add_test(
        NAME grafo_mvca_executa
        COMMAND grafo_mvca
    )

    # Se existir uma pasta tests/ com seu próprio CMakeLists.txt, inclui-a
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
        add_subdirectory(tests)
    endif()
endif()

# ---------------------------------------------------------------------------
# Instalação (opcional: make install)
# ---------------------------------------------------------------------------

include(GNUInstallDirs)

install(TARGETS grafo grafo_mvca
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES graph.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)