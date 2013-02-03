object frmMain: TfrmMain
  Left = 760
  Top = 622
  Width = 334
  Height = 68
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'QstMedia'
  Color = clBtnFace
  Constraints.MaxHeight = 68
  Constraints.MaxWidth = 334
  Constraints.MinHeight = 68
  Constraints.MinWidth = 334
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object edtNow: TEdit
    Left = 138
    Top = 4
    Width = 70
    Height = 21
    ReadOnly = True
    TabOrder = 1
    Text = '00:00:00'
  end
  object btnStop: TButton
    Left = 262
    Top = 4
    Width = 46
    Height = 25
    Caption = 'Stop'
    TabOrder = 3
    OnClick = btnStopClick
  end
  object btnPlay: TButton
    Left = 212
    Top = 4
    Width = 46
    Height = 25
    Caption = 'Play'
    TabOrder = 2
    OnClick = btnPlayClick
  end
  object proPlay: TProgressBar
    Left = 4
    Top = 4
    Width = 130
    Height = 25
    Min = 0
    Max = 100
    Smooth = True
    TabOrder = 0
  end
  object timPlay: TTimer
    Enabled = False
    Interval = 100
    OnTimer = timPlayTimer
    Left = 56
  end
end
