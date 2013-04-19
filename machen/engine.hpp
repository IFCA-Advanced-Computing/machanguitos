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
/** @file engine.hpp
    @brief Engine common functions.
    @author Luis Cabellos
*/

//------------------------------------------------------------------------------
#ifndef ENGINE_HPP
#define ENGINE_HPP

//------------------------------------------------------------------------------
namespace Engine {

    /** initialize engine.
        @param argc the number of arguments.
        @param argv Argument vector.
        @ingroup Engine
     */
    void initialize( int argc, char * argv[] );
    /** Ends the application.
        @returns Error Exit Code.
        @ingroup Engine
     */
    int abort();
    /** Blocks until all clients have call this function.
        @ingroup Engine
     */
    void clientsBarrier();
    /** Terminates engine execution environment.
        @ingroup Engine
     */
    void end();
    /** Check if current process is the main server.
        @retval true if current process is the main server.
        @ingroup Engine
     */
    bool isServer();
    /** Check if current process is the data server.
        @retval true if current process is the data server.
        @ingroup Engine
     */
    bool isDataServer();
    /** Check if current process is single process.
        @retval true if current process is single.
        @ingroup Engine
     */
    bool isSingleProcess();

}//namespace Engine

//------------------------------------------------------------------------------
#endif//ENGINE_HPP

//------------------------------------------------------------------------------
