#include "Async.h"

namespace Async
{
	/**
	 * @brief RunnableThread 类的构造函数。
	 *
	 * 初始化一个线程对象，用于执行指定的 QRunnable 对象。
	 *
	 * @param runnable 要执行的 QRunnable 对象指针。
	 * @param parent 父对象指针，默认为 nullptr。
	 */
	RunnableThread::RunnableThread(QRunnable *runnable, QObject *parent)
		: QThread(parent),
		  m_runnable(runnable)
	{
	}

	/**
	 * @brief 重写 QThread 的 run 方法。
	 *
	 * 执行关联的 QRunnable 对象的 run 方法，并在完成后根据 autoDelete 属性决定是否删除该对象。
	 */
	void RunnableThread::run()
	{
		m_runnable->run();
		if (m_runnable->autoDelete())
			delete m_runnable;
	}

	/** @var QRunnable *RunnableThread::m_runnable
	 * @brief 存储要执行的 QRunnable 对象指针。
	 */
}