#pragma warning( disable : 4311 4312 4244 )

#include <gm\gmThread.h>
#include <gm\gmMachine.h>

#include "AM_Object.hpp"

#include "AM_GameMonkey.hpp"

namespace AutoMatic
{
	GameMonkey::GameMonkey(void)
		: _gmMachine(0)
	{
		_gmMachine = new gmMachine;
	}

	GameMonkey::~GameMonkey(void)
	{
		delete _gmMachine;
	}

//****************************************************************************//

	GameMonkeyObject::GameMonkeyObject(Object * object_, GameMonkey * gameMonkey_)
		: _gameMonkey(gameMonkey_), _object(object_)
	{
		_gmTableObject = _gameMonkey->getMachine()->AllocTableObject();
	}

	GameMonkeyObject::~GameMonkeyObject(void)
	{
		_gmTableObject->Destruct(_gameMonkey->getMachine());
	}

	int GM_CDECL GameMonkeyObject::SetState(gmThread * a_thread)
	{
		GM_CHECK_NUM_PARAMS(2);
		if(GM_STRING != a_thread->ParamType(0)){ return GM_EXCEPTION; }
		
		switch( a_thread->ParamType(1) )
		{
		case GM_INT:
			{
				a_thread->Param(1).m_value.m_int;
			}
			break;
		case GM_FLOAT:
			{
				a_thread->Param(1).m_value.m_float;
			}
			break;
		case GM_STRING:
			{
				//a_thread->Param(1).m_value.m_string;
			}
			break;
		}

		return GM_OK;
	}

	int GM_CDECL SquareRoot(gmThread * a_thread)
	{
		float fValue;
		GM_CHECK_NUM_PARAMS(1);
		if(a_thread->ParamType(0) == GM_INT)
		{
			fValue = (float)a_thread->Param(0).m_value.m_int;
		}
		else if(a_thread->ParamType(0) == GM_FLOAT)
		{
			fValue = a_thread->Param(0).m_value.m_float;
		}
		else
		{
			return GM_EXCEPTION;
		}
		a_thread->PushFloat(sqrtf(fValue));
		return GM_OK;
	}
}
