cmake_minimum_required(VERSION 3.16)
project(forest-test VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE lib/glm)

add_library(tinygltf INTERFACE)
target_include_directories(tinygltf INTERFACE lib/tinygltf)

add_library(vma INTERFACE)
target_include_directories(vma INTERFACE lib/vma/include)

find_package(Vulkan REQUIRED)

file(GLOB_RECURSE SOURCES 
    src/*.cpp
    src/*.c
)

file(GLOB_RECURSE HEADERS
    src/*.h
    src/*.hpp
)

set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE 
    src
    ${Vulkan_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glm
    tinygltf
    vma
    Vulkan::Vulkan
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        ${COCOA_LIBRARY} 
        ${IOKIT_LIBRARY} 
        ${COREVIDEO_LIBRARY}
    )
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE 
        ${CMAKE_DL_LIBS}
        pthread
        X11
    )
endif()

find_program(GLSLC_EXECUTABLE glslc)

function(compile_shader SHADER_SOURCE)
    get_filename_component(SHADER_FILENAME ${SHADER_SOURCE} NAME)
    set(SPIRV_OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/${SHADER_FILENAME}.spv")

    add_custom_command(
        OUTPUT ${SPIRV_OUTPUT}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders"
        COMMAND ${GLSLC_EXECUTABLE} 
                -I${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/include
                -o ${SPIRV_OUTPUT} 
                ${SHADER_SOURCE}
        DEPENDS ${SHADER_SOURCE}
        COMMENT "Compiling shader ${SHADER_FILENAME} -> assets/shaders"
        VERBATIM
    )

    set(SPIRV_BINARY_FILES ${SPIRV_BINARY_FILES} ${SPIRV_OUTPUT} PARENT_SCOPE)
endfunction()

if(GLSLC_EXECUTABLE)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders)
        file(GLOB_RECURSE SHADER_SOURCES 
            ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*.vert
            ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*.frag
            ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*.comp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*.geom
            ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*.tesc
            ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/*.tese
        )

        set(SPIRV_BINARY_FILES "")

        foreach(SHADER ${SHADER_SOURCES})
            compile_shader(${SHADER})
        endforeach()

        add_custom_target(shaders ALL DEPENDS ${SPIRV_BINARY_FILES})
        add_dependencies(${PROJECT_NAME} shaders)
    else()
        message(WARNING "The folder src/shaders does not exist")
    endif()
else()
    message(WARNING "glslc not found. Shaders will not be compiled.")
endif()

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
endif()