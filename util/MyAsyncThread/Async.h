#ifndef ASYNC_H
#define ASYNC_H

#include <QCoreApplication>
#include <QFuture>
#include <QFutureInterface>
#include <QFutureWatcher>
#include <QRunnable>
#include <QThread>
#include <QThreadPool>
#include <functional>
#include <type_traits>

// 异步加载

namespace Async
{
    /**
     * @class MemberCallable
     * @brief 成员函数调用器模板类。
     *
     * 用于将成员函数与对象封装为可调用对象。
     *
     * @tparam Function 成员函数类型。
     */
    template <typename Function>
    class MemberCallable;

    /**
     * @brief 特化为 const 成员函数的 MemberCallable 模板类。
     *
     * @tparam Result 返回类型。
     * @tparam Obj 对象类型。
     * @tparam Args 参数类型列表。
     */
    template <typename Result, typename Obj, typename... Args>
    class MemberCallable<Result(Obj::*)(Args...) const>
    {
    public:
        /**
         * @brief 构造函数。
         *
         * @param function const 成员函数指针。
         * @param obj 对象指针。
         */
        MemberCallable(Result(Obj::* function)(Args...) const, const Obj *obj)
            : m_function(function),
              m_obj(obj)
        {
        }

        /**
         * @brief 重载 operator()，调用成员函数。
         *
         * @param args 参数列表。
         * @return Result 成员函数的返回类型。
         */
        Result operator()(Args&&... args) const
        {
            return ((*m_obj).*m_function)(std::forward<Args>(args)...);
        }

    private:
        /** @var Result(Obj::* m_function)(Args...) const
         * @brief const 成员函数指针。
         */
        Result(Obj::* m_function)(Args...) const;

        /** @var const Obj *m_obj
         * @brief 对象指针。
         */
        const Obj *m_obj;
    };

    /**
     * @brief 特化为非 const 成员函数的 MemberCallable 模板类。
     *
     * @tparam Result 返回类型。
     * @tparam Obj 对象类型。
     * @tparam Args 参数类型列表。
     */
    template <typename Result, typename Obj, typename... Args>
    class MemberCallable<Result(Obj::*)(Args...)>
    {
    public:
        /**
         * @brief 构造函数。
         *
         * @param function 非 const 成员函数指针。
         * @param obj 对象指针。
         */
        MemberCallable(Result(Obj::* function)(Args...), Obj *obj)
            : m_function(function),
              m_obj(obj)
        {
        }

        /**
         * @brief 重载 operator()，调用成员函数。
         *
         * @param args 参数列表。
         * @return Result 成员函数的返回类型。
         */
        Result operator()(Args&&... args) const
        {
            return ((*m_obj).*m_function)(std::forward<Args>(args)...);
        }

    private:
        /** @var Result(Obj::* m_function)(Args...)
         * @brief 非 const 成员函数指针。
         */
        Result(Obj::* m_function)(Args...);

        /** @var Obj *m_obj
         * @brief 对象指针。
         */
        Obj *m_obj;
    };

template <typename HasArity/*true_type or false_type*/,
          typename Function, unsigned index, typename T>
struct functionTakesArgumentArityDispatch;

template <typename Function, unsigned index, typename T>
struct functionTakesArgumentArityDispatch<std::false_type, Function, index, T>
        : public std::false_type
{
};
    /**
     * @struct functionTraits
     * @brief 函数特征模板类。
     *
     * 用于获取函数的返回类型和参数类型。
     *
     * @tparam Callable 可调用对象类型。
     */
    template <typename Callable>
    struct functionTraits;

    /**
     * @brief 函数类型的 functionTraits 特化。
     *
     * @tparam Result 返回类型。
     * @tparam Args 参数类型列表。
     */
    template <typename Result, typename... Args>
    struct functionTraits<Result(Args...)>
    {
        /** @var ResultType
         * @brief 函数返回类型。
         */
        using ResultType = Result;

        /** @var arity
         * @brief 函数参数数量。
         */
        static const unsigned arity = sizeof...(Args);

