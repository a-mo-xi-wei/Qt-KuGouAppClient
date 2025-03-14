//
// Created by WeiWang on 25-3-14.
//
#include "mylog.h"

// mylog.cpp
namespace mylog {
    // 通用实现（非Windows或非DLL编译时使用）
#if !defined(_WIN32) || !defined(SPDLOG_COMPILED_LIB)
    logger& logger::get() {
        static logger instance;
        return instance;
    }
    // Windows DLL导出专用实现
#else
    __declspec(dllexport) logger &logger::get() {
        static logger instance;
        return instance;
    }

    bool logger::init(std::string_view log_file_path) {
        namespace fs = std::filesystem;
        if (_is_inited) {
            qDebug() << "Logger already initialized.";
            return true;
        }
        try {
            // check log path and try to create log directory
            fs::path log_path(log_file_path);
            fs::path log_dir = log_path.parent_path();
            if (!fs::exists(log_dir)) {
                fs::create_directories(log_dir);
                qDebug() << "Log directory created: " << log_dir.string();
            } else {
                qDebug() << "Log directory exists: " << log_dir.string();
            }
            // initialize spdlog
            constexpr std::size_t log_buffer_size = 32 * 1024; // 32kb
            // constexpr std::size_t max_file_size = 50 * 1024 * 1024; // 50mb
            spdlog::init_thread_pool(log_buffer_size, std::thread::hardware_concurrency());
            std::vector<spdlog::sink_ptr> sinks;
            //按时间（每日）或文件大小自动轮换日志文件
            auto daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(log_path.string(), 0, 2);
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
            /*auto logger = std::make_shared<spdlog::logger>("client_logger", sinks.begin(), sinks.end());
            spdlog::set_default_logger(logger);
            qDebug() << "Logger address:" << logger.get();
            spdlog::set_pattern("%s(%#): [%L %D %T.%e %P %t %!] %v");
            spdlog::flush_on(spdlog::level::warn);
            spdlog::set_level(_log_level);*/
            // 创建独立logger实例
            _logger = std::make_shared<spdlog::logger>("client_logger",sinks.begin(),sinks.end());
            _logger->set_pattern("%s(%#): [%L %D %T.%e %P %t %!] %v");
            _logger->set_level(_log_level);
            spdlog::flush_on(spdlog::level::warn);
            qDebug() << "Logger address:" << _logger.get();

            _is_inited = true;
            return true;
        } catch (std::exception_ptr &e) {
            return false;
        }
        //_is_inited = true;
        //return true;
    }

#endif
} // namespace mylog
