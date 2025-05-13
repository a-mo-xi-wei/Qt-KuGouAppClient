/*
  Thread - 线程管理类
*/

#ifndef THREAD_H
#define THREAD_H

#include <thread>
#include <atomic>
#include <functional>
#include <mutex>
#include <condition_variable>

/** @namespace Util
 *  @brief 通用工具命名空间
 */
namespace Util {

/** @class Thread
 *  @brief 线程管理类
 *
 *  该类提供线程的启动、停止、暂停、恢复等功能，支持单次或循环执行模式。
 */
class Thread {
public:
    /** @enum State
     *  @brief 线程状态
     */
    enum State {
        Stoped,     ///< 停止状态（从未启动或已停止）
        Running,    ///< 运行状态
        Paused      ///< 暂停状态
    };

    /** @brief 构造函数
     *
     *  初始化 Thread 对象，默认非单次模式。
     */
    Thread();

    /** @brief 构造函数（指定模式）
     *
     *  初始化 Thread 对象，指定是否为单次模式。
     *
     *  @param single_mode 是否为单次模式
     */
    Thread(bool single_mode);

    /** @brief 析构函数
     *
     *  销毁 Thread 对象，停止线程并释放资源。
     */
    virtual ~Thread();

    /** @brief 获取线程状态
     *
     *  @return 当前线程状态
     */
    State state() const;

    /** @brief 启动线程
     */
    void start();

    /** @brief 停止线程
     */
    void stop();

    /** @brief 暂停线程
     */
    void pause();

    /** @brief 恢复线程
     */
    void resume();

    /** @brief 设置单次模式
     *
     *  @param single_mode 是否为单次模式
     */
    void setSingleMode(const bool &single_mode);

    /** @brief 设置休眠时间
     *
     *  设置每次运行后的休眠时间（毫秒）。
     *
     *  @param time 休眠时间（<0 表示持续运行，=0 表示运行一次后休眠）
     */
    void setSleepTime(const int time) { m_sleep_time = time; }

    /** @brief 设置线程执行函数
     *
     *  设置线程的执行函数，可替代重载的 run 方法。
     *
     *  @param run_func 执行函数
     */
    void setThreadFunc(std::function<void()> run_func) { m_run_func = run_func; }

    /** @brief 获取停止标志
     *
     *  @return true 表示线程已停止
     */
    bool stopFlag() { return m_is_stop; }

protected:
    /** @brief 线程执行函数
     *
     *  可由子类重载，定义线程的具体任务。
     */
    virtual void run();

private:
    /** @brief 线程入口函数
     *
     *  内部线程执行逻辑，管理运行、暂停和停止状态。
     */
    void threadFunc();

protected:
    /** @brief 线程互斥锁
     */
    std::recursive_mutex m_mutex_thread;

    /** @brief 线程对象
     */
    std::thread* m_thread = nullptr;

    /** @brief 条件变量互斥锁
     */
    std::mutex m_mutex;

    /** @brief 条件变量
     */
    std::condition_variable m_condition;

    /** @brief 暂停标志
     */
    std::atomic_bool m_is_pause;

    /** @brief 停止标志
     */
    std::atomic_bool m_is_stop;

    /** @brief 线程状态
     */
    State m_state;

    /** @brief 休眠时间
     *
     *  每次运行后的休眠时间（毫秒，<0 表示持续运行，=0 表示运行一次后休眠）。
     */
    int m_sleep_time = 0;

    /** @brief 执行函数
     *
     *  可替代重载的 run 方法。
     */
    std::function<void()> m_run_func = nullptr;
};

} // namespace Util

#endif // THREAD_H