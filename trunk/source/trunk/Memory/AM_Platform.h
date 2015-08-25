#ifdef AUTOMATIC_PLATFORM

#ifndef _AutoMatic_Platform_h_
#define _AutoMatic_Platform_h_

namespace AutoMatic
{
	#define AutoMatic_Platform_WIN32 1
	#define AutoMatic_Platform_LINUX 2
	#define AutoMatic_Platform_APPLE 3

	#if defined( __WIN32__ ) || defined( _WIN32 )
		# define AutoMatic_Platform AutoMatic_Platform_WIN32
	#elif defined( __APPLE_CC__)
		# define AutoMatic_Platform AutoMatic_Platform_APPLE
	#else
		# define AutoMatic_Platform AutoMatic_Platform_LINUX
	#endif

	// Windows Settings
	#if AutoMatic_Platform == AutoMatic_Platform_WIN32

		#ifdef _DEBUG
			# define AutoMatic_Debug_Mode 1
		#else
			# define AutoMatic_Debug_Mode 0
		#endif

		#define snprintf _snprintf
		#define vsnprintf _vsnprintf
		// #define unlink _unlink

	#elif AutoMatic_Platform == AutoMatic_Platform_LINUX || AutoMatic_Platform == AutoMatic_Platform_APPLE

		#ifdef DEBUG
			# define AutoMatic_Debug_Mode 1
		#else
			# define AutoMatic_Debug_Mode 0
		#endif

	#endif

}

#endif //_AutoMatic_Platform_h_

#endif //AUTOMATIC_PLATFORM
