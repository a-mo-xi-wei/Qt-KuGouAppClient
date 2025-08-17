cmake_minimum_required(VERSION 3.10)

project(Util VERSION 1.0 LANGUAGES CXX)

#异步线程模块
add_subdirectory(MyAsyncThread)

#Spdlog日志模块
add_subdirectory(MySpdlog)

#QsLog日志模块
add_subdirectory(MyQsLog)

#播放器核心模块
add_subdirectory(MyPlayer)

#数据库连接池模块
add_subdirectory(MyDBPool)

#JQHttpServer模块
add_subdirectory(JQHttpServer)

#常用简易模块
add_subdirectory(Common)
