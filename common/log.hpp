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
/** @file log.hpp
    @brief Log utilities.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#ifndef LOG_H
#define LOG_H

#include <iostream>

//------------------------------------------------------------------------------
namespace Util{
    //--------------------------------------------------------------------------
    /** Log Level of a log message.
        @ingroup Util
     */
    enum class LogLevel : unsigned char {
        LOG_ERROR = 0, ///< Error Log.
            LOG_WARNING = 1, ///< Warning Log.
            LOG_INFO = 2, ///< Info Log.
            LOG_VERBOSE = 3, ///< Verbose Log.
            LOG_DEBUG = 4, ///< Debug Log.
    };

    //--------------------------------------------------------------------------
    /** Check if a Util::LogLevel is enabled and should be printed.
        @param ll Log Level to check.
        @ingroup Util
     */
    bool isEnabled( LogLevel ll );
    /** Print the stamp prefix of a Log using LogLevel info.
        @param ll Log Level to stamp.
        @ingroup Util
     */
    void printLogStamp( LogLevel ll );
    /** Sets the application log level.
        @param ll new Log Level Name.
        @ingroup Util
     */
    void setLogLevel( int ll );
    /** Sets the application log level.
        @param str Log Level Name. One of "error", "warning", "info", "verbose", "debug".
        @ingroup Util
     */
    void setLogLevel( const std::string & str );
    /** Gets the application log level.
        @returns current Log Level.
        @ingroup Util
     */
    LogLevel getLogLevel();

    //--------------------------------------------------------------------------
    /** Print a type in the log.
        @ingroup Util
     */
    template<typename X> void printLog( const X & x ){
        std::clog << x << std::endl;
    }

    //--------------------------------------------------------------------------
    /** print a type list in the log.
        @ingroup Util
     */
    template<typename X, typename... XS> void printLog( const X & x, XS... xs ){
        std::clog << x;
        printLog( xs... );
    }

    //--------------------------------------------------------------------------
    /** Log a message with a defined Log Level if the level is enabled.
        @param ll Log Level to use.
        @param xs List of types to log.
        @ingroup Util
     */
    template<typename... XS> void LOG( LogLevel ll, XS... xs ){
        if( isEnabled( ll ) ){
            printLogStamp( ll );
            printLog( xs... );
        }
    }

    //--------------------------------------------------------------------------
    /** Log an Error message.
        @ingroup Util
     */
    template<typename... XS> inline void LOGE( XS... xs ){
        LOG( LogLevel::LOG_ERROR, xs... );
    }

    //--------------------------------------------------------------------------
    /** Log a Warning message.
        @ingroup Util
     */
    template<typename... XS> inline void LOGW( XS... xs ){
        LOG( LogLevel::LOG_WARNING, xs... );
    }

    //--------------------------------------------------------------------------
    /** Log a Info message.
        @ingroup Util
     */
    template<typename... XS> inline void LOGI( XS... xs ){
        LOG( LogLevel::LOG_INFO, xs... );
    }

    //--------------------------------------------------------------------------
    /** Log a Verbose message.
        @ingroup Util
     */
    template<typename... XS> inline void LOGV( XS... xs ){
        LOG( LogLevel::LOG_VERBOSE, xs... );
    }

    //--------------------------------------------------------------------------
    /** Log a Debug message.
        @ingroup Util
     */
    template<typename... XS> inline void LOGD( XS... xs ){
        LOG( LogLevel::LOG_DEBUG, xs... );
    }
}

//------------------------------------------------------------------------------
#endif//LOG_H

//------------------------------------------------------------------------------
