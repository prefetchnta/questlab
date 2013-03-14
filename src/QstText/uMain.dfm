object frmMain: TfrmMain
  Left = 663
  Top = 271
  Width = 800
  Height = 600
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'QstText'
  Color = clBtnFace
  Constraints.MinHeight = 600
  Constraints.MinWidth = 800
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object pnlHead: TPanel
    Left = 0
    Top = 0
    Width = 792
    Height = 50
    Align = alTop
    TabOrder = 0
    object edtCPage: TEdit
      Left = 8
      Top = 8
      Width = 73
      Height = 21
      MaxLength = 5
      TabOrder = 0
    end
    object btnCPage: TButton
      Left = 88
      Top = 8
      Width = 97
      Height = 25
      Caption = '&Code Page'
      TabOrder = 1
      OnClick = btnCPageClick
    end
    object edtGoto: TEdit
      Left = 192
      Top = 8
      Width = 73
      Height = 21
      MaxLength = 7
      TabOrder = 2
      Text = '1'
    end
    object btnGoto: TButton
      Left = 272
      Top = 8
      Width = 97
      Height = 25
      Caption = '&Goto Line'
      TabOrder = 3
      OnClick = btnGotoClick
    end
    object lstType: TComboBox
      Left = 376
      Top = 8
      Width = 113
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 4
      Text = 'QuestBAT'
      Items.Strings = (
        'QuestBAT')
    end
    object btnRefresh: TButton
      Left = 496
      Top = 8
      Width = 89
      Height = 25
      Caption = '&Refresh'
      TabOrder = 5
      OnClick = btnRefreshClick
    end
    object btnAction: TButton
      Left = 592
      Top = 8
      Width = 89
      Height = 25
      Caption = '&Action'
      TabOrder = 6
      OnClick = btnActionClick
    end
    object btnSave: TButton
      Left = 688
      Top = 8
      Width = 97
      Height = 25
      Caption = '&Save As'
      TabOrder = 7
      OnClick = btnSaveClick
    end
  end
  object pnlMain: TPanel
    Left = 0
    Top = 50
    Width = 792
    Height = 466
    Align = alClient
    TabOrder = 1
  end
  object pnlFoot: TPanel
    Left = 0
    Top = 516
    Width = 792
    Height = 50
    Align = alBottom
    TabOrder = 2
    object btnLoad: TButton
      Left = 8
      Top = 8
      Width = 97
      Height = 25
      Caption = '&Load File'
      TabOrder = 0
      OnClick = btnLoadClick
    end
  end
  object dlgOpen: TOpenDialog
    Filter = '*.*|*.*'
    InitialDir = '.\script'
    Options = [ofNoChangeDir, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Open TXT file'
    Left = 120
    Top = 104
  end
  object dlgSave: TSaveDialog
    Filter = '*.*|*.*'
    InitialDir = '.\script'
    Options = [ofHideReadOnly, ofNoChangeDir, ofPathMustExist, ofNoReadOnlyReturn, ofEnableSizing]
    Title = 'Save TXT file'
    Left = 152
    Top = 104
  end
end