        /**
         * @struct argument
         * @brief 获取指定索引的参数类型。
         *
         * @tparam i 参数索引。
         */
        template <unsigned i>
        struct argument
        {
            /** @var type
             * @brief 第 i 个参数的类型。
             */
            using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
        };
    };

    /**
     * @brief 函数指针的 functionTraits 特化。
     *
     * @tparam Result 返回类型。
     * @tparam Args 参数类型列表。
     */
    template <typename Result, typename... Args>
    struct functionTraits<Result(*)(Args...)> : public functionTraits<Result(Args...)>
    {
    };

    /**
     * @brief const 函数指针的 functionTraits 特化。
     *
     * @tparam Result 返回类型。
     * @tparam Args 参数类型列表。
     */
    template <typename Result, typename... Args>
    struct functionTraits<Result(* const)(Args...)> : public functionTraits<Result(Args...)>
    {
    };

    /**
     * @brief 成员函数的 functionTraits 特化。
     *
     * @tparam Type 对象类型。
     * @tparam Result 返回类型。
     * @tparam Args 参数类型列表。
     */
    template <typename Type, typename Result, typename... Args>
    struct functionTraits<Result(Type::*)(Args...)> : public functionTraits<Result(Type&, Args...)>
    {
    };

    /**
     * @brief const 成员函数的 functionTraits 特化。
     *
     * @tparam Type 对象类型。
     * @tparam Result 返回类型。
     * @tparam Args 参数类型列表。
     */
    template <typename Type, typename Result, typename... Args>
    struct functionTraits<Result(Type::*)(Args...) const> : public functionTraits<Result(Type&, Args...)>
    {
    };
    /**
     * @class AsyncJob
     * @brief 异步任务类。
     *
     * @tparam ResultType 返回类型。
     * @tparam Function 函数类型。
     * @tparam Args 参数类型列表。
     */
// const pointer to member function
template <typename Type, typename Result, typename... Args>
struct functionTraits<Result(Type::* const)(Args...)> : public functionTraits<Result(Type&,Args...)>
{
};

// const pointer to const member function
template <typename Type, typename Result, typename... Args>
struct functionTraits<Result(Type::* const)(Args...) const> : public functionTraits<Result(Type&,Args...)>
{
};


// callables. only works if operator() is not overloaded.
template <typename Callable>
struct functionTraits
{
    using callableTraits = functionTraits<decltype(&Callable::operator())>;
    using ResultType = typename callableTraits::ResultType;
    static const unsigned arity = callableTraits::arity - 1; // ignore object pointer arg

