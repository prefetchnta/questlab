object frmMain: TfrmMain
  Left = 602
  Top = 508
  Width = 800
  Height = 64
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'QstMenu'
  Color = clMenuBar
  Constraints.MaxHeight = 64
  Constraints.MinHeight = 64
  Constraints.MinWidth = 800
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = mnuMain
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object barDummy: TToolBar
    Left = 0
    Top = 0
    Width = 792
    Height = 34
    Align = alClient
    TabOrder = 0
  end
  object mnuMain: TMainMenu
    Left = 752
    Top = 8
  end
  object dlgOpen: TOpenDialog
    Filter = '*.qst|*.qst'
    InitialDir = '.\script'
    Options = [ofNoChangeDir, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Open QST file'
    Left = 720
    Top = 8
  end
end
