﻿// ---------------------------------------------------------------------------

#pragma hdrstop

#include "TFileLoader.h"

#include <ComObj.hpp>
#include <Vcl.OleCtrls.hpp>
#include <Vcl.FileCtrl.hpp>
#include "objbase.h"
#include <math.h>

#include "TFormAddDataFiles.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

double CtR = M_PI / 180;

vector<double>M = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.8, 0.8};
vector<int>N = {16, 70, 19, 67, 16, 70, 14, 50};
vector<double>Dr = {0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 1.5, 1.5};
vector<double>Of = {0.022, 0.032, 0.022, 0.032, 0.022, 0.032, 0.026, 0.038};
vector<double>Tol = {
	0.093 - 0.035, 0.15 - 0.073, 0.082 - 0.038, 0.142 - 0.07, 0.08 - 0.035,
	0.14 - 0.073, 0.066 - 0.039, 0.119 - 0.079};

vector<UnicodeString>gears_id = {
	"ПГТС.721144.007", "ПГТС.721134.015", "ПГТС.721164.005", "ПГТС.721134.016",
	"ПГТС.721164.006", "ПГТС.721134.014", "ПГТС.721164.007", "ПГТС.721124.006"};

/*
 class TFileLoader {
 public:
 TFileLoader();
 ~TFileLoader();

 TStringList* pFileNames;

 void vAppendFile(UnicodeString sNewFile);
 void vDeleteFile(int iDeleteFileIndex);
 int iGetCountFiles();
 UnicodeString sGetFile(int iNumber);

 private:
 };
 */
TFileList::TFileList() {
	pFileNames = new TStringList();
}

TFileList::~TFileList() {
	delete pFileNames;
}

void TFileList::vAppendFile(UnicodeString sNewFile) {
	pFileNames->Append(sNewFile);
}

void TFileList::vDeleteFile(int iDeleteFileIndex) {
	if ((iDeleteFileIndex >= 0) && (iDeleteFileIndex < pFileNames->Count)) {
		pFileNames->Delete(iDeleteFileIndex);
	}
}

int TFileList::iGetCountFiles() {
	return pFileNames->Count;
}

UnicodeString TFileList::sGetFile(int iIndex) {
	if ((iIndex >= 0) && (iIndex < pFileNames->Count)) {
		return pFileNames->Strings[iIndex];
	}
	else {
		return "";
	}
}

void TFileList::vShowFilesListBox(TListBox* lsb) {
	lsb->Clear();
	for (int i = 0; i < pFileNames->Count; i++) {
		lsb->Items->Add(pFileNames->Strings[i]);
	}

}

////////////////////////////////////////////////////////////
////

Variant ExcelApp1, ExcelBooks1, Book1, Sheets1, ExcelApp, ExcelBooks, ExcelBook,
	ExcelSheet;
TJSONObject *o = new TJSONObject();
TJSONArray *Gearboxes = new TJSONArray();

