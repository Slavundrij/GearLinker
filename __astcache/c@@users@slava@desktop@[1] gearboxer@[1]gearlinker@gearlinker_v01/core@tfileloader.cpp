// ---------------------------------------------------------------------------

#pragma hdrstop

#include "TFileLoader.h"

#include <ComObj.hpp>
#include <Vcl.OleCtrls.hpp>
#include <Vcl.FileCtrl.hpp>
#include "objbase.h"
#include <math.h>

// ---------------------------------------------------------------------------
#pragma package(smart_init)

double CtR = M_PI / 180;

vector<double> M = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.8, 0.8};
vector<double> N = {16, 70, 19, 67, 16, 70, 14, 50};
vector<double> Dr = {0.99, 0.99, 0.99, 0.99, 0.99, 0.99, 1.5, 1.5};
vector<double> Of = {0.022, 0.032, 0.022, 0.032, 0.022, 0.032, 0.026, 0.038};
vector<double> Tol = {0.093-0.035, 0.15-0.073, 0.082-0.038, 0.142-0.07, 0.093-0.035, 0.15-0.073, 0.066-0.039, 0.119-0.079};

vector<UnicodeString> gears_id = {"ПГТС.721144.007", "ПГТС.721134.015", "ПГТС.721164.005", "ПГТС.721134.016", "ПГТС.721164.006", "ПГТС.721134.014", "ПГТС.721164.007", "ПГТС.721124.006"};

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

Variant ExcelApp, ExcelBooks, ExcelBook, ExcelSheet;

void vLoadGearsFromExcel(TList* suspGearList, TList* stanGearList, TList* goodGearList, AnsiString FileName, TMemo* memoLog)
{

    ExcelApp = CreateOleObject("Excel.Application");

    Variant qwe = "S:\\100000245-QS.xls";

    try {
		ExcelApp.OlePropertySet("Visible", false); // setup Show Excel.
		ExcelBooks = ExcelApp.OlePropertyGet("Workbooks");
		ExcelBook = ExcelBooks.OleFunction("Open", WideString(FileName));
		memoLog->Lines->Add(FileName);

		int iRowsCount = ExcelApp.OlePropertyGet("ActiveSheet").OlePropertyGet("UsedRange").OlePropertyGet("Rows").OlePropertyGet("Count");
		int iColsCount = ExcelApp.OlePropertyGet("ActiveSheet").OlePropertyGet("UsedRange").OlePropertyGet("Columns").OlePropertyGet("Count");


		//First of all we parse PGTS////////////////////////////////////////////
		Variant vCells = ExcelApp.OlePropertyGet("Cells");
		AnsiString detailNum = vCells.OlePropertyGet("Item", 5, 3);
		TDesignation PGTS(detailNum);
		////////////////////////////////////////////////////////////////////////

		//Secondly suggesting to start with order № since its common for this///
		//sheet/////////////////////////////////////////////////////////////////
		AnsiString sOrderNum = vCells.OlePropertyGet("Item", 3, 3);
		unsigned int orderNum = sOrderNum.ToInt();
		AnsiString sGearName = vCells.OlePropertyGet("Item", 3, 12);
		////////////////////////////////////////////////////////////////////////

		//Finally we come to gears characteristics & gears themselves///////////     "ПГТС.721164.006"
		vector<unsigned int> filledMeasuarments;

		getFilledMeasurementRows(iRowsCount, 2, &filledMeasuarments);

		//All gears in sheet cycle
		for (int i = 15; i < iColsCount; i++) {
			AnsiString gearNumber = vCells.OlePropertyGet("Item", 9, i);
			if (isCellFilled(gearNumber)) {
				TGear newGear(PGTS, orderNum, sGearName, gearNumber);
				TList* listparams = new TList();
				//fun for taking all measurements
				fillGearMeasurments(&filledMeasuarments, listparams, i);

				newGear.listParams = listparams;
				void *nG = &newGear;
				switch (checkMasurementsData(listparams)) {
					case 0:
						suspGearList->Add(nG);
						break;
					case 1:
						stanGearList->Add(nG);
						break;
					case 2:
						goodGearList->Add(nG);
						break;
					default:
						break;
				}

				//memoLog->Lines->Add(fff->uiOrder);
			}
		}

		buildGoodGearboxes(goodGearList);
		//add unused gears to stanGearList;
		//buildStandartGearboxes(stanGearList);
		//dosmthwthunusedgears(stanGearList);
		//dosmthwthbadgears(suspGearList);
		//delete goodGearList;
		//delete stanGearList;
		//delete suspGearList;
		//Не забыть очищать память после использования!!!!!!!!!!!!!!!!!!!!!!!
		//delete listparams;

		//ExcelSheets = ExcelApp.OlePropertyGet("SheetsInNewWorkbook");
	}
    catch (Exception &E) {
        memoLog->Lines->Add("--- Cant open file:");
    }

	ExcelApp.OleProcedure("Quit");  //Quit Excel.
}

