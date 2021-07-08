#pragma once

#include "HistNormTrans.h"

namespace CppCLRWinformsProjekt {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Configuration;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Diagnostics;

	/// <summary>
	/// Zusammenfassung für Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Konstruktorcode hier hinzufügen.
			//
		}

	protected:
		/// <summary>
		/// Verwendete Ressourcen bereinigen.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	protected:
	private: System::Windows::Forms::ListView^ NormList;



	private: System::Windows::Forms::Panel^ panel1;
	private: System::Windows::Forms::Button^ btnRunNorm;
	private: System::Windows::Forms::CheckBox^ checkNyul;
	private: System::Windows::Forms::CheckBox^ checkLinear;
	private: System::Windows::Forms::CheckBox^ checkTumor;
	private: System::Windows::Forms::CheckBox^ checkBaba;
	private: System::Windows::Forms::Label^ labelBits;

	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TrackBar^ trackDepth;
	private: System::Windows::Forms::Label^ labelLambda25;

	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TrackBar^ trackpLO;
	private: System::Windows::Forms::TrackBar^ trackMiles;


	private: System::Windows::Forms::TrackBar^ trackLambda25;
	private: System::Windows::Forms::Label^ labelpLO;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ labelMilesText;
	private: System::Windows::Forms::Label^ labelMiles;


	private:
		/// <summary>
		/// Erforderliche Designervariable.
		/// </summary>
		System::ComponentModel::Container ^components;
	private: System::Windows::Forms::Panel^ panel2;
	private: System::Windows::Forms::Button^ btnSegm;
	private: System::Windows::Forms::RadioButton^ radioSVM;

	private: System::Windows::Forms::RadioButton^ radioKNN;

	private: System::Windows::Forms::RadioButton^ radioRF;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::TrackBar^ trackSize;
	private: System::Windows::Forms::TrackBar^ trackSVM;
	private: System::Windows::Forms::TrackBar^ trackKNN;
	private: System::Windows::Forms::TrackBar^ trackTreeDepth;
	private: System::Windows::Forms::TrackBar^ trackTrees;
	private: System::Windows::Forms::Label^ label15;
	private: System::Windows::Forms::Label^ label14;
	private: System::Windows::Forms::Label^ label13;
	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Panel^ panel3;
	private: System::Windows::Forms::Label^ label19;
	private: System::Windows::Forms::Label^ label18;
	private: System::Windows::Forms::Label^ label17;
	private: System::Windows::Forms::Label^ label16;
	private: System::Windows::Forms::TrackBar^ trackPage;

	private: System::Windows::Forms::TrackBar^ trackSlice;

	private: System::Windows::Forms::Button^ btnVisReset;

	private: System::Windows::Forms::Button^ btnVisualize;



		int nextID;
		Visualizer* vis;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Erforderliche Methode für die Designerunterstützung.
		/// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
		/// </summary>
		void InitializeComponent(void)
		{
			this->NormList = (gcnew System::Windows::Forms::ListView());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->labelMilesText = (gcnew System::Windows::Forms::Label());
			this->labelMiles = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->labelpLO = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->labelLambda25 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->trackpLO = (gcnew System::Windows::Forms::TrackBar());
			this->trackMiles = (gcnew System::Windows::Forms::TrackBar());
			this->trackLambda25 = (gcnew System::Windows::Forms::TrackBar());
			this->labelBits = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->trackDepth = (gcnew System::Windows::Forms::TrackBar());
			this->btnRunNorm = (gcnew System::Windows::Forms::Button());
			this->checkNyul = (gcnew System::Windows::Forms::CheckBox());
			this->checkLinear = (gcnew System::Windows::Forms::CheckBox());
			this->checkTumor = (gcnew System::Windows::Forms::CheckBox());
			this->checkBaba = (gcnew System::Windows::Forms::CheckBox());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->trackSVM = (gcnew System::Windows::Forms::TrackBar());
			this->trackKNN = (gcnew System::Windows::Forms::TrackBar());
			this->trackTreeDepth = (gcnew System::Windows::Forms::TrackBar());
			this->trackTrees = (gcnew System::Windows::Forms::TrackBar());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->trackSize = (gcnew System::Windows::Forms::TrackBar());
			this->radioSVM = (gcnew System::Windows::Forms::RadioButton());
			this->radioKNN = (gcnew System::Windows::Forms::RadioButton());
			this->radioRF = (gcnew System::Windows::Forms::RadioButton());
			this->btnSegm = (gcnew System::Windows::Forms::Button());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->trackPage = (gcnew System::Windows::Forms::TrackBar());
			this->trackSlice = (gcnew System::Windows::Forms::TrackBar());
			this->btnVisReset = (gcnew System::Windows::Forms::Button());
			this->btnVisualize = (gcnew System::Windows::Forms::Button());
			this->panel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackpLO))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackMiles))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackLambda25))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackDepth))->BeginInit();
			this->panel2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackSVM))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackKNN))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackTreeDepth))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackTrees))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackSize))->BeginInit();
			this->panel3->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackPage))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackSlice))->BeginInit();
			this->SuspendLayout();
			// 
			// NormList
			// 
			this->NormList->HideSelection = false;
			this->NormList->Location = System::Drawing::Point(12, 12);
			this->NormList->MultiSelect = false;
			this->NormList->Name = L"NormList";
			this->NormList->Size = System::Drawing::Size(539, 493);
			this->NormList->Sorting = System::Windows::Forms::SortOrder::Descending;
			this->NormList->TabIndex = 1;
			this->NormList->UseCompatibleStateImageBehavior = false;
			this->NormList->View = System::Windows::Forms::View::Details;
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->labelMilesText);
			this->panel1->Controls->Add(this->labelMiles);
			this->panel1->Controls->Add(this->label5);
			this->panel1->Controls->Add(this->labelpLO);
			this->panel1->Controls->Add(this->label4);
			this->panel1->Controls->Add(this->labelLambda25);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->trackpLO);
			this->panel1->Controls->Add(this->trackMiles);
			this->panel1->Controls->Add(this->trackLambda25);
			this->panel1->Controls->Add(this->labelBits);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->trackDepth);
			this->panel1->Controls->Add(this->btnRunNorm);
			this->panel1->Controls->Add(this->checkNyul);
			this->panel1->Controls->Add(this->checkLinear);
			this->panel1->Controls->Add(this->checkTumor);
			this->panel1->Controls->Add(this->checkBaba);
			this->panel1->Location = System::Drawing::Point(565, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(270, 306);
			this->panel1->TabIndex = 5;
			// 
			// labelMilesText
			// 
			this->labelMilesText->AutoSize = true;
			this->labelMilesText->Location = System::Drawing::Point(22, 150);
			this->labelMilesText->Name = L"labelMilesText";
			this->labelMilesText->Size = System::Drawing::Size(88, 13);
			this->labelMilesText->TabIndex = 17;
			this->labelMilesText->Text = L"1 20 40 60 80 99";
			// 
			// labelMiles
			// 
			this->labelMiles->AutoSize = true;
			this->labelMiles->Location = System::Drawing::Point(199, 136);
			this->labelMiles->Name = L"labelMiles";
			this->labelMiles->Size = System::Drawing::Size(28, 13);
			this->labelMiles->TabIndex = 16;
			this->labelMiles->Text = L"M05";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(196, 117);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(57, 13);
			this->label5->TabIndex = 15;
			this->label5->Text = L"Milestones";
			this->label5->Click += gcnew System::EventHandler(this, &Form1::label5_Click);
			// 
			// labelpLO
			// 
			this->labelpLO->AutoSize = true;
			this->labelpLO->Location = System::Drawing::Point(200, 193);
			this->labelpLO->Name = L"labelpLO";
			this->labelpLO->Size = System::Drawing::Size(33, 13);
			this->labelpLO->TabIndex = 14;
			this->labelpLO->Text = L"1.0 %";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(193, 175);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(27, 13);
			this->label4->TabIndex = 13;
			this->label4->Text = L"pLO";
			// 
			// labelLambda25
			// 
			this->labelLambda25->AutoSize = true;
			this->labelLambda25->Location = System::Drawing::Point(23, 88);
			this->labelLambda25->Name = L"labelLambda25";
			this->labelLambda25->Size = System::Drawing::Size(28, 13);
			this->labelLambda25->TabIndex = 12;
			this->labelLambda25->Text = L"0.40";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(6, 71);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(57, 13);
			this->label3->TabIndex = 11;
			this->label3->Text = L"Lambda25";
			// 
			// trackpLO
			// 
			this->trackpLO->Location = System::Drawing::Point(10, 179);
			this->trackpLO->Maximum = 30;
			this->trackpLO->Minimum = 1;
			this->trackpLO->Name = L"trackpLO";
			this->trackpLO->Size = System::Drawing::Size(176, 45);
			this->trackpLO->TabIndex = 10;
			this->trackpLO->Value = 10;
			this->trackpLO->Scroll += gcnew System::EventHandler(this, &Form1::trackpLO_Scroll);
			// 
			// trackMiles
			// 
			this->trackMiles->LargeChange = 2;
			this->trackMiles->Location = System::Drawing::Point(10, 117);
			this->trackMiles->Maximum = 12;
			this->trackMiles->Minimum = 1;
			this->trackMiles->Name = L"trackMiles";
			this->trackMiles->Size = System::Drawing::Size(176, 45);
			this->trackMiles->TabIndex = 9;
			this->trackMiles->Value = 5;
			this->trackMiles->Scroll += gcnew System::EventHandler(this, &Form1::trackMiles_Scroll);
			// 
			// trackLambda25
			// 
			this->trackLambda25->LargeChange = 4;
			this->trackLambda25->Location = System::Drawing::Point(83, 75);
			this->trackLambda25->Maximum = 48;
			this->trackLambda25->Minimum = 32;
			this->trackLambda25->Name = L"trackLambda25";
			this->trackLambda25->Size = System::Drawing::Size(184, 45);
			this->trackLambda25->TabIndex = 8;
			this->trackLambda25->Value = 40;
			this->trackLambda25->Scroll += gcnew System::EventHandler(this, &Form1::trackBar1_Scroll);
			// 
			// labelBits
			// 
			this->labelBits->AutoSize = true;
			this->labelBits->Location = System::Drawing::Point(27, 248);
			this->labelBits->Name = L"labelBits";
			this->labelBits->Size = System::Drawing::Size(13, 13);
			this->labelBits->TabIndex = 7;
			this->labelBits->Text = L"8";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(4, 230);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(80, 13);
			this->label2->TabIndex = 6;
			this->label2->Text = L"Color depth bits";
			// 
			// trackDepth
			// 
			this->trackDepth->LargeChange = 2;
			this->trackDepth->Location = System::Drawing::Point(80, 230);
			this->trackDepth->Minimum = 2;
			this->trackDepth->Name = L"trackDepth";
			this->trackDepth->Size = System::Drawing::Size(187, 45);
			this->trackDepth->TabIndex = 5;
			this->trackDepth->Value = 8;
			this->trackDepth->Scroll += gcnew System::EventHandler(this, &Form1::trackDepth_Scroll);
			// 
			// btnRunNorm
			// 
			this->btnRunNorm->Location = System::Drawing::Point(111, 276);
			this->btnRunNorm->Name = L"btnRunNorm";
			this->btnRunNorm->Size = System::Drawing::Size(75, 23);
			this->btnRunNorm->TabIndex = 4;
			this->btnRunNorm->Text = L"Start";
			this->btnRunNorm->UseVisualStyleBackColor = true;
			this->btnRunNorm->Click += gcnew System::EventHandler(this, &Form1::btnRunNorm_Click);
			// 
			// checkNyul
			// 
			this->checkNyul->AutoSize = true;
			this->checkNyul->Location = System::Drawing::Point(133, 40);
			this->checkNyul->Name = L"checkNyul";
			this->checkNyul->Size = System::Drawing::Size(85, 17);
			this->checkNyul->TabIndex = 3;
			this->checkNyul->Text = L"Nyúl method";
			this->checkNyul->UseVisualStyleBackColor = true;
			this->checkNyul->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkNyul_CheckedChanged);
			// 
			// checkLinear
			// 
			this->checkLinear->AutoSize = true;
			this->checkLinear->Checked = true;
			this->checkLinear->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkLinear->Location = System::Drawing::Point(4, 40);
			this->checkLinear->Name = L"checkLinear";
			this->checkLinear->Size = System::Drawing::Size(101, 17);
			this->checkLinear->TabIndex = 2;
			this->checkLinear->Text = L"Linear transform";
			this->checkLinear->UseVisualStyleBackColor = true;
			this->checkLinear->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkLinear_CheckedChanged);
			// 
			// checkTumor
			// 
			this->checkTumor->AutoSize = true;
			this->checkTumor->Location = System::Drawing::Point(133, 15);
			this->checkTumor->Name = L"checkTumor";
			this->checkTumor->Size = System::Drawing::Size(80, 17);
			this->checkTumor->TabIndex = 1;
			this->checkTumor->Text = L"BraTS data";
			this->checkTumor->UseVisualStyleBackColor = true;
			this->checkTumor->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkTumor_CheckedChanged);
			// 
			// checkBaba
			// 
			this->checkBaba->AutoSize = true;
			this->checkBaba->Checked = true;
			this->checkBaba->CheckState = System::Windows::Forms::CheckState::Checked;
			this->checkBaba->Location = System::Drawing::Point(4, 15);
			this->checkBaba->Name = L"checkBaba";
			this->checkBaba->Size = System::Drawing::Size(98, 17);
			this->checkBaba->TabIndex = 0;
			this->checkBaba->Text = L"iSeg-2017 data";
			this->checkBaba->UseVisualStyleBackColor = true;
			this->checkBaba->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBaba_CheckedChanged);
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->label15);
			this->panel2->Controls->Add(this->label14);
			this->panel2->Controls->Add(this->label13);
			this->panel2->Controls->Add(this->label12);
			this->panel2->Controls->Add(this->label11);
			this->panel2->Controls->Add(this->label10);
			this->panel2->Controls->Add(this->label9);
			this->panel2->Controls->Add(this->label8);
			this->panel2->Controls->Add(this->trackSVM);
			this->panel2->Controls->Add(this->trackKNN);
			this->panel2->Controls->Add(this->trackTreeDepth);
			this->panel2->Controls->Add(this->trackTrees);
			this->panel2->Controls->Add(this->label7);
			this->panel2->Controls->Add(this->label6);
			this->panel2->Controls->Add(this->trackSize);
			this->panel2->Controls->Add(this->radioSVM);
			this->panel2->Controls->Add(this->radioKNN);
			this->panel2->Controls->Add(this->radioRF);
			this->panel2->Controls->Add(this->btnSegm);
			this->panel2->Location = System::Drawing::Point(852, 12);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(270, 493);
			this->panel2->TabIndex = 6;
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(25, 414);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(19, 13);
			this->label15->TabIndex = 18;
			this->label15->Text = L"15";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(16, 397);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(74, 13);
			this->label14->TabIndex = 17;
			this->label14->Text = L"Ensemble size";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(28, 317);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(13, 13);
			this->label13->TabIndex = 16;
			this->label13->Text = L"5";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(20, 300);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(56, 13);
			this->label12->TabIndex = 15;
			this->label12->Text = L"Value of K";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(25, 227);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(19, 13);
			this->label11->TabIndex = 14;
			this->label11->Text = L"15";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(17, 210);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(78, 13);
			this->label10->TabIndex = 13;
			this->label10->Text = L"Max tree depth";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(25, 167);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(19, 13);
			this->label9->TabIndex = 12;
			this->label9->Text = L"50";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(16, 150);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(82, 13);
			this->label8->TabIndex = 11;
			this->label8->Text = L"Number of trees";
			// 
			// trackSVM
			// 
			this->trackSVM->Enabled = false;
			this->trackSVM->Location = System::Drawing::Point(93, 397);
			this->trackSVM->Maximum = 50;
			this->trackSVM->Minimum = 1;
			this->trackSVM->Name = L"trackSVM";
			this->trackSVM->Size = System::Drawing::Size(159, 45);
			this->trackSVM->TabIndex = 10;
			this->trackSVM->Value = 15;
			this->trackSVM->Scroll += gcnew System::EventHandler(this, &Form1::trackSVM_Scroll);
			// 
			// trackKNN
			// 
			this->trackKNN->Enabled = false;
			this->trackKNN->Location = System::Drawing::Point(93, 310);
			this->trackKNN->Maximum = 25;
			this->trackKNN->Minimum = 1;
			this->trackKNN->Name = L"trackKNN";
			this->trackKNN->Size = System::Drawing::Size(159, 45);
			this->trackKNN->TabIndex = 9;
			this->trackKNN->Value = 5;
			this->trackKNN->Scroll += gcnew System::EventHandler(this, &Form1::trackKNN_Scroll);
			// 
			// trackTreeDepth
			// 
			this->trackTreeDepth->Location = System::Drawing::Point(93, 216);
			this->trackTreeDepth->Maximum = 50;
			this->trackTreeDepth->Minimum = 5;
			this->trackTreeDepth->Name = L"trackTreeDepth";
			this->trackTreeDepth->Size = System::Drawing::Size(159, 45);
			this->trackTreeDepth->TabIndex = 8;
			this->trackTreeDepth->Value = 15;
			this->trackTreeDepth->Scroll += gcnew System::EventHandler(this, &Form1::trackTreeDepth_Scroll);
			// 
			// trackTrees
			// 
			this->trackTrees->LargeChange = 25;
			this->trackTrees->Location = System::Drawing::Point(93, 150);
			this->trackTrees->Maximum = 255;
			this->trackTrees->Minimum = 5;
			this->trackTrees->Name = L"trackTrees";
			this->trackTrees->Size = System::Drawing::Size(159, 45);
			this->trackTrees->SmallChange = 5;
			this->trackTrees->TabIndex = 7;
			this->trackTrees->Value = 50;
			this->trackTrees->Scroll += gcnew System::EventHandler(this, &Form1::trackTrees_Scroll);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(32, 34);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(25, 13);
			this->label7->TabIndex = 6;
			this->label7->Text = L"10k";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(13, 15);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(80, 13);
			this->label6->TabIndex = 5;
			this->label6->Text = L"Train Data Size";
			// 
			// trackSize
			// 
			this->trackSize->LargeChange = 4;
			this->trackSize->Location = System::Drawing::Point(93, 10);
			this->trackSize->Maximum = 12;
			this->trackSize->Name = L"trackSize";
			this->trackSize->Size = System::Drawing::Size(159, 45);
			this->trackSize->TabIndex = 4;
			this->trackSize->Value = 4;
			this->trackSize->Scroll += gcnew System::EventHandler(this, &Form1::trackSize_Scroll);
			// 
			// radioSVM
			// 
			this->radioSVM->AutoSize = true;
			this->radioSVM->Location = System::Drawing::Point(13, 369);
			this->radioSVM->Name = L"radioSVM";
			this->radioSVM->Size = System::Drawing::Size(189, 17);
			this->radioSVM->TabIndex = 3;
			this->radioSVM->Text = L"Support Vector Machine Ensemble";
			this->radioSVM->UseVisualStyleBackColor = true;
			this->radioSVM->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioSVM_CheckedChanged);
			// 
			// radioKNN
			// 
			this->radioKNN->AutoSize = true;
			this->radioKNN->Location = System::Drawing::Point(13, 276);
			this->radioKNN->Name = L"radioKNN";
			this->radioKNN->Size = System::Drawing::Size(123, 17);
			this->radioKNN->TabIndex = 2;
			this->radioKNN->Text = L"K Nearest Neighbors";
			this->radioKNN->UseVisualStyleBackColor = true;
			this->radioKNN->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioKNN_CheckedChanged);
			// 
			// radioRF
			// 
			this->radioRF->AutoSize = true;
			this->radioRF->Checked = true;
			this->radioRF->Location = System::Drawing::Point(13, 117);
			this->radioRF->Name = L"radioRF";
			this->radioRF->Size = System::Drawing::Size(97, 17);
			this->radioRF->TabIndex = 1;
			this->radioRF->TabStop = true;
			this->radioRF->Text = L"Random Forest";
			this->radioRF->UseVisualStyleBackColor = true;
			this->radioRF->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioRF_CheckedChanged);
			// 
			// btnSegm
			// 
			this->btnSegm->Location = System::Drawing::Point(93, 458);
			this->btnSegm->Name = L"btnSegm";
			this->btnSegm->Size = System::Drawing::Size(91, 23);
			this->btnSegm->TabIndex = 0;
			this->btnSegm->Text = L"Segmentation";
			this->btnSegm->UseVisualStyleBackColor = true;
			this->btnSegm->Click += gcnew System::EventHandler(this, &Form1::btnSegm_Click);
			// 
			// panel3
			// 
			this->panel3->Controls->Add(this->label19);
			this->panel3->Controls->Add(this->label18);
			this->panel3->Controls->Add(this->label17);
			this->panel3->Controls->Add(this->label16);
			this->panel3->Controls->Add(this->trackPage);
			this->panel3->Controls->Add(this->trackSlice);
			this->panel3->Controls->Add(this->btnVisReset);
			this->panel3->Controls->Add(this->btnVisualize);
			this->panel3->Location = System::Drawing::Point(565, 329);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(270, 176);
			this->panel3->TabIndex = 7;
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(9, 80);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(32, 13);
			this->label19->TabIndex = 7;
			this->label19->Text = L"Page";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(30, 53);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(19, 13);
			this->label18->TabIndex = 6;
			this->label18->Text = L"50";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(10, 36);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(68, 13);
			this->label17->TabIndex = 5;
			this->label17->Text = L"Slice number";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(10, 9);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(162, 13);
			this->label16->TabIndex = 4;
			this->label16->Text = L"Nothing selected for visualization";
			// 
			// trackPage
			// 
			this->trackPage->Enabled = false;
			this->trackPage->LargeChange = 2;
			this->trackPage->Location = System::Drawing::Point(83, 80);
			this->trackPage->Maximum = 9;
			this->trackPage->Name = L"trackPage";
			this->trackPage->Size = System::Drawing::Size(179, 45);
			this->trackPage->TabIndex = 3;
			this->trackPage->Scroll += gcnew System::EventHandler(this, &Form1::trackPage_Scroll);
			// 
			// trackSlice
			// 
			this->trackSlice->Enabled = false;
			this->trackSlice->Location = System::Drawing::Point(83, 36);
			this->trackSlice->Maximum = 154;
			this->trackSlice->Name = L"trackSlice";
			this->trackSlice->Size = System::Drawing::Size(179, 45);
			this->trackSlice->TabIndex = 2;
			this->trackSlice->Value = 50;
			this->trackSlice->Scroll += gcnew System::EventHandler(this, &Form1::trackSlice_Scroll);
			// 
			// btnVisReset
			// 
			this->btnVisReset->Enabled = false;
			this->btnVisReset->Location = System::Drawing::Point(158, 150);
			this->btnVisReset->Name = L"btnVisReset";
			this->btnVisReset->Size = System::Drawing::Size(75, 23);
			this->btnVisReset->TabIndex = 1;
			this->btnVisReset->Text = L"Switch off";
			this->btnVisReset->UseVisualStyleBackColor = true;
			this->btnVisReset->Click += gcnew System::EventHandler(this, &Form1::btnVisReset_Click);
			// 
			// btnVisualize
			// 
			this->btnVisualize->Location = System::Drawing::Point(25, 150);
			this->btnVisualize->Name = L"btnVisualize";
			this->btnVisualize->Size = System::Drawing::Size(75, 23);
			this->btnVisualize->TabIndex = 0;
			this->btnVisualize->Text = L"Visualize";
			this->btnVisualize->UseVisualStyleBackColor = true;
			this->btnVisualize->Click += gcnew System::EventHandler(this, &Form1::btnVisualize_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1127, 514);
			this->Controls->Add(this->panel3);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->NormList);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Name = L"Form1";
			this->Text = L"MRI Histogram Normalization";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackpLO))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackMiles))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackLambda25))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackDepth))->EndInit();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackSVM))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackKNN))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackTreeDepth))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackTrees))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackSize))->EndInit();
			this->panel3->ResumeLayout(false);
			this->panel3->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackPage))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackSlice))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e) {
		NormList->Columns->Add(L"Name",100);
		NormList->Columns->Add(L"Data", 80);
		NormList->Columns->Add(L"Algorithm", 100);
		NormList->Columns->Add(L"Parameters", 150);
		NormList->Columns->Add(L"Depth bits", 80);

		char buffer[20];
		nextID = 1000000;
		FILE* F = fopen("config.dat", "rt");
		if (F)
		{
			fgets(buffer, 20, F);
			fclose(F);
			sscanf(buffer, "%d", &nextID);
			if (nextID < 1000000) nextID = 1000000;
		}
		ReloadItems();
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
	}
