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
/** @file scriptvalue.h
    @brief ScriptValue Declaration.
    @author Luis Cabellos
 */
//------------------------------------------------------------------------------
#ifndef SCRIPTVALUE_H
#define SCRIPTVALUE_H

//------------------------------------------------------------------------------
#include <string>

//------------------------------------------------------------------------------
namespace Util {
    //--------------------------------------------------------------------------
    /** Class to save variable values from script code (e.g: Lua).
        @ingroup Util
     */
    class ScriptValue{
    public:
        /// Type of a ScriptValue variable
        enum class ValueType : char { NIL, BOOLEAN, NUMBER, STRING };

        /// Construct an empty value. Defaults to numeric 0.
        ScriptValue();
        /// Copy Constructor.
        ScriptValue( const ScriptValue & val );
        /// Move Constructor.
        ScriptValue( ScriptValue && val ) noexcept;
        /// Copy assign.
        ScriptValue & operator=( const ScriptValue & val );
        /// Move assign.
        ScriptValue & operator=( ScriptValue && val ) noexcept;
        ~ScriptValue();

        /// Construct a boolean value.
        explicit ScriptValue( const bool val );
        /// Construct a numeric value.
        explicit ScriptValue( const double val );
        /// Construct a string value.
        explicit ScriptValue( const char * val );
        /// Construct a string value.
        explicit ScriptValue( const std::string & val );

        /// Get the Value Type of this instance
        ValueType getType() const;

        /** Returns the boolean value of this variable.
            @param d default value to return
            @returns the value, or default if this variable is not boolean type.
         */
        bool getBoolean( const bool d = false ) const;
        /** Returns the numeric value of this variable.
            @param d default value to return
            @returns the value, or default if this variable is not numeric type.
         */
        double getNumber( const double d = 0 ) const;
        /** Returns the string value of this variable.
            @param d default value to return
            @returns the value, or default if this variable is not string type.
         */
        std::string getString( const std::string & d ="" ) const;

    private:
        /// Erase old value data.
        void erase();

        /// Value of a script variable.
        union Value{
            /// value for numeric variables.
            double valn;
            /// value for boolean variables.
            bool valb;
            /// value for string variables.
            std::string * vals;

            Value();
            ~Value();
        };

        /// value if this instance.
        Value m_val;
        /// type of the value in this instance.
        ValueType m_type;
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
        if( val ){
            m_val.vals = new std::string(val);
            m_type = ValueType::STRING;
        }else{
            m_type = ValueType::NIL;
        }
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

    //--------------------------------------------------------------------------
    std::ostream & operator<<( std::ostream &out, const ScriptValue &val );

}//namespace Util

//------------------------------------------------------------------------------
#endif//SCRIPTVALUE_H

//------------------------------------------------------------------------------
