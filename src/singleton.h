//------------------------------------------------------------------------------
#ifndef SINGLETON_H
#define SINGLETON_H

//------------------------------------------------------------------------------
/** Singleton behaviour to add to a Class.

    Implements the Singleton Design Pattern. Creates the class instance the
    first time that it is accessed.

    @ingroup Util
 */
template<class T>
class Singleton{
 public:
    /// Returns the singleton instance of a Class.
    static T * instance();

 private:
    /// singleton instance of a Class.
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

//------------------------------------------------------------------------------
template<class T> T * Singleton<T>::s_singleton = nullptr;

//------------------------------------------------------------------------------
#endif//SINGLETON_H

//------------------------------------------------------------------------------
