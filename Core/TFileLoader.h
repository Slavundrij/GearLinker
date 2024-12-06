// ---------------------------------------------------------------------------

#ifndef TFileLoaderH
#define TFileLoaderH

#include "TFormAddDataFiles.h"
#include "Types.h"
#include <System.Classes.hpp>
#include <Vcl.StdCtrls.hpp>
#include <vector>
#include <System.JSON.hpp>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

// ---------------------------------------------------------------------------
vector<UnicodeString>gears_id = {
	"厦已.721144.007", "厦已.721134.015", "厦已.721164.005", "厦已.721134.016",
	"厦已.721164.006", "厦已.721134.014", "厦已.721164.007", "厦已.721124.006"};

//map<Key:PGTS, map<Key:NominalDiam, Pair<Allowance, Dr>>>
extern map<UnicodeString, map<double, pair<double, double>>> RollerSizes;

vector<double>PGTS44_07 = {0.059, 0.058, 0.056};
vector<double>PGTS34_15 = {0.076, 0.077, 0.071};
vector<double>PGTS64_05 = {0.043, 0.044, 0.041};
vector<double>PGTS34_16 = {0.072, 0.072, 0.07};
vector<double>PGTS64_06 = {0.045, 0.045, 0.043};
vector<double>PGTS34_14 = {0.067, 0.067, 0.065};

vector<double>PGTS44_07_nom = {9.822, 9.793, 10.035};
vector<double>PGTS34_15_nom = {36.445, 36.409, 36.715};
vector<double>PGTS64_05_nom = {11.099, 11.068, 11.325};
vector<double>PGTS34_16_nom = {35.203, 35.168, 35.463};
vector<double>PGTS64_06_nom = {9.822, 9.793, 10.035};
vector<double>PGTS34_14_nom = {36.445, 36.409, 36.715};

vector<double>PGTS64_07 = {0.027};
vector<double>PGTS24_06 = {0.04};
vector<double>Dr = {1, 0.99, 1.074, 1.5};

extern vector<double>M;
extern vector<int>N;
extern vector<double>Of;

extern UnicodeString baseMeasure;
// ---------------------------------------------------------------------------

struct possiblePair {
	TGear* Gear1;
	TGear* Gear2;
	UnicodeString wNum;
	double criteria;
};


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

void BuildGearboxes(TList* suspGearList, TList* stanGearList, TList* goodGearList, TMemo* memoLog, TMemo* memoInfo, TList* UsedGearList, TFileList* FileList, TMemo* memoRes);

void vLoadGearsFromExcel(TList* suspGearList, TList* stanGearList, TList* goodGearList, AnsiString FileName, TMemo* memoLog, TMemo* memoInfo);
void getFilledMeasurementRows(unsigned int rowCnt, unsigned int Col,
	vector<unsigned int>* measurements);
int fillGearMeasurments(vector<unsigned int>* measurements, TGear* Gear,
	TList* listparams, int col);
int checkMasurementsData(TList* listparams, UnicodeString _PGTS);
bool measureInLimits(stMeasurement* Measurment);
bool isCellFilled(AnsiString cell);

void buildGoodGearboxes(TList* goodList);
bool findGear(TList* goodList, UnicodeString desig, vector<TGear*>* gears);

bool findSpecialGear(TList* gearList, int number, vector<TGear*>* measurements, TMemo* memoLog);
void buildStandartGearboxes(TList* stanList, TList* UsedGearList, TMemo* memoLog, TMemo* memoInfo, TMemo* memoRes);

double diameter(TGear* gear, TMemo* memoLog);

bool cellIsMeasure(UnicodeString str);

int gearWheel(int n);
double f(double x, int m, double Dr, double Ek);
double digitalCalc(int n, double Dr, double Ek, TMemo* memoLog);
double calculateTolerance(TGear* gear, TGear* wheel, int n, TMemo* memoLog);
UnicodeString correctName(UnicodeString ustr);

void PaintUsedGears(TList* UsedGearList, TFileList* FileList, TMemo* memoInfo);

possiblePair* findWorstPair(TList* _FindMatches, TMemo* _memoLog);
void clearAllRepeats(TList* FindMatches, UnicodeString wNum, TMemo* memoInfo);
UnicodeString parseFileName(AnsiString _FileName);

bool ContainsSubstring(const UnicodeString& str, const UnicodeString& subStr);

#endif
