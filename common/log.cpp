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
/** @file log.cpp
    @brief Log utils definitions.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#include "log.hpp"
#include <ctime>
#include <map>
#include <algorithm>
#include <cctype>
#include <iomanip>

//------------------------------------------------------------------------------
namespace Util{
    //--------------------------------------------------------------------------
    using namespace std;

    //--------------------------------------------------------------------------
    /** Current Log Level.
        @ingroup Util
     */
    LogLevel s_currentLevel = LogLevel::LOG_DEBUG;
    /** Log Level tags for output messages.
        @ingroup Util
     */
    const char * s_tags[] {"[E] ", "[W] ", "[I] ", "[V] ", "[D] "};

    //--------------------------------------------------------------------------
    bool isEnabled( LogLevel ll ){
        return (ll <= s_currentLevel);
    }

    //--------------------------------------------------------------------------
    void printLogStamp( stringstream & buf, LogLevel ll ){
        time_t rawtime;
        tm * timeinfo;

        time( &rawtime );
        timeinfo = localtime( &rawtime );

        buf << s_tags[static_cast<unsigned char>(ll)];
        buf << setfill(' ') << setw(2) << timeinfo->tm_mday;
        buf << " " << setfill('0') << setw(2) << timeinfo->tm_hour
            << ":" << setfill('0') << setw(2) << timeinfo->tm_min
            << ":" << setfill('0') << setw(2) << timeinfo->tm_sec
            << " - ";
    }

    //--------------------------------------------------------------------------
    LogLevel getLogLevel(){
        return s_currentLevel;
    }

    //--------------------------------------------------------------------------
    void setLogLevel( int ll ){
        auto minll = static_cast<decltype(ll)>( LogLevel::LOG_ERROR );
        auto maxll = static_cast<decltype(ll)>( LogLevel::LOG_DEBUG );
        if( ll >= minll and ll <= maxll ){
            s_currentLevel = static_cast<LogLevel>(ll);
        }else{
            LOGD( "Invalid Log Level ", ll );
        }
    }

    //--------------------------------------------------------------------------
    void setLogLevel( const string & str ){
        const static map<string, LogLevel> s_names {
            {"error", LogLevel::LOG_ERROR},
            {"warning", LogLevel::LOG_WARNING},
            {"info", LogLevel::LOG_INFO},
            {"verbose", LogLevel::LOG_VERBOSE},
            {"debug", LogLevel::LOG_DEBUG},
        };

        string low {str};
        transform( begin(str), end(str), begin(low), [](char x){ return tolower(x); } );
        const auto && got = s_names.find( low );
        if( got != s_names.cend() ){
            s_currentLevel = got->second;
        }
    }
}

//------------------------------------------------------------------------------
