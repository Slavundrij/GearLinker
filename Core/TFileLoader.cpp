// ---------------------------------------------------------------------------

#pragma hdrstop

#include "TFileLoader.h"

#include <ComObj.hpp>
#include <Vcl.OleCtrls.hpp>
#include <Vcl.FileCtrl.hpp>
#include "objbase.h"
#include <math.h>
#include <algorithm>

#include "TFormAddDataFiles.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)

double CtR = M_PI / 180;

map<UnicodeString, map<double, pair<double, double>>> RollerSizes = {
	{
		gears_id[0], { {9.822, pair(0.059, 1)}, {9.793, pair(0.058, 0.99)}, {10.035, pair(0.056, 1.074)} }
	},
	{
		gears_id[1], { {36.445, pair(0.076, 1)}, {36.409, pair(0.077, 0.99)}, {36.715, pair(0.071, 1.074)} }
	},
	{
		gears_id[2], { {11.099, pair(0.043, 1)}, {11.068, pair(0.044, 0.99)}, {11.325, pair(0.041, 1.074)} }
	},
	{
		gears_id[3], { {35.203, pair(0.072, 1)}, {35.168, pair(0.072, 0.99)}, {35.463, pair(0.07, 1.074)} }
	},
	{
		gears_id[4], { {9.822, pair(0.045, 1)}, {9.793, pair(0.045, 0.99)}, {10.035, pair(0.043, 1.074)} }
	},
	{
		gears_id[5], { {36.445, pair(0.067, 1)}, {36.409, pair(0.067, 0.99)}, {36.715, pair(0.065, 1.074)} }
	},
	{
		gears_id[6], { {13.799, pair(0.027, 1.5)} }
	},
	{
		gears_id[7], { {41.936, pair(0.04, 1.5)} }
	}
};
vector<double> M = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.8, 0.8};
vector<int> N = {16, 70, 19, 67, 16, 70, 14, 50};
vector<double>Of = {0.022, 0.032, 0.022, 0.032, 0.02, 0.03, 0.01, 0.01};
UnicodeString baseMeasure = "9.6.1.";

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

////////////////////////////////////////////////////////////////////////////////

Variant ExcelApp1, ExcelBooks1, Book1, Sheets1, ExcelApp, ExcelBooks, ExcelBook,
	ExcelSheet;

