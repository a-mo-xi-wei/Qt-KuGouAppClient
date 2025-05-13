/*
  Thread - 线程管理类实现
*/

#include "thread.h"

using namespace std;

/** @namespace Util
 *  @brief 通用工具命名空间
 */
namespace Util {

/** @brief 构造函数
 *
 *  初始化 Thread 对象，默认非单次模式。
 */
Thread::Thread()
    : m_thread(nullptr),
      m_is_pause(false),
      m_is_stop(false),
      m_state(Stoped) {
    setSingleMode(false);
}

/** @brief 构造函数（指定模式）
 *
 *  初始化 Thread 对象，指定是否为单次模式。
 *
 *  @param single_mode 是否为单次模式
 */
Thread::Thread(bool single_mode)
    : m_thread(nullptr),
      m_is_pause(false),
      m_is_stop(false),
      m_state(Stoped) {
    setSingleMode(single_mode);
}

/** @brief 析构函数
 *
 *  销毁 Thread 对象，停止线程并释放资源。
 */
Thread::~Thread() {
    stop();
}

/** @brief 获取线程状态
 *
 *  @return 当前线程状态
 */
Thread::State Thread::state() const {
    return m_state;
}

/** @brief 启动线程
 */
void Thread::start() {
    m_mutex_thread.lock();

    //线程已经退出了，需要销毁再重新new一个
    if (m_state == Stoped && m_thread != nullptr)
    {
        m_thread->join(); // wait for thread finished
        delete m_thread;
        m_thread = nullptr;
    }

    if (m_thread == nullptr) {
        m_is_pause = false;
        m_is_stop = false;
        m_state = Running;
        m_thread = new thread(&Thread::threadFunc, this);
    } else {
        resume();
    }
    m_mutex_thread.unlock();
}

/** @brief 停止线程
 */
void Thread::stop() {
    m_mutex_thread.lock();
    if (m_thread != nullptr) {
        m_is_pause = false;
        m_is_stop = true;
        m_condition.notify_all();  // Notify one waiting thread, if there is one.
        m_thread->join(); // wait for thread finished
        delete m_thread;
        m_thread = nullptr;
        m_state = Stoped;
    }
    m_mutex_thread.unlock();
}

/** @brief 暂停线程
 */
void Thread::pause() {
    m_mutex_thread.lock();
    if (m_thread != nullptr) {
        m_is_pause = true;
        m_state = Paused;
    }
    m_mutex_thread.unlock();
}

/** @brief 恢复线程
 */
void Thread::resume() {
    m_mutex_thread.lock();
    if (m_thread != nullptr) {
        m_is_pause = false;
        m_condition.notify_all();
        m_state = Running;
    }
    m_mutex_thread.unlock();
}

/** @brief 设置单次模式
 *
 *  @param single_mode 是否为单次模式
 */
void Thread::setSingleMode(const bool &single_mode) {
    if (single_mode) {
        m_sleep_time = 0;
    } else if (m_sleep_time == 0) {
        m_sleep_time = -1;
    }
}

/** @brief 线程执行函数
 *
 *  可由子类重载，定义线程的具体任务。
 */
void Thread::run() {
    if (m_run_func) {
        m_run_func();
    }
}

/** @brief 线程入口函数
 *
 *  内部线程执行逻辑，管理运行、暂停和停止状态。
 */
void Thread::threadFunc() {
    while (!m_is_stop) {
        m_state = Running;
        run();
        if (m_is_stop) {
            break;
            } else if (m_sleep_time > 0) {
            unique_lock<mutex> locker(m_mutex);
            m_state = Paused;
            if (m_condition.wait_for(locker, std::chrono::milliseconds(m_sleep_time)) == std::cv_status::timeout) {
            }
        } else if (m_is_pause) {
            unique_lock<mutex> locker(m_mutex);
            m_state = Paused;
            do {
                m_condition.wait(locker); // Unlock _mutex and wait to be notified
            }while (m_is_pause);

            locker.unlock();
        } else if (m_sleep_time == 0) {
            break;
        }
    }
    m_is_pause = false;
    m_is_stop = false;
    m_state = Stoped;
}

} // namespace Util