cmake_minimum_required(VERSION 3.10)
project(MeuProjeto)

# Definindo variáveis com os diretórios principais
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(LIBS_DIR ${CMAKE_SOURCE_DIR}/libs)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

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
# add_executable(main ${SRC_DIR}/main.c ${SRC_DIR}/test.c)
add_executable(main)
add_sources_to_target(main ${SRC_DIR})
target_include_directories(main PUBLIC ${INCLUDE_DIR})
# target_link_libraries(main PRIVATE mylib mylib2 legal)

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

# Configurando a build para colocar o executável final em bin/
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)