    template <unsigned i>
    struct argument
    {
        using type = typename callableTraits::template argument<i+1>::type; // ignore object pointer arg
    };
};

// lvalue ref callables
template <typename Callable>
struct functionTraits<Callable&> : public functionTraits<Callable>
{
};

// const lvalue ref callables
template <typename Callable>
struct functionTraits<const Callable&> : public functionTraits<Callable>
{
};

// rvalue ref callables
template <typename Callable>
struct functionTraits<Callable&&> : public functionTraits<Callable>
{
};



template <typename Function, unsigned index, typename T>
struct functionTakesArgumentArityDispatch<std::true_type, Function, index, T>
        : public std::is_same<T, typename functionTraits<Function>::template argument<index>::type>
{
};



template <typename Function, unsigned index, typename T>
struct functionTakesArgument : public functionTakesArgumentArityDispatch<
        std::integral_constant<bool, (functionTraits<Function>::arity > index)>,
        Function, index, T>
{
};

template <typename ResultType, typename Function, typename... Args>
void runAsyncReturnVoidDispatch(std::true_type, QFutureInterface<ResultType>, Function &&function, Args&&... args)
{
    function(std::forward<Args>(args)...);
}

// non-void function that does not take QFutureInterface
template <typename ResultType, typename Function, typename... Args>
void runAsyncReturnVoidDispatch(std::false_type, QFutureInterface<ResultType> futureInterface, Function &&function, Args&&... args)
{
    futureInterface.reportResult(function(std::forward<Args>(args)...));
}

// function that takes QFutureInterface
template <typename ResultType, typename Function, typename... Args>
void runAsyncQFutureInterfaceDispatch(std::true_type, QFutureInterface<ResultType> futureInterface, Function &&function, Args&&... args)
{
    function(futureInterface, std::forward<Args>(args)...);
}

// function that does not take QFutureInterface
template <typename ResultType, typename Function, typename... Args>
void runAsyncQFutureInterfaceDispatch(std::false_type, QFutureInterface<ResultType> futureInterface, Function &&function, Args&&... args)
{
    runAsyncReturnVoidDispatch(std::is_void<std::result_of_t<Function(Args...)>>(),
                               futureInterface, std::forward<Function>(function), std::forward<Args>(args)...);
}

// function, function pointer, or other callable object that is no member pointer
template <typename ResultType, typename Function, typename... Args,
          typename = std::enable_if_t<!std::is_member_pointer<std::decay_t<Function>>::value>
         >
void runAsyncMemberDispatch(QFutureInterface<ResultType> futureInterface, Function &&function, Args&&... args)
{
    runAsyncQFutureInterfaceDispatch(functionTakesArgument<Function, 0, QFutureInterface<ResultType>&>(),
                                     futureInterface, std::forward<Function>(function), std::forward<Args>(args)...);
}

// Function = member function
template <typename ResultType, typename Function, typename Obj, typename... Args,
          typename = std::enable_if_t<std::is_member_pointer<std::decay_t<Function>>::value>
         >
void runAsyncMemberDispatch(QFutureInterface<ResultType> futureInterface, Function &&function, Obj &&obj, Args&&... args)
{
    // Wrap member function with object into callable
    runAsyncImpl(futureInterface,
                 MemberCallable<std::decay_t<Function>>(std::forward<Function>(function), std::forward<Obj>(obj)),
                 std::forward<Args>(args)...);
}

// cref to function/callable
template <typename ResultType, typename Function, typename... Args>
void runAsyncImpl(QFutureInterface<ResultType> futureInterface,
                  std::reference_wrapper<Function> functionWrapper, Args&&... args)
{
    runAsyncMemberDispatch(futureInterface, functionWrapper.get(), std::forward<Args>(args)...);
}

// function/callable, no cref
template <typename ResultType, typename Function, typename... Args>
void runAsyncImpl(QFutureInterface<ResultType> futureInterface,
                  Function &&function, Args&&... args)
{
    runAsyncMemberDispatch(futureInterface, std::forward<Function>(function),
                           std::forward<Args>(args)...);
}
/*
template <class T>
std::decay_t<T>
decayCopy(T&& v)
{
    return std::forward<T>(v);
}
*/
template<typename T>
auto decayCopy(T&& arg) -> std::decay_t<T> {
    return std::forward<T>(arg); // 避免不必要的拷贝
}

    template <typename ResultType, typename Function, typename... Args>
    class AsyncJob : public QRunnable
    {
    public:
        /**
         * @brief 构造函数。
         *
         * 初始化异步任务并报告开始状态。
         *
         * @param function 要执行的函数。
         * @param args 参数列表。
         */
        AsyncJob(Function &&function, Args&&... args)
          // decay copy like std::thread
            : data(decayCopy(std::forward<Function>(function)), decayCopy(std::forward<Args>(args))...)
        {
        // we need to report it as started even though it isn't yet, because someone might
        // call waitForFinished on the future, which does _not_ block if the future is not started
            futureInterface.setRunnable(this);
            futureInterface.reportStarted();
        }

        /**
         * @brief 析构函数。
         *
         * 确保任务完成时报告结束状态。
         */
        ~AsyncJob()
        {
            // QThreadPool can delete runnables even if they were never run (e.g. QThreadPool::clear).
            // Since we reported them as started, we make sure that we always report them as finished.
            // reportFinished only actually sends the signal if it wasn't already finished.
            futureInterface.reportFinished();
        }