void vLoadGearsFromExcel(TList* suspGearList, TList* stanGearList,
	TList* goodGearList, AnsiString FileName, TMemo* memoLog, TMemo* memoInfo) {
	ExcelApp = CreateOleObject("Excel.Application");

	try {
		ExcelApp.OlePropertySet("Visible", false); // setup Show Excel.
		ExcelBook = ExcelApp.OlePropertyGet("Workbooks").OleFunction("Open",
			WideString(FileName)); // Open Excel File.
		ExcelSheet = ExcelBook.OlePropertyGet("WorkSheets").OlePropertyGet
			("Item", 1);
		memoInfo->Lines->Add(Format("Обрабатываю файл %s",
			WideString(FileName)));

		int iRowsCount = ExcelApp.OlePropertyGet("ActiveSheet").OlePropertyGet
			("UsedRange").OlePropertyGet("Rows").OlePropertyGet("Count");
		int iColsCount = ExcelApp.OlePropertyGet("ActiveSheet").OlePropertyGet
			("UsedRange").OlePropertyGet("Columns").OlePropertyGet("Count");

		// First of all we parse PGTS////////////////////////////////////////////
		Variant vCells = ExcelSheet.OlePropertyGet("Cells");
		AnsiString detailNum = vCells.OlePropertyGet("Item", 5, 3);
		TDesignation PGTS(detailNum);
		////////////////////////////////////////////////////////////////////////

		// Secondly suggesting to start with order № since its common for this///
		// sheet/////////////////////////////////////////////////////////////////
		AnsiString sOrderNum = vCells.OlePropertyGet("Item", 3, 3);
		unsigned int orderNum = sOrderNum.ToInt();
		UnicodeString sGearName = vCells.OlePropertyGet("Item", 3, 12);
		sGearName = correctName(sGearName);
		////////////////////////////////////////////////////////////////////////

		// Finally we come to gears characteristics & gears themselves///////////     "ПГТС.721164.006"
		vector<unsigned int>filledMeasuarments;

		getFilledMeasurementRows(iRowsCount, 2, &filledMeasuarments);

		int quantity = 0;

		// All gears in sheet cycle
		for (int i = 15; i < iColsCount; i++) {
			UnicodeString gearNumber = vCells.OlePropertyGet("Item", 9, i);
			int color = vCells.OlePropertyGet("Item", 9, i).OlePropertyGet
				("Interior").OlePropertyGet("ColorIndex");
			// && color != 6
			if (isCellFilled(gearNumber) && color != 6) {
				TGear* newGear = new TGear(PGTS, orderNum, sGearName,
				gearNumber);
				TList* listparams = new TList();
				// fun for taking all measurements
				fillGearMeasurments(&filledMeasuarments, listparams, i);
				newGear->listParams = listparams;
				switch (checkMasurementsData(listparams)) {
				case 0:
					suspGearList->Add(newGear);
					break;
				case 1:
					quantity++;
					stanGearList->Add(newGear);
					break;
				case 2:
					goodGearList->Add(newGear);
					break;
				default:
					break;
				}
			}
		}
		// FormAddDataFiles->ProgressReset();
		memoInfo->Lines->Add(Format("Найдено %d деталей %s", quantity,
			PGTS.getStringDesignation()));

		// progressbar.clear
	}
	catch (Exception &E) {
		memoLog->Lines->Add(Format("Cant open file: %s", WideString(FileName)));
	}
	ExcelBook.OleProcedure("Close");
	ExcelApp.OleProcedure("Quit");
}

void BuildGearboxes(TList* suspGearList, TList* stanGearList,
	TList* goodGearList, TMemo* memoLog, TMemo* memoInfo, TList* UsedGearList,
	TFileList* FileList) {
	auto t = time(nullptr);
	auto tm = *localtime(&t);
	o->AddPair(new TJSONPair("Date", Format("%d.%d.%d", tm.tm_mday,
		tm.tm_mon + 1, tm.tm_year + 1900)));
	o->AddPair("Gearboxes", Gearboxes);

	memoInfo->Lines->Add("--------------------------------------------");
	memoInfo->Lines->Add("Расчет соединений...");
	memoInfo->Lines->Add("--------------------------------------------");

	// buildGoodGearboxes(goodGearList);
	buildStandartGearboxes(stanGearList, UsedGearList, memoLog, memoInfo);

	memoInfo->Lines->Add("--------------------------------------------");
	memoInfo->Lines->Add("Закраска использованных деталей...");
	memoInfo->Lines->Add("--------------------------------------------");
	memoInfo->Lines->Add("--------------------------------------------");
	memoInfo->Lines->Add("Сохраните файлы excel и json в удобное место");
	memoInfo->Lines->Add("--------------------------------------------");

	PaintUsedGears(UsedGearList, FileList, memoInfo);

	remove("DB65.json");

	if (FormAddDataFiles->SaveDialog1->Execute()) {
		// auto_ptr<TStreamWriter>DestFileStream(new TStreamWriter("DB65.json",
		// TEncoding::UTF8));

		auto_ptr<TStreamWriter>DestFileStream
			(new TStreamWriter(FormAddDataFiles->SaveDialog1->FileName +
			".json", TEncoding::UTF8));

		DestFileStream->Write(o->ToString());
		DestFileStream->Close();
	}

	delete stanGearList;
	delete UsedGearList;
	delete goodGearList;
	delete suspGearList;
	memoInfo->Lines->Add("Готово!");
}

