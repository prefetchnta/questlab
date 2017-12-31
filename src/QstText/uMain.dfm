object frmMain: TfrmMain
  Left = 663
  Top = 271
  Width = 800
  Height = 600
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'QstText'
  Color = clBtnFace
  Constraints.MinHeight = 300
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
    Height = 45
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
      Width = 89
      Height = 25
      Caption = 'Code Page'
      TabOrder = 1
      OnClick = btnCPageClick
    end
    object lstType: TComboBox
      Left = 184
      Top = 8
      Width = 107
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 2
      Text = ' QstBatch'
      Items.Strings = (
        ' QstBatch'
        ' Filter2D'
        ' Python CUI'
        ' Python GUI'
        ' Lua CUI'
        ' Lua GUI')
    end
    object btnRefresh: TButton
      Left = 298
      Top = 8
      Width = 89
      Height = 25
      Caption = 'Refresh'
      TabOrder = 3
      OnClick = btnRefreshClick
    end
    object btnAction: TButton
      Left = 394
      Top = 8
      Width = 89
      Height = 25
      Caption = 'Action'
      TabOrder = 4
      OnClick = btnActionClick
    end
    object btnSave: TButton
      Left = 666
      Top = 8
      Width = 89
      Height = 25
      Caption = 'Save As'
      TabOrder = 7
      OnClick = btnSaveClick
    end
    object btnLoad: TButton
      Left = 490
      Top = 8
      Width = 89
      Height = 25
      Caption = 'Load File'
      TabOrder = 5
      OnClick = btnLoadClick
    end
    object edtSPage: TEdit
      Left = 586
      Top = 8
      Width = 73
      Height = 21
      MaxLength = 5
      TabOrder = 6
    end
  end
  object pnlMain: TPanel
    Left = 0
    Top = 45
    Width = 792
    Height = 525
    Align = alClient
    TabOrder = 1
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
    Options = [ofOverwritePrompt, ofHideReadOnly, ofNoChangeDir, ofPathMustExist, ofNoReadOnlyReturn, ofEnableSizing]
    Title = 'Save TXT file'
    Left = 152
    Top = 104
  end
end
