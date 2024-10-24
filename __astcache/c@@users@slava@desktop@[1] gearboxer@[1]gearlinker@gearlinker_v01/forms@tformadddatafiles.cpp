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

void __fastcall TFormAddDataFiles::FormCreate(TObject *Sender) {
	FileList = new TFileList();
	SuspGearList = new TList();
	StanGearList = new TList();
	GoodGearList = new TList();
}

// ---------------------------------------------------------------------------
void __fastcall TFormAddDataFiles::btnLoadFilesClick(TObject *Sender) {
    //
    memLog->Clear();

    if (FileList->iGetCountFiles() > 0) {
        for (int i = 0; i < FileList->iGetCountFiles(); i++) {
			vLoadGearsFromExcel(SuspGearList, StanGearList, GoodGearList, FileList->sGetFile(i), memLog);
        }
        // Идем по файлам

    }
    else {
        ShowMessage("Choose files");
    }
}
// ---------------------------------------------------------------------------
