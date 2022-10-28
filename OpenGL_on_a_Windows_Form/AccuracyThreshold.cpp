#include "stdafx.h"
#include "AccuracyThreshold.h"

// /*
System::Void CppCLRWinformsProjekt::AccuracyThreshold::submit(System::Object^ sender, System::EventArgs^ e)
{
	try
	{
		threshold = System::Convert::ToDouble(textBox->Text);

		if (threshold < 0 || threshold > 1)
			throw std::invalid_argument("threshold out of range");
	}
	catch (...)
	{
		textBox->Text = "Invalid Argument";
		threshold = 0.9;
		return;
	}

	this->Close();
}

System::Void CppCLRWinformsProjekt::AccuracyThreshold::cancel(System::Object^ sender, System::EventArgs^ e)
{
	threshold = 0.9;
	this->Close();
}

double CppCLRWinformsProjekt::AccuracyThreshold::getThreshold()
{
	return threshold;
}