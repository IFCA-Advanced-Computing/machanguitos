//------------------------------------------------------------------------------
#ifndef SCRIPTVALUE_H
#define SCRIPTVALUE_H

#include <iostream>
#include <string>

namespace Util {
    //--------------------------------------------------------------------------
    class ScriptValue{
    public:
        enum class ValueType : char { NIL, BOOLEAN, NUMBER, STRING };

        ScriptValue();
        ScriptValue( const ScriptValue & val );
        ScriptValue( ScriptValue && val );
        ScriptValue & operator=( const ScriptValue & val );
        ScriptValue & operator=( ScriptValue && val );
        ~ScriptValue();

        explicit ScriptValue( const bool val );
        explicit ScriptValue( const double val );
        explicit ScriptValue( const char * val );
        explicit ScriptValue( const std::string & val );

        ValueType getType() const;

        bool getBoolean( const bool d = false ) const;
        double getNumber( const double d = 0 ) const;
        std::string getString( const std::string & s ="" ) const;

    private:
        union Value{
            double valn;
            bool valb;
            std::string * vals;

            Value();
            ~Value();
        };

        Value m_val;
        ValueType m_type;

        void erase();
    };

    //--------------------------------------------------------------------------
    inline ScriptValue::ScriptValue( const double val ){
        m_val.valn = val;
        m_type = ValueType::NUMBER;
    }

    //--------------------------------------------------------------------------
    inline ScriptValue::ScriptValue( const bool val ){
        m_val.valb = val;
        m_type = ValueType::BOOLEAN;
    }

    //--------------------------------------------------------------------------
    inline ScriptValue::ScriptValue( const char * val ){
        m_val.vals = new std::string(val);
        m_type = ValueType::STRING;
    }

    //--------------------------------------------------------------------------
    inline ScriptValue::ScriptValue( const std::string & val ){
        m_val.vals = new std::string(val);
        m_type = ValueType::STRING;
    }

    //--------------------------------------------------------------------------
    inline ScriptValue::ValueType ScriptValue::getType() const{
        return m_type;
    }

    //--------------------------------------------------------------------------
    inline bool ScriptValue::getBoolean( const bool d ) const{
        return ( m_type == ValueType::BOOLEAN ) ? m_val.valb : d;
    }

    //--------------------------------------------------------------------------
    inline double ScriptValue::getNumber( const double d ) const{
        return ( m_type == ValueType::NUMBER ) ? m_val.valn : d;
    }

    //--------------------------------------------------------------------------
    inline std::string ScriptValue::getString( const std::string & d ) const{
        return ( m_type == ValueType::STRING ) ? (*m_val.vals) : d;
    }

    //--------------------------------------------------------------------------
    inline ScriptValue::Value::Value(){
        this->vals = nullptr;
    }

    //--------------------------------------------------------------------------
    inline ScriptValue::Value::~Value(){
        // empty
    }

}//namespace Util

//------------------------------------------------------------------------------
#endif//SCRIPTVALUE_H

//------------------------------------------------------------------------------