        /**
         * @brief 获取异步任务的 QFuture 对象。
         *
         * @return QFuture<ResultType> 异步任务的未来对象。
         */
        QFuture<ResultType> future() { return futureInterface.future(); }

        /**
         * @brief 重写 QRunnable 的 run 方法。
         *
         * 执行异步任务并处理线程优先级和取消状态。
         */
        void run() override
        {
            if (priority != QThread::InheritPriority)
                if (QThread *thread = QThread::currentThread())
                    if (thread != qApp->thread())
                        thread->setPriority(priority);
            if (futureInterface.isCanceled()) {
                futureInterface.reportFinished();
                return;
            }
            runHelper(std::make_index_sequence<std::tuple_size<Data>::value>());
        }

        /**
         * @brief 设置线程池。
         *
         * @param pool 线程池指针。
         */
        void setThreadPool(QThreadPool *pool)
        {
            futureInterface.setThreadPool(pool);
        }

        /**
         * @brief 设置线程优先级。
         *
         * @param p 线程优先级。
         */
        void setThreadPriority(QThread::Priority p)
        {
            priority = p;
        }

    private:
        /** @var Data
         * @brief 数据元组类型，存储函数和参数。
         */
        using Data = std::tuple<std::decay_t<Function>, std::decay_t<Args>...>;

        /**
         * @brief 运行辅助函数。
         *
         * @tparam index 索引序列。
         */
        template <std::size_t... index>
        void runHelper(std::index_sequence<index...>)
        {
        // invalidates data, which is moved into the call
            runAsyncImpl(futureInterface, std::move(std::get<index>(data))...);
            if (futureInterface.isSuspended())
                futureInterface.waitForResume();
            futureInterface.reportFinished();
        }

        /** @var Data data
         * @brief 存储函数和参数的元组。
         */
        Data data;

        /** @var QFutureInterface<ResultType> futureInterface
         * @brief 异步任务的未来接口。
         */
        QFutureInterface<ResultType> futureInterface;

        /** @var QThread::Priority priority
         * @brief 线程优先级，默认值为 QThread::InheritPriority。
         */
        QThread::Priority priority = QThread::InheritPriority;
    };

    /**
     * @class RunnableThread
     * @brief 运行 QRunnable 的线程类。
     */
    class RunnableThread : public QThread
    {
    public:
        /**
         * @brief 构造函数。
         *
         * @param runnable 要执行的 QRunnable 对象指针。
         * @param parent 父对象指针，默认为 nullptr。
         */
        explicit RunnableThread(QRunnable *runnable, QObject *parent = 0);

    protected:
        /**
         * @brief 重写 QThread 的 run 方法。
         *
         * 执行关联的 QRunnable 对象的 run 方法。
         */
        void run();

