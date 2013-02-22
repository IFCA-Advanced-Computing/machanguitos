//------------------------------------------------------------------------------
#ifndef SCRIPTVALUE_H
#define SCRIPTVALUE_H

namespace Util {
    //--------------------------------------------------------------------------
    class ScriptValue{
    public:
        enum class ValueType : char { BOOLEAN, NUMBER };

        ScriptValue();
        explicit ScriptValue( const bool val );
        explicit ScriptValue( const double val );

        ValueType getType() const;

        bool getBoolean( const bool d = false ) const;
        double getNumber( const double d = 0 ) const;

    private:
        union{
            double number;
            bool tval;
        } m_val;

        ValueType m_type;
    };

    //--------------------------------------------------------------------------
    inline ScriptValue::ScriptValue( const double val ){
        m_val.number = val;
        m_type = ValueType::NUMBER;
    }
    
    //--------------------------------------------------------------------------
    inline ScriptValue::ScriptValue( const bool val ){
        m_val.tval = val;
        m_type = ValueType::BOOLEAN;
    }
    
    //--------------------------------------------------------------------------
    inline ScriptValue::ValueType ScriptValue::getType() const{
        return m_type;
    }

    //--------------------------------------------------------------------------
    inline bool ScriptValue::getBoolean( const bool d ) const{
        if( m_type == ValueType::BOOLEAN ){
            return m_val.tval;
        }else{
            return d;
        }
    }

    //--------------------------------------------------------------------------
    inline double ScriptValue::getNumber( const double d ) const{
        if( m_type == ValueType::NUMBER ){
            return m_val.number;
        }else{
            return d;
        }
    }

}//namespace Util

//------------------------------------------------------------------------------
#endif//SCRIPTVALUE_H

//------------------------------------------------------------------------------
