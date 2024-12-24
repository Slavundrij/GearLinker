// ---------------------------------------------------------------------------

#pragma hdrstop

#include "Types.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

vector<UnicodeString> gears_id = { "ПГТС.721144.007", "ПГТС.721134.015",
    "ПГТС.721164.005", "ПГТС.721134.016", "ПГТС.721164.006", "ПГТС.721134.014",
    "ПГТС.721164.007" };
UnicodeString Outshaft = "ПГТС.721124.006";
/////////////////////////////////////////////////////////////
TDesignation::TDesignation()
{
    flagExist = false;
}

TDesignation::TDesignation(UnicodeString _sDesignation)
{
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
    } catch (...) {
        uiClassification = 0;
        flagExist = false;
    }
    try {
        uiRegistrationNumber = (_sDesignation.SubString(13, 3)).ToInt();
    } catch (...) {
        uiRegistrationNumber = 0;
        flagExist = false;
    }
    try {
        //uiVariation = (_sDesignation.SubString(17, 2)).ToInt();
        uiVariation = 0;
    } catch (...) {
        uiVariation = 0;
    }
}

TDesignation::TDesignation(UnicodeString _sCodeDeveloper,
    unsigned int _uiClassification, unsigned int _uiRegistrationNumber,
    unsigned int _uiVariation)
{
    sCodeDeveloper = _sCodeDeveloper;
    uiClassification = _uiClassification;
    uiRegistrationNumber = _uiRegistrationNumber;
    uiVariation = _uiVariation;
    flagExist = true;
}

TDesignation::~TDesignation()
{
    flagExist = false;
}

void TDesignation::vUpdate(UnicodeString _sCodeDeveloper,
    unsigned int _uiClassification, unsigned int _uiRegistrationNumber,
    unsigned int _uiVariation)
{
    sCodeDeveloper = _sCodeDeveloper;
    uiClassification = _uiClassification;
    uiRegistrationNumber = _uiRegistrationNumber;
    uiVariation = _uiVariation;
    flagExist = true;
}

UnicodeString TDesignation::getStringDesignation()
{
    if (flagExist) {
        //if (uiVariation == 0) {
        return sCodeDeveloper + "." + Format("%.6d", uiClassification) + "." +
               Format("%.3d", uiRegistrationNumber);
        /*}
		else {
			return sCodeDeveloper + "." + Format("%:6", uiClassification) + "." +
				Format("{:3}", uiRegistrationNumber) + "-" + Format("{:2}",
				uiVariation);
		}
        */
    } else {
        return "";
    }
}

////////////////////////////////////////////////////
TGear::TGear(TDesignation _Desgination, unsigned int _uiOrder,
    UnicodeString _sName, UnicodeString _sNumber)
{
    invalidateReason = -1;
    Desgination = _Desgination;
    uiOrder = _uiOrder;
    sName = _sName;
    sNumber = _sNumber;
}

TGear::~TGear() {}

////////////////////////////////////////////////////
TGearing::TGearing(TGear* gear1, TGear* gear2)
{
    Gear1 = gear1;
    Gear2 = gear2;
}

TGearing::~TGearing() {}

TGearbox_DB65::TGearbox_DB65()
{
    TAxis axis1, axis2, axis3, axis4, axis5;
    Axis1 = axis1;
    Axis2 = axis2;
    Axis3 = axis3;
    Axis4 = axis4;
    Axis5 = axis5;
}

TGearbox_DB65::~TGearbox_DB65() {}

void TGearbox_DB65::vSetGearing1(TGearing* _gearing, TGear* Motor)
{
    Axis1.vSetJoint1(Axis1.eGetJoint1(Motor), Motor);
    Axis1.vSetJoint2(Axis1.eGetJoint2(_gearing->Gear1), _gearing->Gear1);
    Axis2.vSetJoint1(Axis2.eGetJoint1(_gearing->Gear2), _gearing->Gear2);
}

void TGearbox_DB65::vSetGearing2(TGearing* _gearing)
{
    Axis2.vSetJoint2(Axis2.eGetJoint2(_gearing->Gear1), _gearing->Gear1);
    Axis3.vSetJoint1(Axis3.eGetJoint1(_gearing->Gear2), _gearing->Gear2);
}

void TGearbox_DB65::vSetGearing3(TGearing* _gearing)
{
    Axis3.vSetJoint2(Axis3.eGetJoint2(_gearing->Gear1), _gearing->Gear1);
    Axis4.vSetJoint1(Axis4.eGetJoint1(_gearing->Gear2), _gearing->Gear2);
}

