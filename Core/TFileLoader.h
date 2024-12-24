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
vector<UnicodeString> gears_id = { "ПГТС.721144.007", "ПГТС.721134.015",
    "ПГТС.721164.005", "ПГТС.721134.016", "ПГТС.721164.006", "ПГТС.721134.014",
    "ПГТС.721164.007", "ПГТС.721124.006" };

//map<Key:PGTS, map<Key:NominalDiam, Pair<Allowance, Dr>>>
extern map<UnicodeString, map<double, pair<double, double> > > RollerSizes;

vector<double> Dr = { 1, 0.99, 1.074, 1.5 };

extern vector<double> M;
extern vector<int> N;
extern vector<double> Of;

extern UnicodeString baseMeasure;

vector<UnicodeString> denialReasons = { "окончательный контроль -",
    "подозрительные измерения -", "основной размер пустой -",
    "высокое биение -", "деталь вне допуска -", "нестандартная причина -" };
// ---------------------------------------------------------------------------

struct possiblePair
{
    TGear* Gear1;
    TGear* Gear2;
    UnicodeString wNum;
    double criteria;
};

class TFileList
{
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

void BuildGearboxes(TList* suspGearList, TList* stanGearList,
    TList* goodGearList, TMemo* memoLog, TMemo* memoInfo, TList* UsedGearList,
    TFileList* FileList, TMemo* memoRes);

void vLoadGearsFromExcel(TList* suspGearList, TList* stanGearList,
    TList* goodGearList, AnsiString FileName, TMemo* memoLog, TMemo* memoInfo);
void getFilledMeasurementRows(
    unsigned int rowCnt, unsigned int Col, vector<unsigned int>* measurements);
int fillGearMeasurments(vector<unsigned int>* measurements, TGear* Gear,
    TList* listparams, int col);
int checkMasurementsData(TList* listparams, TGear* Gear);
bool measureInLimits(stMeasurement* Measurment);
bool isCellFilled(AnsiString cell);

void buildGoodGearboxes(TList* goodList);
bool findGear(TList* goodList, UnicodeString desig, vector<TGear*>* gears);

bool findSpecialGear(
    TList* gearList, int number, vector<TGear*>* measurements, TMemo* memoLog);
void buildStandartGearboxes(TList* stanList, TList* UsedGearList,
    TMemo* memoLog, TMemo* memoInfo, TMemo* memoRes);

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

bool ContainsSubstring(const UnicodeString &str, const UnicodeString &subStr);

#endif

