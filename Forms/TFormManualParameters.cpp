//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormManualParameters.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormManualParameters *FormManualParameters;
//---------------------------------------------------------------------------
__fastcall TFormManualParameters::TFormManualParameters(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormManualParameters::FormCreate(TObject *Sender)
{
	//MinShiftsGrid
	MinShiftsGrid->Cells[0][0] = "Наименование";
	MinShiftsGrid->Cells[1][0] = "Значение";
	//ModuleGrid
	ModuleGrid->Cells[0][0] = "Наименование";
	ModuleGrid->Cells[1][0] = "Значение";
	//TeethGrid
	TeethGrid->Cells[0][0] = "Наименование";
	TeethGrid->Cells[1][0] = "Значение";
	//WidthGrid
	WidthGrid->Cells[0][0] = "Диаметр ролика";
	for (int j = 0; j < 4; j++) {
		WidthGrid->Cells[0][j+1] = Dr[j];
	}

	for (int i = 0; i < gears_id.size(); i++) {
		MinShiftsGrid->Cells[0][i+1] = gears_id[i];
		ModuleGrid->Cells[0][i+1] = gears_id[i];
		TeethGrid->Cells[0][i+1] = gears_id[i];
		WidthGrid->Cells[i+1][0] = gears_id[i];
	}

	DefaultSizeComboBox->Items->Add("9.6.1.");
	DefaultSizeComboBox->Items->Add("9.6.2.");
	DefaultSizeComboBox->Items->Add("9.6.3.");

	FormManualParameters->updateParamGrids();
}
//---------------------------------------------------------------------------

void resizeGrid(TStringGrid *Grid, bool need_full_resize) {
	vector<int> maxWidth(Grid->ColCount, 0);
	vector<int> maxHeight(Grid->RowCount, 0);
	for (int ACol = 0; ACol < Grid->ColCount; ACol++) {
		for (int ARow = 0; ARow < Grid->RowCount; ARow++) {
			if (maxWidth.at(ACol) < Grid->Canvas->TextWidth(Grid->Cells[ACol][ARow])) {
				maxWidth[ACol] = Grid->Canvas->TextWidth(Grid->Cells[ACol][ARow]);
			}
			if (maxHeight.at(ARow) < Grid->Canvas->TextHeight(Grid->Cells[ACol][ARow])) {
				maxHeight[ARow] = Grid->Canvas->TextHeight(Grid->Cells[ACol][ARow]);
			}
		}
	}
	for (int ACol = 0; ACol < Grid->ColCount; ACol++) {
		Grid->ColWidths[ACol] = maxWidth.at(ACol) + 10;
	}
	for (int ARow = 0; ARow < Grid->RowCount; ARow++) {
		Grid->RowHeights[ARow] = maxHeight.at(ARow) + 10;
	}
}
void __fastcall TFormManualParameters::AbortButtonClick(TObject *Sender)
{
	FormManualParameters->Hide();
}
//---------------------------------------------------------------------------
void __fastcall TFormManualParameters::SaveButtonClick(TObject *Sender)
{
	Of.clear();
	for (int i = 0; i < MinShiftsGrid->RowCount-1; i++) {
		Of.push_back((MinShiftsGrid->Cells[1][i+1]).ToDouble());
	}
	M.clear();
	for (int i = 0; i < ModuleGrid->RowCount-1; i++) {
		M.push_back((ModuleGrid->Cells[1][i+1]).ToDouble());
	}
	N.clear();
	for (int i = 0; i < TeethGrid->RowCount-1; i++) {
		N.push_back((TeethGrid->Cells[1][i+1]).ToDouble());
	}

	/*
    for (int i = 0; i < gears_id.size(); i++) {
		//map<double, pair<double, double>> Single_PGTS = RollerSizes[gears_id[i]];
		for (int j = 0; j < Dr.size(); j++) {
			auto iter = RollerSizes[gears_id[i]].begin();
			while (iter != RollerSizes[gears_id[i]].end()) {
				if (iter->second.second == Dr[j]) {
					iter->second = pair((WidthGrid->Cells[i+1][j+1]).ToDouble(), Dr[j]);
					break;
				} else {
					++iter;
				}
			}
		}
	}*/

	for (int i = 1; i < WidthGrid->ColCount; i++) {
		//map<double, pair<double, double>> Single_PGTS = RollerSizes[gears_id[i]];
		for (int j = 1; j < WidthGrid->RowCount; j++) {
			auto iter = RollerSizes[WidthGrid->Cells[i][0]].begin();
			while (iter != RollerSizes[WidthGrid->Cells[i][0]].end()) {
				if (iter->second.second == WidthGrid->Cells[0][j]) {
					iter->second = pair((WidthGrid->Cells[i][j]).ToDouble(), (WidthGrid->Cells[0][j]).ToDouble());
					break;
				} else {
					++iter;
				}
			}
		}
	}

	/*
	PGTS44_07.clear();
	PGTS34_15.clear();
	PGTS64_05.clear();
	PGTS34_16.clear();
	PGTS64_06.clear();
	PGTS34_14.clear();
	for (int j = 0; j < 3; j++) {
		PGTS44_07.push_back(WidthGrid->Cells[1][j+1].ToDouble());
	}
	for (int j = 0; j < 3; j++) {
		PGTS34_15.push_back(WidthGrid->Cells[2][j+1].ToDouble());
	}
	for (int j = 0; j < 3; j++) {
		PGTS64_05.push_back(WidthGrid->Cells[3][j+1].ToDouble());
	}
	for (int j = 0; j < 3; j++) {
		PGTS34_16.push_back(WidthGrid->Cells[4][j+1].ToDouble());
	}
	for (int j = 0; j < 3; j++) {
		PGTS64_06.push_back(WidthGrid->Cells[5][j+1].ToDouble());
	}
	for (int j = 0; j < 3; j++) {
		PGTS34_14.push_back(WidthGrid->Cells[6][j+1].ToDouble());
	}
	*/

	baseMeasure = DefaultSizeComboBox->Text;

    FormManualParameters->Hide();
}
//---------------------------------------------------------------------------

void TFormManualParameters::updateParamGrids(void) {
    for (int i = 0; i < Of.size(); i++) {
		MinShiftsGrid->Cells[1][i+1] = Of[i];
	}
	for (int i = 0; i < M.size(); i++) {
		ModuleGrid->Cells[1][i+1] = M[i];
	}
	for (int i = 0; i < N.size(); i++) {
		TeethGrid->Cells[1][i+1] = N[i];
	}
	for (int i = 0; i < gears_id.size(); i++) {
		map<double, pair<double, double>> Single_PGTS = RollerSizes[gears_id[i]];
		for (int j = 0; j < Dr.size(); j++) {
			auto iter = Single_PGTS.begin();
			while (iter != Single_PGTS.end()) {
				if (iter->second.second == Dr[j]) {
					WidthGrid->Cells[i+1][j+1] = iter->second.first;
					break;
				} else {
					++iter;
				}
			}
		}
	}
	/*
	for (int j = 0; j < 3; j++) {
		WidthGrid->Cells[1][j+1] = PGTS44_07[j];
	}
	for (int j = 0; j < 3; j++) {
		WidthGrid->Cells[2][j+1] = PGTS34_15[j];
	}
	for (int j = 0; j < 3; j++) {
		WidthGrid->Cells[3][j+1] = PGTS64_05[j];
	}
	for (int j = 0; j < 3; j++) {
		WidthGrid->Cells[4][j+1] = PGTS34_16[j];
	}
	for (int j = 0; j < 3; j++) {
		WidthGrid->Cells[5][j+1] = PGTS64_06[j];
	}
	for (int j = 0; j < 3; j++) {
		WidthGrid->Cells[6][j+1] = PGTS34_14[j];
	}
	for (int j = 0; j < 3; j++) {
		WidthGrid->Cells[7][4] = PGTS64_07[0];
	}
	for (int j = 0; j < 3; j++) {
		WidthGrid->Cells[8][4] = PGTS24_06[0];
	}
    */

	for (int i = 0; i < DefaultSizeComboBox->Items->Count; i++) {
		if (baseMeasure == DefaultSizeComboBox->Items->Strings[i]) {
			DefaultSizeComboBox->ItemIndex = i;
		}
	}

	resizeGrid(MinShiftsGrid, 1);
	resizeGrid(ModuleGrid, 1);
	resizeGrid(TeethGrid, 1);
	resizeGrid(WidthGrid, 1);
}
//---------------------------------------------------------------------------
