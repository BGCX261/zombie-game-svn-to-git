#include "AM_Eve_Jumps.hpp"
#include "AM_Eve_Parsers.hpp"
#include "AM_FileWatcher.hpp"
#include "AM_Eve_MarketOrder.hpp"
#include "AM_Eve_MarketReporter.hpp"

#include "gui/EveGui.h"

[STAThreadAttribute]
int __stdcall WinMain(HINSTANCE hInstance_, HINSTANCE hPrevInstance_, LPSTR cmdLine_, int nShowCmd_)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	EveTradeAnalysis::EveGui^ eveGui = gcnew EveTradeAnalysis::EveGui();

	// Create the main window and run it
	Application::Run(eveGui);
}
