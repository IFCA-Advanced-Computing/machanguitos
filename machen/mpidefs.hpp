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
/** @file mpidefs.hpp
    @brief Declaration of MPI definitions and utils.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#ifndef MPIDEFS_HPP
#define MPIDEFS_HPP

//------------------------------------------------------------------------------
#include "common/log.hpp"

//------------------------------------------------------------------------------
namespace Engine{
    /** MPI message Tags.
        @ingroup Engine
     */
    enum MpiTag {
        /// send a new log level Tag.
        SETLOGLEVEL = 1,
        /// End Simulation Message Tag.
        END = 20,
    };

    /** MPI message Tags for Client Server.
        @ingroup Engine
     */
    enum MpiTagCS {
        /// Create Class Message Tag.
        CREATECLASS = 21,
        /// Create Agents Message Tag.
        CREATEAGENTS = 22,
        /// Run Agents Message Tag.
        RUNAGENTS = 23,
        /// Set Start Time Message Tag.
        SETSTARTTIME = 24,
        /// Set Data Store info.
        SETDATASTORE = 25,
        /// Set Data Path Tag.
        SETDATAPATH = 26,
        /// create a Raster Layer Tag.
        CREATERASTERCLIENT = 27,
    };

    /** MPI message Tags for Data Server.
        @ingroup Engine
     */
    enum MpiTagDS {
        /// create a Raster Layer Tag.
        CREATERASTER = 41,
    };

    /// MPI Rank identifier of server.
    constexpr int SERVER_RANK = 0;
    /// MPI Rank identifier of data server.
    constexpr int DATASERVER_RANK = 1;

    /** Set the log level of all clients.
        @param ll new Log Level
     */
    void setRemoteLogLevel( Util::LogLevel ll );
    /// End the Remote Servers.
    void remoteEnd();
}

//------------------------------------------------------------------------------
#endif//MPIDEFS_HPP

//------------------------------------------------------------------------------
