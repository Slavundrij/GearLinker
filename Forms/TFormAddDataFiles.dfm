object FormAddDataFiles: TFormAddDataFiles
  Left = 0
  Top = 0
  Caption = #1055#1086#1076#1073#1086#1088
  ClientHeight = 620
  ClientWidth = 1080
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 688
    Top = 90
    Width = 149
    Height = 13
    Caption = #1057#1087#1080#1089#1086#1082' '#1076#1086#1073#1072#1074#1083#1077#1085#1085#1099#1093' '#1092#1072#1081#1083#1086#1074
  end
  object Label2: TLabel
    Left = 280
    Top = 90
    Width = 111
    Height = 13
    Caption = #1057#1087#1080#1089#1086#1082' '#1092#1072#1081#1083#1086#1074' '#1055#1072#1087#1082#1080
  end
  object Label3: TLabel
    Left = 65
    Top = 90
    Width = 69
    Height = 13
    Caption = #1057#1087#1080#1089#1086#1082' '#1087#1072#1087#1086#1082
  end
  object Label4: TLabel
    Left = 65
    Top = 35
    Width = 65
    Height = 13
    Caption = #1042#1099#1073#1086#1088' '#1076#1080#1089#1082#1072
  end
  object imgProgress: TImage
    Left = 10
    Top = 10
    Width = 200
    Height = 22
  end
  object lbxFile: TFileListBox
    Left = 240
    Top = 110
    Width = 200
    Height = 400
    ItemHeight = 13
    TabOrder = 0
    OnDblClick = lbxFileDblClick
  end
  object cbxDrive: TDriveComboBox
    Left = 10
    Top = 55
    Width = 200
    Height = 19
    DirList = lbxDir
    TabOrder = 1
  end
  object lbxDir: TDirectoryListBox
    Left = 10
    Top = 110
    Width = 200
    Height = 500
    FileList = lbxFile
    TabOrder = 2
  end
  object lbxResult: TListBox
    Left = 460
    Top = 110
    Width = 610
    Height = 400
    ItemHeight = 13
    TabOrder = 3
    OnDblClick = lbxResultDblClick
  end
  object btnLoadFiles: TButton
    Left = 990
    Top = 530
    Width = 80
    Height = 80
    Caption = #1057#1086#1073#1088#1072#1090#1100
    TabOrder = 4
    OnClick = btnLoadFilesClick
  end
  object memLog: TMemo
    Left = 240
    Top = 10
    Width = 720
    Height = 75
    Lines.Strings = (
      'memLog')
    TabOrder = 5
    Visible = False
  end
  object btnInfo: TButton
    Left = 975
    Top = 10
    Width = 95
    Height = 25
    Caption = 'INFO'
    TabOrder = 6
  end
  object btnSystem: TButton
    Left = 975
    Top = 50
    Width = 95
    Height = 25
    Caption = #1074#1082#1083'. LOG'
    TabOrder = 7
    OnClick = btnSystemClick
  end
  object memInfo: TMemo
    Left = 240
    Top = 530
    Width = 720
    Height = 80
    Lines.Strings = (
      'memInfo')
    ScrollBars = ssVertical
    TabOrder = 8
  end
  object SaveDialog1: TSaveDialog
    Left = 216
    Top = 96
  end
end