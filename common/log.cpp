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
    const char * s_tags[] {" [E] ", " [W] ", " [I] ", " [V] ", " [D] "};

    //--------------------------------------------------------------------------
    bool isEnabled( LogLevel ll ){
        return (ll <= s_currentLevel);
    }

    //--------------------------------------------------------------------------
    void printLogStamp( LogLevel ll ){
        time_t rawtime;
        tm * timeinfo;

        time( &rawtime );
        timeinfo = localtime( &rawtime );
        
        cout << timeinfo->tm_mday << " " << timeinfo->tm_hour 
             << ":" << timeinfo->tm_min << ":" << timeinfo->tm_sec;

        cout << s_tags[static_cast<unsigned char>(ll)];
    }
}

//------------------------------------------------------------------------------
