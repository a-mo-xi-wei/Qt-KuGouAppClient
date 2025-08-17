//
// Created by WeiWang on 25-3-14.
//
#include "mylog.h"

// mylog.cpp
namespace mylog {
// 通用实现（非Windows或非DLL编译时使用）
#if !defined(_WIN32) || !defined(SPDLOG_COMPILED_LIB)
logger &logger::get()
{
    static logger instance;
    return instance;
}
// Windows DLL导出专用实现
#else
__declspec(dllexport) logger &logger::get()
{
    static logger instance;
    return instance;
}
#endif

bool logger::init(std::string_view log_file_path)
{
    namespace fs = std::filesystem;
    if (_is_inited.exchange(true)) {
        return true;
    }
    try {
        // check log path and try to create log directory
        fs::path log_path(log_file_path);
        fs::path log_dir = log_path.parent_path();
        if (!fs::exists(log_dir)) {
            fs::create_directories(log_dir);
        }
        // initialize spdlog
        constexpr std::size_t log_buffer_size = 32 * 1024; // 32kb
        // constexpr std::size_t max_file_size = 50 * 1024 * 1024; // 50mb
        spdlog::init_thread_pool(log_buffer_size, std::thread::hardware_concurrency());
        std::vector<spdlog::sink_ptr> sinks;
        //按时间（每日）或文件大小自动轮换日志文件
        auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
            log_path.string(),
            0,
            0,
            false,
            3); // 保留最近3个日志文件)
        sinks.push_back(daily_sink);
        //每次启动程序时清空原有日志文件
        // auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_path.string(), true);
        // sinks.push_back(file_sink);

#if defined(_DEBUG) && defined(WIN32) && !defined(NO_CONSOLE_LOG)
        auto ms_sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
        sinks.push_back(ms_sink);
#endif //  _DEBUG

#if !defined(WIN32) && !defined(NO_CONSOLE_LOG)
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        sinks.push_back(console_sink);
#endif
        /*/
        auto logger = std::make_shared<spdlog::logger>("client_logger", sinks.begin(), sinks.end());
        spdlog::set_default_logger(logger);
        spdlog::set_pattern("%s(%#): [%L %D %T.%e %P %t %!] %v");
        spdlog::flush_on(spdlog::level::warn);
        spdlog::set_level(_log_level);
        */
        _logger = std::make_shared<spdlog::async_logger>("client_logger",
                                                         sinks.begin(),
                                                         sinks.end(),
                                                         spdlog::thread_pool(),
                                                         spdlog::async_overflow_policy::block);
        _logger->set_pattern("%s(%#): [%L %D %T.%e %P %t %!] %v");
        spdlog::flush_on(spdlog::level::info);
        _logger->set_level(_log_level);
        // 注册为全局日志记录器（当前模块）
        spdlog::set_default_logger(_logger);
        //_is_inited = true;
        return true;
    } catch (std::exception &e) {
        qCritical() << "Logger init failed:" << e.what();
        _is_inited = false;
        return false;
    }
}

void logger::shutdown()
{
    if (_logger) {
        _logger->flush();
        spdlog::drop(_logger->name());
        _logger.reset();
    }
    _is_inited = false;
}

void logger::set_level(spdlog::level::level_enum lvl)
{
    _log_level = lvl;
    if (_logger) {
        _logger->set_level(lvl);
    }
}

void logger::set_flush_on(spdlog::level::level_enum lvl)
{
    if (_logger) {
        _logger->flush_on(lvl);
    }
}

const char *logger::get_shortname(std::string_view path)
{
    const size_t pos = path.find_last_of("/\\");
    return (pos == std::string_view::npos) ? path.data() : path.data() + pos + 1;
}
} // namespace mylog
