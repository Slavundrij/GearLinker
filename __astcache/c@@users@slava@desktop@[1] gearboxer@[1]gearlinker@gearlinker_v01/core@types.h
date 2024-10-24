// ---------------------------------------------------------------------------

#ifndef TypesH
#define TypesH

#include <System.Classes.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
// ---------------------------------------------------------------------------

enum typeJoint{Gear = 1, Motor = 2, OutShaft = 3, None = 0};

struct stMeasurement {
    UnicodeString sKeyMeasure;
    double fNominalValue;
    double fToleranceUp;
    double fToleranceDown;

    double fMeasure;

    bool bValidData = false;
};


/*Класс описания ПГТС.xxxxxx.xxx-xx*/
class TDesignation {
public:
    TDesignation();
	TDesignation(UnicodeString _sDesignation);
	TDesignation(UnicodeString _sCodeDeveloper, unsigned int _uiClassification,
		unsigned int _uiRegistrationNumber, unsigned int _uiVariation);

    ~TDesignation();

    void vUpdate(UnicodeString _sCodeDeveloper, unsigned int _uiClassification,
        unsigned int _uiRegistrationNumber, unsigned int _uiVariation);

	UnicodeString getStringDesignation();

private:
    bool flagExist = false;

    UnicodeString sCodeDeveloper;
    unsigned int uiClassification;
    unsigned int uiRegistrationNumber;
    unsigned int uiVariation;

};

/////////////////////////////////////////////////////////////
/*Шестерня, имеет Номер ПГТС.., Номер заказа, Название, Список измерений*/
class TGear {
public:
	TGear(TDesignation _Desgination, unsigned int _uiOrder,
		UnicodeString _sName, UnicodeString _sNumber);
    ~TGear();

    TDesignation Desgination; // Номер детали - ПГТС. ...
    unsigned int uiOrder; // Номер заказа
	UnicodeString sName; // Название детали (чтоб было)
    UnicodeString sNumber; // Номер детали (на нем отмечено типо)
    TList *listParams;

private:
};

class TGearing {
public:
	TGearing(TGear* gear1, TGear* gear2);
	~TGearing();

	TGear* Gear1;
	TGear* Gear2;

private:
};

class TAxis {
public:
	TAxis();
	~TAxis();

	void vSetJoint1(enum typeJoint eTypeJoint, TGear* _Gear);
	void vSetJoint2(enum typeJoint eTypeJoint, TGear* _Gear);

	enum typeJoint eGetJoint1(TGear* _Gear);
	enum typeJoint eGetJoint2(TGear* _Gear);

private:
	enum typeJoint eTypeJoint1;
	TGear* Gear1;

	enum typeJoint eTypeJoint2;
	TGear* Gear2;
};

class TGearbox_DB65 {
public:
	TGearbox_DB65();
	~TGearbox_DB65();

	void vSetGearing1(TGearing* _gearing);
	void vSetGearing2(TGearing* _gearing);
	void vSetGearing3(TGearing* _gearing);
	void vSetGearing4(TGearing* _gearing);

	void vSave2JSON();

private:

	TAxis Axis1;
	TAxis Axis2;
	TAxis Axis3;
	TAxis Axis4;
	TAxis Axis5;
};

#endif
