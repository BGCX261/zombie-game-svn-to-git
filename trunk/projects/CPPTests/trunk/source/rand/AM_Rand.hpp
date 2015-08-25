#ifndef _AM_Rand_hpp_
#define _AM_Rand_hpp_

namespace AutoMatic
{
	/// Test srand() and rand() functions.
	/** This test will ensure that calling srand() with the same seed will result
		* in the same set of random values, for at least 5 consecutive calls to rand(),
		* for at least 11 different seeds (0..10).
	*/
	void RunRandomSeedAndNumberGenerationTest(void);
}

#endif //_AM_Rand_hpp_