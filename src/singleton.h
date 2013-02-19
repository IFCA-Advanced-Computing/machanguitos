//------------------------------------------------------------------------------
#ifndef SINGLETON_H
#define SINGLETON_H

//------------------------------------------------------------------------------
template<class T>
class Singleton{
 public:
    static T * instance();

 private:
    static T * s_singleton;
};

//------------------------------------------------------------------------------
template<class T>
inline T * Singleton<T>::instance(){
    if( nullptr == s_singleton ){
        s_singleton = new T();
    }

    return s_singleton;
}

template<class T> T * Singleton<T>::s_singleton = nullptr;

//------------------------------------------------------------------------------
#endif//SINGLETON_H

//------------------------------------------------------------------------------
