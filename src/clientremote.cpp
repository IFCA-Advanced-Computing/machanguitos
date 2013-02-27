//------------------------------------------------------------------------------
#include "clientremote.h"

#include <iostream>

//------------------------------------------------------------------------------
namespace Engine{
    //--------------------------------------------------------------------------
    ClientRemote::ClientRemote(){
        // empty
    }

    //--------------------------------------------------------------------------
    ClientRemote::~ClientRemote(){
        // empty
    }

    //--------------------------------------------------------------------------
    bool ClientRemote::createClass( const std::string & name ){
        std::cout << " remote create class " << name << std::endl;
        return true;
    }

    //--------------------------------------------------------------------------
    void ClientRemote::createAgents( const std::string & name, int n ){
        std::cout << " remote create " << n << " of " << name << std::endl;
    }

    //--------------------------------------------------------------------------
    void ClientRemote::runAgents( const double delta ){
        std::cout << " remote run agents\n";
    }

    
}

//------------------------------------------------------------------------------
