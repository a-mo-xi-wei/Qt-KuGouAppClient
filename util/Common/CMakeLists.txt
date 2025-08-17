cmake_minimum_required(VERSION 3.10)
project(Common LANGUAGES CXX)

#JWT模块
add_subdirectory(SJwt)

#配置文件模块
add_subdirectory(MyConfigFile)

#http请求模块
add_subdirectory(MyHttpClient)

#通用配置文件处理模块
add_subdirectory(SApp)

set(COMMON_SOURCE
        ${SJWT_SOURCES}
        ${MYCONFIGFILE_SOURCES}
        ${MYHTTPCLIENT_SOURCES}
        ${SAPP_SOURCES}
)

add_library(Common SHARED ${COMMON_SOURCE})

target_compile_definitions(Common PRIVATE COMMON_LIBRARY)

target_include_directories(Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/MyConfigFile
        ${CMAKE_CURRENT_SOURCE_DIR}/MyHttpClient
        ${CMAKE_CURRENT_SOURCE_DIR}/SJwt
        ${CMAKE_CURRENT_SOURCE_DIR}/SApp
)

target_link_libraries(Common PUBLIC Qt::Widgets Qt::Core Qt::Network MyQsLog)