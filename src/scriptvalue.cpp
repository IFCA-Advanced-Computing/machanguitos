//------------------------------------------------------------------------------
#include "scriptvalue.h"

#include <cassert>

//------------------------------------------------------------------------------
namespace Util {
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
            m_val.vals = new std::string(*val.m_val.vals);
            break;
        }
    }

    //--------------------------------------------------------------------------
    ScriptValue::ScriptValue( ScriptValue && val ) noexcept : m_type{val.m_type} {
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
            m_val.vals = std::move(val.m_val.vals);
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
            m_val.vals = new std::string(*val.m_val.vals);
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
            m_val.vals = std::move(val.m_val.vals);
            val.m_val.vals = nullptr;
            break;
        }
        return (*this);
    }

}//namespace Util

//------------------------------------------------------------------------------
