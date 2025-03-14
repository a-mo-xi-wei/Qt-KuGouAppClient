#ifndef MYLOG_MACRO_DEFINE_
#define MYLOG_MACRO_DEFINE_

#include "mylog.h"

#define LOG_LEVEL_TRACE    0
#define LOG_LEVEL_DEBUG    1
#define LOG_LEVEL_INFO     2
#define LOG_LEVEL_WARN     3
#define LOG_LEVEL_ERROR    4
#define LOG_LEVEL_FATAL    5
#define LOG_LEVEL_CLOSE    6


// got short filename(exclude file directory)
#define __FILENAME__ (mylog::logger::get_shortname(__FILE__))

#if (LOGGER_LEVEL <= LOG_LEVEL_TRACE)
#	define	 LOG_TRACE(fmt, ...) 	mylog::logger::get().log({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::trace, fmt, ##__VA_ARGS__);
#	define 	 PRINT_TRACE(fmt,...) 	mylog::logger::get().printf({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::trace, fmt, ##__VA_ARGS__);
#	define	 STREAM_TRACE() 		mylog::logger::log_stream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::trace, mylog::logger::get().get_logger())
#else
#	define	 LOG_TRACE(fmt, ...)
#	define 	 PRINT_TRACE(fmt,...)
#	define	 STREAM_TRACE() mylog::logger_none::get()
#endif

#if (LOGGER_LEVEL <= LOG_LEVEL_DEBUG)
#	define	 LOG_DEBUG(fmt, ...) 	mylog::logger::get().log({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::debug, fmt, ##__VA_ARGS__);
#	define 	 PRINT_DEBUG(fmt,...) 	mylog::logger::get().printf({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::debug, fmt, ##__VA_ARGS__);
#	define	 STREAM_DEBUG() 		mylog::logger::log_stream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::debug, mylog::logger::get().get_logger())
#else
#	define	 LOG_DEBUG(fmt, ...)
#	define 	 PRINT_DEBUG(fmt,...)
#	define	 STREAM_DEBUG() mylog::logger_none::get()
#endif

#if (LOGGER_LEVEL <= LOG_LEVEL_INFO)
#	define	 LOG_INFO(fmt, ...)     mylog::logger::get().log({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::info, fmt, ##__VA_ARGS__);
#	define 	 PRINT_INFO(fmt,...) 	mylog::logger::get().printf({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::info, fmt, ##__VA_ARGS__);
#	define	 STREAM_INFO() 			mylog::logger::log_stream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::info, mylog::logger::get().get_logger())
#else
#	define	 LOG_INFO(fmt, ...)
#	define 	 PRINT_INFO(fmt,...)
#	define	 STREAM_INFO() mylog::logger_none::get()
#endif

#if (LOGGER_LEVEL <= LOG_LEVEL_WARN)
#	define	 LOG_WARN(fmt, ...) 	mylog::logger::get().log({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::warn, fmt, ##__VA_ARGS__);
#	define 	 PRINT_WARN(fmt,...) 	mylog::logger::get().printf({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::warn, fmt, ##__VA_ARGS__);
#	define	 STREAM_WARN() 			mylog::logger::log_stream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::warn, mylog::logger::get().get_logger())
#else
#	define	 LOG_WARN(fmt, ...)
#	define 	 PRINT_WARN(fmt,...)
#	define	 STREAM_WARN() mylog::logger_none::get()
#endif

#if (LOGGER_LEVEL <= LOG_LEVEL_ERROR)
#	define	 LOG_ERROR(fmt, ...) 	mylog::logger::get().log({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::err, fmt, ##__VA_ARGS__);
#	define 	 PRINT_ERROR(fmt,...) 	mylog::logger::get().printf({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::err, fmt, ##__VA_ARGS__);
#	define	 STREAM_ERROR() 		mylog::logger::log_stream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::err, mylog::logger::get().get_logger())
#else
#	define	 LOG_ERROR(fmt, ...)
#	define 	 PRINT_ERROR(fmt,...)
#	define	 STREAM_ERROR() mylog::logger_none::get()
#endif

#if (LOGGER_LEVEL <= LOG_LEVEL_FATAL)
#	define	 LOG_FATAL(fmt, ...) 	mylog::logger::get().log({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::critical, fmt, ##__VA_ARGS__);
#	define 	 PRINT_FATAL(fmt,...) 	mylog::logger::get().printf({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::critical, fmt, ##__VA_ARGS__);
#	define	 STREAM_FATAL() 		mylog::logger::log_stream({__FILENAME__, __LINE__, __FUNCTION__}, spdlog::level::critical, mylog::logger::get().get_logger())
#else
#	define	 LOG_FATAL(fmt, ...)
#	define 	 PRINT_FATAL(fmt,...)
#	define	 STREAM_FATAL() mylog::logger_none::get()
#endif

#endif // MYLOG_MACRO_DEFINE_

