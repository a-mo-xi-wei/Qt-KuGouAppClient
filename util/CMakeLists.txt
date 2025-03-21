cmake_minimum_required(VERSION 3.10)

project(Util VERSION 1.0 LANGUAGES CXX)

# 全局默认设定：可执行文件和 DLL 到 bin/, 静态库和导入库（.dll.a）到 lib/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

#异步线程模块
add_subdirectory(MyAsyncThread)

#Spdlog日志模块
add_subdirectory(MySpdlog)

#QsLog日志模块
add_subdirectory(MyQsLog)

#JWT模块
add_subdirectory(SJwt)

#数据库连接池模块
add_subdirectory(MyDBPool)

#配置文件模块
add_subdirectory(MyConfigFile)

#JQHttpServer模块
add_subdirectory(JQHttpServer)

#http请求模块
add_subdirectory(MyHttpClient)