void PaintUsedGears(TList* UsedGearList, TFileList* FileList, TMemo* memoInfo) {

	for (int i = 0; i < FileList->iGetCountFiles(); i++) {
		AnsiString FileName = FileList->sGetFile(i);
		ExcelApp = CreateOleObject("Excel.Application");
		try {
			ExcelApp.OlePropertySet("Visible", false); // setup Show Excel.
			ExcelBook = ExcelApp.OlePropertyGet("Workbooks").OleFunction("Open",
				WideString(FileName)); // Open Excel File.
			ExcelSheet = ExcelBook.OlePropertyGet("WorkSheets").OlePropertyGet
				("Item", 1);

			int iRowsCount = ExcelApp.OlePropertyGet("ActiveSheet")
				.OlePropertyGet("UsedRange").OlePropertyGet("Rows")
				.OlePropertyGet("Count");
			int iColsCount = ExcelApp.OlePropertyGet("ActiveSheet")
				.OlePropertyGet("UsedRange").OlePropertyGet("Columns")
				.OlePropertyGet("Count");

			Variant vCells = ExcelSheet.OlePropertyGet("Cells");
			AnsiString detailNum = vCells.OlePropertyGet("Item", 5, 3);
			TDesignation PGTS(detailNum);

			for (int g = 0; g < UsedGearList->Count; g++) {
				TGear* gear = (TGear*)UsedGearList->Items[g];
				if (gear->Desgination.getStringDesignation()
					== PGTS.getStringDesignation()) {
					for (int i = 15; i < iColsCount; i++) {
						UnicodeString gearNumber =
							vCells.OlePropertyGet("Item", 9, i);
						if (gear->sNumber == gearNumber) {
							vCells.OlePropertyGet("Item", 9, i).OlePropertyGet
								("Interior").OlePropertySet("ColorIndex", 6);
							break;
						}
					}
				}
			}
		}
		catch (...) {

		}
		ExcelBook.OleProcedure("Save");
		ExcelBook.OleProcedure("Close");
		ExcelApp.OleProcedure("Quit");
	}
}

void buildGoodGearboxes(TList* goodList) {
	vector<TGear*>gears;
	while (goodList->Count != 0) {
		bool enoughGears = true;
		for (int i = 0; i < gears_id.size(); i++) {
			enoughGears &= findGear(goodList, gears_id.at(i), &gears);
		}
		if (enoughGears) {
			TDesignation desMot("");
			TGear Motor(desMot, 0, L"Двигатель", 0);
			TDesignation desBlanc("");
			TGear blanc(desBlanc, 0, "", 0);

			TGearing gearing1(gears.at(0), gears.at(1));
			TGearing gearing2(gears.at(2), gears.at(3));
			TGearing gearing3(gears.at(4), gears.at(5));
			TGearing gearing4(gears.at(6), gears.at(7));

			TGearbox_DB65 DB65;
			DB65.vSetGearing1(&gearing1, &Motor);
			DB65.vSetGearing2(&gearing2);
			DB65.vSetGearing3(&gearing3);
			DB65.vSetGearing4(&gearing4, &blanc);

			DB65.vSave2JSON(Gearboxes);
		}
		else {
			return;
		}
	}
	return;
}

