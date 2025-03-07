#ifndef SINGLETON_H
#define SINGLETON_H

#include <assert.h>

#define initialiseSingleton(type) \
    template <> type * Singleton < type > :: mSingleton = 0

#define initialiseTemplateSingleton(temp,type) \
    template <> temp< type > * Singleton < temp< type > > :: mSingleton = 0

#define createFileSingleton( type ) \
    initialiseSingleton( type ); \
    type the##type

template < class type > class Singleton
{
public:
    Singleton()
    {
        assert(this->mSingleton == 0);
        this->mSingleton = static_cast<type*>(this);
    }
    virtual ~Singleton()
    {
        this->mSingleton = 0;
    }

    static type & getSingleton() { assert(mSingleton); return *mSingleton; }
    static type* getSingletonPtr() { return mSingleton; }

protected:
    static type* mSingleton;
};

#endif // SINGLETON_H