void buildGoodGearboxes(TList* goodList) {
	vector<TGear*> gears;
	while(goodList->Count != 0) {
		bool enoughGears = true;
		for (int i = 0; i<gears_id.size(); i++) {
			enoughGears &= findGear(goodList, gears_id.at(i), &gears);
		}
		if (enoughGears) {
			TGearing gearing1(gears.at(0), gears.at(1));
			TGearing gearing2(gears.at(2), gears.at(3));
			TGearing gearing3(gears.at(4), gears.at(5));
			TGearing gearing4(gears.at(6), gears.at(7));

			TGearbox_DB65 DB65;
			DB65.vSetGearing1(&gearing1);
			DB65.vSetGearing2(&gearing2);
			DB65.vSetGearing3(&gearing3);
			DB65.vSetGearing4(&gearing4);

			DB65.vSave2JSON();
		} else {
			return;
		}
	}
	return;
}

void buildStandartGearboxes(TList* stanList) {
	vector<TGear*> gears1;
	vector<TGear*> gears2;
	while(stanList->Count != 0) {
		bool enoughGears1 = true;
		bool enoughGears2 = true;

		for (int i = 0; i < gears_id.size()/2; i++) {
			enoughGears1 &= findSpecialGear(stanList, i*2, &gears1);
			enoughGears2 &= findSpecialGear(stanList, (i+1)*2, &gears2);
		}

		if (enoughGears1) {
			TGearing gearing1(gears1.at(0), gears1.at(1));
			TGearing gearing2(gears1.at(2), gears1.at(3));
			TGearing gearing3(gears1.at(4), gears1.at(5));
			TGearing gearing4(gears1.at(6), gears1.at(7));

			TGearbox_DB65 DB65;
			DB65.vSetGearing1(&gearing1);
			DB65.vSetGearing2(&gearing2);
			DB65.vSetGearing3(&gearing3);
			DB65.vSetGearing4(&gearing4);

			DB65.vSave2JSON();
		}

		if (enoughGears2) {
			TGearing gearing1_1(gears2.at(0), gears2.at(1));
			TGearing gearing2_1(gears2.at(2), gears2.at(3));
			TGearing gearing3_1(gears2.at(4), gears2.at(5));
			TGearing gearing4_1(gears2.at(6), gears2.at(7));

			TGearbox_DB65 DB65_1;
			DB65_1.vSetGearing1(&gearing1_1);
			DB65_1.vSetGearing2(&gearing2_1);
			DB65_1.vSetGearing3(&gearing3_1);
			DB65_1.vSetGearing4(&gearing4_1);

			DB65_1.vSave2JSON();
		}

		if (!enoughGears1 && !enoughGears2) {
			return;
		}
	}
	return;
}

bool findSpecialGear(TList* gearList, int number, vector<TGear*>* measurements) {
	for (int i = 0; i<gearList->Count; i++) {
		TGear* gear = (TGear*)gearList->Items[i];
		if (gear->Desgination.getStringDesignation() == gears_id[number]) {
			double tolerance = calculateTolerance(gear, number);
            return false;
		}
	}
	return false;
}

