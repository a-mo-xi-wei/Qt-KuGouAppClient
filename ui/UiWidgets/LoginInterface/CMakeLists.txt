# 当前目录下的源码
file(GLOB SRC_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SRC_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# 子目录 smallWidget 下的源码
file(GLOB SRC_SMALL_CPP ${CMAKE_CURRENT_SOURCE_DIR}/smallWidget/*.cpp)
file(GLOB SRC_SMALL_H ${CMAKE_CURRENT_SOURCE_DIR}/smallWidget/*.h)

# 聚合为一个变量，并传回父作用域
set(LOGIN_INTERFACE_SOURCES
        ${SRC_CPP}
        ${SRC_H}
        ${SRC_SMALL_CPP}
        ${SRC_SMALL_H}
        PARENT_SCOPE
)