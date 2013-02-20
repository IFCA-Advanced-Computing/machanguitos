//------------------------------------------------------------------------------
#include "agentfactory.h"

#include <cassert>

#include <boost/filesystem.hpp>

//------------------------------------------------------------------------------
namespace Agent{
    using namespace boost::filesystem;

    //--------------------------------------------------------------------------
    AgentClass * AgentFactory::createClass( const std::string & name ){
        return nullptr;
    }

    //--------------------------------------------------------------------------
    AgentClass * AgentFactory::getClass( const std::string & name ) const{
        return nullptr;
    }

    //--------------------------------------------------------------------------
    void AgentFactory::setDatadir( const std::string & dir ){
        assert( is_directory(dir) && "not directory" );
        m_dir = dir;        
    }
}

//------------------------------------------------------------------------------
