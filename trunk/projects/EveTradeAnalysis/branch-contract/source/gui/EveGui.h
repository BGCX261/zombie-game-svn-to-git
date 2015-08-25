#pragma once

#include <vector>

#include "AM_Eve_MarketAnalyser.hpp"
#include "AM_Eve_MarketReporter.hpp"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace AutoMatic
{
	class FileWatcher;
	namespace Eve
	{
		class Jumps;
		class MarketAnalyser;
	}
}

namespace EveTradeAnalysis
{

	/// <summary>
	/// Summary for EveGui
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>

	class EveGuiReporter;

	public ref class EveGui : public System::Windows::Forms::Form
	{
	private:
		std::vector<AutoMatic::Eve::ProfitIndex> * _profits;
		EveGuiReporter * _reporter;

		AutoMatic::Eve::Jumps * _jumps;
		AutoMatic::FileWatcher * _fileWatcher;
	private: System::Windows::Forms::CheckBox^  check_shipVolume;
	private: System::Windows::Forms::CheckBox^  check_availableFunds;
	private: System::Windows::Forms::CheckBox^  check_ignoreJumpToStart;
	private: System::Windows::Forms::CheckBox^  check_ignoreSellOrderVolume;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::NumericUpDown^  numeric_ignoreSellOrderVolumes;
	private: System::Windows::Forms::GroupBox^  group_profitPerTrip;
	private: System::Windows::Forms::TextBox^  info_profitPerTrip;
	private: System::Windows::Forms::GroupBox^  group_security;
	private: System::Windows::Forms::TextBox^  info_security;
	private: System::Windows::Forms::GroupBox^  group_ROI;
	private: System::Windows::Forms::TextBox^  info_ROI;


	private: System::Windows::Forms::TabControl^  tab_analysis;
	private: System::Windows::Forms::TabPage^  tabPage_jumps;
	private: System::Windows::Forms::TabPage^  tabPage_contracts;
	private: System::Windows::Forms::CheckBox^  check_useROI;
	private: System::Windows::Forms::CheckBox^  check_ignoreJumpsPerTrip;




					 AutoMatic::Eve::MarketAnalyser * _marketAnalyser;

	public:
		EveGui(void);

	public:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EveGui();

		void createJumps(void);
		void EveGui::setProfitInfo(AutoMatic::Eve::ProfitIndex const & profit_);

	private: System::Windows::Forms::FolderBrowserDialog^  openFolder;

	private: System::Windows::Forms::TabControl^  tab_menu;
	private: System::Windows::Forms::TabPage^  tabPage_settings;
	private: System::Windows::Forms::TabPage^  tabPage_conversion;




	private: System::Windows::Forms::GroupBox^  group_loadedFiles;
	private: System::Windows::Forms::GroupBox^  group_directoryWatched;
	private: System::Windows::Forms::TextBox^  info_directoryWatched;


	private: System::Windows::Forms::GroupBox^  group_jumpsFile;
	private: System::Windows::Forms::TextBox^  info_jumpsFile;
	private: System::Windows::Forms::GroupBox^  group_itemsFile;


	private: System::Windows::Forms::TextBox^  info_itemsFile;

	private: System::Windows::Forms::GroupBox^  group_systemsFile;
	private: System::Windows::Forms::TextBox^  info_systemsFile;
	private: System::Windows::Forms::GroupBox^  group_analysedFiles;
	private: System::Windows::Forms::Button^  button_clearAnalysedFiles;
	private: System::Windows::Forms::ListBox^  list_analysedFiles;
	private: System::Windows::Forms::Timer^  timer_update;



	private: System::Windows::Forms::Label^  label_shipVolume;

	private: System::Windows::Forms::TextBox^  text_availableFunds;
	private: System::Windows::Forms::TextBox^  text_shipVolume;


	private: System::Windows::Forms::Label^  label_availableFunds;



	private: System::Windows::Forms::Button^  button_convertToCSV;
	private: System::Windows::Forms::GroupBox^  group_convertDB;

	private: System::Windows::Forms::RadioButton^  radio_itemsDB;

	private: System::Windows::Forms::RadioButton^  radio_jumpsDB;

	private: System::Windows::Forms::RadioButton^  radio_solarSystemsDB;




	private: System::Windows::Forms::OpenFileDialog^  openFile;

	private: System::Windows::Forms::GroupBox^  group_settings;
	private: System::Windows::Forms::GroupBox^  group_profitIndex;
	private: System::Windows::Forms::TextBox^  info_profitIndex;
	private: System::Windows::Forms::GroupBox^  group_profit;
	private: System::Windows::Forms::GroupBox^  group_infoPerUnit;

	private: System::Windows::Forms::TextBox^  info_incomePerUnit;

	private: System::Windows::Forms::GroupBox^  group_costPerUnit;
	private: System::Windows::Forms::TextBox^  info_costPerUnit;

	private: System::Windows::Forms::GroupBox^  group_jumps;
	private: System::Windows::Forms::TextBox^  info_jumps;
	private: System::Windows::Forms::GroupBox^  group_trips;
	private: System::Windows::Forms::TextBox^  info_trips;
	private: System::Windows::Forms::TextBox^  info_profit;
	private: System::Windows::Forms::GroupBox^  group_profitPerUnit;
	private: System::Windows::Forms::TextBox^  info_profitPerUnit;
	private: System::Windows::Forms::GroupBox^  group_jumpsPerTrip;
	private: System::Windows::Forms::TextBox^  info_jumpsPerTrip;
	private: System::Windows::Forms::GroupBox^  group_jumpsToStart;
	private: System::Windows::Forms::TextBox^  info_jumpsToStart;
	private: System::Windows::Forms::GroupBox^  group_fileAnalysed;
	private: System::Windows::Forms::TextBox^  info_fileAnalysed;
	private: System::Windows::Forms::GroupBox^  group_volumePerUnit;
	private: System::Windows::Forms::TextBox^  info_volumePerUnit;
	private: System::Windows::Forms::GroupBox^  group_unitsPerTrip;
	private: System::Windows::Forms::TextBox^  info_unitsPerTrip;
	private: System::Windows::Forms::GroupBox^  group_volumePerTrip;
	private: System::Windows::Forms::TextBox^  info_volumePerTrip;
	private: System::ComponentModel::IContainer^  components;

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->tab_menu = (gcnew System::Windows::Forms::TabControl());
			this->tabPage_settings = (gcnew System::Windows::Forms::TabPage());
			this->group_settings = (gcnew System::Windows::Forms::GroupBox());
			this->check_useROI = (gcnew System::Windows::Forms::CheckBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->numeric_ignoreSellOrderVolumes = (gcnew System::Windows::Forms::NumericUpDown());
			this->check_ignoreSellOrderVolume = (gcnew System::Windows::Forms::CheckBox());
			this->check_ignoreJumpToStart = (gcnew System::Windows::Forms::CheckBox());
			this->check_shipVolume = (gcnew System::Windows::Forms::CheckBox());
			this->check_availableFunds = (gcnew System::Windows::Forms::CheckBox());
			this->label_availableFunds = (gcnew System::Windows::Forms::Label());
			this->text_shipVolume = (gcnew System::Windows::Forms::TextBox());
			this->label_shipVolume = (gcnew System::Windows::Forms::Label());
			this->text_availableFunds = (gcnew System::Windows::Forms::TextBox());
			this->tabPage_conversion = (gcnew System::Windows::Forms::TabPage());
			this->button_convertToCSV = (gcnew System::Windows::Forms::Button());
			this->group_convertDB = (gcnew System::Windows::Forms::GroupBox());
			this->radio_itemsDB = (gcnew System::Windows::Forms::RadioButton());
			this->radio_jumpsDB = (gcnew System::Windows::Forms::RadioButton());
			this->radio_solarSystemsDB = (gcnew System::Windows::Forms::RadioButton());
			this->openFile = (gcnew System::Windows::Forms::OpenFileDialog());
			this->group_ROI = (gcnew System::Windows::Forms::GroupBox());
			this->info_ROI = (gcnew System::Windows::Forms::TextBox());
			this->group_security = (gcnew System::Windows::Forms::GroupBox());
			this->info_security = (gcnew System::Windows::Forms::TextBox());
			this->group_profitPerTrip = (gcnew System::Windows::Forms::GroupBox());
			this->info_profitPerTrip = (gcnew System::Windows::Forms::TextBox());
			this->group_analysedFiles = (gcnew System::Windows::Forms::GroupBox());
			this->button_clearAnalysedFiles = (gcnew System::Windows::Forms::Button());
			this->list_analysedFiles = (gcnew System::Windows::Forms::ListBox());
			this->group_volumePerTrip = (gcnew System::Windows::Forms::GroupBox());
			this->info_volumePerTrip = (gcnew System::Windows::Forms::TextBox());
			this->group_unitsPerTrip = (gcnew System::Windows::Forms::GroupBox());
			this->info_unitsPerTrip = (gcnew System::Windows::Forms::TextBox());
			this->group_volumePerUnit = (gcnew System::Windows::Forms::GroupBox());
			this->info_volumePerUnit = (gcnew System::Windows::Forms::TextBox());
			this->group_jumpsPerTrip = (gcnew System::Windows::Forms::GroupBox());
			this->info_jumpsPerTrip = (gcnew System::Windows::Forms::TextBox());
			this->group_jumpsToStart = (gcnew System::Windows::Forms::GroupBox());
			this->info_jumpsToStart = (gcnew System::Windows::Forms::TextBox());
			this->group_fileAnalysed = (gcnew System::Windows::Forms::GroupBox());
			this->info_fileAnalysed = (gcnew System::Windows::Forms::TextBox());
			this->group_profitPerUnit = (gcnew System::Windows::Forms::GroupBox());
			this->info_profitPerUnit = (gcnew System::Windows::Forms::TextBox());
			this->group_infoPerUnit = (gcnew System::Windows::Forms::GroupBox());
			this->info_incomePerUnit = (gcnew System::Windows::Forms::TextBox());
			this->group_costPerUnit = (gcnew System::Windows::Forms::GroupBox());
			this->info_costPerUnit = (gcnew System::Windows::Forms::TextBox());
			this->group_jumps = (gcnew System::Windows::Forms::GroupBox());
			this->info_jumps = (gcnew System::Windows::Forms::TextBox());
			this->group_trips = (gcnew System::Windows::Forms::GroupBox());
			this->info_trips = (gcnew System::Windows::Forms::TextBox());
			this->group_profit = (gcnew System::Windows::Forms::GroupBox());
			this->info_profit = (gcnew System::Windows::Forms::TextBox());
			this->group_profitIndex = (gcnew System::Windows::Forms::GroupBox());
			this->info_profitIndex = (gcnew System::Windows::Forms::TextBox());
			this->group_loadedFiles = (gcnew System::Windows::Forms::GroupBox());
			this->group_itemsFile = (gcnew System::Windows::Forms::GroupBox());
			this->info_itemsFile = (gcnew System::Windows::Forms::TextBox());
			this->group_systemsFile = (gcnew System::Windows::Forms::GroupBox());
			this->info_systemsFile = (gcnew System::Windows::Forms::TextBox());
			this->group_directoryWatched = (gcnew System::Windows::Forms::GroupBox());
			this->info_directoryWatched = (gcnew System::Windows::Forms::TextBox());
			this->group_jumpsFile = (gcnew System::Windows::Forms::GroupBox());
			this->info_jumpsFile = (gcnew System::Windows::Forms::TextBox());
			this->timer_update = (gcnew System::Windows::Forms::Timer(this->components));
			this->openFolder = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->tab_analysis = (gcnew System::Windows::Forms::TabControl());
			this->tabPage_jumps = (gcnew System::Windows::Forms::TabPage());
			this->tabPage_contracts = (gcnew System::Windows::Forms::TabPage());
			this->check_ignoreJumpsPerTrip = (gcnew System::Windows::Forms::CheckBox());
			this->tab_menu->SuspendLayout();
			this->tabPage_settings->SuspendLayout();
			this->group_settings->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numeric_ignoreSellOrderVolumes))->BeginInit();
			this->tabPage_conversion->SuspendLayout();
			this->group_convertDB->SuspendLayout();
			this->group_ROI->SuspendLayout();
			this->group_security->SuspendLayout();
			this->group_profitPerTrip->SuspendLayout();
			this->group_analysedFiles->SuspendLayout();
			this->group_volumePerTrip->SuspendLayout();
			this->group_unitsPerTrip->SuspendLayout();
			this->group_volumePerUnit->SuspendLayout();
			this->group_jumpsPerTrip->SuspendLayout();
			this->group_jumpsToStart->SuspendLayout();
			this->group_fileAnalysed->SuspendLayout();
			this->group_profitPerUnit->SuspendLayout();
			this->group_infoPerUnit->SuspendLayout();
			this->group_costPerUnit->SuspendLayout();
			this->group_jumps->SuspendLayout();
			this->group_trips->SuspendLayout();
			this->group_profit->SuspendLayout();
			this->group_profitIndex->SuspendLayout();
			this->group_loadedFiles->SuspendLayout();
			this->group_itemsFile->SuspendLayout();
			this->group_systemsFile->SuspendLayout();
			this->group_directoryWatched->SuspendLayout();
			this->group_jumpsFile->SuspendLayout();
			this->tab_analysis->SuspendLayout();
			this->tabPage_jumps->SuspendLayout();
			this->SuspendLayout();
			// 
			// tab_menu
			// 
			this->tab_menu->Controls->Add(this->tabPage_settings);
			this->tab_menu->Controls->Add(this->tabPage_conversion);
			this->tab_menu->Location = System::Drawing::Point(4, 143);
			this->tab_menu->Name = L"tab_menu";
			this->tab_menu->SelectedIndex = 0;
			this->tab_menu->Size = System::Drawing::Size(229, 301);
			this->tab_menu->TabIndex = 1;
			// 
			// tabPage_settings
			// 
			this->tabPage_settings->Controls->Add(this->group_settings);
			this->tabPage_settings->Location = System::Drawing::Point(4, 22);
			this->tabPage_settings->Name = L"tabPage_settings";
			this->tabPage_settings->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_settings->Size = System::Drawing::Size(221, 275);
			this->tabPage_settings->TabIndex = 0;
			this->tabPage_settings->Text = L"Settings";
			this->tabPage_settings->UseVisualStyleBackColor = true;
			// 
			// group_settings
			// 
			this->group_settings->Controls->Add(this->check_ignoreJumpsPerTrip);
			this->group_settings->Controls->Add(this->check_useROI);
			this->group_settings->Controls->Add(this->label1);
			this->group_settings->Controls->Add(this->numeric_ignoreSellOrderVolumes);
			this->group_settings->Controls->Add(this->check_ignoreSellOrderVolume);
			this->group_settings->Controls->Add(this->check_ignoreJumpToStart);
			this->group_settings->Controls->Add(this->check_shipVolume);
			this->group_settings->Controls->Add(this->check_availableFunds);
			this->group_settings->Controls->Add(this->label_availableFunds);
			this->group_settings->Controls->Add(this->text_shipVolume);
			this->group_settings->Controls->Add(this->label_shipVolume);
			this->group_settings->Controls->Add(this->text_availableFunds);
			this->group_settings->Location = System::Drawing::Point(7, 3);
			this->group_settings->Name = L"group_settings";
			this->group_settings->Size = System::Drawing::Size(208, 266);
			this->group_settings->TabIndex = 0;
			this->group_settings->TabStop = false;
			this->group_settings->Text = L"Enter";
			this->group_settings->Enter += gcnew System::EventHandler(this, &EveGui::group_settings_Enter);
			// 
			// check_useROI
			// 
			this->check_useROI->AutoSize = true;
			this->check_useROI->Location = System::Drawing::Point(6, 150);
			this->check_useROI->Name = L"check_useROI";
			this->check_useROI->Size = System::Drawing::Size(110, 17);
			this->check_useROI->TabIndex = 17;
			this->check_useROI->Text = L"Use ROI as Index";
			this->check_useROI->UseVisualStyleBackColor = true;
			this->check_useROI->CheckedChanged += gcnew System::EventHandler(this, &EveGui::check_useROI_CheckedChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 94);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(170, 13);
			this->label1->TabIndex = 16;
			this->label1->Text = L"Infinite Sell Volume Above Number";
			// 
			// numeric_ignoreSellOrderVolumes
			// 
			this->numeric_ignoreSellOrderVolumes->Enabled = false;
			this->numeric_ignoreSellOrderVolumes->Location = System::Drawing::Point(74, 110);
			this->numeric_ignoreSellOrderVolumes->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1000000000, 0, 0, 0});
			this->numeric_ignoreSellOrderVolumes->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numeric_ignoreSellOrderVolumes->Name = L"numeric_ignoreSellOrderVolumes";
			this->numeric_ignoreSellOrderVolumes->Size = System::Drawing::Size(124, 20);
			this->numeric_ignoreSellOrderVolumes->TabIndex = 15;
			this->numeric_ignoreSellOrderVolumes->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) {1, 0, 0, 0});
			this->numeric_ignoreSellOrderVolumes->ValueChanged += gcnew System::EventHandler(this, &EveGui::numeric_ignoreSellOrderVolumes_ValueChanged);
			// 
			// check_ignoreSellOrderVolume
			// 
			this->check_ignoreSellOrderVolume->AutoSize = true;
			this->check_ignoreSellOrderVolume->Location = System::Drawing::Point(6, 111);
			this->check_ignoreSellOrderVolume->Name = L"check_ignoreSellOrderVolume";
			this->check_ignoreSellOrderVolume->Size = System::Drawing::Size(59, 17);
			this->check_ignoreSellOrderVolume->TabIndex = 14;
			this->check_ignoreSellOrderVolume->Text = L"Enable";
			this->check_ignoreSellOrderVolume->UseVisualStyleBackColor = true;
			this->check_ignoreSellOrderVolume->CheckedChanged += gcnew System::EventHandler(this, &EveGui::check_ignoreSellOrderVolume_CheckedChanged);
			// 
			// check_ignoreJumpToStart
			// 
			this->check_ignoreJumpToStart->AutoSize = true;
			this->check_ignoreJumpToStart->Location = System::Drawing::Point(6, 198);
			this->check_ignoreJumpToStart->Name = L"check_ignoreJumpToStart";
			this->check_ignoreJumpToStart->Size = System::Drawing::Size(126, 17);
			this->check_ignoreJumpToStart->TabIndex = 13;
			this->check_ignoreJumpToStart->Text = L"Ignore Jumps to Start";
			this->check_ignoreJumpToStart->UseVisualStyleBackColor = true;
			this->check_ignoreJumpToStart->CheckedChanged += gcnew System::EventHandler(this, &EveGui::check_ignoreJumpToStart_CheckedChanged);
			// 
			// check_shipVolume
			// 
			this->check_shipVolume->AutoSize = true;
			this->check_shipVolume->Location = System::Drawing::Point(6, 74);
			this->check_shipVolume->Name = L"check_shipVolume";
			this->check_shipVolume->Size = System::Drawing::Size(59, 17);
			this->check_shipVolume->TabIndex = 12;
			this->check_shipVolume->Text = L"Enable";
			this->check_shipVolume->UseVisualStyleBackColor = true;
			this->check_shipVolume->CheckedChanged += gcnew System::EventHandler(this, &EveGui::check_shipVolume_CheckedChanged);
			// 
			// check_availableFunds
			// 
			this->check_availableFunds->AutoSize = true;
			this->check_availableFunds->Location = System::Drawing::Point(6, 38);
			this->check_availableFunds->Name = L"check_availableFunds";
			this->check_availableFunds->Size = System::Drawing::Size(59, 17);
			this->check_availableFunds->TabIndex = 11;
			this->check_availableFunds->Text = L"Enable";
			this->check_availableFunds->UseVisualStyleBackColor = true;
			this->check_availableFunds->CheckedChanged += gcnew System::EventHandler(this, &EveGui::check_availableFunds_CheckedChanged);
			// 
			// label_availableFunds
			// 
			this->label_availableFunds->AutoSize = true;
			this->label_availableFunds->Location = System::Drawing::Point(6, 19);
			this->label_availableFunds->Name = L"label_availableFunds";
			this->label_availableFunds->Size = System::Drawing::Size(82, 13);
			this->label_availableFunds->TabIndex = 1;
			this->label_availableFunds->Text = L"Available Funds";
			// 
			// text_shipVolume
			// 
			this->text_shipVolume->Enabled = false;
			this->text_shipVolume->Location = System::Drawing::Point(74, 71);
			this->text_shipVolume->Name = L"text_shipVolume";
			this->text_shipVolume->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->text_shipVolume->Size = System::Drawing::Size(124, 20);
			this->text_shipVolume->TabIndex = 0;
			this->text_shipVolume->TextChanged += gcnew System::EventHandler(this, &EveGui::text_shipVolume_TextChanged);
			// 
			// label_shipVolume
			// 
			this->label_shipVolume->AutoSize = true;
			this->label_shipVolume->Location = System::Drawing::Point(6, 58);
			this->label_shipVolume->Name = L"label_shipVolume";
			this->label_shipVolume->Size = System::Drawing::Size(66, 13);
			this->label_shipVolume->TabIndex = 3;
			this->label_shipVolume->Text = L"Ship Volume";
			// 
			// text_availableFunds
			// 
			this->text_availableFunds->Enabled = false;
			this->text_availableFunds->Location = System::Drawing::Point(74, 35);
			this->text_availableFunds->Name = L"text_availableFunds";
			this->text_availableFunds->Size = System::Drawing::Size(124, 20);
			this->text_availableFunds->TabIndex = 2;
			this->text_availableFunds->TextChanged += gcnew System::EventHandler(this, &EveGui::text_availableFunds_TextChanged);
			// 
			// tabPage_conversion
			// 
			this->tabPage_conversion->Controls->Add(this->button_convertToCSV);
			this->tabPage_conversion->Controls->Add(this->group_convertDB);
			this->tabPage_conversion->Location = System::Drawing::Point(4, 22);
			this->tabPage_conversion->Name = L"tabPage_conversion";
			this->tabPage_conversion->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_conversion->Size = System::Drawing::Size(221, 275);
			this->tabPage_conversion->TabIndex = 1;
			this->tabPage_conversion->Text = L"Conversion";
			this->tabPage_conversion->UseVisualStyleBackColor = true;
			// 
			// button_convertToCSV
			// 
			this->button_convertToCSV->Location = System::Drawing::Point(31, 144);
			this->button_convertToCSV->Name = L"button_convertToCSV";
			this->button_convertToCSV->Size = System::Drawing::Size(151, 47);
			this->button_convertToCSV->TabIndex = 4;
			this->button_convertToCSV->Text = L"Open and Convert To CSV";
			this->button_convertToCSV->UseVisualStyleBackColor = true;
			// 
			// group_convertDB
			// 
			this->group_convertDB->Controls->Add(this->radio_itemsDB);
			this->group_convertDB->Controls->Add(this->radio_jumpsDB);
			this->group_convertDB->Controls->Add(this->radio_solarSystemsDB);
			this->group_convertDB->Location = System::Drawing::Point(6, 6);
			this->group_convertDB->Name = L"group_convertDB";
			this->group_convertDB->Size = System::Drawing::Size(200, 100);
			this->group_convertDB->TabIndex = 3;
			this->group_convertDB->TabStop = false;
			this->group_convertDB->Text = L"Select";
			// 
			// radio_itemsDB
			// 
			this->radio_itemsDB->AutoSize = true;
			this->radio_itemsDB->Location = System::Drawing::Point(7, 68);
			this->radio_itemsDB->Name = L"radio_itemsDB";
			this->radio_itemsDB->Size = System::Drawing::Size(93, 17);
			this->radio_itemsDB->TabIndex = 2;
			this->radio_itemsDB->TabStop = true;
			this->radio_itemsDB->Text = L"Items Datbase";
			this->radio_itemsDB->UseVisualStyleBackColor = true;
			// 
			// radio_jumpsDB
			// 
			this->radio_jumpsDB->AutoSize = true;
			this->radio_jumpsDB->Location = System::Drawing::Point(7, 44);
			this->radio_jumpsDB->Name = L"radio_jumpsDB";
			this->radio_jumpsDB->Size = System::Drawing::Size(104, 17);
			this->radio_jumpsDB->TabIndex = 1;
			this->radio_jumpsDB->TabStop = true;
			this->radio_jumpsDB->Text = L"Jumps Database";
			this->radio_jumpsDB->UseVisualStyleBackColor = true;
			// 
			// radio_solarSystemsDB
			// 
			this->radio_solarSystemsDB->AutoSize = true;
			this->radio_solarSystemsDB->Location = System::Drawing::Point(7, 20);
			this->radio_solarSystemsDB->Name = L"radio_solarSystemsDB";
			this->radio_solarSystemsDB->Size = System::Drawing::Size(140, 17);
			this->radio_solarSystemsDB->TabIndex = 0;
			this->radio_solarSystemsDB->TabStop = true;
			this->radio_solarSystemsDB->Text = L"Solar Systems Database";
			this->radio_solarSystemsDB->UseVisualStyleBackColor = true;
			// 
			// openFile
			// 
			this->openFile->FileName = L"openFile";
			// 
			// group_ROI
			// 
			this->group_ROI->Controls->Add(this->info_ROI);
			this->group_ROI->Location = System::Drawing::Point(125, 227);
			this->group_ROI->Name = L"group_ROI";
			this->group_ROI->Size = System::Drawing::Size(113, 42);
			this->group_ROI->TabIndex = 15;
			this->group_ROI->TabStop = false;
			this->group_ROI->Text = L"ROI";
			// 
			// info_ROI
			// 
			this->info_ROI->Location = System::Drawing::Point(7, 16);
			this->info_ROI->Name = L"info_ROI";
			this->info_ROI->ReadOnly = true;
			this->info_ROI->Size = System::Drawing::Size(99, 20);
			this->info_ROI->TabIndex = 1;
			// 
			// group_security
			// 
			this->group_security->Controls->Add(this->info_security);
			this->group_security->Location = System::Drawing::Point(244, 227);
			this->group_security->Name = L"group_security";
			this->group_security->Size = System::Drawing::Size(113, 42);
			this->group_security->TabIndex = 14;
			this->group_security->TabStop = false;
			this->group_security->Text = L"Lowest Security";
			// 
			// info_security
			// 
			this->info_security->Location = System::Drawing::Point(7, 16);
			this->info_security->Name = L"info_security";
			this->info_security->ReadOnly = true;
			this->info_security->Size = System::Drawing::Size(99, 20);
			this->info_security->TabIndex = 1;
			// 
			// group_profitPerTrip
			// 
			this->group_profitPerTrip->Controls->Add(this->info_profitPerTrip);
			this->group_profitPerTrip->Location = System::Drawing::Point(125, 49);
			this->group_profitPerTrip->Name = L"group_profitPerTrip";
			this->group_profitPerTrip->Size = System::Drawing::Size(113, 40);
			this->group_profitPerTrip->TabIndex = 12;
			this->group_profitPerTrip->TabStop = false;
			this->group_profitPerTrip->Text = L"Profit Per Trip";
			// 
			// info_profitPerTrip
			// 
			this->info_profitPerTrip->Location = System::Drawing::Point(6, 16);
			this->info_profitPerTrip->Name = L"info_profitPerTrip";
			this->info_profitPerTrip->ReadOnly = true;
			this->info_profitPerTrip->Size = System::Drawing::Size(100, 20);
			this->info_profitPerTrip->TabIndex = 1;
			// 
			// group_analysedFiles
			// 
			this->group_analysedFiles->Controls->Add(this->button_clearAnalysedFiles);
			this->group_analysedFiles->Controls->Add(this->list_analysedFiles);
			this->group_analysedFiles->Location = System::Drawing::Point(624, 149);
			this->group_analysedFiles->Name = L"group_analysedFiles";
			this->group_analysedFiles->Size = System::Drawing::Size(109, 291);
			this->group_analysedFiles->TabIndex = 13;
			this->group_analysedFiles->TabStop = false;
			this->group_analysedFiles->Text = L"Analysed Files";
			// 
			// button_clearAnalysedFiles
			// 
			this->button_clearAnalysedFiles->Location = System::Drawing::Point(5, 256);
			this->button_clearAnalysedFiles->Name = L"button_clearAnalysedFiles";
			this->button_clearAnalysedFiles->Size = System::Drawing::Size(100, 23);
			this->button_clearAnalysedFiles->TabIndex = 1;
			this->button_clearAnalysedFiles->Text = L"Clear";
			this->button_clearAnalysedFiles->UseVisualStyleBackColor = true;
			this->button_clearAnalysedFiles->Click += gcnew System::EventHandler(this, &EveGui::button_clearAnalysedFiles_Click);
			// 
			// list_analysedFiles
			// 
			this->list_analysedFiles->FormattingEnabled = true;
			this->list_analysedFiles->Location = System::Drawing::Point(6, 19);
			this->list_analysedFiles->Name = L"list_analysedFiles";
			this->list_analysedFiles->Size = System::Drawing::Size(100, 225);
			this->list_analysedFiles->TabIndex = 0;
			this->list_analysedFiles->MouseDoubleClick += gcnew System::Windows::Forms::MouseEventHandler(this, &EveGui::list_analysedFiles_MouseDoubleClick);
			this->list_analysedFiles->SelectedIndexChanged += gcnew System::EventHandler(this, &EveGui::list_analysedFiles_SelectedIndexChanged);
			// 
			// group_volumePerTrip
			// 
			this->group_volumePerTrip->Controls->Add(this->info_volumePerTrip);
			this->group_volumePerTrip->Location = System::Drawing::Point(125, 181);
			this->group_volumePerTrip->Name = L"group_volumePerTrip";
			this->group_volumePerTrip->Size = System::Drawing::Size(113, 45);
			this->group_volumePerTrip->TabIndex = 12;
			this->group_volumePerTrip->TabStop = false;
			this->group_volumePerTrip->Text = L"Volume Per Trip";
			// 
			// info_volumePerTrip
			// 
			this->info_volumePerTrip->Location = System::Drawing::Point(6, 20);
			this->info_volumePerTrip->Name = L"info_volumePerTrip";
			this->info_volumePerTrip->ReadOnly = true;
			this->info_volumePerTrip->Size = System::Drawing::Size(100, 20);
			this->info_volumePerTrip->TabIndex = 1;
			// 
			// group_unitsPerTrip
			// 
			this->group_unitsPerTrip->Controls->Add(this->info_unitsPerTrip);
			this->group_unitsPerTrip->Location = System::Drawing::Point(244, 49);
			this->group_unitsPerTrip->Name = L"group_unitsPerTrip";
			this->group_unitsPerTrip->Size = System::Drawing::Size(113, 40);
			this->group_unitsPerTrip->TabIndex = 11;
			this->group_unitsPerTrip->TabStop = false;
			this->group_unitsPerTrip->Text = L"Units Per Trip";
			// 
			// info_unitsPerTrip
			// 
			this->info_unitsPerTrip->Location = System::Drawing::Point(6, 16);
			this->info_unitsPerTrip->Name = L"info_unitsPerTrip";
			this->info_unitsPerTrip->ReadOnly = true;
			this->info_unitsPerTrip->Size = System::Drawing::Size(100, 20);
			this->info_unitsPerTrip->TabIndex = 1;
			// 
			// group_volumePerUnit
			// 
			this->group_volumePerUnit->Controls->Add(this->info_volumePerUnit);
			this->group_volumePerUnit->Location = System::Drawing::Point(6, 181);
			this->group_volumePerUnit->Name = L"group_volumePerUnit";
			this->group_volumePerUnit->Size = System::Drawing::Size(113, 45);
			this->group_volumePerUnit->TabIndex = 10;
			this->group_volumePerUnit->TabStop = false;
			this->group_volumePerUnit->Text = L"Volume Per Unit";
			// 
			// info_volumePerUnit
			// 
			this->info_volumePerUnit->Location = System::Drawing::Point(6, 20);
			this->info_volumePerUnit->Name = L"info_volumePerUnit";
			this->info_volumePerUnit->ReadOnly = true;
			this->info_volumePerUnit->Size = System::Drawing::Size(100, 20);
			this->info_volumePerUnit->TabIndex = 1;
			// 
			// group_jumpsPerTrip
			// 
			this->group_jumpsPerTrip->Controls->Add(this->info_jumpsPerTrip);
			this->group_jumpsPerTrip->Location = System::Drawing::Point(244, 133);
			this->group_jumpsPerTrip->Name = L"group_jumpsPerTrip";
			this->group_jumpsPerTrip->Size = System::Drawing::Size(113, 45);
			this->group_jumpsPerTrip->TabIndex = 9;
			this->group_jumpsPerTrip->TabStop = false;
			this->group_jumpsPerTrip->Text = L"Jumps Per Trip";
			// 
			// info_jumpsPerTrip
			// 
			this->info_jumpsPerTrip->Location = System::Drawing::Point(6, 20);
			this->info_jumpsPerTrip->Name = L"info_jumpsPerTrip";
			this->info_jumpsPerTrip->ReadOnly = true;
			this->info_jumpsPerTrip->Size = System::Drawing::Size(100, 20);
			this->info_jumpsPerTrip->TabIndex = 1;
			// 
			// group_jumpsToStart
			// 
			this->group_jumpsToStart->Controls->Add(this->info_jumpsToStart);
			this->group_jumpsToStart->Location = System::Drawing::Point(125, 133);
			this->group_jumpsToStart->Name = L"group_jumpsToStart";
			this->group_jumpsToStart->Size = System::Drawing::Size(113, 45);
			this->group_jumpsToStart->TabIndex = 8;
			this->group_jumpsToStart->TabStop = false;
			this->group_jumpsToStart->Text = L"Jumps To Start";
			// 
			// info_jumpsToStart
			// 
			this->info_jumpsToStart->Location = System::Drawing::Point(6, 20);
			this->info_jumpsToStart->Name = L"info_jumpsToStart";
			this->info_jumpsToStart->ReadOnly = true;
			this->info_jumpsToStart->Size = System::Drawing::Size(100, 20);
			this->info_jumpsToStart->TabIndex = 1;
			// 
			// group_fileAnalysed
			// 
			this->group_fileAnalysed->Controls->Add(this->info_fileAnalysed);
			this->group_fileAnalysed->Location = System::Drawing::Point(6, 6);
			this->group_fileAnalysed->Name = L"group_fileAnalysed";
			this->group_fileAnalysed->Size = System::Drawing::Size(351, 42);
			this->group_fileAnalysed->TabIndex = 7;
			this->group_fileAnalysed->TabStop = false;
			this->group_fileAnalysed->Text = L"File Analysed";
			// 
			// info_fileAnalysed
			// 
			this->info_fileAnalysed->Location = System::Drawing::Point(6, 16);
			this->info_fileAnalysed->Name = L"info_fileAnalysed";
			this->info_fileAnalysed->ReadOnly = true;
			this->info_fileAnalysed->Size = System::Drawing::Size(338, 20);
			this->info_fileAnalysed->TabIndex = 0;
			// 
			// group_profitPerUnit
			// 
			this->group_profitPerUnit->Controls->Add(this->info_profitPerUnit);
			this->group_profitPerUnit->Location = System::Drawing::Point(244, 91);
			this->group_profitPerUnit->Name = L"group_profitPerUnit";
			this->group_profitPerUnit->Size = System::Drawing::Size(113, 40);
			this->group_profitPerUnit->TabIndex = 6;
			this->group_profitPerUnit->TabStop = false;
			this->group_profitPerUnit->Text = L"Profit Per Unit";
			// 
			// info_profitPerUnit
			// 
			this->info_profitPerUnit->Location = System::Drawing::Point(6, 16);
			this->info_profitPerUnit->Name = L"info_profitPerUnit";
			this->info_profitPerUnit->ReadOnly = true;
			this->info_profitPerUnit->Size = System::Drawing::Size(100, 20);
			this->info_profitPerUnit->TabIndex = 1;
			// 
			// group_infoPerUnit
			// 
			this->group_infoPerUnit->Controls->Add(this->info_incomePerUnit);
			this->group_infoPerUnit->Location = System::Drawing::Point(125, 91);
			this->group_infoPerUnit->Name = L"group_infoPerUnit";
			this->group_infoPerUnit->Size = System::Drawing::Size(113, 40);
			this->group_infoPerUnit->TabIndex = 5;
			this->group_infoPerUnit->TabStop = false;
			this->group_infoPerUnit->Text = L"Income Per Unit";
			// 
			// info_incomePerUnit
			// 
			this->info_incomePerUnit->Location = System::Drawing::Point(6, 16);
			this->info_incomePerUnit->Name = L"info_incomePerUnit";
			this->info_incomePerUnit->ReadOnly = true;
			this->info_incomePerUnit->Size = System::Drawing::Size(100, 20);
			this->info_incomePerUnit->TabIndex = 1;
			// 
			// group_costPerUnit
			// 
			this->group_costPerUnit->Controls->Add(this->info_costPerUnit);
			this->group_costPerUnit->Location = System::Drawing::Point(6, 91);
			this->group_costPerUnit->Name = L"group_costPerUnit";
			this->group_costPerUnit->Size = System::Drawing::Size(113, 40);
			this->group_costPerUnit->TabIndex = 4;
			this->group_costPerUnit->TabStop = false;
			this->group_costPerUnit->Text = L"Cost Per Unit";
			// 
			// info_costPerUnit
			// 
			this->info_costPerUnit->Location = System::Drawing::Point(6, 16);
			this->info_costPerUnit->Name = L"info_costPerUnit";
			this->info_costPerUnit->ReadOnly = true;
			this->info_costPerUnit->Size = System::Drawing::Size(100, 20);
			this->info_costPerUnit->TabIndex = 1;
			// 
			// group_jumps
			// 
			this->group_jumps->Controls->Add(this->info_jumps);
			this->group_jumps->Location = System::Drawing::Point(6, 133);
			this->group_jumps->Name = L"group_jumps";
			this->group_jumps->Size = System::Drawing::Size(113, 45);
			this->group_jumps->TabIndex = 3;
			this->group_jumps->TabStop = false;
			this->group_jumps->Text = L"Total Jumps";
			// 
			// info_jumps
			// 
			this->info_jumps->Location = System::Drawing::Point(6, 20);
			this->info_jumps->Name = L"info_jumps";
			this->info_jumps->ReadOnly = true;
			this->info_jumps->Size = System::Drawing::Size(100, 20);
			this->info_jumps->TabIndex = 1;
			// 
			// group_trips
			// 
			this->group_trips->Controls->Add(this->info_trips);
			this->group_trips->Location = System::Drawing::Point(244, 181);
			this->group_trips->Name = L"group_trips";
			this->group_trips->Size = System::Drawing::Size(113, 45);
			this->group_trips->TabIndex = 2;
			this->group_trips->TabStop = false;
			this->group_trips->Text = L"Trips";
			// 
			// info_trips
			// 
			this->info_trips->Location = System::Drawing::Point(6, 20);
			this->info_trips->Name = L"info_trips";
			this->info_trips->ReadOnly = true;
			this->info_trips->Size = System::Drawing::Size(100, 20);
			this->info_trips->TabIndex = 1;
			// 
			// group_profit
			// 
			this->group_profit->Controls->Add(this->info_profit);
			this->group_profit->Location = System::Drawing::Point(6, 227);
			this->group_profit->Name = L"group_profit";
			this->group_profit->Size = System::Drawing::Size(113, 42);
			this->group_profit->TabIndex = 1;
			this->group_profit->TabStop = false;
			this->group_profit->Text = L"Profit";
			// 
			// info_profit
			// 
			this->info_profit->Location = System::Drawing::Point(7, 16);
			this->info_profit->Name = L"info_profit";
			this->info_profit->ReadOnly = true;
			this->info_profit->Size = System::Drawing::Size(99, 20);
			this->info_profit->TabIndex = 1;
			// 
			// group_profitIndex
			// 
			this->group_profitIndex->Controls->Add(this->info_profitIndex);
			this->group_profitIndex->Location = System::Drawing::Point(6, 49);
			this->group_profitIndex->Name = L"group_profitIndex";
			this->group_profitIndex->Size = System::Drawing::Size(113, 40);
			this->group_profitIndex->TabIndex = 0;
			this->group_profitIndex->TabStop = false;
			this->group_profitIndex->Text = L"Profit Index";
			// 
			// info_profitIndex
			// 
			this->info_profitIndex->Location = System::Drawing::Point(6, 16);
			this->info_profitIndex->Name = L"info_profitIndex";
			this->info_profitIndex->ReadOnly = true;
			this->info_profitIndex->Size = System::Drawing::Size(100, 20);
			this->info_profitIndex->TabIndex = 0;
			// 
			// group_loadedFiles
			// 
			this->group_loadedFiles->Controls->Add(this->group_itemsFile);
			this->group_loadedFiles->Controls->Add(this->group_systemsFile);
			this->group_loadedFiles->Controls->Add(this->group_directoryWatched);
			this->group_loadedFiles->Controls->Add(this->group_jumpsFile);
			this->group_loadedFiles->Location = System::Drawing::Point(4, 8);
			this->group_loadedFiles->Name = L"group_loadedFiles";
			this->group_loadedFiles->Size = System::Drawing::Size(725, 135);
			this->group_loadedFiles->TabIndex = 3;
			this->group_loadedFiles->TabStop = false;
			this->group_loadedFiles->Text = L"Filesystem Information";
			// 
			// group_itemsFile
			// 
			this->group_itemsFile->Controls->Add(this->info_itemsFile);
			this->group_itemsFile->Location = System::Drawing::Point(367, 75);
			this->group_itemsFile->Name = L"group_itemsFile";
			this->group_itemsFile->Size = System::Drawing::Size(355, 54);
			this->group_itemsFile->TabIndex = 5;
			this->group_itemsFile->TabStop = false;
			this->group_itemsFile->Text = L"Items File";
			// 
			// info_itemsFile
			// 
			this->info_itemsFile->Location = System::Drawing::Point(6, 19);
			this->info_itemsFile->Name = L"info_itemsFile";
			this->info_itemsFile->ReadOnly = true;
			this->info_itemsFile->Size = System::Drawing::Size(343, 20);
			this->info_itemsFile->TabIndex = 1;
			this->info_itemsFile->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &EveGui::info_itemsFile_MouseUp);
			// 
			// group_systemsFile
			// 
			this->group_systemsFile->Controls->Add(this->info_systemsFile);
			this->group_systemsFile->Location = System::Drawing::Point(367, 15);
			this->group_systemsFile->Name = L"group_systemsFile";
			this->group_systemsFile->Size = System::Drawing::Size(355, 54);
			this->group_systemsFile->TabIndex = 4;
			this->group_systemsFile->TabStop = false;
			this->group_systemsFile->Text = L"Solar Systems File";
			// 
			// info_systemsFile
			// 
			this->info_systemsFile->Location = System::Drawing::Point(6, 19);
			this->info_systemsFile->Name = L"info_systemsFile";
			this->info_systemsFile->ReadOnly = true;
			this->info_systemsFile->Size = System::Drawing::Size(343, 20);
			this->info_systemsFile->TabIndex = 1;
			this->info_systemsFile->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &EveGui::info_systemsFile_MouseUp);
			// 
			// group_directoryWatched
			// 
			this->group_directoryWatched->Controls->Add(this->info_directoryWatched);
			this->group_directoryWatched->Location = System::Drawing::Point(6, 15);
			this->group_directoryWatched->Name = L"group_directoryWatched";
			this->group_directoryWatched->Size = System::Drawing::Size(355, 54);
			this->group_directoryWatched->TabIndex = 3;
			this->group_directoryWatched->TabStop = false;
			this->group_directoryWatched->Text = L"Directory Watched";
			// 
			// info_directoryWatched
			// 
			this->info_directoryWatched->Location = System::Drawing::Point(6, 19);
			this->info_directoryWatched->Name = L"info_directoryWatched";
			this->info_directoryWatched->ReadOnly = true;
			this->info_directoryWatched->Size = System::Drawing::Size(343, 20);
			this->info_directoryWatched->TabIndex = 1;
			this->info_directoryWatched->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &EveGui::info_directoryWatched_MouseUp);
			// 
			// group_jumpsFile
			// 
			this->group_jumpsFile->Controls->Add(this->info_jumpsFile);
			this->group_jumpsFile->Location = System::Drawing::Point(6, 75);
			this->group_jumpsFile->Name = L"group_jumpsFile";
			this->group_jumpsFile->Size = System::Drawing::Size(355, 54);
			this->group_jumpsFile->TabIndex = 0;
			this->group_jumpsFile->TabStop = false;
			this->group_jumpsFile->Text = L"Jumps File";
			// 
			// info_jumpsFile
			// 
			this->info_jumpsFile->Location = System::Drawing::Point(6, 19);
			this->info_jumpsFile->Name = L"info_jumpsFile";
			this->info_jumpsFile->ReadOnly = true;
			this->info_jumpsFile->Size = System::Drawing::Size(343, 20);
			this->info_jumpsFile->TabIndex = 1;
			this->info_jumpsFile->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &EveGui::info_jumpsFile_MouseUp);
			// 
			// timer_update
			// 
			this->timer_update->Enabled = true;
			this->timer_update->Interval = 500;
			this->timer_update->Tick += gcnew System::EventHandler(this, &EveGui::timer_update_Tick);
			// 
			// tab_analysis
			// 
			this->tab_analysis->Controls->Add(this->tabPage_jumps);
			this->tab_analysis->Controls->Add(this->tabPage_contracts);
			this->tab_analysis->Location = System::Drawing::Point(239, 143);
			this->tab_analysis->Name = L"tab_analysis";
			this->tab_analysis->SelectedIndex = 0;
			this->tab_analysis->Size = System::Drawing::Size(379, 301);
			this->tab_analysis->TabIndex = 17;
			// 
			// tabPage_jumps
			// 
			this->tabPage_jumps->Controls->Add(this->group_fileAnalysed);
			this->tabPage_jumps->Controls->Add(this->group_ROI);
			this->tabPage_jumps->Controls->Add(this->group_security);
			this->tabPage_jumps->Controls->Add(this->group_costPerUnit);
			this->tabPage_jumps->Controls->Add(this->group_infoPerUnit);
			this->tabPage_jumps->Controls->Add(this->group_jumps);
			this->tabPage_jumps->Controls->Add(this->group_profitPerUnit);
			this->tabPage_jumps->Controls->Add(this->group_profitPerTrip);
			this->tabPage_jumps->Controls->Add(this->group_trips);
			this->tabPage_jumps->Controls->Add(this->group_profit);
			this->tabPage_jumps->Controls->Add(this->group_volumePerTrip);
			this->tabPage_jumps->Controls->Add(this->group_jumpsToStart);
			this->tabPage_jumps->Controls->Add(this->group_profitIndex);
			this->tabPage_jumps->Controls->Add(this->group_unitsPerTrip);
			this->tabPage_jumps->Controls->Add(this->group_jumpsPerTrip);
			this->tabPage_jumps->Controls->Add(this->group_volumePerUnit);
			this->tabPage_jumps->Location = System::Drawing::Point(4, 22);
			this->tabPage_jumps->Name = L"tabPage_jumps";
			this->tabPage_jumps->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_jumps->Size = System::Drawing::Size(371, 275);
			this->tabPage_jumps->TabIndex = 0;
			this->tabPage_jumps->Text = L"Jumps";
			this->tabPage_jumps->UseVisualStyleBackColor = true;
			// 
			// tabPage_contracts
			// 
			this->tabPage_contracts->Location = System::Drawing::Point(4, 22);
			this->tabPage_contracts->Name = L"tabPage_contracts";
			this->tabPage_contracts->Padding = System::Windows::Forms::Padding(3);
			this->tabPage_contracts->Size = System::Drawing::Size(371, 275);
			this->tabPage_contracts->TabIndex = 1;
			this->tabPage_contracts->Text = L"Contracts";
			this->tabPage_contracts->UseVisualStyleBackColor = true;
			// 
			// check_ignoreJumpsPerTrip
			// 
			this->check_ignoreJumpsPerTrip->AutoSize = true;
			this->check_ignoreJumpsPerTrip->Location = System::Drawing::Point(6, 173);
			this->check_ignoreJumpsPerTrip->Name = L"check_ignoreJumpsPerTrip";
			this->check_ignoreJumpsPerTrip->Size = System::Drawing::Size(128, 17);
			this->check_ignoreJumpsPerTrip->TabIndex = 18;
			this->check_ignoreJumpsPerTrip->Text = L"Ignore Jumps per Trip";
			this->check_ignoreJumpsPerTrip->UseVisualStyleBackColor = true;
			this->check_ignoreJumpsPerTrip->CheckedChanged += gcnew System::EventHandler(this, &EveGui::check_ignoreJumpsPerTrip_CheckedChanged);
			// 
			// EveGui
			// 
			this->ClientSize = System::Drawing::Size(882, 454);
			this->Controls->Add(this->tab_analysis);
			this->Controls->Add(this->group_loadedFiles);
			this->Controls->Add(this->tab_menu);
			this->Controls->Add(this->group_analysedFiles);
			this->Name = L"EveGui";
			this->Text = L"Eve Market Analyser";
			this->tab_menu->ResumeLayout(false);
			this->tabPage_settings->ResumeLayout(false);
			this->group_settings->ResumeLayout(false);
			this->group_settings->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numeric_ignoreSellOrderVolumes))->EndInit();
			this->tabPage_conversion->ResumeLayout(false);
			this->group_convertDB->ResumeLayout(false);
			this->group_convertDB->PerformLayout();
			this->group_ROI->ResumeLayout(false);
			this->group_ROI->PerformLayout();
			this->group_security->ResumeLayout(false);
			this->group_security->PerformLayout();
			this->group_profitPerTrip->ResumeLayout(false);
			this->group_profitPerTrip->PerformLayout();
			this->group_analysedFiles->ResumeLayout(false);
			this->group_volumePerTrip->ResumeLayout(false);
			this->group_volumePerTrip->PerformLayout();
			this->group_unitsPerTrip->ResumeLayout(false);
			this->group_unitsPerTrip->PerformLayout();
			this->group_volumePerUnit->ResumeLayout(false);
			this->group_volumePerUnit->PerformLayout();
			this->group_jumpsPerTrip->ResumeLayout(false);
			this->group_jumpsPerTrip->PerformLayout();
			this->group_jumpsToStart->ResumeLayout(false);
			this->group_jumpsToStart->PerformLayout();
			this->group_fileAnalysed->ResumeLayout(false);
			this->group_fileAnalysed->PerformLayout();
			this->group_profitPerUnit->ResumeLayout(false);
			this->group_profitPerUnit->PerformLayout();
			this->group_infoPerUnit->ResumeLayout(false);
			this->group_infoPerUnit->PerformLayout();
			this->group_costPerUnit->ResumeLayout(false);
			this->group_costPerUnit->PerformLayout();
			this->group_jumps->ResumeLayout(false);
			this->group_jumps->PerformLayout();
			this->group_trips->ResumeLayout(false);
			this->group_trips->PerformLayout();
			this->group_profit->ResumeLayout(false);
			this->group_profit->PerformLayout();
			this->group_profitIndex->ResumeLayout(false);
			this->group_profitIndex->PerformLayout();
			this->group_loadedFiles->ResumeLayout(false);
			this->group_itemsFile->ResumeLayout(false);
			this->group_itemsFile->PerformLayout();
			this->group_systemsFile->ResumeLayout(false);
			this->group_systemsFile->PerformLayout();
			this->group_directoryWatched->ResumeLayout(false);
			this->group_directoryWatched->PerformLayout();
			this->group_jumpsFile->ResumeLayout(false);
			this->group_jumpsFile->PerformLayout();
			this->tab_analysis->ResumeLayout(false);
			this->tabPage_jumps->ResumeLayout(false);
			this->ResumeLayout(false);

		}
#pragma endregion
//private: System::Void check_maxJumpsToStart_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
//private: System::Void check_maxJumpsPerTip_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
//private: System::Void numeric_maxJumpsToStart_ValueChanged(System::Object^  sender, System::EventArgs^  e);
//private: System::Void numeric_maxJumpsPerTrip_ValueChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void text_shipVolume_TextChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void text_availableFunds_TextChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void timer_update_Tick(System::Object^  sender, System::EventArgs^  e);
private: System::Void info_directoryWatched_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void info_systemsFile_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void info_jumpsFile_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void info_itemsFile_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void check_availableFunds_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void check_shipVolume_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void button_clearAnalysedFiles_Click(System::Object^  sender, System::EventArgs^  e);
private: System::Void list_analysedFiles_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void check_ignoreJumpToStart_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void check_ignoreSellOrderVolume_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void numeric_ignoreSellOrderVolumes_ValueChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void list_analysedFiles_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void group_settings_Enter(System::Object^  sender, System::EventArgs^  e) {
				 }
private: System::Void check_useROI_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
private: System::Void check_ignoreJumpsPerTrip_CheckedChanged(System::Object^  sender, System::EventArgs^  e);
};
}
