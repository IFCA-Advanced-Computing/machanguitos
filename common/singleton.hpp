/*******************************************************************************
Machanguitos is The Easiest Multi-Agent System in the world. Work done at The
Institute of Physics of Cantabria (IFCA).
Copyright (C) 2013  Luis Cabellos

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.
*******************************************************************************/
/** @file singleton.hpp
    @brief Singleton Declaration.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#ifndef SINGLETON_HPP
#define SINGLETON_HPP

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
#endif//SINGLETON_HPP

//------------------------------------------------------------------------------
