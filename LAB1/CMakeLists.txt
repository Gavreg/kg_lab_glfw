cmake_minimum_required(VERSION 3.11)

project(kg_glwf)

set(CMAKE_CXX_STANDARD 17)


# < Зависимости >

# Windows
if(WIN32) 

  # Легче скачать glfw автоматически, чем устанавливать вручную
  include(FetchContent)
  FetchContent_Declare(
    glfw
    URL https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE 
  )
  FetchContent_MakeAvailable(glfw)

# Всё остальное
else()

  find_package(glfw3 REQUIRED)

endif()

# Поиск OpenGL
find_package(OpenGL REQUIRED)


# < Исходники >

FILE(GLOB SOURCES "*.cpp")
FILE(GLOB HEADERS "*.h")

add_executable(${PROJECT_NAME} 
	${SOURCES} 
  ${HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
  glfw OpenGL::GL 
  ${OPENGL_glu_LIBRARY}
)


# < Параметры компиляции >

if(NOT WIN32) 

	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

	set(CMAKE_CXX_FLAGS_RELEASE_INIT "-O2")
	set(CMAKE_CXX_FLAGS_DEBUG_INIT "-O0 -pipe") # Максимальная скорость компиляции
	
endif()