double calculateTolerance(TGear* gear, int n) {
	double alphaP = acos(M[n] * N[n] * cos(20 * CtR) / (diameter(gear) - Dr[n]));
	double Eg = (M_PI / 2 + N[n] * (tan(alphaP) - alphaP - (tan(20 * CtR) - 20 * CtR)) - Dr[n] / (M[n] * cos(20* CtR))) / (2 * tan(20*CtR));
	int m = gearWheel(n);
	double Ek = -Eg - Of[m] / M[m] - Of[n] / M[n];
	double Xp = digitalCalc(n, Ek);
	return (N[m] * M[m] * cos(20*CtR) / cos(Xp) + Dr[m]);
}

double digitalCalc(int n, double Ek) {
	double x = 0;
	double dx = 0;
	int iter = 0;
	int b = 5;
	double eps = 0.0001;
	while (1) {
		dx = b * f(x, n, Ek);
		x = x + dx;
		if (abs(dx) < eps) break;
		iter++;
		if (iter > 250) break;
	}
    return x;
}

double f(double x, int n, double Ek) {
	int m = gearWheel(n);
	return (x - (tan(x) - x - Dr[m]/(N[m] * M[m] * cos(20 * CtR)) - (tan(20 * CtR) - 20 * CtR) + (M_PI / 2 - 2 * Ek * tan(20*CtR))/N[m]));
}

int gearWheel(int n) {
	if (n % 2 == 0) {
		return n - 1;
	} else {
		return n + 1;
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

bool findGear(TList* goodList, UnicodeString desig, vector<TGear*>* gears) {
	for (int i = 0; i<goodList->Count; i++) {
		TGear* gear = (TGear*)goodList->Items[i];
		if (gear->Desgination.getStringDesignation() == desig) {
			gears->push_back(gear);
			goodList->Remove(gear);
			return true;
		}
	}
	return false;
}

void getFilledMeasurementRows(unsigned int rowCnt, unsigned int Col, vector<unsigned int>* measurements) {
	int myCase = 0;
	
	try {
		Variant MYCells = ExcelApp.OlePropertyGet("Cells");
		for (unsigned int i = 1; i < rowCnt; i++) {

			AnsiString currStr = MYCells.OlePropertyGet("Item", i, Col);
			
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
	} catch (...) {
		return;
	}
}

int fillGearMeasurments(vector<unsigned int>* measurements, TList* listparams, int col) {
	try {
		Variant MYCells = ExcelApp.OlePropertyGet("Cells");
		for (int i = 0; i < measurements->size(); i++) {
			AnsiString currStr = MYCells.OlePropertyGet("Item", measurements->at(i), col);
			if (isCellFilled(currStr)) {
				stMeasurement* Measurment = new stMeasurement;
				Measurment->fMeasure = currStr.ToDouble();
				currStr = MYCells.OlePropertyGet("Item", measurements->at(i), 2);
				Measurment->sKeyMeasure = currStr;
				currStr = MYCells.OlePropertyGet("Item", measurements->at(i), 11);
				Measurment->fNominalValue = currStr.ToDouble();
				currStr = MYCells.OlePropertyGet("Item", measurements->at(i), 12);
				Measurment->fToleranceUp = currStr.ToDouble();
				currStr = MYCells.OlePropertyGet("Item", measurements->at(i), 13);
				Measurment->fToleranceDown = currStr.ToDouble();
				void *mrmt = Measurment;
				listparams->Add(mrmt);
				//delete Measurment;
			}
		}
		return checkMasurementsData(listparams);
	} catch(...) {
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
		} else if (Measurment->sKeyMeasure == "9.6.2.") {
			e_operator = measureInLimits(Measurment);
		} else if (Measurment->sKeyMeasure == "9.6.3.") {
			e_roller = measureInLimits(Measurment);
		} else if (Measurment->sKeyMeasure == "9.9.") {
			e_runout = measureInLimits(Measurment);
		}
	}
	if (e_roller) {
		return 1;
	} else {
		return 2;
    }
}

bool measureInLimits(stMeasurement* Measurment) {
	if (Measurment->fMeasure >= Measurment->fNominalValue + Measurment->fToleranceDown && Measurment->fMeasure <= Measurment->fNominalValue + Measurment->fToleranceUp) {
		return true;
	} else {
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
