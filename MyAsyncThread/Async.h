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


//异步加载

namespace  Async
{
/*
  可调用对象，将函数指针和对象一起封装调用
*/

template <typename Function>
class MemberCallable;

template <typename Result, typename Obj, typename... Args>
class MemberCallable<Result(Obj::*)(Args...) const>
{
public:
    MemberCallable(Result(Obj::* function)(Args...) const, const Obj *obj)
        : m_function(function),
          m_obj(obj)
    {
    }

    Result operator()(Args&&... args) const
    {
        return ((*m_obj).*m_function)(std::forward<Args>(args)...);
    }

private:
    Result(Obj::* m_function)(Args...) const;
    const Obj *m_obj;
};

template <typename Result, typename Obj, typename... Args>
class MemberCallable<Result(Obj::*)(Args...)>
{
public:
    MemberCallable(Result(Obj::* function)(Args...), Obj *obj)
        : m_function(function),
          m_obj(obj)
    {
    }

    Result operator()(Args&&... args) const
    {
        return ((*m_obj).*m_function)(std::forward<Args>(args)...);
    }

private:
    Result(Obj::* m_function)(Args...);
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


// functionTraits


// for callables. defined below.
template <typename Callable>
struct functionTraits;

// 函数
template <typename Result, typename... Args>
struct functionTraits<Result(Args...)>
{
    using ResultType = Result;
    static const unsigned arity = sizeof...(Args); // TODO const -> constexpr with MSVC2015

    template <unsigned i>
    struct argument
    {
        using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
    };
};

// 函数指针
template <typename Result, typename... Args>
struct functionTraits<Result(*)(Args...)> : public functionTraits<Result(Args...)>
{
};

// const 函数指针
template <typename Result, typename... Args>
struct functionTraits<Result(* const)(Args...)> : public functionTraits<Result(Args...)>
{
};

// 成员函数
template <typename Type, typename Result, typename... Args>
struct functionTraits<Result(Type::*)(Args...)> : public functionTraits<Result(Type&,Args...)>
{
};

// const 成员函数
template <typename Type, typename Result, typename... Args>
struct functionTraits<Result(Type::*)(Args...) const> : public functionTraits<Result(Type&,Args...)>
{
};

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
    static const unsigned arity = callableTraits::arity - 1; // ignore object pointer arg // TODO const -> constexpr with MSVC2015

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

template <class T>
std::decay_t<T>
decayCopy(T&& v)
{
    return std::forward<T>(v);
}


template <typename ResultType, typename Function, typename... Args>
class AsyncJob : public QRunnable
{
public:
    AsyncJob(Function &&function, Args&&... args)
          // decay copy like std::thread
        : data(decayCopy(std::forward<Function>(function)), decayCopy(std::forward<Args>(args))...)
    {
        // we need to report it as started even though it isn't yet, because someone might
        // call waitForFinished on the future, which does _not_ block if the future is not started
        futureInterface.setRunnable(this);
        futureInterface.reportStarted();
    }

    ~AsyncJob()
    {
        // QThreadPool can delete runnables even if they were never run (e.g. QThreadPool::clear).
        // Since we reported them as started, we make sure that we always report them as finished.
        // reportFinished only actually sends the signal if it wasn't already finished.
        futureInterface.reportFinished();
    }

    QFuture<ResultType> future() { return futureInterface.future(); }

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

    void setThreadPool(QThreadPool *pool)
    {
        futureInterface.setThreadPool(pool);
    }

    void setThreadPriority(QThread::Priority p)
    {
        priority = p;
    }

private:
    using Data = std::tuple<std::decay_t<Function>, std::decay_t<Args>...>;
    template <std::size_t... index>
    void runHelper(std::index_sequence<index...>)
    {
        // invalidates data, which is moved into the call
        runAsyncImpl(futureInterface, std::move(std::get<index>(data))...);
        if (futureInterface.isSuspended())
            futureInterface.waitForResume();
        futureInterface.reportFinished();
    }

    Data data;
    QFutureInterface<ResultType> futureInterface;
    QThread::Priority priority = QThread::InheritPriority;
};

class  RunnableThread : public QThread
{
public:
    explicit RunnableThread(QRunnable *runnable, QObject *parent = 0);

protected:
    void run();

private:
    QRunnable *m_runnable;
};

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
    auto job = new AsyncJob<ResultType,Function,Args...>
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