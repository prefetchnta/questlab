object frmMain: TfrmMain
  Left = 808
  Top = 397
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'xSelectDir'
  ClientHeight = 378
  ClientWidth = 265
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
  object lstDir: TDirectoryListBox
    Left = 8
    Top = 8
    Width = 249
    Height = 297
    ItemHeight = 16
    TabOrder = 0
  end
  object lstDrv: TDriveComboBox
    Left = 8
    Top = 312
    Width = 249
    Height = 19
    DirList = lstDir
    TabOrder = 1
  end
  object btnOK: TButton
    Left = 8
    Top = 340
    Width = 121
    Height = 33
    Caption = 'OKAY'
    Default = True
    TabOrder = 2
    OnClick = btnOKClick
  end
  object btnCancel: TButton
    Left = 136
    Top = 340
    Width = 121
    Height = 33
    Cancel = True
    Caption = 'CANCEL'
    TabOrder = 3
    OnClick = btnCancelClick
  end
end
