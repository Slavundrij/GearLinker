// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormAddDataFiles.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TFormAddDataFiles *FormAddDataFiles;

int ResultForm = 0;

TFileList* FileList;

TList* SuspGearList;
TList* StanGearList;
TList* GoodGearList;
TList* UsedGearList;

// ---------------------------------------------------------------------------
__fastcall TFormAddDataFiles::TFormAddDataFiles(TComponent* Owner)
    : TForm(Owner) {
}

// ---------------------------------------------------------------------------
void __fastcall TFormAddDataFiles::FormClose(TObject *Sender,
    TCloseAction &Action)

{
    if (ResultForm == 0) {
        Application->Terminate();
    }
}
// ---------------------------------------------------------------------------

void __fastcall TFormAddDataFiles::lbxFileDblClick(TObject *Sender) {
    FileList->vAppendFile(lbxDir->Directory + "\\" +
        lbxFile->Items->Strings[lbxFile->ItemIndex]);
    FileList->vShowFilesListBox(lbxResult);
}
// ---------------------------------------------------------------------------

void __fastcall TFormAddDataFiles::lbxResultDblClick(TObject *Sender) {
    FileList->vDeleteFile(lbxResult->ItemIndex);
    FileList->vShowFilesListBox(lbxResult);
}
// ---------------------------------------------------------------------------

void __fastcall TFormAddDataFiles::btnLoadFilesClick(TObject *Sender) {
	//
	SuspGearList = new TList();
    StanGearList = new TList();
	GoodGearList = new TList();
	UsedGearList = new TList();
	memLog->Clear();
	memInfo->Clear();

    if (FileList->iGetCountFiles() > 0) {
		for (int i = 0; i < FileList->iGetCountFiles(); i++) {
			vLoadGearsFromExcel(SuspGearList, StanGearList, GoodGearList, FileList->sGetFile(i), memLog, memInfo);
            FormAddDataFiles->ProgressSet(i, FileList->iGetCountFiles());
		}
		BuildGearboxes(SuspGearList, StanGearList, GoodGearList, memLog, memInfo, UsedGearList, FileList);
        FormAddDataFiles->ProgressReset();
	}
    else {
        ShowMessage("Choose files");
	}
}

// ---------------------------------------------------------------------------
void __fastcall TFormAddDataFiles::btnSystemClick(TObject *Sender) {
	memLog->Visible = !memLog->Visible;

    if (memLog->Visible == true) {
        btnSystem->Caption = "����. LOG";
    }
    else {
        btnSystem->Caption = "���. LOG";
    }
}
// ---------------------------------------------------------------------------

void __fastcall TFormAddDataFiles::CreateParams(Controls::TCreateParams &Params)
{
    TForm::CreateParams(Params);
    Params.ExStyle = Params.ExStyle | WS_EX_APPWINDOW;
    Params.WndParent = ParentWindow;
}

/* */

void TFormAddDataFiles::ProgressReset() {
    imgProgress->Canvas->Pen->Color = clBtnFace;
    imgProgress->Canvas->Brush->Color = clBtnFace;
    imgProgress->Canvas->Rectangle(0, 0, imgProgress->Width,
        imgProgress->Height);
}

void TFormAddDataFiles::ProgressSet(int Cur, int Max) {
    imgProgress->Canvas->Pen->Color = clSilver;
    imgProgress->Canvas->Brush->Color = clBtnFace;
    imgProgress->Canvas->Rectangle(0, 0, imgProgress->Width,
        imgProgress->Height);

    imgProgress->Canvas->Pen->Color = clSilver;
    imgProgress->Canvas->Brush->Color = clGreen;    
    imgProgress->Canvas->Rectangle(0, 0, imgProgress->Width * Cur/Max,
        imgProgress->Height);

        imgProgress->Canvas->Font->Color = clBlack;
            AnsiString Text = IntToStr(Cur) + "/" + IntToStr(Max);
            imgProgress->Canvas->Font->Size = 10;

            int X = imgProgress->Width / 2 - imgProgress->Canvas->TextWidth(Text) / 2;
            int Y = imgProgress->Height / 2 - imgProgress->Canvas->TextHeight(Text) / 2;

            imgProgress->Canvas->TextOut(X, Y, Text);
        
}
/* */

/* ---- */
void setLeft(TControl* component, int _Left) {
    component->Left = _Left;
}

void setTop(TControl* component, int _Top) {
    component->Top = _Top;
}

void setWidth(TControl* component, int _Width) {
    component->Width = _Width;
}

void setHeight(TControl* component, int _Height) {
    component->Height = _Height;
}

void setPosition(TControl* component, int _Top, int _Left) {
    setTop(component, _Top);
    setLeft(component, _Left);
}

void setSize(TControl* component, int _Width, int _Height) {
    setWidth(component, _Width);
    setHeight(component, _Height);
}
/* ---- */

void __fastcall TFormAddDataFiles::FormResize(TObject *Sender) {
    //
    if (this->ClientWidth < FORM_MIN_WIDTH) {
        this->ClientWidth = FORM_MIN_WIDTH;
    }
    if (this->ClientHeight < FORM_MIN_HEIGHT) {
        this->ClientHeight = FORM_MIN_HEIGHT;
    }

    setPosition(Label4, 35, 65);
    setPosition(cbxDrive, 55, 10);

    setPosition(Label3, 90, 65);

    setPosition(lbxDir, 110, 10);
    setHeight(lbxDir, this->ClientHeight - 120);

    setPosition(memLog, 10, 240);
    setSize(memLog, this->ClientWidth - 360, 80);

    setPosition(Label2, 90, 280);

    setPosition(lbxFile, 110, 240);
    setSize(lbxFile, 200, this->ClientHeight - 220);

    setPosition(memInfo, this->ClientHeight - 90, 240);
    setSize(memInfo, this->ClientWidth - 360, 80);

    setPosition(lbxResult, 110, 460);
    setSize(lbxResult, this->ClientWidth - 470, this->ClientHeight - 220);

    setPosition(Label1, 90, lbxResult->Left + (lbxResult->Width / 2) -
        Label1->Width / 2);

    setPosition(btnInfo, 10, this->ClientWidth - 105);

    setPosition(btnSystem, 50, this->ClientWidth - 105);

    setPosition(btnLoadFiles, this->ClientHeight - 90, this->ClientWidth - 90);
}
// ---------------------------------------------------------------------------
void __fastcall TFormAddDataFiles::FormCreate(TObject *Sender)
{
    FileList = new TFileList();
}
//---------------------------------------------------------------------------
