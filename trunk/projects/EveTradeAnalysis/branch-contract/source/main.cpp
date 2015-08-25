#pragma warning( once : 4561 )

#include <list>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <conio.h>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

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

	/*
	case 'c': // convert.
	case 'C':
	{
	std::string inFilename, outFilename;
	std::cout << "\nEnter Database Jumps file: ";
	std::cin >> inFilename;
	std::cout << "\nEnter CSV Jumps file: ";
	std::cin >> outFilename;
	std::cout << "Converting... ";
	if(!AutoMatic::Eve::ConvertDBJumpsToCSV(inFilename, outFilename))
	{
	std::cout << "failed." << std::endl;
	}
	else
	{
	std::cout << "done." << std::endl;
	}
	}
	break;
	*/
}
