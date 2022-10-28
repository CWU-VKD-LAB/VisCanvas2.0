#include "stdafx.h"
#include "pch.h"
#include "EmptySpotHBPicker.h"
#include <iostream>
#include <fstream> 
#include <msclr\marshal_cppstd.h>
#include <vector>
#include "KVPair.h"
#include "Scheme.h"
#include "ClassScheme.h"
#include "OpenGL.h"
#include <cstdlib>


System::Void CppCLRWinformsProjekt::EmptySpotHBPicker::drawTables() {

	//Read data:
	std::stringstream test(*esHBs);
	std::string segment;
	std::vector<std::string> seglist;

	numRows = -1;
	numDimensions = 0;
	int curColumn = 0;
	this->dataGridView->RowCount = 1;
	this->dataGridView->ColumnCount = 1;

	//Get num of columns:
	getline(test, segment);
	for (std::size_t i = 0; i < segment.length(); i++) if (segment[i] == ',') numDimensions++;

	//Add columns:
	this->dataGridView->ColumnCount = numDimensions;

	//Name columns:
	int pos = 0;
	int curCell = 0;
	std::string token;
	std::vector<std::string> colNames = std::vector<std::string>();
	while ((pos = segment.find(",")) != std::string::npos)
	{
		//Get cur token:
		token = segment.substr(0, pos);
		this->dataGridView->Columns[curCell]->Name = gcnew String(token.c_str());
		colNames.push_back(token);
		//Erase used data and move to next cell:
		segment.erase(0, pos + 1);
		curCell++;
	}

	//this->dataGridView->Columns[numDimensions + 1]->Name = ("class");
	
	//Generate rows:
	while (getline(test, segment))
	{
		this->dataGridView->Rows->Add();
		numRows++;
	}

	std::stringstream test1(*esHBs);
	// strip column names from data
	getline(test1, segment);
	//int rowSpan = 0;
	int esCase = -1;
	int maxInd = -1;
	double max = -1.0;
	int start = 0;
	int prev = -1;
	std::vector<int> recInds = std::vector<int>();
	//Fill rows:
	for (int i = 0; i <= numRows; i++)
	{
		//Get data from current line:
		getline(test1, segment);

		//Identify row:
		DataGridViewRow^ r = this->dataGridView->Rows[i];

		//Parse String and set values:
		int pos = 0;
		int curCell = 0;
		std::string token;
		while ((pos = segment.find(",")) != std::string::npos)
		{
			//Get cur token:
			token = segment.substr(0, pos);
			if (curCell == 0 && esCase != std::stoi(token)) {
				esCase = std::stoi(token);
			}
			r->Cells[gcnew String(colNames[curCell].c_str())]->Value = gcnew String(token.c_str());
			//Erase used data and move to next cell:
			segment.erase(0, pos + 1);
			curCell++;
		}
	
		// Checks ESCase index.  If it changes add saved rows to dataGridView with highlighted recommendations.
		if (curCell == 1 && esCase > -1 ) {
			prev = esCase;
			// Converts ES Case to int and saves in esCase
			try {
				esCase = std::stoi(msclr::interop::marshal_as<std::string>(r->Cells[0]->Value->ToString()));
			}
			catch (...) {
				// First row of each ESCase should be just a number but in case it has a space at the end.
				std::string tmp = msclr::interop::marshal_as<std::string>(r->Cells[0]->Value->ToString());
				tmp.resize(tmp.length() - 1);
				esCase = std::stoi(tmp);
			}

			// Colors first row of each ESCase
			for (int j = 0; j < dataGridView->ColumnCount; j++) {
				r->Cells[j]->Style->BackColor = Color::LightGray;
				
			}
			if (maxInd > -1) {
				recInds.push_back(maxInd);
				for (int j = 0; j < dataGridView->ColumnCount; j++) {
					this->dataGridView->Rows[maxInd]->Cells[j]->Style->BackColor = Color::LightGreen;
					this->dataGridView->Rows[maxInd]->Selected = true;
				}
			}
			max = -1.0;
			maxInd = -1;
		} // If ESCase does not change, check compatability and update recommendation
		else {
			double compat = std::stod(msclr::interop::marshal_as<std::string>(r->Cells[" Compatibility %"]->Value->ToString()));
			if (std::stoi(msclr::interop::marshal_as<std::string>(r->Cells[" ES Case Class"]->Value->ToString())) !=
				std::stoi(msclr::interop::marshal_as<std::string>(r->Cells[" HB Dominant Class"]->Value->ToString()))) {
				// Multiply HB purity by an arbitrary threshold to favor higher purity over higher compatability. 
				// 1.0 would signify no bias.
				compat += 100 - .7 * std::stod(msclr::interop::marshal_as<std::string>(r->Cells[" HB Dominant Class Purity %"]->Value->ToString()));
			}
			else {
				compat += std::stod(msclr::interop::marshal_as<std::string>(r->Cells[" HB Dominant Class Purity %"]->Value->ToString()));
			}
			if (compat > max) {
				max = compat;
				maxInd = i;
			}
		}
	}
	// Highlight recommended for last ESCase
	if (maxInd > -1) {
		recInds.push_back(maxInd);
		for (int j = 0; j < dataGridView->ColumnCount; j++) {
			this->dataGridView->Rows[maxInd]->Cells[j]->Style->BackColor = Color::LightGreen;
			this->dataGridView->Rows[maxInd]->Selected = true;
		}
	}
	this->dataGridView->Rows[0]->Selected = false;
}