void buildStandartGearboxes(TList* stanList, TList* UsedGearList,
	TMemo* memoLog, TMemo* memoInfo) {
	vector<UnicodeString>names;
	int gearingN[4] = {0, };
	int gearboxN = 0;
	ExcelApp1 = CreateOleObject("Excel.Application");
	ExcelApp1.OlePropertySet("Visible", true);
	ExcelBooks1 = ExcelApp1.OlePropertyGet("Workbooks"); // Open Excel File.
	ExcelApp1.OlePropertySet("SheetsInNewWorkbook", 5);
	ExcelBooks1.OleProcedure("Add");
	Book1 = ExcelBooks1.OlePropertyGet("Item", 1);
	Sheets1 = Book1.OlePropertyGet("Worksheets");

	while (stanList->Count != 0) {
		vector<TGear*>gears1;
		vector<TGear*>gears2;
		bool enoughGears1 = true;
		bool enoughGears2 = true;

		// vector<int> usedGears = {
		/*
		 for (int i = 0; i < gears_id.size()/2; i++) {
		 while (gearIsFree(gears_id[i*2])) {

		 }

		 }
		 */

		for (int i = 0; i < gears_id.size() / 2; i++) {
			bool eG1 = findSpecialGear(stanList, i * 2, &gears1, memoLog);
			bool eG2 = findSpecialGear(stanList, i * 2 + 1, &gears2, memoLog);
			if (eG1)
				gearingN[i]++;
			if (eG2)
				gearingN[i]++;
			enoughGears1 &= eG1;
			enoughGears2 &= eG2;
		}

		if (enoughGears1) {
			gearboxN++;
			for (int j = 0; j < gears1.size(); j++) {
				UsedGearList->Add(gears1.at(j));
			}

			TDesignation desMot("");
			TGear Motor(desMot, 0, L"Двигатель", 0);
			TDesignation desBlanc("");
			TGear blanc(desBlanc, 0, "", 0);

			for (int h = 0; h < gears1.size(); h++) {
				names.push_back(gears1.at(h)->sNumber);
			}

			TGearing gearing1(gears1.at(0), gears1.at(1));
			TGearing gearing2(gears1.at(2), gears1.at(3));
			TGearing gearing3(gears1.at(4), gears1.at(5));
			TGearing gearing4(gears1.at(6), gears1.at(7));

			TGearbox_DB65 DB65;
			DB65.vSetGearing1(&gearing1, &Motor);
			DB65.vSetGearing2(&gearing2);
			DB65.vSetGearing3(&gearing3);
			DB65.vSetGearing4(&gearing4, &blanc);

			DB65.vSave2JSON(Gearboxes);
		}

		if (enoughGears2) {
			gearboxN++;
			for (int j = 0; j < gears2.size(); j++) {
				UsedGearList->Add(gears2.at(j));
			}
			TDesignation desMot("");
			TGear Motor(desMot, 0, L"Двигатель", 0);
			TDesignation desBlanc("");
			TGear blanc(desBlanc, 0, "", 0);
			TGearing gearing1_1(gears2.at(0), gears2.at(1));
			TGearing gearing2_1(gears2.at(2), gears2.at(3));
			TGearing gearing3_1(gears2.at(4), gears2.at(5));
			TGearing gearing4_1(gears2.at(6), gears2.at(7));

			for (int h = 0; h < gears2.size(); h++) {
				names.push_back(gears2.at(h)->sNumber);
			}

			TGearbox_DB65 DB65_1;
			DB65_1.vSetGearing1(&gearing1_1, &Motor);
			DB65_1.vSetGearing2(&gearing2_1);
			DB65_1.vSetGearing3(&gearing3_1);
			DB65_1.vSetGearing4(&gearing4_1, &blanc);

			DB65_1.vSave2JSON(Gearboxes);
		}

		if (!enoughGears1 && !enoughGears2) {
			for (int i = 0; i < 4; i++) {
				memoInfo->Lines->Add(Format("Найдено зацеплений %s+%s: %d",
					gears_id[i*2], gears_id[i*2 + 1], gearingN[i]));
			}

			Variant ExcelSheet1 = Sheets1.OlePropertyGet("Item", 1);
			ExcelSheet1.OlePropertySet("Name", WideString("ПГТС.33811.022"));
			ExcelSheet1.OleProcedure("Activate");
			Variant vCells1 = ExcelSheet1.OlePropertyGet("Cells");
			vCells1.OlePropertyGet("Item", 1, 1).OlePropertySet("Value",
				WideString("Двигатель"));
			vCells1.OlePropertyGet("Item", 1, 2).OlePropertySet("Value",
				WideString("Шестерня ведущая"));
			for (int i = 0; i < names.size() / 8; i++) {
				vCells1.OlePropertyGet("Item", i + 2, 2).OlePropertySet("Value",
					WideString(names.at(i*8)));
			}

			ExcelSheet1 = Sheets1.OlePropertyGet("Item", 2);
			ExcelSheet1.OlePropertySet("Name", WideString("ПГТС.33811.015"));
			ExcelSheet1.OleProcedure("Activate");
			vCells1 = ExcelSheet1.OlePropertyGet("Cells");
			vCells1.OlePropertyGet("Item", 1, 1).OlePropertySet("Value",
				WideString("Колесо промежуточное"));
			vCells1.OlePropertyGet("Item", 1, 2).OlePropertySet("Value",
				WideString("Шестерня промежуточная"));
			for (int i = 0; i < names.size() / 8; i++) {
				vCells1.OlePropertyGet("Item", i + 2, 1).OlePropertySet("Value",
					WideString(names.at(i*8 + 1)));
				vCells1.OlePropertyGet("Item", i + 2, 2).OlePropertySet("Value",
					WideString(names.at(i*8 + 2)));
			}

			ExcelSheet1 = Sheets1.OlePropertyGet("Item", 3);
			ExcelSheet1.OlePropertySet("Name", WideString("ПГТС.33811.016"));
			ExcelSheet1.OleProcedure("Activate");
			vCells1 = ExcelSheet1.OlePropertyGet("Cells");
			vCells1.OlePropertyGet("Item", 1, 1).OlePropertySet("Value",
				WideString("Колесо промежуточное"));
			vCells1.OlePropertyGet("Item", 1, 2).OlePropertySet("Value",
				WideString("Шестерня промежуточная"));
			for (int i = 0; i < names.size() / 8; i++) {
				vCells1.OlePropertyGet("Item", i + 2, 1).OlePropertySet("Value",
					WideString(names.at(i*8 + 3)));
				vCells1.OlePropertyGet("Item", i + 2, 2).OlePropertySet("Value",
					WideString(names.at(i*8 + 4)));
			}

			ExcelSheet1 = Sheets1.OlePropertyGet("Item", 4);
			ExcelSheet1.OlePropertySet("Name", WideString("ПГТС.33811.014"));
			ExcelSheet1.OleProcedure("Activate");
			vCells1 = ExcelSheet1.OlePropertyGet("Cells");
			vCells1.OlePropertyGet("Item", 1, 1).OlePropertySet("Value",
				WideString("Колесо промежуточное"));
			vCells1.OlePropertyGet("Item", 1, 2).OlePropertySet("Value",
				WideString("Шестерня промежуточная"));
			for (int i = 0; i < names.size() / 8; i++) {
				vCells1.OlePropertyGet("Item", i + 2, 1).OlePropertySet("Value",
					WideString(names.at(i*8 + 5)));
				vCells1.OlePropertyGet("Item", i + 2, 2).OlePropertySet("Value",
					WideString(names.at(i*8 + 6)));
			}

			ExcelSheet1 = Sheets1.OlePropertyGet("Item", 5);
			ExcelSheet1.OlePropertySet("Name", WideString("Редукторы"));
			ExcelSheet1.OleProcedure("Activate");
			vCells1 = ExcelSheet1.OlePropertyGet("Cells");

            vCells1.OlePropertyGet("Item", 2, 1).OlePropertySet("Value",
				WideString("Вал выходной"));
			Variant cellRange = ExcelSheet1.OlePropertyGet("Range",WideString("A3:A4"));
			cellRange.OleProcedure("Merge");
			cellRange.OlePropertySet("Value", WideString("Блок шестерен 017"));
			cellRange = ExcelSheet1.OlePropertyGet("Range",WideString("A5:A6"));
			cellRange.OleProcedure("Merge");
			cellRange.OlePropertySet("Value", WideString("Блок шестерен 016"));
			cellRange = ExcelSheet1.OlePropertyGet("Range",WideString("A7:A8"));
			cellRange.OleProcedure("Merge");
			cellRange.OlePropertySet("Value", WideString("Блок шестерен 015"));
			vCells1.OlePropertyGet("Item", 9, 1).OlePropertySet("Value",
				WideString("Ведущая шестерня"));

			vCells1.OlePropertyGet("Item", 2, 2).OlePropertySet("Value",
				WideString(gears_id[7]));
			vCells1.OlePropertyGet("Item", 3, 2).OlePropertySet("Value",
				WideString(gears_id[6]));
			vCells1.OlePropertyGet("Item", 4, 2).OlePropertySet("Value",
				WideString(gears_id[5]));
			vCells1.OlePropertyGet("Item", 5, 2).OlePropertySet("Value",
				WideString(gears_id[4]));
			vCells1.OlePropertyGet("Item", 6, 2).OlePropertySet("Value",
				WideString(gears_id[3]));
			vCells1.OlePropertyGet("Item", 7, 2).OlePropertySet("Value",
				WideString(gears_id[2]));
			vCells1.OlePropertyGet("Item", 8, 2).OlePropertySet("Value",
				WideString(gears_id[1]));
			vCells1.OlePropertyGet("Item", 9, 2).OlePropertySet("Value",
				WideString(gears_id[0]));
			for (int i = 0; i < names.size() / 8; i++) {
				vCells1.OlePropertyGet("Item", 1, i+3).OlePropertySet("Value",
					WideString(Format("Редуктор %d", i+1)));
				for (int o = 7; o > -1; o--) {
					vCells1.OlePropertyGet("Item", abs(o - 7) + 2, i+3).OlePropertySet("Value",
						WideString(names.at(o)));
				}
			}
			vCells1.OlePropertySet("ColumnWidth",30);

			/*
			 Book1.OleProcedure("SaveAs", WideString(vAsCurDir) + WideString("\\builds.xlsx"));
			 Book1.OleProcedure("Close");
			 ExcelApp1.OleProcedure("Quit");
			 */

			memoInfo->Lines->Add(Format("Собрано редукторов: %d", gearboxN));
			return;
		}
	}
	return;
}

