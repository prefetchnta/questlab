object frmMain: TfrmMain
  Left = 872
  Top = 545
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'frmMain'
  ClientHeight = 0
  ClientWidth = 111
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  PixelsPerInch = 96
  TextHeight = 13
  object dlgOpen: TOpenDialog
    Filter = '*.*|*.*'
    InitialDir = '.'
    Options = [ofNoChangeDir, ofPathMustExist, ofFileMustExist, ofEnableSizing]
    Title = 'Open File'
    Left = 8
  end
  object dlgSave: TSaveDialog
    Filter = '*.*|*.*'
    InitialDir = '.'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofNoChangeDir, ofPathMustExist, ofNoReadOnlyReturn, ofEnableSizing]
    Title = 'Save File'
    Left = 40
  end
end
