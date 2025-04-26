cmake_minimum_required(VERSION 3.10)

project(MyDBPool VERSION 1.0)

file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

file(GLOB SOURCE_COMMON_CPP ${CMAKE_CURRENT_SOURCE_DIR}/../MyCommon/*.cpp)
file(GLOB SOURCE_COMMON_C ${CMAKE_CURRENT_SOURCE_DIR}/../MyCommon/*.c)
file(GLOB SOURCE_COMMON_H ${CMAKE_CURRENT_SOURCE_DIR}/../MyCommon/*.h)

set(SOURCE_FILE
    ${SOURCE_CPP}
    ${SOURCE_H}
    ${SOURCE_COMMON_CPP}
    ${SOURCE_COMMON_C}
    ${SOURCE_COMMON_H}
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILE})

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../MyCommon
        ${CMAKE_CURRENT_SOURCE_DIR}/../../tools/zlib/include
)

#message(STATUS "+++++++++++++++++catch libzlib.dll.a at ${ZLIB_IMPORT_LIB}")
#message(STATUS "+++++++++++++++++catch zlib.dll at ${ZLIB_DLL}")

target_link_libraries(${PROJECT_NAME} Qt6::Sql Qt6::Network Qt6::Widgets MyQsLog ${ZLIB_IMPORT_LIB})

#[[
        静态库本身不依赖DLL，但如果有其他可执行文件（如KuGouServer）依赖这个静态库，静态库（.lib）在编译时直接嵌入代码。
        并且静态库内部使用了zlib的动态链接，那么可执行文件运行时还是需要zlib.dll !!!
]]
