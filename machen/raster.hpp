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
/** @file raster.hpp
    @brief Util::Raster Declaration.
    @author Luis Cabellos
*/
//------------------------------------------------------------------------------
#ifndef RASTER_H
#define RASTER_H

//------------------------------------------------------------------------------
#include <string>
#include <tuple>

//------------------------------------------------------------------------------
namespace Data {
    /// Type of Raster to create.
    enum class RasterNewType : unsigned char {
        /// On memory Raster.
        RNT_EMPTY,
        /// Raster from file.
            RNT_FILE,
    };

    //--------------------------------------------------------------------------
    /** Data for used when create a New Raster.
        @ingroup Data
     */
    class RasterNewData{
    public:
        /** RasterNewData constructor when created in memory.
            @param key name of the Raster object.
            @param layers number of layers.
            @param w width in pixels.
            @param h height in pixels.
            @param x0 left value of the Raster area.
            @param x1 rigth value of the Raster area.
            @param y0 top value of the Raster area.
            @param y1 bottom value of the Raster area.
            @param d default value of layer.
            @param isf true if layer values are Float32, else they will be Bytes.
         */
        explicit RasterNewData( const std::string & key, int layers, int w, int h,
                                double x0, double x1, double y0, double y1,
                                double d, bool isf );
        /** RasterNewData constructor when loaded from file.
            @param key name of the Raster object.
            @param file filename.
            @param x0 left value of the Raster area.
            @param x1 rigth value of the Raster area.
            @param y0 top value of the Raster area.
            @param y1 bottom value of the Raster area.
         */
        explicit RasterNewData( const std::string & key, const std::string & file,
                       double x0, double x1, double y0, double y1 );

        /// type of new raster data.
        RasterNewType rasterType;
        /// name of the raster.
        std::string key;
        /// filename of the raster.
        std::string filename;
        /// updatescript
        std::string updatescript;
        /// number of layers
        int layers;
        /// width of the raster.
        int w;
        /// height of the raster.
        int h;
        /// left value of the raster.
        double x0;
        /// right value of the raster.
        double x1;
        /// top value of the raster.
        double y0;
        /// bottom value of the raster.
        double y1;
        /// default value of the raster.
        double d;
        /// is float datatype
        bool isFloat;
    };

    //--------------------------------------------------------------------------
    inline RasterNewData::RasterNewData( const std::string & key, int l,
                                         int w, int h,
                                         double x0, double x1, double y0, double y1,
                                         double d, bool isf  )
        : rasterType{RasterNewType::RNT_EMPTY}, key{ key }, layers{l}, w{ w }, h{ h },
        x0{ x0 }, x1{ x1 }, y0{ y0 }, y1{ y1 }, d{ d }, isFloat{ isf }
    {
        // empty
    }

    //--------------------------------------------------------------------------
    inline RasterNewData::RasterNewData( const std::string & key, const std::string & file,
                                         double x0, double x1, double y0, double y1 )
        : rasterType{RasterNewType::RNT_FILE}, key{ key }, filename{ file }, layers{1},
        w{ 1 }, h{ 1 }, x0{ x0 }, x1{ x1 }, y0{ y0 }, y1{ y1 }, d{ 0 }, isFloat{ false }
    {
        // empty
    }

    //--------------------------------------------------------------------------
    /** Raster Layer Data
        @ingroup Data
     */
    class Raster {
    public:
        /** Raster constructor.
            @param key name of the Raster object.
            @param layers number of layers.
            @param w width in pixels.
            @param h height in pixels.
            @param x0 left value of the Raster area.
            @param x1 rigth value of the Raster area.
            @param y0 top value of the Raster area.
            @param y1 bottom value of the Raster area.
            @param d default value of layer.
            @param isFloat true if layer values are Float32, else they will be Bytes.
         */
        Raster( const std::string & key, int layers, int w, int h,
                double x0, double x1, double y0, double y1,
                double d, bool isFloat );
        /// Desctructor.
        virtual ~Raster();

        /** Check if a position is inside de raster area.
            @param x horizontal position.
            @param y vertical position.
            @retval true if is a valid position.
         */
        bool validPosition( double x, double y );
        /** Get the real pixel of a position in the raster area.
            @param x horizontal position.
            @param y vertical position.
            @returns the pixel position.
        */
        std::tuple<int,int> getPosition( double x, double y );

        /** Raster getter from an area position.
            @param layer layer to use.
            @param x horizontal position.
            @param y vertical position.
            @returns the value on the position.
         */
        virtual double getValue( int layer, double x, double y )=0;
        /** Raster setter from an area position.
            @param layer layer to use.
            @param x horizontal position.
            @param y vertical position.
            @param val new value.
         */
        virtual void setValue( int layer, double x, double y, double val )=0;
        /** Increment a raster value.
            @param layer layer to use.
            @param x horizontal position.
            @param y vertical position.
            @param val increment ammount.
         */
        virtual void incrementValue( int layer, double x, double y, double val )=0;
        /** Save raster data in a file.
            @param filename filename.
         */
        virtual void save( const std::string & filename )=0;
        /** Set a Lua update source for a Raster object.
            @param filename lua filename.
         */
        virtual void setRasterUpdate( const std::string & filename )=0;
        /** Call update raster function for current step.
            @param delta increment of current simulation step.
        */
        virtual void update( const double delta )=0;
        /** Raster getter from a pixel position.
            @param layer layer to use.
            @param i horizontal position.
            @param j vertical position.
            @returns the value of the pixel.
         */
        virtual double getPixelValue( int layer, int i, int j )=0;
        /** Raster setter from a pixel position.
            @param layer layer to use.
            @param i horizontal position.
            @param j vertical position.
            @param val new value.
         */
        virtual void setPixelValue( int layer, int i, int j, double val )=0;
        /** Load new data from a new file.
            @param filename filename of new raster image.
         */
        virtual void loadImage( const std::string & filename )=0;

        /// name of the raster.
        std::string key;

    protected:
        /// number of layers.
        int m_layers;
        /// width of the raster.
        int m_w;
        /// height of the raster.
        int m_h;
        /// left value of the raster.
        double m_x0;
        /// right value of the raster.
        double m_x1;
        /// top value of the raster.
        double m_y0;
        /// bottom value of the raster.
        double m_y1;
        /// horizontal increment.
        double m_hx;
        /// vertical increment.
        double m_hy;
        /// default value of pixels.
        double m_d;
        /// true if pixels type is Float32 else type is Byte8.
        bool m_isFloat;
    };
}//namespace Data


//------------------------------------------------------------------------------
#endif//RASTER_H

//------------------------------------------------------------------------------