    private:
        /** @var QRunnable *m_runnable
         * @brief 存储要执行的 QRunnable 对象指针。
         */
        QRunnable *m_runnable;
    };
/**
     * @brief 异步运行函数。
     *
     * 在指定线程池或独立线程中执行函数，并返回 QFuture 对象。
     *
     * @tparam Function 函数类型。
     * @tparam Args 参数类型列表。
     * @tparam ResultType 返回类型，默认推导自 functionTraits。
     * @param pool 线程池指针，若为空则创建独立线程。
     * @param priority 线程优先级。
     * @param function 要执行的函数。
     * @param args 参数列表。
     * @return QFuture<ResultType> 异步任务的未来对象。
*/

/*
    struct functionTraits<Function>
    {
        using ResultType; // Return type of Function
        struct argument<unsigned index>
        {
            using type; // type of Function argument at index (starting with 0)
        }
    }

    struct functionTakesArgument<Function, unsigned index, ArgumentType>;

    Is derived from std::true_type if Function takes an argument of type ArgumentType at index.
    Otherwise derived from std::false_type.
*/


template <typename F>
using functionResult_t = typename functionTraits<F>::ResultType;




using CallOperatorYes = char;
using CallOperatorNo = struct { char foo[2]; };

template<typename C>
static CallOperatorYes CallOperator(decltype(&C::operator()));

template<typename>
static CallOperatorNo CallOperator(...);

template<typename T>
struct hasCallOperator
{
    static const bool value = (sizeof(CallOperator<T>(0)) == sizeof(CallOperatorYes));
};

template <typename Function>
struct resultType;

template <typename Function, typename Arg>
struct resultTypeWithArgument;

template <typename Function, int index, bool>
struct resultTypeTakesArguments;

template <typename Function, bool>
struct resultTypeIsMemberFunction;

template <typename Function, bool>
struct resultTypeIsFunctionLike;

template <typename Function, bool>
struct resultTypeHasCallOperator;

template <typename Function, typename ResultType>
struct resultTypeWithArgument<Function, QFutureInterface<ResultType>&>
{
    using type = ResultType;
};

template <typename Function, typename Arg>
struct resultTypeWithArgument
{
    using type = functionResult_t<Function>;
};

template <typename Function, int index>
struct resultTypeTakesArguments<Function, index, true>
        : public resultTypeWithArgument<Function, typename functionTraits<Function>::template argument<index>::type>
{
};

template <typename Function, int index>
struct resultTypeTakesArguments<Function, index, false>
{
    using type = functionResult_t<Function>;
};

template <typename Function>
struct resultTypeIsFunctionLike<Function, true>
        : public resultTypeTakesArguments<Function, 0, (functionTraits<Function>::arity > 0)>
{
};

template <typename Function>
struct resultTypeIsMemberFunction<Function, true>
        : public resultTypeTakesArguments<Function, 1, (functionTraits<Function>::arity > 1)>
{
};

template <typename Function>
struct resultTypeIsMemberFunction<Function, false>
{
    using type = void;
};

template <typename Function>
struct resultTypeIsFunctionLike<Function, false>
        : public resultTypeIsMemberFunction<Function, std::is_member_function_pointer<Function>::value>
{
};

template <typename Function>
struct resultTypeHasCallOperator<Function, false>
        : public resultTypeIsFunctionLike<Function, std::is_function<std::remove_pointer_t<std::decay_t<Function>>>::value>
{
};

template <typename Callable>
struct resultTypeHasCallOperator<Callable, true>
        : public resultTypeTakesArguments<Callable, 0, (functionTraits<Callable>::arity > 0)>
{
};

template <typename Function>
struct resultType
        : public resultTypeHasCallOperator<Function, hasCallOperator<Function>::value>
{
};

template <typename Function>
struct resultType<Function&> : public resultType<Function>
{
};

template <typename Function>
struct resultType<const Function&> : public resultType<Function>
{
};

template <typename Function>
struct resultType<Function &&> : public resultType<Function>
{
};

template <typename Function>
struct resultType<std::reference_wrapper<Function>> : public resultType<Function>
{
};
template <typename Function>
struct resultType<std::reference_wrapper<const Function>> : public resultType<Function>
{
};



/*!
    runAsync类似于 std::thread和std::invoke 函数参数可以是成员函数 操作符 可以是 lambda表达式
    QFutureInterface<ResultType>&作为第一个参数传入到函数中， args传递给函数调用
    函数调用的结果将报告给QFuter 如果函数是（非静态）成员函数，则需要 args中的第一个参数
    是函数被调用的对象。如果给一个线程池，函数就在那里运行。否则会创建一个独立线程
    开始执行。
*/
    template <typename Function, typename... Args,
              typename ResultType = typename resultType<Function>::type>
    QFuture<ResultType>
    runAsync(QThreadPool *pool, QThread::Priority priority, Function &&function, Args&&... args)
    {
        auto job = new AsyncJob<ResultType, Function, Args...>
                (std::forward<Function>(function), std::forward<Args>(args)...);
        job->setThreadPriority(priority);
        QFuture<ResultType> future = job->future();
        if (pool) {
            job->setThreadPool(pool);
            pool->start(job);
        } else {
            auto thread = new RunnableThread(job);
        thread->moveToThread(qApp->thread()); // make sure thread gets deleteLater on main thread
            QObject::connect(thread, &QThread::finished, thread, &QObject::deleteLater);
            thread->start(priority);
        }
        return future;
    }

