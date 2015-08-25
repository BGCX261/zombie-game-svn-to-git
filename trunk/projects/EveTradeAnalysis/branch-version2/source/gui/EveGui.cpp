#include <sstream>
#include <fstream>
#include <boost/lexical_cast.hpp>

#include "AM_Eve_Jumps.hpp"
#include "AM_Eve_Parsers.hpp"
#include "AM_FileWatcher.hpp"
#include "AM_Eve_MarketAnalyser.hpp"
#include "AM_Eve_MarketReporter.hpp"

#include "EveGui.h"

namespace EveTradeAnalysis
{
	class EveGuiReporter : public AutoMatic::Eve::MarketReporter
	{
	public:
		EveGuiReporter(void)
		{
			
		}

		virtual void report(void)
		{
			_reported = true;
		}

		bool _reported;
	};

	EveGui::EveGui(void)
		: _fileWatcher(0), _reporter(0), _marketAnalyser(0), _jumps(0)
	{
		InitializeComponent();
		//
		//TODO: Add the constructor code here
		//
		_profits = new std::vector<AutoMatic::Eve::ProfitIndex>;
		_reporter = new EveGuiReporter();

		_marketAnalyser = new AutoMatic::Eve::MarketAnalyser;
		_reporter->setAnalyser(_marketAnalyser);	

	}

	EveGui::~EveGui()
	{
		if (components)
		{
			delete components;
		}

		timer_update->Enabled = false; 

		delete _profits;
		delete _jumps;
		delete _fileWatcher;
		delete _marketAnalyser;
		delete _reporter;
	}

	void EveGui::createJumps(void)
	{
		bool enableTimer = timer_update->Enabled;
		timer_update->Enabled = false;

		delete _jumps;
		_jumps = new AutoMatic::Eve::Jumps;
		_marketAnalyser->setJumps(_jumps);

		timer_update->Enabled = enableTimer;
	}

	std::string GetString(System::String^ string_)
	{
		std::string outputString;
		for (int i = 0; i < string_->Length; ++i)
		{
			outputString += string_[i];
		}

		return outputString;
	}

	System::String^ GetString(std::string const & string_)
	{
		System::String^ outputString = gcnew System::String("");
		for (std::string::const_iterator i = string_.begin(); i != string_.end(); ++i)
		{
			outputString = System::String::Concat(outputString, (wchar_t)*i);
		}

		return outputString;
	}

	template<class T>
	System::String^ GetString(T const & t_)
	{
		std::stringstream s;
		s.precision(2);
		s << std::fixed << t_;
		return GetString(s.str());
	}

	System::Void EveGui::timer_update_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		if(_fileWatcher)
		{
			_fileWatcher->Update();
			if(_reporter->_reported)
			{
				_reporter->_reported = false;

				for (std::list<AutoMatic::Eve::ProfitIndex>::iterator profit = _reporter->_profits.begin(); 
					profit != _reporter->_profits.end(); ++profit)
				{
					(*profit)._file = _reporter->_file;
					_profits->push_back(*profit);
					setProfitInfo(*profit);
					break;
				}
			}
		}
	}

	System::Void EveGui::info_directoryWatched_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
	{
		info_directoryWatched->Text = "";

		timer_update->Enabled = false;
		if(System::Windows::Forms::DialogResult::OK == openFolder->ShowDialog())
		{
			std::string path = GetString(openFolder->SelectedPath);
			delete _fileWatcher;
			
			_fileWatcher = new AutoMatic::FileWatcher(path);
			_fileWatcher->InstallChangedFileSetCallback(_reporter);
			_reporter->setPath(path);

			info_directoryWatched->Text = openFolder->SelectedPath;
		}
		timer_update->Enabled = true;
	}

	System::Void EveGui::info_systemsFile_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
	{
		info_systemsFile->Text = "";

		createJumps();

		timer_update->Enabled = false;
		if(System::Windows::Forms::DialogResult::OK == openFile->ShowDialog())
		{
			std::string filename = GetString(openFile->FileName);

			if(AutoMatic::Eve::LoadSystems(*_jumps, filename, true))
			{
				info_systemsFile->Text = openFile->FileName;
			}
		}
		timer_update->Enabled = true;
	}

	System::Void EveGui::info_jumpsFile_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
	{
		info_jumpsFile->Text = "";

		createJumps();

		timer_update->Enabled = false;
		if(System::Windows::Forms::DialogResult::OK == openFile->ShowDialog())
		{
			std::string filename = GetString(openFile->FileName);

			if(AutoMatic::Eve::LoadJumps(*_jumps, filename, true))
			{
				info_jumpsFile->Text = openFile->FileName;
			}
		}
		timer_update->Enabled = true;
	}

	System::Void EveGui::info_itemsFile_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
	{
		info_itemsFile->Text = "";

		timer_update->Enabled = false;
		if(System::Windows::Forms::DialogResult::OK == openFile->ShowDialog())
		{
			std::string filename = GetString(openFile->FileName);

			if(AutoMatic::Eve::LoadItemVolumes(*_marketAnalyser, filename))
			{
				info_itemsFile->Text = openFile->FileName;
			}
		}
		timer_update->Enabled = true;
	}
}