// ===submitOrderButton_Click===:
// Desc: Submits selections/changes made to table.
System::Void CppCLRWinformsProjekt::EmptySpotHBPicker::submitButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	Int32 selectedRowCount = this->dataGridView->SelectedRows->Count;
	if (!selectedRowCount > 0) {
	//	// Display Error message for too few selections. No selections were made.
	}
	//else if (0 && selectedRowCount <= emptys->size()) {
	//	// Display Error Message for too many selections
	//	//MessageBox.show("Some Text", "Some title", MessageBoxButtons.OK, MessageBoxIcon.Error);
	//}
	else
	{
		int hbind = 0;
		int esind = 0;
		int e = 0;
		std::vector<int> esinds;
		for (int i; i < this->emptyhbs->size(); i++) {
			this->emptyhbs->at(i) = -1;
		}

		for (int i = 0; i < selectedRowCount; i++)
		{
			// index of selected row mod (number of hbs + 1) gives index of empty spot
			e = (int)this->dataGridView->SelectedRows[i]->Index % ((int)this->dataGridView->RowCount / emptys->size()) + 1 + i;
			esind = std::stoi(msclr::interop::marshal_as<std::string>(this->dataGridView->SelectedRows[i]->Cells["ES Case"]->Value->ToString()));
			// (index of selected row mod number of cases) - 1 gives index of hyperblock
			for (int j = 0; j < emptys->size(); j++) {
				if (emptys->at(j).at(0) == (double)esind) {
					e = j;
				}
			}

			hbind = std::stoi(msclr::interop::marshal_as<std::string>(this->dataGridView->SelectedRows[i]->Cells[" HB Name"]->Value->ToString()));
			if (hbind >= 0 && std::find(esinds.begin(), esinds.end(), esind) == esinds.end()) {
				this->emptyhbs->at(e) = hbind;
			}
		}
		submit(this->parent);
	}
}

// ===visualizeButton_Click===:
// Desc: Visualizes clusters with selected emtpy spots added
System::Void CppCLRWinformsProjekt::EmptySpotHBPicker::visualizeButton_Click(System::Object^ sender, System::EventArgs^ e)
{
	Int32 selectedRowCount = this->dataGridView->SelectedRows->Count;
	if (!selectedRowCount > 0) {
		//	// Display Error message for too few selections. No selections were made.
	}
	//else if (0 && selectedRowCount <= emptys->size()) {
	//	// Display Error Message for too many selections
	//	//MessageBox.show("Some Text", "Some title", MessageBoxButtons.OK, MessageBoxIcon.Error);
	//}
	else
	{
		int hbind = 0;
		int esind = 0;
		int e = 0;
		std::vector<int> esinds;
		for (int i; i < this->emptyhbs->size(); i++) {
			this->emptyhbs->at(i)=-1;
		}
		for (int i = 0; i < selectedRowCount; i++)
		{
			// index of selected row mod (number of hbs + 1) gives index of empty spot
			e = (int)this->dataGridView->SelectedRows[i]->Index % ((int)this->dataGridView->RowCount / emptys->size()) + i;
			esind = std::stoi(msclr::interop::marshal_as<std::string>(this->dataGridView->SelectedRows[i]->Cells["ES Case"]->Value->ToString()));
			for (int j = 0; j < emptys->size(); j++) {
				if (emptys->at(j).at(0) == (double)esind) {
					e = j;
				}
			}

			// Finds HB Name
			hbind = std::stoi(msclr::interop::marshal_as<std::string>(this->dataGridView->SelectedRows[i]->Cells[" HB Name"]->Value->ToString()));
			if (hbind >= 0 && std::find(esinds.begin(), esinds.end(), esind) == esinds.end()) {
				this->emptyhbs->at(e) = hbind;
			}
		}
		visualize(this->parent);
	}
}

// ===sortAscending===:
// Desc: sort.
System::Void CppCLRWinformsProjekt::EmptySpotHBPicker::submit(OpenGLForm::COpenGL^ sender)//, std::vector<int> eshbs)
{
	sender->updateEmptys(0, *emptyhbs);
	this->Close();
}

// ===sortdescending===:
// Desc: sort.
System::Void CppCLRWinformsProjekt::EmptySpotHBPicker::visualize(OpenGLForm::COpenGL^ sender)
{
	sender->updateEmptys(1, *emptyhbs);
}