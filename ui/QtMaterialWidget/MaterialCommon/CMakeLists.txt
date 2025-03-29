project(MaterialCommon VERSION 1.0 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)  # 启用 AUTOMOC
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 包含所有头文件和源文件
file(GLOB SOURCE_H ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SOURCE_CPP ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(SOURCE_FILES
    ${SOURCE_CPP}
    ${SOURCE_H}
)

# 创建动态库
add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})

target_compile_definitions(${PROJECT_NAME} PRIVATE MATERIALCOMMON_LIBRARY)

# 指定库的包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 链接Qt依赖（根据实际需要添加）
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Widgets Qt6::StateMachine)
