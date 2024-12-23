// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormAddDataFiles.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TFormAddDataFiles* FormAddDataFiles;

int ResultForm = 0;

TFileList* FileList;

TList* SuspGearList;
TList* StanGearList;
TList* GoodGearList;
TList* UsedGearList;

// ---------------------------------------------------------------------------

__fastcall TFormAddDataFiles::TFormAddDataFiles(TComponent* Owner) :
    TForm(Owner)
{
}
// ---------------------------------------------------------------------------

void __fastcall TFormAddDataFiles::FormClose(
    TObject* Sender, TCloseAction &Action)
{
    if (ResultForm == 0) {
        Application->Terminate();
    }
}
// ---------------------------------------------------------------------------

void __fastcall TFormAddDataFiles::lbxFileDblClick(TObject* Sender)
{
    FileList->vAppendFile(
        lbxDir->Directory + "\\" + lbxFile->Items->Strings[lbxFile->ItemIndex]);
    lbxFile->Items->Delete(lbxFile->ItemIndex);
    FileList->vShowFilesListBox(lbxResult);
}
// ---------------------------------------------------------------------------

void __fastcall TFormAddDataFiles::lbxResultDblClick(TObject* Sender)
{
    UnicodeString _fileName = FileList->sGetFile(lbxResult->ItemIndex)
                                  .SubString(lbxDir->Directory.Length() + 2,
                                      FileList->sGetFile(0).Length());
    lbxFile->Items->Add(_fileName);
    FileList->vDeleteFile(lbxResult->ItemIndex);
    FileList->vShowFilesListBox(lbxResult);
}
// ---------------------------------------------------------------------------

void __fastcall TFormAddDataFiles::btnLoadFilesClick(TObject* Sender)
{
    //
    SuspGearList = new TList();
    StanGearList = new TList();
    GoodGearList = new TList();
    UsedGearList = new TList();
    memLog->Clear();
    memInfo->Clear();
    vector<AnsiString> Repeats = {};

    if (FileList->iGetCountFiles() > 0) {
        for (int i = 0; i < FileList->iGetCountFiles(); i++) {
            if (find(begin(Repeats), end(Repeats), FileList->sGetFile(i)) ==
                end(Repeats)) {
                Repeats.push_back(FileList->sGetFile(i));
                vLoadGearsFromExcel(SuspGearList, StanGearList, GoodGearList,
                    FileList->sGetFile(i), memLog, memInfo);
            }
            FormAddDataFiles->ProgressSet(i + 1, FileList->iGetCountFiles());
        }
        UserHintForm->Memo1->Clear();
        BuildGearboxes(SuspGearList, StanGearList, GoodGearList, memLog,
            memInfo, UsedGearList, FileList, UserHintForm->Memo1);
        UserHintForm->Show();
        FormAddDataFiles->ProgressReset();
    } else {
        ShowMessage("Choose files");
    }
}

// ---------------------------------------------------------------------------

