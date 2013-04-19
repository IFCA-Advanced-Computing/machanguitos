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
/** @file mpiworker.hpp
    @brief Engine::MPIWorker Declarations
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#ifndef MPIWORKER_HPP
#define MPIWORKER_HPP

namespace Engine {

    //--------------------------------------------------------------------------
    /** Define a MPI Worker main loop to run in a MPI program.
       @ingroup Engine
     */
    class MPIWorker {
    public:
        MPIWorker();
        virtual ~MPIWorker();
    };

    //--------------------------------------------------------------------------
    inline MPIWorker::~MPIWorker(){
        // empty
    }

}//namespace Engine

//------------------------------------------------------------------------------
#endif//MPIWORKER_HPP

//------------------------------------------------------------------------------
