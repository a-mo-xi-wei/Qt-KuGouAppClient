#ifndef MYLOG_LOGGER_
#define MYOG_LOGGER_

#include <filesystem>
#include <sstream>
#include <memory>

#if WIN32
#ifndef NOMINMAX
#	undef min
#	undef max
#endif
#endif

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/logger.h>
#include <spdlog/fmt/fmt.h>
#include <spdlog/fmt/bundled/printf.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <QObject>
#include <QDebug>
#include <utility>

/// spdlog wrap class
///
#if defined(SPDLOG_COMPILED_LIB)
#define SPDLOG_COMPILED_EXPORT Q_DECL_EXPORT
#else
#define SPDLOG_COMPILED_EXPORT Q_DECL_IMPORT
#endif

namespace mylog {
class SPDLOG_COMPILED_EXPORT logger final {
public:
	/// let logger like stream
	struct log_stream : public std::ostringstream
	{
	public:
		log_stream(const spdlog::source_loc& _loc, spdlog::level::level_enum _lvl, std::string_view _prefix)
			: _loc(_loc)
			, _lvl(_lvl)
			, _prefix(_prefix)
		{
		}

		~log_stream(){flush();}

		void flush(){mylog::logger::get().log(_loc, _lvl, (_prefix + str()).c_str());}

	private:
		spdlog::source_loc _loc;
		spdlog::level::level_enum _lvl = spdlog::level::info;
		std::string _prefix;
	};

public:
	// 单例访问
	static logger& get();

	// 初始化日志系统（跨模块共享）
	bool init(std::string_view log_file_path);

	void shutdown();

/*
	void shutdown(){
		spdlog::shutdown();
	}

	template <typename... Args>
	void log(const spdlog::source_loc& loc, spdlog::level::level_enum lvl, const char* fmt, const Args &... args)
	{
		spdlog::log(loc, lvl, fmt, args...);
	}

	template <typename... Args>
	void printf(const spdlog::source_loc& loc, spdlog::level::level_enum lvl, const char* fmt, const Args &... args)
	{
		spdlog::log(loc, lvl, fmt::sprintf(fmt, args...).c_str());
	}
*/
	// 日志接口
	template <typename... Args>
	void log(const spdlog::source_loc& loc,
			spdlog::level::level_enum lvl,
			const char* fmt, const Args&... args) {
		if (_logger) {
			_logger->log(loc, lvl, fmt, args...);
		}
	}

	template <typename... Args>
	void printf(const spdlog::source_loc& loc,
				spdlog::level::level_enum lvl,
				const char* fmt, const Args&... args) {
		if (_logger) {
			_logger->log(loc, lvl, fmt::sprintf(fmt, args...).c_str());
		}
	}

	spdlog::level::level_enum level() {return _log_level;}

/*
	void set_level(spdlog::level::level_enum lvl) {
		_log_level = lvl;
		spdlog::set_level(lvl);

	}

	void set_flush_on(spdlog::level::level_enum lvl) {
		spdlog::flush_on(lvl);
	}

	static const char* get_shortname(std::string_view path) {
		if (path.empty())
			return path.data();

		size_t pos = path.find_last_of("/\\");
		return path.data() + ((pos == path.npos) ? 0 : pos + 1);
	}
	*/

	void set_level(spdlog::level::level_enum lvl);
	void set_flush_on(spdlog::level::level_enum lvl);
	// 工具函数
	static const char* get_shortname(std::string_view path);

private:
	logger() = default;
	~logger() = default;

	logger(const logger&) = delete;
	void operator=(const logger&) = delete;

private:
	std::shared_ptr<spdlog::logger> _logger;  // 实际日志记录器
	std::atomic_bool _is_inited = false;
	spdlog::level::level_enum _log_level = spdlog::level::trace;

};

class logger_none {
public:
	logger_none() = default;

    static logger_none& get() {
        static logger_none logger;
        return logger;
    }

	// logger_none& operator<<(const char* content) {
	// 	return *this;
	// }
	template<typename T>
	logger_none& operator<<(const T&) { return *this; }
};



} // namespace mylog

#endif // MYLOG_LOGGER_