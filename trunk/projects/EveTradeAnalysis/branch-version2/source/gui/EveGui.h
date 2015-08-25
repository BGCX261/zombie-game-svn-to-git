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














					 AutoMatic::Eve::MarketAnalyser * _marketAnalyser;

	public:
		EveGui(void);

	public:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~EveGui();

		void createJumps(void);

	private: System::Windows::Forms::FolderBrowserDialog^  openFolder;








	private: System::Windows::Forms::GroupBox^  group_loadedFiles;
	private: System::Windows::Forms::GroupBox^  group_directoryWatched;
	private: System::Windows::Forms::TextBox^  info_directoryWatched;


	private: System::Windows::Forms::GroupBox^  group_jumpsFile;
	private: System::Windows::Forms::TextBox^  info_jumpsFile;
	private: System::Windows::Forms::GroupBox^  group_itemsFile;


	private: System::Windows::Forms::TextBox^  info_itemsFile;

	private: System::Windows::Forms::GroupBox^  group_systemsFile;
	private: System::Windows::Forms::TextBox^  info_systemsFile;



	private: System::Windows::Forms::Timer^  timer_update;

























	private: System::Windows::Forms::OpenFileDialog^  openFile;































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
			this->openFile = (gcnew System::Windows::Forms::OpenFileDialog());
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
			this->group_loadedFiles->SuspendLayout();
			this->group_itemsFile->SuspendLayout();
			this->group_systemsFile->SuspendLayout();
			this->group_directoryWatched->SuspendLayout();
			this->group_jumpsFile->SuspendLayout();
			this->SuspendLayout();
			// 
			// openFile
			// 
			this->openFile->FileName = L"openFile";
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
			// EveGui
			// 
			this->ClientSize = System::Drawing::Size(741, 456);
			this->Controls->Add(this->group_loadedFiles);
			this->Name = L"EveGui";
			this->Text = L"Eve Market Analyser";
			this->group_loadedFiles->ResumeLayout(false);
			this->group_itemsFile->ResumeLayout(false);
			this->group_itemsFile->PerformLayout();
			this->group_systemsFile->ResumeLayout(false);
			this->group_systemsFile->PerformLayout();
			this->group_directoryWatched->ResumeLayout(false);
			this->group_directoryWatched->PerformLayout();
			this->group_jumpsFile->ResumeLayout(false);
			this->group_jumpsFile->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
private: System::Void timer_update_Tick(System::Object^  sender, System::EventArgs^  e);
private: System::Void info_directoryWatched_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void info_systemsFile_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void info_jumpsFile_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
private: System::Void info_itemsFile_MouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e);
};
}
