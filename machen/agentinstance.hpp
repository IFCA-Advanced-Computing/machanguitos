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
/** @file agentinstance.hpp
    @brief Agent::AgentInstance Declaration.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#ifndef AGENTINSTANCE_HPP
#define AGENTINSTANCE_HPP

//------------------------------------------------------------------------------
#include <string>
#include <map>
#include "common/scriptvalue.hpp"
#include "common/agentid.hpp"

//------------------------------------------------------------------------------
struct lua_State;
namespace Data{
    class Raster;
}

//------------------------------------------------------------------------------
namespace Agent{
    //--------------------------------------------------------------------------
    /** private name of AgentInstance objects in Lua tables.
        @ingroup Agent
     */
    constexpr const char * SCRIPT_GLOBAL_AGENT_OBJ = "__go";

    //--------------------------------------------------------------------------
    class AgentClass;

    class RasterLoc{
    public:
        int layer;
        int i;
        int j;

        bool operator <(const RasterLoc & b) const;
    };

    inline bool RasterLoc::operator <( const RasterLoc & b ) const{
        return (layer < b.layer) or (i < b.i) or (j < b.j);
    }

    //--------------------------------------------------------------------------
    /** Instance of an AgentClass.
        @ingroup Agent
     */
    class AgentInstance {
    public:
        /** AgentInstance constructor. It should receive a AgentClass instance.
            @param c AgentClass instance of the Agent.
            @param id AgentInstance identifier.
         */
        AgentInstance( AgentClass * c, AgentId && id );
        /// initialize the Agent calling its 'init' function on Lua.
        void init();
        /** Update the Agent calling its 'update' fuction on Lua.
            @param delta time increment in seconds between updates.
         */
        void update( const double delta );
        /** Output the variables.
           @param t is the simulation time.
         */
        void outVars( const double t ) const;
        /** Put the Agent variable on Lua stack.
            @param L Lua State.
            @param key name of the variable to get.
         */
        int pushData( lua_State * L, const std::string & key ) const;
        /** Save a Lua value in an Agent variable.
            @param L Lua State.
            @param key name of the variable to save.
         */
        void newData( lua_State * L, const std::string & key );
        double getRasterValue( Data::Raster & raster, int layer, double x, double y );
        bool setRasterValue( Data::Raster & raster, int layer, double x, double y, double v );
    private:
        void insertCache( std::string id, int l, int i, int j, double v );
        std::tuple<bool,double> findCache( std::string id, int l, int i, int j );
       /// Agent::AgentInstance internal state.
        enum class AgentState{ AS_NORMAL, AS_INIT, AS_UPDATE };

        /// AgentClass instance of this Agent.
        AgentClass * m_class;
        /// list of variables of the Agent.
        std::map<std::string, Util::ScriptValue> m_vals;
        /// identifier of instance.
        AgentId m_id;
        /// AgentInstante run state.
        AgentState m_state {AgentState::AS_NORMAL};
        /// cache variables of rasters.
        std::map< std::string, std::map< RasterLoc, double> > m_cache;
    };

}

//------------------------------------------------------------------------------
#endif//AGENTINSTANCE_HPP

//------------------------------------------------------------------------------
