#pragma once

namespace CppCLRWinformsProjekt {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class AccuracyThreshold : public System::Windows::Forms::Form
	{
	public: AccuracyThreshold()
	{
		InitializeComponent();
	}

	protected:~AccuracyThreshold()
	{
		if (components)
		{
			delete components;
		}
	}

	private: System::ComponentModel::Container^ components;
	private: System::Windows::Forms::TextBox^ textBox;
	private: System::Windows::Forms::Label^ label;
	private: System::Windows::Forms::Button^ submitBtn;
	private: System::Windows::Forms::Button^ cancelBtn;
	private: double threshold = 0.9;

#pragma region

		   void InitializeComponent(void)
		   {
			   this->textBox = (gcnew System::Windows::Forms::TextBox());
			   this->label = (gcnew System::Windows::Forms::Label());
			   this->submitBtn = (gcnew System::Windows::Forms::Button());
			   this->cancelBtn = (gcnew System::Windows::Forms::Button());
			   this->SuspendLayout();

			   // 
			   // label
			   // 
			   this->label->Location = System::Drawing::Point(5, 30);
			   this->label->Name = L"Enter a hypercube purity threshold between [0,1]:";
			   this->label->Size = System::Drawing::Size(240, 30);
			   this->label->TabIndex = 1;
			   this->label->Text = L"Enter a hypercube purity threshold between [0,1]:";

			   // 
			   // textbox
			   // 
			   this->textBox->Location = System::Drawing::Point(25, 60);
			   this->textBox->Name = L"textbox";
			   this->textBox->Size = System::Drawing::Size(200, 30);
			   this->textBox->TabIndex = 1;
			   this->textBox->Text = L"";

			   // 
			   // cancel Button
			   // 
			   this->cancelBtn->Location = System::Drawing::Point(25, 90);
			   this->cancelBtn->Name = L"Cancel";
			   this->cancelBtn->Size = System::Drawing::Size(100, 40);
			   this->cancelBtn->TabIndex = 1;
			   this->cancelBtn->Text = L"Cancel";
			   this->cancelBtn->UseVisualStyleBackColor = true;
			   this->cancelBtn->Click += gcnew System::EventHandler(this, &AccuracyThreshold::cancel);

			   // 
			   // submit Button
			   // 
			   this->submitBtn->Location = System::Drawing::Point(125, 90);
			   this->submitBtn->Name = L"Submit";
			   this->submitBtn->Size = System::Drawing::Size(100, 40);
			   this->submitBtn->TabIndex = 1;
			   this->submitBtn->Text = L"Submit";
			   this->submitBtn->UseVisualStyleBackColor = true;
			   this->submitBtn->Click += gcnew System::EventHandler(this, &AccuracyThreshold::submit);

			   // 
			   // Form1
			   // 
			   this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			   this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			   this->ClientSize = System::Drawing::Size(250, 150);
			   this->Controls->Add(this->label);
			   this->Controls->Add(this->textBox);
			   this->Controls->Add(this->submitBtn);
			   this->Controls->Add(this->cancelBtn);
			   this->Name = L"Enter Hypercube Threshold";
			   this->Text = L"Enter Hypercube Threshold";
			   this->ResumeLayout(false);
			   this->MaximizeBox = false;
			   this->MinimizeBox = false;
			   this->CenterToScreen();
		   }

#pragma endregion

	private: System::Void submit(System::Object^ sender, System::EventArgs^ e);
	private: System::Void cancel(System::Object^ sender, System::EventArgs^ e);
	public: double getThreshold();
	};
}