    /**
     * @brief 当结果准备好时调用槽函数。
     *
     * @tparam R 接收者类型。
     * @tparam T 结果类型。
     * @param future 未来对象。
     * @param receiver 接收者对象指针。
     * @param member 槽函数指针。
     * @return const QFuture<T>& 未来对象引用。
     */

/*!
  根据线程优先级创建一个线程执行
 */
template <typename Function, typename... Args,
          typename ResultType = typename resultType<Function>::type>
QFuture<ResultType>
runAsync(QThread::Priority priority, Function &&function, Args&&... args)
{
    return runAsync(static_cast<QThreadPool *>(nullptr), priority,
                    std::forward<Function>(function), std::forward<Args>(args)...);
}

/*!
   创建一个线程(QThread::InheritPriority)执行函数
 */
template <typename Function, typename... Args,
          typename = std::enable_if_t<
                !std::is_same<std::decay_t<Function>, QThreadPool>::value
                && !std::is_same<std::decay_t<Function>, QThread::Priority>::value
              >,
          typename ResultType = typename resultType<Function>::type>
QFuture<ResultType>
runAsync(Function &&function, Args&&... args)
{
    return runAsync(static_cast<QThreadPool *>(nullptr),
                    QThread::InheritPriority, std::forward<Function>(function),
                    std::forward<Args>(args)...);
}

/*!
   在线程池中执行相应函数
 */
template <typename Function, typename... Args,
          typename = std::enable_if_t<!std::is_same<std::decay_t<Function>, QThread::Priority>::value>,
          typename ResultType = typename resultType<Function>::type>
QFuture<ResultType>
runAsync(QThreadPool *pool, Function &&function, Args&&... args)
{
    return runAsync(pool, QThread::InheritPriority, std::forward<Function>(function),
                    std::forward<Args>(args)...);
}/*!
   执行结果读取函数
   创建一个QFutureWatcher读取相应结果传递相应函数中，
   如果有更复杂操作就不要使用

     */
    template <typename R, typename T>
    const QFuture<T> &onResultReady(const QFuture<T> &future, R *receiver, void(R::*member)(const T &))
    {
        auto watcher = new QFutureWatcher<T>();
        QObject::connect(watcher, &QFutureWatcherBase::finished, watcher, &QObject::deleteLater);
        QObject::connect(watcher, &QFutureWatcherBase::resultReadyAt, receiver,
                         [receiver, member, watcher](int index) {
                             (receiver->*member)(watcher->future().resultAt(index));
                         });
        watcher->setFuture(future);
        return future;
    }

/*!

*/
template <typename T, typename Function>
const QFuture<T> &onResultReady(const QFuture<T> &future, QObject *guard, Function f)
{
    auto watcher = new QFutureWatcher<T>();
    QObject::connect(watcher, &QFutureWatcherBase::finished, watcher, &QObject::deleteLater);
    QObject::connect(watcher, &QFutureWatcherBase::resultReadyAt, guard, [f, watcher](int index) {
        f(watcher->future().resultAt(index));
    });
    watcher->setFuture(future);
    return future;
}

/*!

*/
template <typename T, typename Function>
const QFuture<T> &onResultReady(const QFuture<T> &future, Function f)
{
    auto watcher = new QFutureWatcher<T>();
    QObject::connect(watcher, &QFutureWatcherBase::finished, watcher, &QObject::deleteLater);
    QObject::connect(watcher, &QFutureWatcherBase::resultReadyAt, [f, watcher](int index) {
        f(watcher->future().resultAt(index));
    });
    watcher->setFuture(future);
    return future;
}

}

#endif // ASYNC_H