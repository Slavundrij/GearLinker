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
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall lbxFileDblClick(TObject *Sender);
    void __fastcall lbxResultDblClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall btnLoadFilesClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TFormAddDataFiles(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAddDataFiles *FormAddDataFiles;
//---------------------------------------------------------------------------
#endif