void __fastcall TFormAddDataFiles::btnSystemClick(TObject* Sender)
{
    memLog->Visible = !memLog->Visible;

    if (memLog->Visible == true) {
        btnSystem->Caption = "����. LOG";
    } else {
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
// ---------------------------------------------------------------------------

/* */

void TFormAddDataFiles::ProgressReset()
{
    imgProgress->Canvas->Pen->Color = clBtnFace;
    imgProgress->Canvas->Brush->Color = clBtnFace;
    imgProgress->Canvas->Rectangle(
        0, 0, imgProgress->Width, imgProgress->Height);
}

void TFormAddDataFiles::ProgressSet(int Cur, int Max)
{
    imgProgress->Canvas->Pen->Color = clSilver;
    imgProgress->Canvas->Brush->Color = clBtnFace;
    imgProgress->Canvas->Rectangle(
        0, 0, imgProgress->Width, imgProgress->Height);

    imgProgress->Canvas->Pen->Color = clSilver;
    imgProgress->Canvas->Brush->Color = clGreen;
    imgProgress->Canvas->Rectangle(
        0, 0, imgProgress->Width * Cur / Max, imgProgress->Height);

    imgProgress->Canvas->Font->Color = clBlack;
    AnsiString Text = IntToStr(Cur) + "/" + IntToStr(Max);
    imgProgress->Canvas->Font->Size = 10;

    int X = imgProgress->Width / 2 - imgProgress->Canvas->TextWidth(Text) / 2;
    int Y = imgProgress->Height / 2 - imgProgress->Canvas->TextHeight(Text) / 2;

    imgProgress->Canvas->TextOut(X, Y, Text);
}
/* */

/* ---- */
void setLeft(TControl* component, int _Left)
{
    component->Left = _Left;
}

void setTop(TControl* component, int _Top)
{
    component->Top = _Top;
}

void setWidth(TControl* component, int _Width)
{
    component->Width = _Width;
}

void setHeight(TControl* component, int _Height)
{
    component->Height = _Height;
}

void setPosition(TControl* component, int _Top, int _Left)
{
    setTop(component, _Top);
    setLeft(component, _Left);
}

void setSize(TControl* component, int _Width, int _Height)
{
    setWidth(component, _Width);
    setHeight(component, _Height);
}
/* ---- */
// ---------------------------------------------------------------------------

void __fastcall TFormAddDataFiles::FormResize(TObject* Sender)
{
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
    //setHeight(lbxDir, this->ClientHeight - 120);
    setSize(lbxDir, cbxDrive->Width, this->ClientHeight - 120);

    setPosition(Label2, 90, cbxDrive->Left + cbxDrive->Width + 10);

    setPosition(lbxFile, 110, cbxDrive->Left + cbxDrive->Width + 10);
    setSize(lbxFile, 400, this->ClientHeight - 400);

    setPosition(AddAllFiles_btn,
        lbxFile->Top + lbxFile->Height / 2 - AddAllFiles_btn->Height / 2,
        lbxFile->Left + lbxFile->Width + 10);

    setPosition(
        lbxResult, 110, AddAllFiles_btn->Left + AddAllFiles_btn->Width + 10);
    setSize(lbxResult, this->ClientWidth - lbxResult->Left - 10,
        this->ClientHeight - 400);

    setPosition(memInfo, lbxFile->Top + lbxFile->Height + 10,
        cbxDrive->Left + cbxDrive->Width + 10);
    setSize(memInfo, lbxFile->Width + lbxResult->Width - 100,
        this->ClientHeight - memInfo->Top - 10);

    setPosition(Label1, 90,
        lbxResult->Left + (lbxResult->Width / 2) - Label1->Width / 2);

    setPosition(memLog, 10, cbxDrive->Left + cbxDrive->Width + 10);
    setSize(memLog, lbxFile->Width + lbxResult->Width - 120, 80);

    setPosition(btnInfo, 10, memLog->Left + memLog->Width + 10);
    setSize(
        btnInfo, this->ClientWidth - btnInfo->Left - 10, memLog->Height / 2);

    setPosition(btnSystem, 50, memLog->Left + memLog->Width + 10);
    setSize(btnSystem, this->ClientWidth - btnSystem->Left - 10,
        memLog->Height / 2);

    setPosition(
        btnLoadFiles, memInfo->Top, memInfo->Left + memInfo->Width + 10);
    setSize(btnLoadFiles, this->ClientWidth - btnLoadFiles->Left - 10,
        memInfo->Height * 0.8);

    setPosition(ChangeNumbers_btn,
        btnLoadFiles->Top + btnLoadFiles->Height + 10, btnLoadFiles->Left);
    setSize(ChangeNumbers_btn, this->ClientWidth - btnLoadFiles->Left - 10,
        memInfo->Height - btnLoadFiles->Height - 10);
}
// ---------------------------------------------------------------------------

void __fastcall TFormAddDataFiles::FormCreate(TObject* Sender)
{
    FileList = new TFileList();
}
//---------------------------------------------------------------------------

void __fastcall TFormAddDataFiles::AddAllFiles_btnClick(TObject* Sender)
{
    if (lbxFile->Items->Count != 0) {
        while (lbxFile->Items->Count != 0) {
            FileList->vAppendFile(
                lbxDir->Directory + "\\" + lbxFile->Items->Strings[0]);
            lbxFile->Items->Delete(0);
        }
    } else if (FileList->iGetCountFiles() != 0) {
        while (FileList->iGetCountFiles() != 0) {
            UnicodeString _fileName = FileList->sGetFile(0).SubString(
                lbxDir->Directory.Length() + 2, FileList->sGetFile(0).Length());
            lbxFile->Items->Add(_fileName);
            FileList->vDeleteFile(0);
        }
    }
    FileList->vShowFilesListBox(lbxResult);
}
//---------------------------------------------------------------------------

void __fastcall TFormAddDataFiles::ChangeNumbers_btnClick(TObject* Sender)
{
    FormManualParameters->updateParamGrids();
    FormManualParameters->Show();
}
//---------------------------------------------------------------------------