void vLoadGearsFromExcel(TList* suspGearList, TList* stanGearList,
	TList* goodGearList, AnsiString FileName, TMemo* memoLog, TMemo* memoInfo) {
	ExcelApp = CreateOleObject("Excel.Application");
	ExcelApp.OlePropertySet("DisplayAlerts", 0);

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
		unsigned int orderNum = 0;
		try {
			orderNum = sOrderNum.ToInt();
		} catch (...) {
		}
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
				newGear->parentFile = parseFileName(FileName);
				TList* listparams = new TList();
				// fun for taking all measurements
				//fillGearMeasurments(&filledMeasuarments, listparams, i);
				//newGear->listParams = listparams;
				switch (fillGearMeasurments(&filledMeasuarments, newGear, listparams, i)) {
				case 0:
					newGear->listParams = listparams;
					suspGearList->Add(newGear);
					break;
				case 1:
					newGear->listParams = listparams;
					quantity++;
					stanGearList->Add(newGear);
					break;
				case 2:
					newGear->listParams = listparams;
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
	ExcelBook.OleProcedure("Close", false);
	ExcelApp.OleProcedure("Quit");
}

void BuildGearboxes(TList* suspGearList, TList* stanGearList,
	TList* goodGearList, TMemo* memoLog, TMemo* memoInfo, TList* UsedGearList,
	TFileList* FileList, TMemo* memoRes) {

	memoInfo->Lines->Add("--------------------------------------------");
	memoInfo->Lines->Add("Расчет соединений...");
	memoInfo->Lines->Add("--------------------------------------------");
    memoInfo->Lines->Add("--------------------------------------------");
	memoInfo->Lines->Add("Сохраните файлы excel и json в удобное место");
	memoInfo->Lines->Add("--------------------------------------------");

	// buildGoodGearboxes(goodGearList);
	buildStandartGearboxes(stanGearList, UsedGearList, memoLog, memoInfo, memoRes);

	memoInfo->Lines->Add("--------------------------------------------");
	memoInfo->Lines->Add("Закраска использованных деталей...");
	memoInfo->Lines->Add("--------------------------------------------");

	PaintUsedGears(UsedGearList, FileList, memoInfo);

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
			//AnsiString detailNum = vCells.OlePropertyGet("Item", 5, 3);
			//TDesignation PGTS(detailNum);
			UnicodeString uFileName = parseFileName(FileName);

			for (int g = 0; g < UsedGearList->Count; g++) {
				TGear* gear = (TGear*)UsedGearList->Items[g];
				if (uFileName == gear->parentFile) {
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

/*
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
*/

int gearInList(TList* stanList, int num) {
	for (int i = 0; i < stanList->Count; i++) {
		TGear* gear = (TGear*)stanList->Items[i];
		if (gear->Desgination.getStringDesignation() == gears_id[num]) {
			return i;
		}
	}
	return -1;
}

void findAllPairs(TList* gearList, TGear* Gear, TList* FindMatches, int num, TMemo* memoLog) {
	int m = gearWheel(num);
	TList* Gear_Wheel = new TList();
	for (int j = 0; j < gearList->Count; j++) {
		TGear* Wheel = (TGear*)gearList->Items[j];
		if (Wheel->Desgination.getStringDesignation() == gears_id[m]) {
			double tolerance = calculateTolerance(Gear, Wheel, num, memoLog);
			double D = diameter(Wheel, memoLog);
			if (D <= tolerance && D >= tolerance - Wheel->extTol) {
				possiblePair* Pair = new possiblePair();
				Pair->Gear1 = Gear;
				Pair->Gear2 = Wheel;
				Pair->wNum = Wheel->sNumber;
				Pair->criteria = D - (tolerance - Wheel->extTol);
				Gear_Wheel->Add(Pair);
				memoLog->Lines->Add(Format("Найдено зацепление %s+%s", Gear->sNumber, Wheel->sNumber));
			}
		}
	}
	if (Gear_Wheel->Count != 0) {
		FindMatches->Add(Gear_Wheel);
	}
	else {
		delete Gear_Wheel;
    }
}

possiblePair* findWorstPair(TList* _FindMatches, TMemo* _memoLog) {
	int min = 1000;
	double worst = 0;
	UnicodeString wNum;
	TList* worstPairs;
	possiblePair *worstPair;
	_memoLog->Lines->Add(_FindMatches->Count);
	for (int el = 0; el < _FindMatches->Count; el++) {
		TList* pairs = (TList*)_FindMatches->Items[el];
		possiblePair *Mypair = (possiblePair*)pairs->Items[0];
		if (pairs->Count < min) {
			worstPairs = pairs;
			min = pairs->Count;
			possiblePair *pair = (possiblePair*)pairs->Items[0];
			wNum = pair->wNum;
			worst = pair->criteria;
			worstPair = pair;
		} else if (pairs->Count == min) {
			for (int i = 0; i < pairs->Count; i++) {
				possiblePair *pair = (possiblePair*)pairs->Items[i];
                if (pair->wNum == wNum) {
					if (pair->criteria < worst) {
						worstPairs = pairs;
						worst = pair->criteria;
						worstPair = pair;
						break;
					}
				}
			}
		}
	}
	clearAllRepeats(_FindMatches, wNum, _memoLog);
	_FindMatches->Remove(worstPairs);
	return worstPair;
}

void clearAllRepeats(TList* FindMatches, UnicodeString wNum, TMemo* memoInfo) {
	for (int el = 0; el < FindMatches->Count; el++) {
		TList* pairs = (TList*)FindMatches->Items[el];
		for (int j = 0; j < pairs->Count; j++) {
			possiblePair *pair = (possiblePair*)pairs->Items[j];
			if (pair->wNum == wNum) {
				pairs->Remove(pair);
			}
		}
		if (pairs->Count == 0) {
			FindMatches->Remove(pairs);
            el--;
		}
    }
}

void buildStandartGearboxes(TList* stanList, TList* UsedGearList,
	TMemo* memoLog, TMemo* memoInfo, TMemo* memoRes) {
	vector<UnicodeString>names;
	vector<TGear*>Gears_and_Wheels_1;
	vector<TGear*>Gears_and_Wheels_2;
	vector<TGear*>Gears_and_Wheels_3;
	vector<TGear*>Gears_and_Wheels_4;
	int gearingN[4] = {0, };
	int gearboxN = 0;

	TList* FindMatches = new TList();

	int gearPos = 0;
	for (int i = 0; i < gears_id.size() / 2; i++) {
		FindMatches->Clear();
        memoLog->Lines->Add("++++++++++++++++++");
		memoLog->Lines->Add("");
		memoLog->Lines->Add(Format("Далее идут все найденные зацепления %s+%s", gears_id[i*2], gears_id[i*2+1]));
		memoLog->Lines->Add("");
		memoLog->Lines->Add("++++++++++++++++++");
		while (1) {
			gearPos = gearInList(stanList, i * 2);
			if (gearPos == -1) {
				break;
			}
			else {
				TGear* Gear = (TGear*)stanList->Items[gearPos];
				stanList->Remove(Gear);
				findAllPairs(stanList, Gear, FindMatches, i * 2, memoLog);
			}
		}
		memoLog->Lines->Add("++++++++++++++++++");
		memoLog->Lines->Add("");
		memoLog->Lines->Add(Format("Далее идут сборки по критерию %s+%s", gears_id[i*2], gears_id[i*2+1]));
		memoLog->Lines->Add("");
		memoLog->Lines->Add("++++++++++++++++++");
		while (FindMatches->Count != 0) {
			possiblePair* worstPair = findWorstPair(FindMatches, memoLog);
			TGear* Gear = worstPair->Gear1;
			TGear* Wheel = worstPair->Gear2;
            if (i == 0) {
				Gears_and_Wheels_1.push_back(Gear);
				Gears_and_Wheels_1.push_back(Wheel);
			} else if (i == 1) {
				Gears_and_Wheels_2.push_back(Gear);
				Gears_and_Wheels_2.push_back(Wheel);
			} else if (i == 2) {
				Gears_and_Wheels_3.push_back(Gear);
				Gears_and_Wheels_3.push_back(Wheel);
			} else if (i == 3) {
				Gears_and_Wheels_4.push_back(Gear);
				Gears_and_Wheels_4.push_back(Wheel);
			}
			memoLog->Lines->Add(Format("Найдено зацепление %s+%s", Gear->sNumber, Wheel->sNumber));
			gearingN[i]++;
		}
	}
    for (int i = 0; i < 4; i++) {
		memoInfo->Lines->Add(Format("Найдено зацеплений %s+%s: %d",
		gears_id[i*2], gears_id[i*2 + 1], gearingN[i]));
	}

	if(Gears_and_Wheels_1.size() != 0 && Gears_and_Wheels_2.size() != 0 &&
		   Gears_and_Wheels_3.size() != 0 && Gears_and_Wheels_4.size() != 0)
	{
		TJSONObject *o = new TJSONObject();
		TJSONArray *Gearboxes = new TJSONArray();
        auto t = time(nullptr);
		auto tm = *localtime(&t);
		o->AddPair(new TJSONPair("Date", Format("%d.%d.%d", tm.tm_mday,
			tm.tm_mon + 1, tm.tm_year + 1900)));
		o->AddPair("Gearboxes", Gearboxes);
		while (Gears_and_Wheels_1.size() != 0 && Gears_and_Wheels_2.size() != 0 &&
			   Gears_and_Wheels_3.size() != 0 && Gears_and_Wheels_4.size() != 0)
		{
			gearboxN++;

			UsedGearList->Add(Gears_and_Wheels_1[0]);
			UsedGearList->Add(Gears_and_Wheels_1[1]);
			UsedGearList->Add(Gears_and_Wheels_2[0]);
			UsedGearList->Add(Gears_and_Wheels_2[1]);
			UsedGearList->Add(Gears_and_Wheels_3[0]);
			UsedGearList->Add(Gears_and_Wheels_3[1]);
			UsedGearList->Add(Gears_and_Wheels_4[0]);
			UsedGearList->Add(Gears_and_Wheels_4[1]);

			TDesignation desMot("");
			TGear Motor(desMot, 0, L"Двигатель", 0);
			TDesignation desBlanc("");
			TGear blanc(desBlanc, 0, "", 0);

			names.push_back(Gears_and_Wheels_1[0]->sNumber);
			names.push_back(Gears_and_Wheels_1[1]->sNumber);
			names.push_back(Gears_and_Wheels_2[0]->sNumber);
			names.push_back(Gears_and_Wheels_2[1]->sNumber);
			names.push_back(Gears_and_Wheels_3[0]->sNumber);
			names.push_back(Gears_and_Wheels_3[1]->sNumber);
			names.push_back(Gears_and_Wheels_4[0]->sNumber);
			names.push_back(Gears_and_Wheels_4[1]->sNumber);

			TGearing gearing1(Gears_and_Wheels_1[0], Gears_and_Wheels_1[1]);
			TGearing gearing2(Gears_and_Wheels_2[0], Gears_and_Wheels_2[1]);
			TGearing gearing3(Gears_and_Wheels_3[0], Gears_and_Wheels_3[1]);
			TGearing gearing4(Gears_and_Wheels_4[0], Gears_and_Wheels_4[1]);

			TGearbox_DB65 DB65;
			DB65.vSetGearing1(&gearing1, &Motor);
			DB65.vSetGearing2(&gearing2);
			DB65.vSetGearing3(&gearing3);
			DB65.vSetGearing4(&gearing4, &blanc);

			DB65.vSave2JSON(Gearboxes);

			Gears_and_Wheels_1.erase(Gears_and_Wheels_1.begin(), Gears_and_Wheels_1.begin()+2);
			Gears_and_Wheels_2.erase(Gears_and_Wheels_2.begin(), Gears_and_Wheels_2.begin()+2);
			Gears_and_Wheels_3.erase(Gears_and_Wheels_3.begin(), Gears_and_Wheels_3.begin()+2);
			Gears_and_Wheels_4.erase(Gears_and_Wheels_4.begin(), Gears_and_Wheels_4.begin()+2);
		}

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

		ExcelApp1 = CreateOleObject("Excel.Application");
		ExcelApp1.OlePropertySet("Visible", true);
		ExcelBooks1 = ExcelApp1.OlePropertyGet("Workbooks"); // Open Excel File.
		ExcelApp1.OlePropertySet("SheetsInNewWorkbook", 5);
		ExcelBooks1.OleProcedure("Add");
		Book1 = ExcelBooks1.OlePropertyGet("Item", 1);
		Sheets1 = Book1.OlePropertyGet("Worksheets");
		Variant ExcelSheet1 = Sheets1.OlePropertyGet("Item", 1);
		ExcelSheet1.OlePropertySet("Name", WideString("ПГТС.33811.022"));
		ExcelSheet1.OleProcedure("Activate");
		Variant vCells1 = ExcelSheet1.OlePropertyGet("Cells");
		vCells1.OlePropertyGet("Item", 1, 1).OlePropertySet("Value",
			WideString("Двигатель"));
		vCells1.OlePropertyGet("Item", 1, 2).OlePropertySet("Value",
			WideString("Шестерня ведущая"));
		vCells1.OlePropertyGet("Item", 1, 3).OlePropertySet("Value",
			WideString("ПГТС.33811.022"));

		for (int i = 0; i < names.size() / 8; i++) {
			vCells1.OlePropertyGet("Item", i + 2, 2).OlePropertySet("Value",
				WideString(names.at(i*8)));
		}
		vCells1.OlePropertySet("ColumnWidth", 30);

		ExcelSheet1 = Sheets1.OlePropertyGet("Item", 2);
		ExcelSheet1.OlePropertySet("Name", WideString("ПГТС.33811.015"));
		ExcelSheet1.OleProcedure("Activate");
		vCells1 = ExcelSheet1.OlePropertyGet("Cells");
		vCells1.OlePropertyGet("Item", 1, 1).OlePropertySet("Value",
			WideString("Колесо промежуточное"));
		vCells1.OlePropertyGet("Item", 1, 2).OlePropertySet("Value",
			WideString("Шестерня промежуточная"));
		vCells1.OlePropertyGet("Item", 1, 3).OlePropertySet("Value",
			WideString("ПГТС.33811.015"));
		for (int i = 0; i < names.size() / 8; i++) {
			vCells1.OlePropertyGet("Item", i + 2, 1).OlePropertySet("Value",
				WideString(names.at(i*8 + 1)));
			vCells1.OlePropertyGet("Item", i + 2, 2).OlePropertySet("Value",
				WideString(names.at(i*8 + 2)));
		}
		vCells1.OlePropertySet("ColumnWidth", 30);

		ExcelSheet1 = Sheets1.OlePropertyGet("Item", 3);
		ExcelSheet1.OlePropertySet("Name", WideString("ПГТС.33811.016"));
		ExcelSheet1.OleProcedure("Activate");
		vCells1 = ExcelSheet1.OlePropertyGet("Cells");
		vCells1.OlePropertyGet("Item", 1, 1).OlePropertySet("Value",
			WideString("Колесо промежуточное"));
		vCells1.OlePropertyGet("Item", 1, 2).OlePropertySet("Value",
			WideString("Шестерня промежуточная"));
            vCells1.OlePropertyGet("Item", 1, 3).OlePropertySet("Value",
			WideString("ПГТС.33811.016"));
		for (int i = 0; i < names.size() / 8; i++) {
			vCells1.OlePropertyGet("Item", i + 2, 1).OlePropertySet("Value",
				WideString(names.at(i*8 + 3)));
			vCells1.OlePropertyGet("Item", i + 2, 2).OlePropertySet("Value",
				WideString(names.at(i*8 + 4)));
		}
		vCells1.OlePropertySet("ColumnWidth", 30);

		ExcelSheet1 = Sheets1.OlePropertyGet("Item", 4);
		ExcelSheet1.OlePropertySet("Name", WideString("ПГТС.33811.014"));
		ExcelSheet1.OleProcedure("Activate");
		vCells1 = ExcelSheet1.OlePropertyGet("Cells");
		vCells1.OlePropertyGet("Item", 1, 1).OlePropertySet("Value",
			WideString("Колесо промежуточное"));
		vCells1.OlePropertyGet("Item", 1, 2).OlePropertySet("Value",
			WideString("Шестерня промежуточная"));
		vCells1.OlePropertyGet("Item", 1, 3).OlePropertySet("Value",
			WideString("ПГТС.33811.014"));
		for (int i = 0; i < names.size() / 8; i++) {
			vCells1.OlePropertyGet("Item", i + 2, 1).OlePropertySet("Value",
				WideString(names.at(i*8 + 5)));
			vCells1.OlePropertyGet("Item", i + 2, 2).OlePropertySet("Value",
				WideString(names.at(i*8 + 6)));
		}
		vCells1.OlePropertySet("ColumnWidth", 30);

		//Gearboxes/////////////////////////////////////////////////////////////
        ExcelSheet1 = Sheets1.OlePropertyGet("Item", 5);
		ExcelSheet1.OlePropertySet("Name", WideString("Редукторы"));
		ExcelSheet1.OleProcedure("Activate");
		vCells1 = ExcelSheet1.OlePropertyGet("Cells");

		int colNum = 0;
		int rowNum = 1;
		bool firstTime = true;
		vCells1.OlePropertyGet("Item", 1, 1).OlePropertySet("ColumnWidth", 16.55);
		vCells1.OlePropertyGet("Item", 1, 2).OlePropertySet("ColumnWidth", 15.27);
		for (int i = 0; i < names.size() / 8; i++) {
			colNum = i % 7;
			if (i % 7 == 0) {
				if (!firstTime) {
					rowNum += 11;
				}
				firstTime = false;

				vCells1.OlePropertyGet("Item", rowNum + 1, 1).OlePropertySet("Value",
					WideString("Вал выходной"));

				string range = "A" + to_string(rowNum + 2) + ":A" + to_string(rowNum + 3);
				Variant cellRange = ExcelSheet1.OlePropertyGet("Range",WideString(range.c_str()));
				cellRange.OleProcedure("Merge");
				cellRange.OlePropertySet("Value", WideString("Блок шестерен 017"));
				range = "A" + to_string(rowNum + 4) + ":A" + to_string(rowNum + 5);
				cellRange = ExcelSheet1.OlePropertyGet("Range",WideString(range.c_str()));
				cellRange.OleProcedure("Merge");
				cellRange.OlePropertySet("Value", WideString("Блок шестерен 016"));
				range = "A" + to_string(rowNum + 6) + ":A" + to_string(rowNum + 7);
				cellRange = ExcelSheet1.OlePropertyGet("Range",WideString(range.c_str()));
				cellRange.OleProcedure("Merge");
				cellRange.OlePropertySet("Value", WideString("Блок шестерен 015"));
				vCells1.OlePropertyGet("Item", rowNum + 8, 1).OlePropertySet("Value",
					WideString("Ведущая шестерня"));

				vCells1.OlePropertyGet("Item", rowNum + 1, 2).OlePropertySet("Value",
					WideString(gears_id[7]));
				vCells1.OlePropertyGet("Item", rowNum + 2, 2).OlePropertySet("Value",
					WideString(gears_id[6]));
				vCells1.OlePropertyGet("Item", rowNum + 3, 2).OlePropertySet("Value",
					WideString(gears_id[5]));
				vCells1.OlePropertyGet("Item", rowNum + 4, 2).OlePropertySet("Value",
					WideString(gears_id[4]));
				vCells1.OlePropertyGet("Item", rowNum + 5, 2).OlePropertySet("Value",
					WideString(gears_id[3]));
				vCells1.OlePropertyGet("Item", rowNum + 6, 2).OlePropertySet("Value",
					WideString(gears_id[2]));
				vCells1.OlePropertyGet("Item", rowNum + 7, 2).OlePropertySet("Value",
					WideString(gears_id[1]));
				vCells1.OlePropertyGet("Item", rowNum + 8, 2).OlePropertySet("Value",
					WideString(gears_id[0]));
			}
			vCells1.OlePropertyGet("Item", rowNum, colNum + 3).OlePropertySet("Value",
				WideString(Format("Редуктор %d", i+1)));
			vCells1.OlePropertyGet("Item", rowNum, colNum + 3).OlePropertySet("ColumnWidth", 14.73);
			for (int o = 7; o > -1; o--) {
				vCells1.OlePropertyGet("Item", rowNum + abs(o - 7) + 1, colNum + 3).OlePropertySet("Value",
					WideString(names.at(o + i * 8)));
			}
		}
	}
	int minCol = 30;
	int minNum = 0;
	for (int i = 0; i < 4; i++) {
		if (minCol > gearingN[i]) {
			minCol = gearingN[i];
			minNum = i*2;
		}
	}
	int secMinCol = 30;
	for (int j = 0; j < 4; j++) {
		if (minCol != gearingN[j]) {
			if (secMinCol > gearingN[j]) {
				secMinCol = gearingN[j];
			}
		}
	}
	memoRes->Lines->Add(Format("Для дальнейшей сборки редукторов не хватает %d деталей %s и %s", secMinCol - minCol, gears_id[minNum], gears_id[minNum+1]));
	memoInfo->Lines->Add(Format("Собрано редукторов: %d", gearboxN));
	return;
}

double diameter(TGear* gear, TMemo* memoLog) {
	for (int i = 0; i < gear->listParams->Count; i++) {
		stMeasurement* st = (stMeasurement*)gear->listParams->Items[i];
		if (st->sKeyMeasure == baseMeasure) {
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
			if(cellIsMeasure(currStr)) {
				measurements->push_back(i);
			}
			if (ContainsSubstring(currStr, "Окончательный контроль")) {
				measurements->push_back(i+1);
			}
		}
	}
	catch (...) {
		return;
	}
}

int fillGearMeasurments(vector<unsigned int> * measurements, TGear* Gear, TList* listparams,
	int col) {
	try {
		Variant MYCells = ExcelApp.OlePropertyGet("Cells");
		for (int i = 0; i < measurements->size(); i++) {
			UnicodeString currStr =
				MYCells.OlePropertyGet("Item", measurements->at(i), col);
			if (isCellFilled(currStr)) {
				stMeasurement* Measurment = new stMeasurement;
				UnicodeString nameStr = MYCells.OlePropertyGet("Item",measurements->at(i), 2);
				nameStr.Trim();
				if (!cellIsMeasure(nameStr)) {
					Measurment->sKeyMeasure = "Окончательный контроль";
					(currStr == "OK") ? Measurment->fMeasure = 1 : Measurment->fMeasure = 0;
				}
				else {
					Measurment->sKeyMeasure = nameStr;
					try {
						Measurment->fMeasure = currStr.ToDouble();
					} catch (...) {
						Measurment->fMeasure = 0;
					}
					currStr = MYCells.OlePropertyGet("Item",
						measurements->at(i), 11);
					try {
						Measurment->fNominalValue = currStr.ToDouble();
					} catch (...) {
						Measurment->fNominalValue = 0;
					}
					if (Measurment->sKeyMeasure == baseMeasure) {
						map<double, pair<double, double>> Nominal_to_all = RollerSizes[Gear->Desgination.getStringDesignation()];
						if (Nominal_to_all.find(Measurment->fNominalValue) != Nominal_to_all.end()) {
							Gear->extTol = Nominal_to_all[Measurment->fNominalValue].first;
							Gear->rollerD = Nominal_to_all[Measurment->fNominalValue].second;
						} else {

						}

						/*
						if (Gear->Desgination.getStringDesignation() == gears_id[0]) {
							auto result = find(begin(PGTS44_07_nom), end(PGTS44_07_nom), Measurment->fNominalValue);
							Gear->extTol = PGTS44_07[result - begin(PGTS44_07_nom)];
							Gear->rollerD = Dr[result - begin(PGTS44_07_nom)];
						}
						else if (Gear->Desgination.getStringDesignation() == gears_id[1]) {
							auto result = find(begin(PGTS34_15_nom), end(PGTS34_15_nom), Measurment->fNominalValue);
							Gear->extTol = PGTS34_15[result - begin(PGTS34_15_nom)];
							Gear->rollerD = Dr[result - begin(PGTS34_15_nom)];
						}
						else if (Gear->Desgination.getStringDesignation() == gears_id[2]) {
							auto result = find(begin(PGTS64_05_nom), end(PGTS64_05_nom), Measurment->fNominalValue);
							Gear->extTol = PGTS64_05[result - begin(PGTS64_05_nom)];
							Gear->rollerD = Dr[result - begin(PGTS64_05_nom)];
						}
						else if (Gear->Desgination.getStringDesignation() == gears_id[3]) {
							auto result = find(begin(PGTS34_16_nom), end(PGTS34_16_nom), Measurment->fNominalValue);
							Gear->extTol = PGTS34_16[result - begin(PGTS34_16_nom)];
							Gear->rollerD = Dr[result - begin(PGTS34_16_nom)];
						}
						else if (Gear->Desgination.getStringDesignation() == gears_id[4]) {
							auto result = find(begin(PGTS64_06_nom), end(PGTS64_06_nom), Measurment->fNominalValue);
							Gear->extTol = PGTS64_06[result - begin(PGTS64_06_nom)];
							Gear->rollerD = Dr[result - begin(PGTS64_06_nom)];
						}
						else if (Gear->Desgination.getStringDesignation() == gears_id[5]) {
							auto result = find(begin(PGTS34_14_nom), end(PGTS34_14_nom), Measurment->fNominalValue);
							Gear->extTol = PGTS34_14[result - begin(PGTS34_14_nom)];
							Gear->rollerD = Dr[result - begin(PGTS34_14_nom)];
						}
						else if (Gear->Desgination.getStringDesignation() == gears_id[6]) {
							Gear->extTol = PGTS64_07[0];
							Gear->rollerD = Dr[3];
						}
						else if (Gear->Desgination.getStringDesignation() == gears_id[7]) {
							Gear->extTol = PGTS24_06[0];
							Gear->rollerD = Dr[3];
						}
                        */
                    }
					currStr = MYCells.OlePropertyGet("Item",
						measurements->at(i), 12);
					try {
						Measurment->fToleranceUp = currStr.ToDouble();
					} catch (...) {
						Measurment->fToleranceUp = 0;
					}
					currStr = MYCells.OlePropertyGet("Item",
						measurements->at(i), 13);
					try {
						Measurment->fToleranceDown = currStr.ToDouble();
					} catch (...) {
						Measurment->fToleranceDown = 0;
					}
				}
				void *mrmt = Measurment;
				listparams->Add(mrmt);
			}
		}
		return checkMasurementsData(listparams, Gear->Desgination.getStringDesignation());
	}
	catch (...) {
		return 0;
	}
}

double calculateTolerance(TGear* gear, TGear* wheel, int n, TMemo* memoLog) {
	double Diam;
	if (N[n] % 2 == 0) {
		Diam = diameter(gear, memoLog) - gear->rollerD;
	}
	else {
		Diam = (diameter(gear, memoLog) - gear->rollerD) / cos(90 * CtR / N[n]);
	}
	double alphaP = acos(M[n] * N[n] * cos(20 * CtR) / Diam);
	double Eg =
		(M_PI / 2 + N[n] * (tan(alphaP) - alphaP - (tan(20 * CtR) - 20 * CtR)) -
		gear->rollerD / (M[n] * cos(20 * CtR))) / (2 * tan(20 * CtR));
	int m = gearWheel(n);
	double Ek = -Eg - Of[m] / M[m] - Of[n] / M[n];
	double Xp = digitalCalc(m, wheel->rollerD, Ek, memoLog);
	if (N[m] % 2 == 0) {
		return (N[m] * M[m] * cos(20 * CtR) / cos(Xp) + wheel->rollerD);
	}
	else {
		return (N[m] * M[m] * cos(20 * CtR) * cos(90 * CtR / N[m]) / cos(Xp)
			+ wheel->rollerD);
	}
}

double digitalCalc(int m, double Dr, double Ek, TMemo* memoLog) {
	double eps = 0.000001;
	double c = 0;
	double a = 0;
	double b = 1;
	while (b - a > eps) {
		c = (a + b) / 2;
		if (f(a, m, Dr, Ek) * f(c, m, Dr, Ek) < 0) {
			b = c;
		}
		else {
			a = c;
		}
	}
	return (a + b) / 2;
}

double f(double x, int m, double Dr, double Ek) {
	return (tan(x) - x - Dr / (N[m] * M[m] * cos(20 * CtR)) -
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

int checkMasurementsData(TList* listparams, UnicodeString _PGTS) {
	bool e_controller = true;
	bool e_operator = true;
	bool e_roller = true;
	bool e_runout = true;
	//if not exist, detail is suspicious
	bool e_OK = true;
    //if not exist, detail is suspicious
	bool is_filled = false;
	double runout = 0;
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
			runout = Measurment->fMeasure;
		}
		else if (Measurment->sKeyMeasure == "Окончательный контроль") {
			e_OK = (Measurment->fMeasure == 1);
		}
		if (Measurment->sKeyMeasure == baseMeasure) {
			is_filled = true;
		}
	}
	if (!e_OK) {
		return 0;
	}
	else if (e_controller && e_operator && !e_roller && !e_runout) {
		return 0;
	}
	else if (!is_filled) {
        return 0;
	}
	else if (runout > 0.1) {
		if (_PGTS != gears_id[7]) {
			return 0;
		} else {
			if (runout > 0.14) {
				return 0;
			}
        }
	}
	else {
		return 1;
	}
}

bool measureInLimits(stMeasurement* Measurment) {
	if (Measurment->sKeyMeasure == "9.9.") {
		if (Measurment->fMeasure >= 0.09) return false;
        else return true;
	}
	else {
		if (Measurment->fMeasure >= Measurment->fNominalValue +
			Measurment->fToleranceDown && Measurment->fMeasure <=
			Measurment->fNominalValue + Measurment->fToleranceUp) {
			return true;
		}
		else {
			return false;
		}
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
//---------------------------------------------------------------------------

bool cellIsMeasure(UnicodeString str) {
	if (str == "9.6.1." || str == "9.6.2." || str == "9.6.3." || str == "9.9.") {
		return true;
	} else {
		return false;
    }
}
//---------------------------------------------------------------------------

UnicodeString correctName(UnicodeString ustr) {
	AnsiString astr = ustr;
	string str = astr.c_str();
	if (str.find("\n") != string::npos) {
		str.erase(str.find("\n"), str.find("\n") + 1);
	}
	return str.c_str();
}
//---------------------------------------------------------------------------

UnicodeString parseFileName(AnsiString _FileName) {
	string sName = _FileName.c_str();
	while (sName.find("\\") != string::npos) {
		sName.erase(0, sName.find("\\") + 1);
	}
	return sName.c_str();
}
//---------------------------------------------------------------------------

bool ContainsSubstring(const UnicodeString& str, const UnicodeString& subStr) {
	return Pos(subStr, str) > 0;
}
//---------------------------------------------------------------------------