void TGearbox_DB65::vSetGearing4(TGearing* _gearing, TGear* OutShaft)
{
    Axis4.vSetJoint2(Axis4.eGetJoint2(_gearing->Gear1), _gearing->Gear1);
    Axis5.vSetJoint1(Axis5.eGetJoint1(_gearing->Gear2), _gearing->Gear2);
    Axis5.vSetJoint2(Axis5.eGetJoint2(OutShaft), OutShaft);
}

void TGearbox_DB65::vSave2JSON(TJSONArray* Gearboxes)
{
    TJSONObject* Gearbox = new TJSONObject();
    //For each DB65
    Gearbox->AddPair(new TJSONPair("Gearbox_name", "DB-65"));
    TJSONObject* Gear = new TJSONObject();
    //For each DB65
    Gearbox->AddPair(new TJSONPair("Gearbox", Gear));

    TJSONArray* Axes = new TJSONArray();
    Gear->AddPair("Axes", Axes);

    vector<TAxis*> Axis = { &Axis1, &Axis2, &Axis3, &Axis4, &Axis5 };

    TJSONObject* joints = new TJSONObject();
    TJSONObject* joint = new TJSONObject();
    for (int i = 0; i < Axis.size(); i++) {
        joints = new TJSONObject();
        joints->AddPair(new TJSONPair("Number", i + 1));
        joint = new TJSONObject();

        joints->AddPair("Joint1", joint);
        joint->AddPair(new TJSONPair(
            "Type", (UnicodeString)numToStr(Axis[i]->eTypeJoint1)));
        joint->AddPair(new TJSONPair("Designation",
            (UnicodeString)Axis[i]->Gear1->Desgination.getStringDesignation()));
        joint->AddPair(
            new TJSONPair("Order", (UnicodeString)Axis[i]->Gear1->uiOrder));
        joint->AddPair(
            new TJSONPair("Title", (UnicodeString)Axis[i]->Gear1->sName));
        joint->AddPair(
            new TJSONPair("Number", (UnicodeString)Axis[i]->Gear1->sNumber));

        joint = new TJSONObject();
        joints->AddPair("Joint2", joint);
        joint->AddPair(new TJSONPair(
            "Type", (UnicodeString)numToStr(Axis[i]->eTypeJoint2)));
        joint->AddPair(new TJSONPair("Designation",
            (UnicodeString)Axis[i]->Gear2->Desgination.getStringDesignation()));
        joint->AddPair(
            new TJSONPair("Order", (UnicodeString)Axis[i]->Gear2->uiOrder));
        joint->AddPair(
            new TJSONPair("Title", (UnicodeString)Axis[i]->Gear2->sName));
        joint->AddPair(
            new TJSONPair("Number", (UnicodeString)Axis[i]->Gear2->sNumber));

        Axes->AddElement(joints);
    }

    Gearboxes->AddElement(Gearbox);
}

UnicodeString numToStr(enum typeJoint type)
{
    if (type == Gear) {
        return "Gear";
    } else if (type == Motor) {
        return "Motor";
    } else if (type == OutShaft) {
        return "OutShaft";
    } else {
        return "None";
    }
}

TAxis::TAxis() {}

TAxis::~TAxis() {}

void TAxis::vSetJoint1(enum typeJoint eTypeJoint, TGear* _Gear)
{
    eTypeJoint1 = eTypeJoint;
    Gear1 = _Gear;
}

void TAxis::vSetJoint2(enum typeJoint eTypeJoint, TGear* _Gear)
{
    eTypeJoint2 = eTypeJoint;
    Gear2 = _Gear;
}

enum typeJoint TAxis::eGetJoint1(TGear* _Gear)
{
    UnicodeString name = _Gear->sName;
    if (name == "Колесо промежуточное" || name == "Шестерня промежуточная") {
        return Gear;
    } else if (name == "Двигатель") {
        return Motor;
    } else if (name == "Вал выходной") {
        return OutShaft;
    } else {
        return None;
    }
}

enum typeJoint TAxis::eGetJoint2(TGear* _Gear)
{
    UnicodeString name = _Gear->Desgination.getStringDesignation();
    for (int i = 0; i < gears_id.size(); i++) {
        if (name == gears_id[i]) {
            return Gear;
        } else if (name == Outshaft) {
            return OutShaft;
        }
    }
    if (_Gear->sName == "Двигатель") {
        return Motor;
    } else {
        return None;
    }
}

