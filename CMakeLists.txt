cmake_minimum_required(VERSION 3.10)
project(Lab1)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ищем OpenGL и FreeGLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Добавляем исполняемый файл
add_executable(Lab1 
    main.cpp
    render.cpp 
    render.h
    dif.cpp
    dif.h
)

# Подключаем заголовочные файлы (GLM и FreeGLUT находятся в системных путях)
target_include_directories(Lab1 PRIVATE ${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

# Подключаем библиотеки
target_link_libraries(Lab1 PRIVATE ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})