// ---------------------------------------------------------------------------

#ifndef TFileLoaderH
#define TFileLoaderH

#include "Types.h"
#include <System.Classes.hpp>
#include <Vcl.StdCtrls.hpp>
#include <vector>
#include <System.JSON.hpp>

using namespace std;

// ---------------------------------------------------------------------------

class TFileList {
public:
    TFileList();
    ~TFileList();

    TStringList* pFileNames;

    void vAppendFile(UnicodeString sNewFile);
    void vDeleteFile(int iDeleteFileIndex);
    int iGetCountFiles();
    UnicodeString sGetFile(int iIndex);

    void vShowFilesListBox(TListBox* lsb);

private:
};

void BuildGearboxes(TList* suspGearList, TList* stanGearList, TList* goodGearList, TMemo* memoLog, TMemo* memoInfo, TList* UsedGearList, TFileList* FileList);

void vLoadGearsFromExcel(TList* suspGearList, TList* stanGearList, TList* goodGearList, AnsiString FileName, TMemo* memoLog, TMemo* memoInfo);
void getFilledMeasurementRows(unsigned int rowCnt, unsigned int Col,
	vector<unsigned int>* measurements);
int fillGearMeasurments(vector<unsigned int>* measurements,
	TList* listparams, int col);
int checkMasurementsData(TList* listparams);
bool measureInLimits(stMeasurement* Measurment);
bool isCellFilled(AnsiString cell);

void buildGoodGearboxes(TList* goodList);
bool findGear(TList* goodList, UnicodeString desig, vector<TGear*>* gears);

bool findSpecialGear(TList* gearList, int number, vector<TGear*>* measurements, TMemo* memoLog);
void buildStandartGearboxes(TList* stanList, TList* UsedGearList, TMemo* memoLog, TMemo* memoInfo);

double diameter(TGear* gear);

int gearWheel(int n);
double f(double x, int n, double Ek);
double digitalCalc(int n, double Ek, TMemo* memoLog);
double calculateTolerance(TGear* gear, int n, TMemo* memoLog);
UnicodeString correctName(UnicodeString ustr);

void PaintUsedGears(TList* UsedGearList, TFileList* FileList, TMemo* memoInfo);

#endif
