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
/** @file scriptvalue.cpp
    @brief Util::ScriptValue Definition.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#include "scriptvalue.hpp"

#include <cassert>
#include <ostream>

//------------------------------------------------------------------------------
namespace Util {
    using namespace std;

    //--------------------------------------------------------------------------
    ScriptValue::ScriptValue() : m_type(ValueType::NIL) {
        // empty
    }

    //--------------------------------------------------------------------------
    ScriptValue::~ScriptValue() {
        erase();
    }

    //--------------------------------------------------------------------------
    void ScriptValue::erase(){
        if( (m_type == ValueType::STRING) and (m_val.vals != nullptr) ){
            delete m_val.vals;
            m_val.vals = nullptr;
        }
    }

    //--------------------------------------------------------------------------
    ScriptValue::ScriptValue( const ScriptValue & val ) : m_type{val.m_type} {
        switch( m_type ){
        case ValueType::NIL: // empty
            break;
        case ValueType::BOOLEAN:
            m_val.valb = val.m_val.valb;
            break;
        case ValueType::NUMBER:
            m_val.valn = val.m_val.valn;
            break;
        case ValueType::STRING:
            assert( val.m_val.vals && "Error null string" );
            m_val.vals = new string(*val.m_val.vals);
            break;
        }
    }

    //--------------------------------------------------------------------------
    ScriptValue::ScriptValue( ScriptValue && val ) noexcept : m_type{move(val.m_type)} {
        switch( m_type ){
        case ValueType::NIL: // empty
            break;
        case ValueType::BOOLEAN:
            m_val.valb = val.m_val.valb;
            break;
        case ValueType::NUMBER:
            m_val.valn = val.m_val.valn;
            break;
        case ValueType::STRING:
            m_val.vals = move(val.m_val.vals);
            val.m_val.vals = nullptr;
            break;
        }
    }

    //--------------------------------------------------------------------------
    ScriptValue & ScriptValue::operator=( const ScriptValue & val ){
        erase();
        m_type = val.m_type;
        switch( m_type ){
        case ValueType::NIL: // empty
            break;
        case ValueType::BOOLEAN:
            m_val.valb = val.m_val.valb;
            break;
        case ValueType::NUMBER:
            m_val.valn = val.m_val.valn;
            break;
        case ValueType::STRING:
            assert( val.m_val.vals && "Error null string" );
            m_val.vals = new string(*val.m_val.vals);
            break;
        }
        return (*this);
    }

    //--------------------------------------------------------------------------
    ScriptValue & ScriptValue::operator=( ScriptValue && val ) noexcept {
        erase();
        m_type = val.m_type;
        switch( m_type ){
        case ValueType::NIL: // empty
            break;
        case ValueType::BOOLEAN:
            m_val.valb = val.m_val.valb;
            break;
        case ValueType::NUMBER:
            m_val.valn = val.m_val.valn;
            break;
        case ValueType::STRING:
            m_val.vals = move(val.m_val.vals);
            val.m_val.vals = nullptr;
            break;
        }
        return (*this);
    }

    //--------------------------------------------------------------------------
    ostream & operator<<( ostream &out, const ScriptValue &val ){
        switch( val.getType() ){
        case ScriptValue::ValueType::NIL:
            out << "<NIL>";
            break;
        case ScriptValue::ValueType::BOOLEAN:
            out << val.getBoolean();
            break;
        case ScriptValue::ValueType::NUMBER:
            out << val.getNumber();
            break;
        case ScriptValue::ValueType::STRING:
            out << val.getString();
            break;
        }
        return out;
    }

}//namespace Util

//------------------------------------------------------------------------------
