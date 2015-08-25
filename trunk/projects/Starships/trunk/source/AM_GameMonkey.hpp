#ifndef _AM_GameMonkey_hpp_
#define _AM_GameMonkey_hpp_

class gmThread;
class gmMachine;
class gmTableObject;

namespace AutoMatic
{
	class Object;

	class GameMonkey
	{
	public:
		GameMonkey(void);
		~GameMonkey(void);

		gmMachine * getMachine(void) { return _gmMachine; }

	private:
		gmMachine * _gmMachine;
	};

//****************************************************************************//

	class GameMonkeyObject
	{
	public:
		GameMonkeyObject(Object * object_, GameMonkey * gameMonkey_);
		~GameMonkeyObject(void);

	private:
		static int GM_CDECL SetState(gmThread * _gmThread);

		GameMonkey * _gameMonkey;
		gmTableObject * _gmTableObject;

		Object * _object;
	};
}

#endif //_AM_GameMonkey_hpp_