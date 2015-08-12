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
#include <map>
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
        /** Returns a Raster object.
            @param key name of the raster
            @returns a pointer to the raster
         */
        std::shared_ptr<Data::Raster> getRaster( const std::string & key ) const;
        /** Create a new Raster object in the simulation.

            @param key name of the Raster object.
            @param l number of layers.
            @param w width in pixels.
            @param h height in pixels.
            @param x0 left value of the Raster area.
            @param x1 rigth value of the Raster area.
            @param y0 top value of the Raster area.
            @param y1 bottom value of the Raster area.
            @param d default value of layer.
            @param isFloat true if layer values are Float32, else they will be Bytes.
        */
        virtual void createRaster( const std::string & key, int l, int w, int h,
                                   double x0, double x1, double y0, double y1,
                                   double d, bool isFloat )=0;
        /** Load a Raster object from a file.

            @param key name of the Raster object.
            @param filename filename.
            @param x0 left value of the Raster area.
            @param x1 rigth value of the Raster area.
            @param y0 top value of the Raster area.
            @param y1 bottom value of the Raster area.
        */
        virtual void loadRaster( const std::string & key,
                                 const std::string & filename,
                                 double x0, double x1, double y0, double y1 )=0;
        /** Create a Raster Proxy for a remote Raster object.

            @param key name of the Raster object.
            @param l number of layers.
            @param w width in pixels.
            @param h height in pixels.
            @param x0 left value of the Raster area.
            @param x1 rigth value of the Raster area.
            @param y0 top value of the Raster area.
            @param y1 bottom value of the Raster area.
            @param d default value of layer.
            @param isFloat true if layer values are Float32, else they will be Bytes.
        */
        virtual void createRasterProxy( const std::string & key, int l, int w, int h,
                                        double x0, double x1, double y0, double y1,
                                        double d, bool isFloat )=0;
        /** Create a Raster Proxy for a remote Raster file.

            @param key name of the Raster object.
            @param filename filename.
            @param x0 left value of the Raster area.
            @param x1 rigth value of the Raster area.
            @param y0 top value of the Raster area.
            @param y1 bottom value of the Raster area.
        */
        virtual void createRasterProxy( const std::string & key,
                                        const std::string & filename,
                                        double x0, double x1,
                                        double y0, double y1 )=0;
        /** Set a Lua update source for a Raster object.

            The Lua file should implement a 'Raster:update' function like:

            @include fireupdate.lua

            @param key name of the Raster object.
            @param filename lua filename.
         */
        virtual void setRasterUpdate( const std::string & key,
                                      const std::string & filename )=0;
        /** Call update layers functions for current step.
            @param delta increment of current simulation step.
        */
        virtual void updateLayers( const double delta )=0;
    protected:
        /// List of Raster object.
        std::map<std::string,std::shared_ptr<Data::Raster>> m_rasters;

    private:
        /// singleton instance of a DataServer.
        static std::shared_ptr<DataServer> s_singleton;
    };

}//namespace Engine

//------------------------------------------------------------------------------
#endif//DATASERVER_H

//------------------------------------------------------------------------------
