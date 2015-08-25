#pragma warning( once : 4561 )


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

	System::Void EveGui::check_ignoreJumpToStart_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		if(_marketAnalyser)
		{
			_marketAnalyser->ignoreJumpsToStart(check_ignoreJumpToStart->Checked);
		}
	}

	System::Void EveGui::check_ignoreJumpsPerTrip_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		if(_marketAnalyser)
		{
			_marketAnalyser->ignoreJumpsPerTrip(check_ignoreJumpToStart->Checked);
		}
	}

	System::Void EveGui::check_ignoreSellOrderVolume_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		if(_marketAnalyser)
		{
			numeric_ignoreSellOrderVolumes->Enabled = check_ignoreSellOrderVolume->Checked;	
			if(!numeric_ignoreSellOrderVolumes->Enabled){ _marketAnalyser->ignoreSellOrderVolumeAbove(-1); }
			else { _marketAnalyser->ignoreSellOrderVolumeAbove(System::Decimal::ToInt16(numeric_ignoreSellOrderVolumes->Value)); }
		}
	}

	//System::Void EveGui::check_maxJumpsToStart_CheckedChanged(System::Object^ sender, System::EventArgs^ e) 
	//{
	//	if(_marketAnalyser)
	//	{
	//		numeric_maxJumpsToStart->Enabled = check_maxJumpsToStart->Checked;	
	//		if(!numeric_maxJumpsToStart->Enabled){ _marketAnalyser->setMaxJumpsToStart(AutoMatic::Eve::MarketAnalyser::DEFAULT_MAX_JUMPS_TO_SELL_ORDER); }
	//		else { _marketAnalyser->setMaxJumpsToStart(System::Decimal::ToInt16(numeric_maxJumpsToStart->Value)); }
	//	}
	//}

	//System::Void EveGui::check_maxJumpsPerTip_CheckedChanged(System::Object^ sender, System::EventArgs^ e) 
	//{
	//	if(_marketAnalyser)
	//	{
	//		numeric_maxJumpsPerTrip->Enabled = check_maxJumpsPerTrip->Checked;
	//		if(!numeric_maxJumpsPerTrip->Enabled){ _marketAnalyser->setMaxJumpsPerTrip(AutoMatic::Eve::MarketAnalyser::DEFAULT_MAX_JUMPS_TO_BUY_ORDER); }
	//		else { _marketAnalyser->setMaxJumpsPerTrip(System::Decimal::ToInt16(numeric_maxJumpsPerTrip->Value)); }
	//	}
	//}

	System::Void EveGui::check_availableFunds_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		if(_marketAnalyser)
		{
			text_availableFunds->Enabled = check_availableFunds->Checked;
			if(!text_availableFunds->Enabled){ _marketAnalyser->setFunds(-1.0); }
		}
	}

	System::Void EveGui::check_shipVolume_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		if(_marketAnalyser)
		{
			text_shipVolume->Enabled = check_shipVolume->Checked;
			if(!text_shipVolume->Enabled){ _marketAnalyser->setVolume(-1.0); }
		}
	}

	System::Void EveGui::check_useROI_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		if(_marketAnalyser)
		{
			_marketAnalyser->setUseROIAsIndex(check_useROI->Checked);
		}
	}

	//System::Void EveGui::numeric_maxJumpsToStart_ValueChanged(System::Object^ sender, System::EventArgs^ e) 
	//{
	//	if(_marketAnalyser)
	//	{
	//		_marketAnalyser->setMaxJumpsToStart(System::Decimal::ToInt16(numeric_maxJumpsToStart->Value));
	//	}
	//}

	//System::Void EveGui::numeric_maxJumpsPerTrip_ValueChanged(System::Object^ sender, System::EventArgs^ e) 
	//{
	//	if(_marketAnalyser)
	//	{
	//		_marketAnalyser->setMaxJumpsPerTrip(System::Decimal::ToInt16(numeric_maxJumpsPerTrip->Value));
	//	}
	//}

	System::Void EveGui::numeric_ignoreSellOrderVolumes_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		if(_marketAnalyser)
		{
			_marketAnalyser->ignoreSellOrderVolumeAbove(System::Decimal::ToInt16(numeric_ignoreSellOrderVolumes->Value));
		}
	}

	System::Void EveGui::text_shipVolume_TextChanged(System::Object^ sender, System::EventArgs^ e) 
	{
		if(!_marketAnalyser){ return; }

		if(text_shipVolume->Text->Length <= 0){ return; }

		try
		{
			std::string volumeString = GetString(text_shipVolume->Text);
			
			double volume = boost::lexical_cast<double>(volumeString);
			_marketAnalyser->setVolume(volume);
		}
		catch (boost::bad_lexical_cast &)
		{
			text_shipVolume->Text = "";
		}
	}

	System::Void EveGui::text_availableFunds_TextChanged(System::Object^ sender, System::EventArgs^ e) 
	{
		if(!_marketAnalyser){ return; }

		if(text_availableFunds->Text->Length <= 0){ return; }

		try
		{
			std::string fundsString = GetString(text_availableFunds->Text);

			double funds = boost::lexical_cast<double>(fundsString);
			_marketAnalyser->setFunds(funds);
		}
		catch (boost::bad_lexical_cast &)
		{
			text_availableFunds->Text = "";
		}
	}

	void EveGui::setProfitInfo(AutoMatic::Eve::ProfitIndex const & profit_)
	{
		info_fileAnalysed->Text = GetString(profit_._file);
		info_profitIndex->Text = GetString(profit_._index); /**< The relative profit index of the trade route. **/
		info_profit->Text = GetString(profit_._profit); /**< The expected total profit after all trips. **/
		info_profitPerTrip->Text = GetString(profit_._profitPerUnit * profit_._unitsShippedPerTrip); /**< The profit per trip. **/
		info_profitPerUnit->Text = GetString(profit_._profitPerUnit); /**< The profit made for every unit sold. **/
		info_costPerUnit->Text = GetString(profit_._unitCost); /**< The cost per unit bought. **/
		info_incomePerUnit->Text = GetString(profit_._unitIncome); /**< The income per unit sold. **/
		info_volumePerUnit->Text = GetString(profit_._unitVolume); /**< Volume per unit in metres cubed. **/
		info_unitsPerTrip->Text = GetString(profit_._unitsShippedPerTrip); /**< The number of units to be carried per trip. **/
		info_volumePerTrip->Text = GetString(profit_._tripVolume); /**< The volume shipped per trip. **/
		info_jumpsToStart->Text = GetString(profit_._jumpsToStart); /**< The number of jumps required to start the trade route. **/
		info_jumpsPerTrip->Text = GetString(profit_._jumpsPerTrip); /**< The number of jumps per trip. **/
		info_jumps->Text = GetString(profit_._jumpsTotal); /**< The total number of jumps, over all trips. **/
		info_trips->Text = GetString(profit_._trips); /**< The number of trips required to transport all units. **/
		info_security->Text = GetString(profit_._lowestSecurity); /**< The lowest security in the trip. **/
		info_ROI->Text = GetString( (profit_._profitPerUnit * profit_._unitsShippedPerTrip) / (profit_._unitsShippedPerTrip * profit_._unitCost) ); /**< The amount made per trip, divided by the amount it costs per trip. **/
		//GetString((*i)._isEstimate); /**< The distance required to travel is only an estimate.  **/
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
					list_analysedFiles->Items->Add(GetString((*profit)._index));
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

	System::Void EveGui::button_clearAnalysedFiles_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		list_analysedFiles->Items->Clear();
		_profits->clear();

		info_fileAnalysed->Text = "";
		info_profitIndex->Text = "";
		info_profit->Text = "";
		info_profitPerUnit->Text = "";
		info_costPerUnit->Text = "";
		info_incomePerUnit->Text = "";
		info_volumePerUnit->Text = "";
		info_unitsPerTrip->Text = "";
		info_volumePerTrip->Text = "";
		info_jumpsToStart->Text = "";
		info_jumpsPerTrip->Text = "";
		info_jumps->Text = "";
		info_trips->Text = "";
		info_profitPerTrip->Text = "";
		info_security->Text = "";
		info_ROI->Text = "";
	}

	System::Void EveGui::list_analysedFiles_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		if( (int)list_analysedFiles->SelectedIndex >= 0 &&  
				(int)list_analysedFiles->SelectedIndex < (*_profits).size() )
		{
			setProfitInfo((*_profits)[(int)list_analysedFiles->SelectedIndex]);
		}
	}

	System::Void EveGui::list_analysedFiles_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
	{
		if( (int)list_analysedFiles->SelectedIndex >= 0 &&  
			(int)list_analysedFiles->SelectedIndex < (*_profits).size() )
		{
			std::vector<AutoMatic::Eve::ProfitIndex>::iterator delIter = _profits->begin();
			for (int i = 0; i < list_analysedFiles->SelectedIndex; ++i){ ++delIter; }
			_profits->erase(delIter);
			list_analysedFiles->Items->Remove(list_analysedFiles->SelectedItem);
		}
	}
}
