#include <string>
#include <ogre.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char **argv)
#endif
{
	Ogre::Root * root = new Ogre::Root("./plugins.cfg", "../configuration/zombie.cfg", "./logs/zombie.log");

	if(!root->restoreConfig() && !root->showConfigDialog()){ return EXIT_FAILURE; }



	delete root;

	return EXIT_SUCCESS;
}

