object frmMain: TfrmMain
  Left = 717
  Top = 494
  Width = 334
  Height = 98
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'QstIndex'
  Color = clBtnFace
  Constraints.MaxHeight = 98
  Constraints.MaxWidth = 334
  Constraints.MinHeight = 98
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
  object tbrCount: TTrackBar
    Left = 0
    Top = 36
    Width = 326
    Height = 32
    Align = alBottom
    Max = 0
    Orientation = trHorizontal
    Frequency = 1
    Position = 0
    SelEnd = 0
    SelStart = 0
    TabOrder = 8
    TickMarks = tmBottomRight
    TickStyle = tsAuto
    OnChange = tbrCountChange
  end
  object edtCount: TEdit
    Left = 96
    Top = 4
    Width = 71
    Height = 21
    TabOrder = 4
    Text = '0/0'
  end
  object edtDelay: TEdit
    Left = 219
    Top = 4
    Width = 40
    Height = 21
    MaxLength = 3
    TabOrder = 6
    Text = '100'
  end
  object btnAdd: TButton
    Left = 50
    Top = 4
    Width = 20
    Height = 25
    Caption = '+'
    TabOrder = 2
    OnClick = btnAddClick
  end
  object btnSub: TButton
    Left = 27
    Top = 4
    Width = 20
    Height = 25
    Caption = '-'
    TabOrder = 1
    OnClick = btnSubClick
  end
  object btnPlay: TButton
    Left = 262
    Top = 4
    Width = 46
    Height = 25
    Caption = 'Play'
    TabOrder = 7
    OnClick = btnPlayClick
  end
  object btnGoto: TButton
    Left = 170
    Top = 4
    Width = 46
    Height = 25
    Caption = 'Goto'
    Default = True
    TabOrder = 5
    OnClick = btnGotoClick
  end
  object btnLast: TButton
    Left = 73
    Top = 4
    Width = 20
    Height = 25
    Caption = '>'
    TabOrder = 3
    OnClick = btnLastClick
  end
  object btnFirst: TButton
    Left = 4
    Top = 4
    Width = 20
    Height = 25
    Caption = '<'
    TabOrder = 0
    OnClick = btnFirstClick
  end
  object timPlay: TTimer
    Enabled = False
    Interval = 100
    OnTimer = timPlayTimer
    Left = 136
    Top = 24
  end
end