bool findSpecialGear(TList* gearList, int number, vector<TGear*> * measurements,
	TMemo* memoLog) {
	double worst = 1000;
	TGear* worstGear;
	for (int i = 0; i < gearList->Count; i++) {
		TGear* gear = (TGear*)gearList->Items[i];
		if (gear->Desgination.getStringDesignation() == gears_id[number]) {
			double tolerance = calculateTolerance(gear, number, memoLog);
			int m = gearWheel(number);
			for (int j = 0; j < gearList->Count; j++) {
				TGear* gear1 = (TGear*)gearList->Items[j];
				if (gear1->Desgination.getStringDesignation() == gears_id[m]) {
					double D = diameter(gear1);
					if (D <= tolerance && D >= tolerance - Tol[m]) {
						double probe = D - (tolerance - Tol[m]);
						if (probe < worst) {
							worst = probe;
							worstGear = (TGear*)gearList->Items[j];
						}
					}
				}
			}
			if (worst != 1000) {
				if (number % 2 == 0) {
					measurements->push_back(gear);
					measurements->push_back(worstGear);
				}
				else {
					measurements->push_back(worstGear);
					measurements->push_back(gear);
				}
				gearList->Remove(gear);
				gearList->Remove(worstGear);
				return true;
			} /* else {
			 gearList->Remove(gear);
			 i--;
			 } */
		}
	}
	return false;
}

