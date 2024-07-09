cmake_minimum_required(VERSION 3.10)
project(MeuProjeto)

# Definindo variáveis com os diretórios principais
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/3rdparty)

# Função para copiar headers das libs para include/lib
function(copy_lib_headers LIB_DIR)
    file(GLOB LIB_DIRS RELATIVE ${LIBS_DIR} ${LIBS_DIR}/*)
    foreach(LIB_NAME ${LIB_DIRS})
        file(GLOB LIB_HEADERS ${LIBS_DIR}/${LIB_NAME}/*.h)
        file(COPY ${LIB_HEADERS} DESTINATION ${INCLUDE_DIR}/lib/${LIB_NAME})
    endforeach()
endfunction()

# Configurando a cópia dos headers das libs para include/lib
file(MAKE_DIRECTORY ${INCLUDE_DIR}/lib)  # Criando o diretório include/lib se não existir
copy_lib_headers(${LIBS_DIR})

# Adicionando comando para buildar somente as bibliotecas
add_custom_target(build_libs
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Copiando headers das libs para include/lib"
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${INCLUDE_DIR}/lib
    COMMAND ${CMAKE_COMMAND} -E make_directory ${INCLUDE_DIR}/lib
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${LIBS_DIR} ${INCLUDE_DIR}/lib
    COMMENT "Buildando somente as bibliotecas"
)

# Função para buscar todos os arquivos .c em um diretório (recursivamente)
function(add_sources_to_target TARGET DIRECTORY)
    file(GLOB_RECURSE SRC_FILES ${DIRECTORY}/*.c)
    target_sources(${TARGET} PRIVATE ${SRC_FILES})
endfunction()

# Configurando a build do executável principal
add_executable(main)
add_sources_to_target(main ${SRC_DIR})
target_include_directories(main PUBLIC ${INCLUDE_DIR})

# Função para adicionar libs dinâmicas de um diretório e configurar dependências
function(add_dynamic_libs_with_dependencies LIB_DIR)
    file(GLOB LIB_DIRS RELATIVE ${LIBS_DIR} ${LIBS_DIR}/*)
    foreach(LIB_NAME ${LIB_DIRS})
        file(GLOB LIB_SOURCES ${LIBS_DIR}/${LIB_NAME}/*.c)
        add_library(${LIB_NAME} SHARED ${LIB_SOURCES})
        set_target_properties(${LIB_NAME} PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${BIN_DIR}/lib
        )
        # Adiciona as libs como dependências do build_libs
        add_dependencies(build_libs ${LIB_NAME})

        # Adiciona as bibliotecas dinâmicas ao executável principal
        target_link_libraries(main PRIVATE ${LIB_NAME})
    endforeach()
endfunction()

# Configurar a construção das libs dinâmicas
add_dynamic_libs_with_dependencies(${LIBS_DIR})

# Adicionando mbedTLS (apenas SHA-256, MD5 e AES)
add_library(mbedtls_sha256 STATIC ${THIRD_PARTY_DIR}/mbedtls/library/sha256.c)
add_library(mbedtls_md5 STATIC ${THIRD_PARTY_DIR}/mbedtls/library/md5.c)
add_library(mbedtls_aes STATIC ${THIRD_PARTY_DIR}/mbedtls/library/aes.c)
add_library(mbedtls_platform_util STATIC ${THIRD_PARTY_DIR}/mbedtls/library/platform_util.c)

target_include_directories(mbedtls_sha256 PUBLIC ${THIRD_PARTY_DIR}/mbedtls/include)
target_include_directories(mbedtls_md5 PUBLIC ${THIRD_PARTY_DIR}/mbedtls/include)
target_include_directories(mbedtls_aes PUBLIC ${THIRD_PARTY_DIR}/mbedtls/include)
target_include_directories(mbedtls_platform_util PUBLIC ${THIRD_PARTY_DIR}/mbedtls/include)

# Linkar as bibliotecas ao executável principal
target_link_libraries(main PRIVATE mbedtls_sha256 mbedtls_md5 mbedtls_aes mbedtls_platform_util)

# Configurando a build para colocar o executável final em bin/
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

# Desativando testes e programas exemplo da mbedTLS
set(ENABLE_TESTING OFF CACHE BOOL "Disable testing" FORCE)
set(ENABLE_PROGRAMS OFF CACHE BOOL "Disable example programs" FORCE)

# Adicionando o subdiretório do mbedTLS
add_subdirectory(${THIRD_PARTY_DIR}/mbedtls)
