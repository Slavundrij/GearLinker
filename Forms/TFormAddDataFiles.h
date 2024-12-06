//---------------------------------------------------------------------------

#ifndef TFormAddDataFilesH
#define TFormAddDataFilesH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.FileCtrl.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Outline.hpp>
#include <Vcl.Samples.DirOutln.hpp>
#include "cspin.h"

#include "TFileLoader.h"
#include "Types.h"
#include "TUserHintForm.h"
#include "TFormManualParameters.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>

/**/
#define FORM_MIN_WIDTH 1080
#define FORM_MIN_HEIGHT 620


/**/
//---------------------------------------------------------------------------
class TFormAddDataFiles : public TForm
{
__published:	// IDE-managed Components
    TFileListBox *lbxFile;
    TDriveComboBox *cbxDrive;
    TDirectoryListBox *lbxDir;
    TListBox *lbxResult;
    TButton *btnLoadFiles;
    TMemo *memLog;
    TButton *btnInfo;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TButton *btnSystem;
    TMemo *memInfo;
    TImage *imgProgress;
	TSaveDialog *SaveDialog1;
	TButton *AddAllFiles_btn;
	TButton *ChangeNumbers_btn;
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall lbxFileDblClick(TObject *Sender);
    void __fastcall lbxResultDblClick(TObject *Sender);
    void __fastcall btnLoadFilesClick(TObject *Sender);
    void __fastcall btnSystemClick(TObject *Sender);
    void __fastcall CreateParams(Controls::TCreateParams &Params);
    void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall AddAllFiles_btnClick(TObject *Sender);
	void __fastcall ChangeNumbers_btnClick(TObject *Sender);


private:	// User declarations
public:		// User declarations
    __fastcall TFormAddDataFiles(TComponent* Owner);
       void ProgressReset();
    void ProgressSet(int Cur, int Max);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAddDataFiles *FormAddDataFiles;
//---------------------------------------------------------------------------
#endif