double calculateTolerance(TGear* gear, int n, TMemo* memoLog) {
	double Diam;
	if (N[n] % 2 == 0) {
		Diam = diameter(gear) - Dr[n];
	}
	else {
		Diam = (diameter(gear) - Dr[n]) / cos(20 * CtR / N[n]);
	}
	double alphaP = acos(M[n] * N[n] * cos(20 * CtR) / Diam);
	double Eg =
		(M_PI / 2 + N[n] * (tan(alphaP) - alphaP - (tan(20 * CtR) - 20 * CtR)) -
		Dr[n] / (M[n] * cos(20 * CtR))) / (2 * tan(20 * CtR));
	int m = gearWheel(n);
	double Ek = -Eg - Of[m] / M[m] - Of[n] / M[n];
	double Xp = digitalCalc(n, Ek, memoLog);
	if (N[m] % 2 == 0) {
		return (N[m] * M[m] * cos(20 * CtR) / cos(Xp) + Dr[m]);
	}
	else {
		return (N[m] * M[m] * cos(20 * CtR) * cos(90 * CtR / N[m]) / cos(Xp)
			+ Dr[m]);
	}
}

double digitalCalc(int n, double Ek, TMemo* memoLog) {
	double eps = 0.0001;
	double c = 0;
	double a = 0;
	double b = 1;
	while (b - a > eps) {
		c = (a + b) / 2;
		if (f(a, n, Ek) * f(c, n, Ek) < 0) {
			b = c;
		}
		else {
			a = c;
		}
	}
	return (a + b) / 2;
}

double f(double x, int n, double Ek) {
	int m = gearWheel(n);
	return (tan(x) - x - Dr[m] / (N[m] * M[m] * cos(20 * CtR)) -
		(tan(20 * CtR) - 20 * CtR) + (M_PI / 2 - 2 * Ek * tan(20 * CtR))
		/ N[m]);
}

int gearWheel(int n) {
	if (n % 2 == 0) {
		return n + 1;
	}
	else {
		return n - 1;
	}
}

double diameter(TGear* gear) {
	for (int i = 0; i < gear->listParams->Count; i++) {
		stMeasurement* st = (stMeasurement*)gear->listParams->Items[i];
		if (st->sKeyMeasure == "9.6.3.") {
			return st->fMeasure;
		}
	}
	return 0;
}

