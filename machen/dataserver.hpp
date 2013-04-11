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
/** @file dataserver.hpp
    @brief Engine::DataServer Declaration.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#ifndef DATASERVER_H
#define DATASERVER_H

//------------------------------------------------------------------------------
#include <memory>
#include "raster.hpp"

//------------------------------------------------------------------------------
namespace Engine {
    //--------------------------------------------------------------------------
    /** Class that controls the simulation global data.
        @ingroup Engine
     */
    class DataServer{
    public:
        /// Returns the singleton instance of a DataServer.
        static std::shared_ptr<DataServer> instance();

        /// End the Data Server.
        virtual void end()=0;
        /** Returns a Raster object.
            @param key name of the raster
            @returns a pointer to the raster
         */
        virtual std::shared_ptr<Util::Raster> getRaster( const std::string & key ) const =0;

    private:
        /// singleton instance of a DataServer.
        static std::shared_ptr<DataServer> s_singleton;
    };

}//namespace Engine

//------------------------------------------------------------------------------
#endif//DATASERVER_H

//------------------------------------------------------------------------------