private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void checkBaba_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	btnRunNorm->Enabled = (checkBaba->Checked || checkTumor->Checked) && (checkLinear->Checked || checkNyul->Checked);
}
private: System::Void checkLinear_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	btnRunNorm->Enabled = (checkBaba->Checked || checkTumor->Checked) && (checkLinear->Checked || checkNyul->Checked);
}
private: System::Void checkTumor_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	btnRunNorm->Enabled = (checkBaba->Checked || checkTumor->Checked) && (checkLinear->Checked || checkNyul->Checked);
}
private: System::Void checkNyul_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	btnRunNorm->Enabled = (checkBaba->Checked || checkTumor->Checked) && (checkLinear->Checked || checkNyul->Checked);
}
private: System::Void trackDepth_Scroll(System::Object^ sender, System::EventArgs^ e) {
	labelBits->Text = Convert::ToString(trackDepth->Value);
}
private: System::Void trackBar1_Scroll(System::Object^ sender, System::EventArgs^ e) {
	System::String^ msg = System::String::Format(L"{0,4:N2}", trackLambda25->Value * 0.01);
	labelLambda25->Text = msg; 
}
private: System::Void trackpLO_Scroll(System::Object^ sender, System::EventArgs^ e) {
	System::String^ msg = System::String::Format(L"{0,3:N1} %", trackpLO->Value * 0.1);
	labelpLO->Text = msg; 

	if (trackMiles->Value == 1) msg = "50";
	else if (trackMiles->Value == 2) msg = "25 75";
	else if (trackMiles->Value == 3) msg = "25 50 75";
	else if (trackMiles->Value == 4) msg = "10 50 90";
	else if (trackMiles->Value == 5) msg = "20 40 60 80";
	else if (trackMiles->Value == 6) msg = "10 25 75 90";
	else if (trackMiles->Value == 7) msg = "20 35 50 65 80";
	else if (trackMiles->Value == 8) msg = "10 25 50 75 90";
	else if (trackMiles->Value == 9) msg = "10 25 40 60 75 90";
	else if (trackMiles->Value == 10) msg = "10 25 40 50 60 75 90";
	else if (trackMiles->Value == 11) msg = "10 20 30 40 60 70 80 90";
	else if (trackMiles->Value == 12) msg = "10 20 30 40 50 60 70 80 90";
	msg = System::String::Format(L"{0:N1} {1} {2:N1}", trackpLO->Value * 0.1, msg, 100.0 - trackpLO->Value * 0.1);
	labelMilesText->Text = msg;
}
private: System::Void label5_Click(System::Object^ sender, System::EventArgs^ e) {
	HistNormLinear andi();
}
private: System::Void trackMiles_Scroll(System::Object^ sender, System::EventArgs^ e) {
	System::String^ msg = System::String::Format(L"M{0,2:D2}", trackMiles->Value);
	labelMiles->Text = msg; 
	if (trackMiles->Value == 1) msg = L"50";
	else if (trackMiles->Value == 2) msg = L"25 75";
	else if (trackMiles->Value == 3) msg = L"25 50 75";
	else if (trackMiles->Value == 4) msg = L"10 50 90";
	else if (trackMiles->Value == 5) msg = L"20 40 60 80";
	else if (trackMiles->Value == 6) msg = L"10 25 75 90";
	else if (trackMiles->Value == 7) msg = L"20 35 50 65 80";
	else if (trackMiles->Value == 8) msg = L"10 25 50 75 90";
	else if (trackMiles->Value == 9) msg = L"10 25 40 60 75 90";
	else if (trackMiles->Value == 10) msg = L"10 25 40 50 60 75 90";
	else if (trackMiles->Value == 11) msg = L"10 20 30 40 60 70 80 90";
	else if (trackMiles->Value == 12) msg = L"10 20 30 40 50 60 70 80 90";
	msg = System::String::Format(L"{0:N1} {1} {2:N1}", trackpLO->Value * 0.1, msg, 100.0 - trackpLO->Value * 0.1);
	labelMilesText->Text = msg;
}
private: System::Void btnRunNorm_Click(System::Object^ sender, System::EventArgs^ e) {
	if (checkBaba->Checked > 0 && checkLinear->Checked > 0)
	{
		HistNormLinear transform(nextID++, INFANT_DATA, trackDepth->Value, trackLambda25->Value * 0.01f);
		transform.run();
		FILE* F = fopen("config.dat", "wt");
		fprintf(F, "%d", nextID);
		fclose(F);
	}
	if (checkTumor->Checked > 0 && checkLinear->Checked > 0)
	{
		HistNormLinear transform(nextID++, TUMOR_DATA, trackDepth->Value, trackLambda25->Value * 0.01f);
		transform.run();
		FILE* F = fopen("config.dat", "wt");
		fprintf(F, "%d", nextID);
		fclose(F);
	}
	if (checkBaba->Checked > 0 && checkNyul->Checked > 0)
	{
		HistNormNyul transform(nextID++, INFANT_DATA, trackDepth->Value, trackpLO->Value * 0.001f, trackMiles->Value - 1);
		transform.run();
		FILE* F = fopen("config.dat", "wt");
		fprintf(F, "%d", nextID);
		fclose(F);
	}
	if (checkTumor->Checked > 0 && checkNyul->Checked > 0)
	{
		HistNormNyul transform(nextID++, TUMOR_DATA, trackDepth->Value, trackpLO->Value * 0.001f, trackMiles->Value - 1);
		transform.run();
		FILE* F = fopen("config.dat", "wt");
		fprintf(F, "%d", nextID);
		fclose(F);
	}

	
	ReloadItems();

/*	NormList->Items->Clear();
	char buffer[256];
	char strName[256];
	int ID, iData, alg, par1, par2, bits;
	F = fopen("normhistres/output.csv","rt");
	if (F)
	{
		while (!feof(F))
		{
			buffer[0] = 0;
			fgets(buffer, 255, F);
			if (strlen(buffer)>10)
			{ 
				int res = sscanf(buffer, "%d,%d,%d,%d,%d,%d", &ID, &iData, &alg, &par1, &par2, &bits);
				if (res >= 6)
				{
					ListViewItem^ item = NormList->Items->Add(Convert::ToString(ID));
					if (iData==1) item->SubItems->Add(L"Infant i-Seg");
					else if (iData == 2) item->SubItems->Add(L"Tumor BraTS");
					else item->SubItems->Add(L"Unknown");
					if (alg == 1)
					{
						item->SubItems->Add(L"Linear");
						System::String^ msg = System::String::Format(L"lambda25={0,4:N2}", par1 * 0.01);
						item->SubItems->Add(Convert::ToString(msg));
					}
					else if (alg == 2)
					{
						item->SubItems->Add(L"Nyúl");
						System::String^ msg = System::String::Format(L"pLO={0,3:N1}%, M{1:D2}", par1 * 0.1, par2+1);
						item->SubItems->Add(Convert::ToString(msg));
					}
					else
					{
						item->SubItems->Add(L"Unknown");
						item->SubItems->Add(L"N/A");
					}
					item->SubItems->Add(Convert::ToString(bits));
				}
			
			}
		}
		fclose(F);
	}*/
}
private: System::Void ReloadItems() {
	NormList->Items->Clear();
	char buffer[256];
	char strName[256];
	int ID, iData, alg, par1, par2, bits;
	FILE* F = fopen("normhistres/output.csv", "rt");
	if (F)
	{
		while (!feof(F))
		{
			buffer[0] = 0;
			fgets(buffer, 255, F);
			if (strlen(buffer) > 10)
			{
				int res = sscanf(buffer, "%d,%d,%d,%d,%d,%d", &ID, &iData, &alg, &par1, &par2, &bits);
				if (res >= 6)
				{
					ListViewItem^ item = NormList->Items->Add(Convert::ToString(ID));
					if (iData == 1) item->SubItems->Add(L"Infant i-Seg");
					else if (iData == 2) item->SubItems->Add(L"Tumor BraTS");
					else item->SubItems->Add(L"Unknown");
					if (alg == 1)
					{
						item->SubItems->Add(L"Linear");
						System::String^ msg = System::String::Format(L"lambda25={0,4:N2}", par1 * 0.01);
						item->SubItems->Add(Convert::ToString(msg));
					}
					else if (alg == 2)
					{
						item->SubItems->Add(L"Nyúl");
						System::String^ msg = System::String::Format(L"pLO={0,3:N1}%, M{1:D2}", par1 * 0.1, par2 + 1);
						item->SubItems->Add(Convert::ToString(msg));
					}
					else
					{
						item->SubItems->Add(L"Unknown");
						item->SubItems->Add(L"N/A");
					}
					item->SubItems->Add(Convert::ToString(bits));
				}

			}
		}
		fclose(F);
	}
}
private: System::Void btnSegm_Click(System::Object^ sender, System::EventArgs^ e) {
	if (NormList->SelectedItems->Count > 0)
	{
		int VOLTYPE = 3;
		int ALG = 0;
		ListViewItem^ item = NormList->SelectedItems[0];
		if (item->SubItems[1]->Text == L"Infant i-Seg") VOLTYPE = 1;
		else if (item->SubItems[1]->Text == L"Tumor BraTS") VOLTYPE = 2;
		if (radioRF->Checked) ALG = 1;
		else if (radioKNN->Checked) ALG = 2;
		else if (radioSVM->Checked) ALG = 3;

		int pos = trackSize->Value;
		int size = 1 + pos % 4;
		if (size == 4) ++size;
		for (int j = 0; j < pos / 4; ++j) size *= 10;

		int par1=0, par2=0;
		if (ALG == 1)
		{
			par1 = trackTrees->Value;
			par2 = trackTreeDepth->Value;
		}
		else if (ALG == 2)
			par1 = trackKNN->Value;
		else if (ALG == 3)
			par1 = trackSVM->Value;

		//item->SubItems[1]->Text;
		System::String^ msg = System::String::Format(L"{0:c} {1:N0} {2:N0} {3:#} {4:N0} {5:N0}", 
			item->Text, VOLTYPE, ALG, size, par1, par2);
//		System::String^ msg = System::String::Format(L"tester.exe");
//		button3->Text = msg;

		Process::Start(L"tester.exe", msg);
	}
//	else
//		button3->Text = L"Error...";
}
private: System::Void trackSize_Scroll(System::Object^ sender, System::EventArgs^ e) {
	int pos = trackSize->Value;
	int size = 1 + pos % 4;
	if (size == 4) ++size;
	for (int j = 0; j < pos / 4; ++j) size *= 10;
	System::String^ msg = System::String::Format(L"{0:#}k", size);
	label7->Text = msg;
}
private: System::Void radioRF_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	trackTrees->Enabled = true;
	trackTreeDepth->Enabled = true;
	trackKNN->Enabled = false;
	trackSVM->Enabled = false;
}
private: System::Void radioKNN_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	trackTrees->Enabled = false;
	trackTreeDepth->Enabled = false;
	trackKNN->Enabled = true;
	trackSVM->Enabled = false;
}
private: System::Void radioSVM_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
	trackTrees->Enabled = false;
	trackTreeDepth->Enabled = false;
	trackKNN->Enabled = false;
	trackSVM->Enabled = true;
}
private: System::Void trackTrees_Scroll(System::Object^ sender, System::EventArgs^ e) {
	label9->Text = Convert::ToString(trackTrees->Value);
}
private: System::Void trackTreeDepth_Scroll(System::Object^ sender, System::EventArgs^ e) {
	label11->Text = Convert::ToString(trackTreeDepth->Value);
}
//private: System::Void panel2_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {	
//}
private: System::Void trackSVM_Scroll(System::Object^ sender, System::EventArgs^ e) {
	label15->Text = Convert::ToString(trackSVM->Value);
}
private: System::Void trackKNN_Scroll(System::Object^ sender, System::EventArgs^ e) {
	label13->Text = Convert::ToString(trackKNN->Value);
}
private: System::Void btnVisualize_Click(System::Object^ sender, System::EventArgs^ e) {
	if (NormList->SelectedItems->Count > 0)
	{
		int VOLTYPE = 3;
		ListViewItem^ item = NormList->SelectedItems[0];
		if (item->SubItems[1]->Text == L"Infant i-Seg") VOLTYPE = 1;
		else if (item->SubItems[1]->Text == L"Tumor BraTS") VOLTYPE = 2;
		if (VOLTYPE == 1 || VOLTYPE == 2)
		{
			if (VOLTYPE == 1) trackSlice->Maximum = 106;
			else trackSlice->Maximum = 143;
			trackSlice->Value = 50;
			label18->Text = Convert::ToString(50);
			vis = new Visualizer(Convert::ToInt32(item->Text));

			btnVisReset->Enabled = true;
			trackSlice->Enabled = true;
			trackPage->Enabled = (VOLTYPE>1);
			trackPage->Value = 0;
			btnVisualize->Enabled = false;
		}
	}
};
private: System::Void btnVisReset_Click(System::Object^ sender, System::EventArgs^ e) {
	delete vis;
	btnVisReset->Enabled = false;
	trackSlice->Enabled = false;
	trackPage->Enabled = false;
	btnVisualize->Enabled = true;
}
private: System::Void trackSlice_Scroll(System::Object^ sender, System::EventArgs^ e) {
	label18->Text = Convert::ToString(trackSlice->Value);
	vis->ShowSlice(trackSlice->Value, trackPage->Value);
}
private: System::Void trackPage_Scroll(System::Object^ sender, System::EventArgs^ e) {
	vis->ShowSlice(trackSlice->Value, trackPage->Value);
}
private: System::Void button3_Click(System::Object^ sender, System::EventArgs^ e) {
}
};
}