bool findGear(TList* goodList, UnicodeString desig, vector<TGear*> * gears) {
	for (int i = 0; i < goodList->Count; i++) {
		TGear* gear = (TGear*)goodList->Items[i];
		if (gear->Desgination.getStringDesignation() == desig) {
			gears->push_back(gear);
			goodList->Remove(gear);
			return true;
		}
	}
	return false;
}

void getFilledMeasurementRows(unsigned int rowCnt, unsigned int Col,
	vector<unsigned int> * measurements) {
	int myCase = 0;

	try {
		Variant MYCells = ExcelApp.OlePropertyGet("Cells");
		for (unsigned int i = 1; i < rowCnt; i++) {

			UnicodeString currStr = MYCells.OlePropertyGet("Item", i, Col);

			switch (myCase) {
			case 1:
				if (isCellFilled(currStr)) {
					measurements->push_back(i);
				}
				break;
			case 2:
				if (isCellFilled(currStr)) {
					measurements->push_back(i);
				}
				break;
			case 3:
				return;

			default:
				break;
			}

			if (currStr == "№ характеристики на чертеже") {
				myCase = 1;
			}
			else if (currStr == "Замер на ВИМ") {
				measurements->pop_back();
				myCase = 2;
			}
			else if (currStr == "Согласование отклонений") {
				measurements->pop_back();
				myCase = 3;
			}
		}
	}
	catch (...) {
		return;
	}
}

int fillGearMeasurments(vector<unsigned int> * measurements, TList* listparams,
	int col) {
	try {
		Variant MYCells = ExcelApp.OlePropertyGet("Cells");
		for (int i = 0; i < measurements->size(); i++) {
			UnicodeString currStr =
				MYCells.OlePropertyGet("Item", measurements->at(i), col);
			if (isCellFilled(currStr)) {
				stMeasurement* Measurment = new stMeasurement;
				Measurment->fMeasure = currStr.ToDouble();
				currStr = MYCells.OlePropertyGet("Item",
					measurements->at(i), 2);
				Measurment->sKeyMeasure = currStr;
				currStr = MYCells.OlePropertyGet("Item",
					measurements->at(i), 11);
				Measurment->fNominalValue = currStr.ToDouble();
				currStr = MYCells.OlePropertyGet("Item",
					measurements->at(i), 12);
				Measurment->fToleranceUp = currStr.ToDouble();
				currStr = MYCells.OlePropertyGet("Item",
					measurements->at(i), 13);
				Measurment->fToleranceDown = currStr.ToDouble();
				void *mrmt = Measurment;
				listparams->Add(mrmt);
				// delete Measurment;
			}
		}
		return checkMasurementsData(listparams);
	}
	catch (...) {
		return 0;
	}
}

int checkMasurementsData(TList* listparams) {
	bool e_controller = false;
	bool e_operator = false;
	bool e_roller = false;
	bool e_runout = false;
	for (int i = 0; i < listparams->Count; i++) {
		stMeasurement* Measurment = (stMeasurement*)listparams->Items[i];
		if (Measurment->sKeyMeasure == "9.6.1.") {
			e_controller = measureInLimits(Measurment);
		}
		else if (Measurment->sKeyMeasure == "9.6.2.") {
			e_operator = measureInLimits(Measurment);
		}
		else if (Measurment->sKeyMeasure == "9.6.3.") {
			e_roller = measureInLimits(Measurment);
		}
		else if (Measurment->sKeyMeasure == "9.9.") {
			e_runout = measureInLimits(Measurment);
		}
	}
	if (e_roller) {
		return 1;
	}
	else {
		return 1;
	}
}

bool measureInLimits(stMeasurement* Measurment) {
	if (Measurment->fMeasure >= Measurment->fNominalValue +
		Measurment->fToleranceDown && Measurment->fMeasure <=
		Measurment->fNominalValue + Measurment->fToleranceUp) {
		return true;
	}
	else {
		return false;
	}
}

bool isCellFilled(AnsiString cell) {
	if (cell != "" && cell != "-") {
		return true;
	}
	else {
		return false;
	}
}

UnicodeString correctName(UnicodeString ustr) {
	AnsiString astr = ustr;
	string str = astr.c_str();
	if (str.find("\n") != string::npos) {
		str.erase(str.find("\n"), str.find("\n") + 1);
	}
	return str.c_str();
}