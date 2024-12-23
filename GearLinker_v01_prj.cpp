//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
USEFORM("Forms\TUserHintForm.cpp", UserHintForm);
USEFORM("Forms\TFormAddDataFiles.cpp", FormAddDataFiles);
USEFORM("Forms\FormStart.cpp", FormLaunch);
USEFORM("Forms\TFormManualParameters.cpp", FormManualParameters);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    try
	{
		CoInitialize(NULL);
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->Title = "E.S.V.A";
		Application->CreateForm(__classid(TFormLaunch), &FormLaunch);
		Application->CreateForm(__classid(TFormAddDataFiles), &FormAddDataFiles);
		Application->CreateForm(__classid(TUserHintForm), &UserHintForm);
		Application->CreateForm(__classid(TFormManualParameters), &FormManualParameters);
		Application->Run();
    }
    catch (Exception &exception)
    {
         Application->ShowException(&exception);
    }
    catch (...)
    {
         try
         {
             throw Exception("");
         }
         catch (Exception &exception)
         {
             Application->ShowException(&exception);
         }
    }
    return 0;
}
//---------------------------------------------------------------------------
