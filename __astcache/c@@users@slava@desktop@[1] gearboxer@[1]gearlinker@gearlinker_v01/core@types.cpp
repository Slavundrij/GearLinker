// ---------------------------------------------------------------------------

#pragma hdrstop

#include "Types.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

/*
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
 */
/////////////////////////////////////////////////////////////
TDesignation::TDesignation() {
    flagExist = false;
}

TDesignation::TDesignation(UnicodeString _sDesignation) {
	// ПГТС.721124.006-00
	flagExist = true;
	try {
		sCodeDeveloper = _sDesignation.SubString(1, 4);
	} catch (...) {
		sCodeDeveloper = "";
		flagExist = false;
	}
    try {
        uiClassification = (_sDesignation.SubString(6, 6)).ToInt();
	}
	catch (...) {
        uiClassification = 0;
		flagExist = false;
    }
    try {
        uiRegistrationNumber = (_sDesignation.SubString(13, 3)).ToInt();
	}
	catch (...) {
        uiRegistrationNumber = 0;
        flagExist = false;
	}
    try {
		uiVariation = (_sDesignation.SubString(17, 2)).ToInt();
	}
	catch (...) {
		uiVariation = 0;
	}

}

TDesignation::TDesignation(UnicodeString _sCodeDeveloper,
    unsigned int _uiClassification, unsigned int _uiRegistrationNumber,
    unsigned int _uiVariation) {

    sCodeDeveloper = _sCodeDeveloper;
    uiClassification = _uiClassification;
    uiRegistrationNumber = _uiRegistrationNumber;
    uiVariation = _uiVariation;
	flagExist = true;
}

TDesignation::~TDesignation() {
    flagExist = false;
}

void TDesignation::vUpdate(UnicodeString _sCodeDeveloper,
    unsigned int _uiClassification, unsigned int _uiRegistrationNumber,
    unsigned int _uiVariation) {

    sCodeDeveloper = _sCodeDeveloper;
    uiClassification = _uiClassification;
    uiRegistrationNumber = _uiRegistrationNumber;
    uiVariation = _uiVariation;
    flagExist = true;
}

UnicodeString TDesignation::getStringDesignation() {
	if (flagExist) {
		if (uiVariation == 0) {
			return sCodeDeveloper + "." + Format("%.6d", uiClassification) + "." +
				Format("%.3d", uiRegistrationNumber);
		}
		else {
			return sCodeDeveloper + "." + Format("%:6", uiClassification) + "." +
				Format("{:3}", uiRegistrationNumber) + "-" + Format("{:2}",
				uiVariation);
		}
	} else {
		return "";
	}
}

/*
 class TGear {
 public:
 TGear();
 ~TGear();

 TDesignation Desgination; // Номер детали - ПГТС. ...
 unsigned int uiOrder; // Номер заказа
 UnicodeString sName; // Название детали (чтоб было)
 UnicodeString sNumber; // Номер детали (на нем отмечено типо)
 TList *listParams;

 private:
 };
 */
////////////////////////////////////////////////////
TGear::TGear(TDesignation _Desgination, unsigned int _uiOrder, UnicodeString _sName, UnicodeString _sNumber) {
	Desgination = _Desgination;
	uiOrder = _uiOrder;
	sName = _sName;
    sNumber = _sNumber;
}

TGear::~TGear() {

}

/*
class TGearing {
public:
	TGearing(TGear* gear1, TGear* gear2);
	~TGearing();

	TGear* Gear1;
	TGear* Gear2;

private:
};
 */
////////////////////////////////////////////////////
TGearing::TGearing(TGear* gear1, TGear* gear2) {
	Gear1 = gear1;
	Gear2 = gear2;
}

TGearing::~TGearing() {
}

/*
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
*/

TGearbox_DB65::TGearbox_DB65() {
	TAxis axis1, axis2, axis3, axis4, axis5;
	Axis1 = axis1;
	Axis2 = axis2;
	Axis3 = axis3;
	Axis4 = axis4;
	Axis5 = axis5;
}

TGearbox_DB65::~TGearbox_DB65() {
}

void TGearbox_DB65::vSetGearing1(TGearing* _gearing) {
	TDesignation desMot("");
	TGear Motor(desMot, 0, "Двигатель", 0);
	Axis1.vSetJoint1(Axis1.eGetJoint1(&Motor), &Motor);
	Axis1.vSetJoint2(Axis1.eGetJoint2(_gearing->Gear1), _gearing->Gear1);
	Axis2.vSetJoint1(Axis2.eGetJoint1(_gearing->Gear2), _gearing->Gear2);
}

void TGearbox_DB65::vSetGearing2(TGearing* _gearing) {
	Axis2.vSetJoint2(Axis2.eGetJoint2(_gearing->Gear1), _gearing->Gear1);
	Axis3.vSetJoint1(Axis3.eGetJoint1(_gearing->Gear2), _gearing->Gear2);
}

void TGearbox_DB65::vSetGearing3(TGearing* _gearing) {
	Axis3.vSetJoint2(Axis3.eGetJoint2(_gearing->Gear1), _gearing->Gear1);
	Axis4.vSetJoint1(Axis4.eGetJoint1(_gearing->Gear2), _gearing->Gear2);
}

void TGearbox_DB65::vSetGearing4(TGearing* _gearing) {
	TDesignation desBlanc("");
	TGear blanc(desBlanc, 0, "", 0);
	Axis4.vSetJoint2(Axis4.eGetJoint2(_gearing->Gear1), _gearing->Gear1);
	Axis5.vSetJoint1(Axis5.eGetJoint1(_gearing->Gear2), _gearing->Gear2);
	Axis5.vSetJoint2(Axis5.eGetJoint2(&blanc), &blanc);
}

void TGearbox_DB65::vSave2JSON() {
	ofstream MyFile("DB65.json");
	auto t = time(nullptr);
	auto tm = *localtime(&t);

	UnicodeString head = Format("  \"Date\": \"{}\".\"{}\".\"{}\",\n\
						\"Gearboxes\": [\n\
							{\n\
							   \"Gearbox_name\": \"DB-65\",\n\
								\"Gearbox\": {\n\
								   \"Axes\": [\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

/*
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
*/

TAxis::TAxis() {
}

TAxis::~TAxis() {
}

void TAxis::vSetJoint1(enum typeJoint eTypeJoint, TGear* _Gear) {
	eTypeJoint1 = eTypeJoint;
	Gear1 = _Gear;
}

void TAxis::vSetJoint2(enum typeJoint eTypeJoint, TGear* _Gear) {
	eTypeJoint2 = eTypeJoint;
	Gear2 = _Gear;
}

enum typeJoint TAxis::eGetJoint1(TGear* _Gear) {
	UnicodeString name = _Gear->sName;
	if(name == "Колесо промежуточное" || name == "Шестерня промежуточная") {
		return Gear;
	} else if (name == "Двигатель") {
		return Motor;
	} else if (name == "Вал выходной") {
		return OutShaft;
	} else {
		return None;
	}
}

enum typeJoint TAxis::eGetJoint2(TGear* _Gear) {
	UnicodeString name = _Gear->sName;
	if(name == "Колесо промежуточное" || name == "Шестерня промежуточная") {
		return Gear;
	} else if (name == "Двигатель") {
		return Motor;
	} else if (name == "Вал выходной") {
		return OutShaft;
	} else {
		return None;
	}
}

