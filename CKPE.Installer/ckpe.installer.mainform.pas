unit CKPE.Installer.MainForm;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms, Controls, Graphics, Dialogs, ExtCtrls, StdCtrls,
  ComCtrls, Spin, ComObj, ActiveX, Windows, JwaWbemCli, TOML, TOML.Types,
  Registry, CKPE.Installer.ApplicationVersion, CKPE.Installer.FileVersion;

type

  { TRange }

  TRange = packed record
    _Beg, _End, _Line: Integer;
  end;

  { TFormMainInstaller }

  TFormMainInstaller = class(TForm)
    Button1: TButton;
    ImageCKPE: TImage;
    ImageAppIcon: TImage;
    ImageListThemesDemo: TImageList;
    LabelCKPEHi: TLabel;
    LabelHi1: TLabel;
    LabelHi2: TLabel;
    LabelRuntimeVersion: TLabel;
    LabelVersionOs: TLabel;
    LabelCaptionApp: TLabel;
    LabelVersionApp: TLabel;
    ListBoxSections: TListBox;
    PageControlSectionsConfig: TPageControl;
    PanelCKPETalk: TPanel;
    PanelCKPEWelcome: TPanel;
    PanelButton: TPanel;
    PanelApplyBtn: TPanel;
    PanelRestoreBtn: TPanel;
    PanelMain: TPanel;
    PanelButtonQuit: TPanel;
    PanelSystemMenu: TPanel;
    PanelSystem: TPanel;
    PanelHeader: TPanel;
    PanelBottomSpace: TPanel;
    PanelMainBody: TPanel;
    TimerScene: TTimer;
    TimerThemeScroll: TTimer;
    procedure Button1Click(Sender: TObject);
    procedure ColorButtonSettingColorChanged(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure ImageSettingClick(Sender: TObject);
    procedure ImageThemeBtnPaint(Sender: TObject);
    procedure ImageThemeBtnMouseEnter(Sender: TObject);
    procedure ImageThemeBtnMouseLeave(Sender: TObject);
    procedure LabelUrlMouseLeave(Sender: TObject);
    procedure LabelUrlMouseEnter(Sender: TObject);
    procedure PanelCustomThemeBtnClick(Sender: TObject);
    procedure CheckBoxSettingChange(Sender: TObject);
    procedure ComboBoxFontChange(Sender: TObject);
    procedure ComboBoxCharsetChange(Sender: TObject);
    procedure ComboBoxSettingChange(Sender: TObject);
    procedure EditSettingChange(Sender: TObject);
    procedure FloatSpinEditSettingChange(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure ListBoxSectionsClick(Sender: TObject);
    procedure PanelApplyBtnClick(Sender: TObject);
    procedure PanelButtonQuitClick(Sender: TObject);
    procedure PanelButtonQuitMouseEnter(Sender: TObject);
    procedure PanelButtonQuitMouseLeave(Sender: TObject);
    procedure PanelHeaderMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure PanelHeaderMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure PanelHeaderMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure PanelMainPaint(Sender: TObject);
    procedure PanelBtnMouseEnter(Sender: TObject);
    procedure PanelBtnMouseLeave(Sender: TObject);
    procedure PanelRestoreBtnClick(Sender: TObject);
    procedure SpinEditSettingChange(Sender: TObject);
    procedure TimerSceneTimer(Sender: TObject);
    procedure TimerThemeScrollTimer(Sender: TObject);
  private
    FPressed: Boolean;
    FPosX, FPosY: Integer;
    FTOMLStrData: TStringList;
    FTOMLStrDataCustomTheme: TStringList;
    FConfig: TTOMLTable;                 // Root TOML table from file
    FCustomTheme: TTOMLTable;            // Root TOML table from file
    FDirectScrollTheme: Integer;
    FPanelTheme: TPanel;
    FMoreThemes: Boolean;
    FHasDarkThemes: Boolean;
    FHasClassicThemes: Boolean;
    FStart, FNoHi: Boolean;
  private
    function GetVersionOsFromCom: String;
    procedure CreateControlLabel(AOwner: TWinControl; AKey: String;
      AX, AY: Integer; var ASize: Integer);
    function GetCommentByKey(AStrs: TStrings;
      const AKey, ADefault: String): String;
    function IsWhitespace(const AChar: Char): Boolean;
    procedure GetSectionAndNameOption(const AStr: String;
      var ASection, ANameOption: String);
    function GetRangeForEdition(AStrs: TStrings; const AStr: String): TRange;
    function MakeNameForControl(const ASection, ANameOption: String): String;
    procedure WriteNewValueOption(const ASection, ANameOption, AValue: String);
    procedure ReadingConfig;
    procedure ParseSection(AStrs: TStrings; ATable: TTOMLTable;
      AParent: TControl; const ANameOption: String;
      const AOffsetX, AOffsetY: Integer);
  public

  end;

const
  CKPE_C_FILENAME_CONFIG = 'CreationKitPlatformExtended.toml';
  CKPE_C_FILENAME_CUSTOMTHEME_CONFIG = 'CreationKitPlatformExtendedCustomTheme.toml';
  CKPE_C_TABSHEET_CUSTOMTHEME_ASSETS = 'Custom Themes Assets';
  CKPE_C_RUNTIME_CORE_DLL = 'CKPE.dll';
  CKPE_C_RUNTIME_COMM_DLL = 'CKPE.Common.dll';

var
  FormMainInstaller: TFormMainInstaller;

implementation

var
  CharsetMap: TStringList;

{$R *.lfm}

{ TFormMainInstaller }

procedure TFormMainInstaller.PanelButtonQuitMouseLeave(Sender: TObject);
begin
  // Return button
  TPanel(Sender).Color := TPanel(Sender).Parent.Color;
  TPanel(Sender).Font.Color := TPanel(Sender).Parent.Font.Color;
end;

procedure TFormMainInstaller.PanelHeaderMouseDown(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  // Move window without titlebar (part 1)
  FPressed := True;
  FPosX := X;
  FPosY := Y;
  (Sender as TControl).Cursor := crSizeAll;
end;

procedure TFormMainInstaller.PanelHeaderMouseMove(Sender: TObject;
  Shift: TShiftState; X, Y: Integer);
begin
  // Move window without titlebar (part 2)
  if FPressed then
  begin
    Left := Left - FPosX + X;
    Top := Top - FPosY + Y;
  end;
end;

procedure TFormMainInstaller.PanelHeaderMouseUp(Sender: TObject;
  Button: TMouseButton; Shift: TShiftState; X, Y: Integer);
begin
  // Move window without titlebar (part 3)
  FPressed := False;
  (Sender as TControl).Cursor := crDefault;
end;

procedure TFormMainInstaller.PanelMainPaint(Sender: TObject);
var
  oldColor: TColor;
  _canvas: TCanvas;
  _rgn: HRGN;
  _pnt: TPoint;
begin
  // Draw round border
  _pnt := ScreenToClient((Sender as TControl).ClientToScreen(Classes.Point(0, 0)));
  _rgn := Windows.CreateRoundRectRgn(-_pnt.x, -_pnt.y, Width - _pnt.x,
    Height - _pnt.y, 30, 30);
  try
    _canvas := TPanel(Sender).Canvas;
    oldColor := _canvas.Brush.Color;
    _canvas.Brush.Color := clGray;
    Windows.FrameRgn(_canvas.Handle, _rgn, _canvas.Brush.Reference.Handle, 1, 1);
  _canvas.Brush.Color := oldColor;
  finally
    Windows.DeleteObject(_rgn);
  end;
end;

procedure TFormMainInstaller.PanelBtnMouseEnter(Sender: TObject);
begin
  (Sender as TWinControl).Color := clLtGray;
end;

procedure TFormMainInstaller.PanelBtnMouseLeave(Sender: TObject);
begin
  (Sender as TWinControl).Color := (Sender as TWinControl).Parent.Color;
end;

procedure TFormMainInstaller.PanelRestoreBtnClick(Sender: TObject);
begin
  PageControlSectionsConfig.Visible := False; // Don't draw anything... speeds up the algorithm
  ReadingConfig;                              //
  PageControlSectionsConfig.Visible := True;  // Draw result
end;

procedure TFormMainInstaller.SpinEditSettingChange(Sender: TObject);
var
  _This: TSpinEdit;
  Range: TRange;
  STemp: String;
  Strs: TStringList;
begin
  _This := Sender as TSpinEdit;
  Strs := TStringList(_This.Tag);
  Range := GetRangeForEdition(Strs, _This.Name);
  if (Range._Beg = -1) or (Range._End = -1) or (Range._Line = -1) then
  begin
    MessageDlg('Internal error save int', mtError, [mbOk], 0);
    Exit;
  end;

  STemp := Strs[Range._Line];
  System.Delete(STemp, Range._Beg, Range._End - Range._Beg);
  System.Insert(IntToStr(_This.Value), STemp, Range._Beg);
  Strs[Range._Line] := STemp;
end;

procedure TFormMainInstaller.TimerSceneTimer(Sender: TObject);
begin
  if FStart then
  begin
    if ImageCKPE.Left < 0 then
      ImageCKPE.Left := ImageCKPE.Left + 15;
    if ImageCKPE.Left >= 0 then
    begin
      ImageCKPE.Left := 0;

      if LabelCKPEHi.Left > 424 then
        LabelCKPEHi.Left := LabelCKPEHi.Left - 25
      else
      begin
        LabelCKPEHi.Left := 424;

        if PanelCKPETalk.Left > 408 then
          PanelCKPETalk.Left := PanelCKPETalk.Left - 25
        else
        begin
          PanelCKPETalk.Left := 408;
          FStart := False;
          TimerScene.Enabled := False;
        end;
      end;
    end;
  end;
end;

procedure TFormMainInstaller.TimerThemeScrollTimer(Sender: TObject);
begin
  if not Assigned(FPanelTheme) then Exit;
  if FDirectScrollTheme = 1 then
  begin
    if FPanelTheme.Left <= -PageControlSectionsConfig.Width then
      FDirectScrollTheme := -PageControlSectionsConfig.Width
    else
    begin
      if (FPanelTheme.Left - 50) <= -PageControlSectionsConfig.Width then
        FPanelTheme.Left := -PageControlSectionsConfig.Width
      else
        FPanelTheme.Left := FPanelTheme.Left - 50;
    end;
  end
  else if FDirectScrollTheme = -1 then
  begin
    if FPanelTheme.Left >= 0 then
      FDirectScrollTheme := 0
    else
    begin
      if (FPanelTheme.Left + 50) >= 0 then
        FPanelTheme.Left := 0
      else
        FPanelTheme.Left := FPanelTheme.Left + 50;
    end;
  end;
end;

function TFormMainInstaller.GetVersionOsFromCom: String;
const
  RPC_C_AUTHN_LEVEL_DEFAULT = 0;
  RPC_C_IMP_LEVEL_IMPERSONATE = 3;
  RPC_C_AUTHN_WINNT = 10;
  RPC_C_AUTHZ_NONE = 0;
  RPC_C_AUTHN_LEVEL_CALL = 3;
  EOAC_NONE = 0;
label
  ErrorHas;
var
  FWbemLocator: IWbemLocator;
  FWbemServices: IWbemServices;
  ppEnum: IEnumWbemClassObject;
  apObjects: IWbemClassObject;
  puReturned: ULONG;
  pVal: OleVariant;
  pType: Integer;
  plFlavor: Integer;
  aname: WideString;
  iname: Integer;
begin
  if SUCCEEDED(CoInitialize(nil)) then
  begin
    if FAILED(CoInitializeSecurity(nil, -1, nil, nil, RPC_C_AUTHN_LEVEL_DEFAULT,
      RPC_C_IMP_LEVEL_IMPERSONATE, nil, EOAC_NONE, nil)) then
    begin
      CoUninitialize;
      goto ErrorHas;
    end;

    // Obtain the initial locator to CIMV2 -------------------------
    if SUCCEEDED(CoCreateInstance(CLSID_WbemLocator, nil, CLSCTX_INPROC_SERVER,
      IID_IWbemLocator, FWbemLocator)) then
    try
      // Connect to CIMV2 through the IWbemLocator::ConnectServer method
      if SUCCEEDED(FWbemLocator.ConnectServer('ROOT\CIMV2', '', '', '', 0,
        '', nil, FWbemServices)) then
      try
        // Set security levels on the proxy -------------------------
        if SUCCEEDED(CoSetProxyBlanket(FWbemServices, RPC_C_AUTHN_WINNT,
          RPC_C_AUTHZ_NONE, nil, RPC_C_AUTHN_LEVEL_CALL,
          RPC_C_IMP_LEVEL_IMPERSONATE, nil, EOAC_NONE)) then
        begin
          if SUCCEEDED(FWbemServices.ExecQuery('WQL', 'SELECT * FROM Win32_OperatingSystem',
            WBEM_FLAG_FORWARD_ONLY or WBEM_FLAG_RETURN_IMMEDIATELY, nil, ppEnum)) then
          begin
            // Get the data from the query
            while (ppEnum.Next(WBEM_INFINITE, 1, apObjects, puReturned) = 0) do
            begin
              if FAILED(apObjects.Get('Name', 0, pVal, pType, plFlavor)) then
              begin
                Result := 'Unknown os version';
                Break;
              end;
              iname := Pos('|', pVal);
              if iname <> 0 then
                Result := Copy(pVal, 1, iname - 1)
              else
                Result := String(aname);
            end;
          end;
        end;
      finally
        FWbemServices := nil;
      end;
    finally
      FWbemLocator := nil;
    end;

    CoUninitialize;
  end
  else
  begin
  ErrorHas:
    Result := 'Unknown os version';
  end;
end;

procedure TFormMainInstaller.CreateControlLabel(AOwner: TWinControl;
  AKey: String; AX, AY: Integer; var ASize: Integer);
var
  _Interval: Integer;
  _RectText: TRect;
begin
  with TLabel.Create(AOwner) do
  begin
    Parent := AOwner;
    Caption := AKey;

    ASize := Canvas.TextHeight(AKey);
    _Interval := (Canvas.TextWidth(AKey) div 300) + 1;
    if _Interval > 1 then
      ASize := ASize * _Interval;

    _RectText.Left := 0;
    _RectText.Top := 0;
    _RectText.Right := 300;
    _RectText.Bottom := ASize;
    DrawText(Canvas.Handle, PChar(AKey), Length(AKey), _RectText,
      DT_LEFT or DT_WORDBREAK or DT_CALCRECT);

    ASize := _RectText.Height;
    SetBounds(AX + 30, AY, 300, ASize);

    Layout := tlCenter;
    AutoSize := False;
    Wordwrap := True;
  end;
end;

function TFormMainInstaller.GetCommentByKey(AStrs: TStrings; const AKey,
  ADefault: String): String;
var
  I, C, Line, LineC, IPos: Integer;
  StrTemp: String;
  ValStr: Boolean;
begin
  Result := '';
  if not Assigned(AStrs) then
    Exit;

  C := AStrs.Count - 1;
  Line := -1;
  for I := 0 to C do
  begin
    if Pos(AKey, Trim(AStrs[I])) = 1 then
    begin
      Line := I;
      Break;
    end;
  end;

  LineC := -1;
  ValStr := False;
  for I := Line - 1 downto 0 do
  begin
    if Pos('#', Trim(AStrs[I])) = 1 then
    begin
      ValStr := True;
      Continue;
    end
    else
    begin
      if ValStr then
        LineC := I;
      Break;
    end;
  end;

  if LineC <> -1 then
  begin
    C := Line - 1;
    for I := LineC to C do
    begin
      StrTemp := StringReplace(AStrs[I], #9, '  ', [rfReplaceAll]);
      IPos := Pos('#', StrTemp);
      if IPos = 0 then
        Continue;
      Result := Result + Trim(Copy(StrTemp, IPos + 1, Length(StrTemp) - IPos)) +
        #13#10;
    end;
  end;

  if Line <> -1 then
  begin
    StrTemp := AStrs[Line];
    C := Length(StrTemp);
    ValStr := False;
    for I := 0 to C do
    begin
      if (StrTemp[I] = '''') or (StrTemp[I] = '"') then
        ValStr := not ValStr;

      if (not ValStr) and (StrTemp[I] = '#') then
        Result := Result + Trim(Copy(StrTemp, I + 1, C - I));
    end;
  end;

  if Result = '' then
    Result := ADefault;
end;

function TFormMainInstaller.IsWhitespace(const AChar: Char): Boolean;
begin
  Result := AChar in [' ', #9, #13, #10];
end;

procedure TFormMainInstaller.GetSectionAndNameOption(const AStr: String;
  var ASection, ANameOption: String);
var
  I: Integer;
begin
  I := Pos('___', AStr);
  if I <> 0 then
  begin
    ASection := Copy(AStr, I + 3, Length(AStr) - I);
    ANameOption := Copy(AStr, 1, I - 1);
  end;
end;

function TFormMainInstaller.GetRangeForEdition(AStrs: TStrings;
  const AStr: String): TRange;
var
  _Section, _NameOption, STemp: String;
  _LineSec, _LineSecEnd, I, C: Integer;
begin
  Result._Beg := -1;
  Result._End := -1;
  Result._Line := -1;

  if not Assigned(AStrs) then
    Exit;

  try
    GetSectionAndNameOption(AStr, _Section, _NameOption);

    C := AStrs.Count - 1;
    _LineSec := -1;
    _LineSecEnd := -1;
    for I := 0 to C do
    begin
      if _LineSec = -1 then
      begin
        if Pos(_Section, Trim(AStrs[I])) = 2 then
          _LineSec := I;
      end
      else
      begin
        // New section
        if Pos('[', Trim(AStrs[I])) = 1 then
          _LineSecEnd := I - 1
        else
        begin
          _LineSecEnd := C;
          Break;
        end;
      end;
    end;

    for I := _LineSec to _LineSecEnd do
    begin
      STemp := AStrs[I];
      if Pos(_NameOption, Trim(STemp)) = 1 then
      begin
        Result._Beg := Pos('=', STemp) + 1;
        while IsWhitespace(STemp[Result._Beg]) do
          Inc(Result._Beg);

        Result._End := Result._Beg + 1;
        while (not IsWhitespace(STemp[Result._End])) and
            (STemp[Result._End] <> '#') and
            (STemp[Result._End] <> #0) do
          Inc(Result._End);

        Result._Line := I;
        Break;
      end;
    end;
  except
    on E: Exception do
    begin
      Result._Beg := -1;
      Result._End := -1;
      Result._Line := -1;
    end;
  end;
end;

function TFormMainInstaller.MakeNameForControl(const ASection,
  ANameOption: String): String;
begin
  Result := ANameOption + '___' + ASection;
end;

procedure TFormMainInstaller.WriteNewValueOption(const ASection, ANameOption,
  AValue: String);
var
  _Img: TImage;
  Range: TRange;
  STemp: String;
begin
  Range := GetRangeForEdition(FTOMLStrData,
    MakeNameForControl(ASection, ANameOption));
  if (Range._Beg = -1) or (Range._End = -1) or (Range._Line = -1) then
  begin
    MessageDlg(Format('No found option %s:%s', [ASection, ANameOption]),
      mtError, [mbOk], 0);
    Exit;
  end;

  STemp := FTOMLStrData[Range._Line];
  System.Delete(STemp, Range._Beg, Range._End - Range._Beg);
  System.Insert(AValue, STemp, Range._Beg);
  FTOMLStrData[Range._Line] := STemp;
end;

procedure TFormMainInstaller.ReadingConfig;
var
  NameSection, NameOption, STemp: String;
  _Value: TTOMLValue;
  TableSection: TTOMLTable;
  _TabSheet: TTabSheet;
  _ScrollBox: TScrollBox;
  _Panel: TPanel;
begin
  // Read file
  if Assigned(FTOMLStrData) then
    FTOMLStrData.Clear
  else
    FTOMLStrData := TStringList.Create;
  if not Assigned(FTOMLStrData) then
    raise Exception.Create('Out of memory');
  try
    FTOMLStrData.LoadFromFile(CKPE_C_FILENAME_CONFIG);
  finally
  end;
  // Read file custom theme
  if Assigned(FTOMLStrDataCustomTheme) then
    FTOMLStrDataCustomTheme.Clear
  else
    FTOMLStrDataCustomTheme := TStringList.Create;
  if not Assigned(FTOMLStrDataCustomTheme) then
    raise Exception.Create('Out of memory');
  try
    FTOMLStrDataCustomTheme.LoadFromFile(CKPE_C_FILENAME_CUSTOMTHEME_CONFIG);
  finally
  end;
  // Clear TabSheets
  FPanelTheme := nil;
  FDirectScrollTheme := 0;
  ListBoxSections.ItemIndex := -1;
  ListBoxSections.Items.Clear;
  PageControlSectionsConfig.Clear;
  // Parse Toml
  FConfig := ParseTOML(FTOMLStrData.Text);
  FCustomTheme := ParseTOML(FTOMLStrDataCustomTheme.Text);
  // Init parms
  FMoreThemes := False;
  FHasDarkThemes := False;
  FHasClassicThemes := False;
  // Added TabSheets
  for NameSection in FConfig.Items.Keys do
  begin
    if not FConfig.TryGetValue(NameSection, _Value) then
      Continue;
    // No table type skips
    if not (_Value is TTOMLTable) then
      Continue;
    // Add to ListBox section
    TableSection := _Value as TTOMLTable;
    ListBoxSections.Items.Add(NameSection);
    // Add TabSheet to PageControl
    _TabSheet := PageControlSectionsConfig.AddTabSheet;
    // Set name TabSheet as section
    _TabSheet.Caption := NameSection;
    // Parse
    ParseSection(FTOMLStrData, TableSection, _TabSheet, NameSection, 0, 0);
  end;
  if FHasDarkThemes or FHasClassicThemes then
  begin
    _TabSheet := PageControlSectionsConfig.AddTabSheet;
    _TabSheet.Caption := 'Themes';
    ListBoxSections.Items.Add('Themes');
    FPanelTheme := TPanel.Create(_TabSheet);

    with FPanelTheme do
    begin
      Parent := _TabSheet;
      Caption := '';
      SetBounds(0, 0,
        PageControlSectionsConfig.Width shl 1,
        PageControlSectionsConfig.Height);
    end;

    with TLabel.Create(FPanelTheme) do
    begin
      Parent := FPanelTheme;
      Caption := 'Choose a theme';
      Font.Size := 20;
      SetBounds(40, 20, PageControlSectionsConfig.Width - 40, 40);
      Font.Color := clGray;
      AutoSize := True;
    end;

    // Scroll window
    _ScrollBox := TScrollBox.Create(FPanelTheme);
    _ScrollBox.Parent := FPanelTheme;
    _ScrollBox.SetBounds(0, 60, PageControlSectionsConfig.Width - 9,
       PageControlSectionsConfig.Height - 69);
    _ScrollBox.BorderStyle := bsNone;
    _ScrollBox.HorzScrollBar.Visible := False;

    if FHasDarkThemes then
    begin
      with TImage.Create(_ScrollBox) do
      begin
        Parent := _ScrollBox;
        SetBounds(30, 10, 192, 160);
        ImageListThemesDemo.GetBitmap(0, Picture.Bitmap);
        Tag := MAKELONG(0, 0);
        OnPaint := @ImageThemeBtnPaint;
        OnMouseEnter := @ImageThemeBtnMouseEnter;
        OnMouseLeave := @ImageThemeBtnMouseLeave;
        OnClick := @ImageSettingClick;
      end;

      with TLabel.Create(_ScrollBox) do
      begin
        Parent := _ScrollBox;
        Caption := 'Enable dark theme. Requires a Windows theme with styling ' +
          '(Aero) to be enabled and may cause graphical problems.';
        AutoSize := False;
        SetBounds(30, 180, PageControlSectionsConfig.Width - 40, 36);
        WordWrap := True;
      end;

      // Dark and Night blue
      if FMoreThemes then
      begin
        with TImage.Create(_ScrollBox) do
        begin
          Parent := _ScrollBox;
          SetBounds(30, 234, 192, 160);
          ImageListThemesDemo.GetBitmap(2, Picture.Bitmap);
          Tag := MAKELONG(2, 0);
          OnPaint := @ImageThemeBtnPaint;
          OnMouseEnter := @ImageThemeBtnMouseEnter;
          OnMouseLeave := @ImageThemeBtnMouseLeave;
          OnClick := @ImageSettingClick;
        end;

        with TImage.Create(_ScrollBox) do
        begin
          Parent := _ScrollBox;
          SetBounds(240, 234, 192, 160);
          ImageListThemesDemo.GetBitmap(3, Picture.Bitmap);
          Tag := MAKELONG(3, 0);
          OnPaint := @ImageThemeBtnPaint;
          OnMouseEnter := @ImageThemeBtnMouseEnter;
          OnMouseLeave := @ImageThemeBtnMouseLeave;
          OnClick := @ImageSettingClick;
        end;

        with TImage.Create(_ScrollBox) do
        begin
          Parent := _ScrollBox;
          SetBounds(30, 412, 192, 160);
          ImageListThemesDemo.GetBitmap(4, Picture.Bitmap);
          Tag := MAKELONG(4, 0);
          OnPaint := @ImageThemeBtnPaint;
          OnMouseEnter := @ImageThemeBtnMouseEnter;
          OnMouseLeave := @ImageThemeBtnMouseLeave;
          OnClick := @ImageSettingClick;
        end;

        with TPanel.Create(_ScrollBox) do
        begin
          Parent := _ScrollBox;
          BevelColor := clBlack;
          BevelOuter := bvNone;
          BevelInner := bvLowered;
          Caption := 'Custom...';
          OnClick := @PanelCustomThemeBtnClick;
          SetBounds(240, 412, 192, 160);
          OnMouseEnter := @PanelBtnMouseEnter;
          OnMouseLeave := @PanelBtnMouseLeave;
        end;

        with TLabel.Create(FPanelTheme) do
        begin
          Parent := FPanelTheme;
          Caption := 'Customizing your theme';
          Font.Size := 20;
          SetBounds(PageControlSectionsConfig.Width + 40, 20,
            PageControlSectionsConfig.Width - 40, 40);
          Font.Color := clGray;
          AutoSize := True;
        end;

        with TLabel.Create(FPanelTheme) do
        begin
          Parent := FPanelTheme;
          Caption := '<< Back';
          SetBounds(PageControlSectionsConfig.Width + 30, 70,
            70, 14);
          AutoSize := True;
          Tag := PtrInt(0);
          OnClick := @PanelCustomThemeBtnClick;
          OnMouseLeave := @LabelUrlMouseLeave;
          OnMouseEnter := @LabelUrlMouseEnter;
        end;

        with TLabel.Create(FPanelTheme) do
        begin
          Parent := FPanelTheme;
          Caption := 'Assets >>';
          Tag := PtrInt(2);
          SetBounds(PageControlSectionsConfig.Width + 100, 70,
            70, 14);
          AutoSize := True;
          OnClick := @PanelCustomThemeBtnClick;
          OnMouseLeave := @LabelUrlMouseLeave;
          OnMouseEnter := @LabelUrlMouseEnter;
        end;

        if FCustomTheme.TryGetValue('Colors', _Value) then
        begin
          // No table type skips
          if _Value is TTOMLTable then
          begin
            // Add to ListBox section
            TableSection := _Value as TTOMLTable;
            // Parse
            ParseSection(FTOMLStrDataCustomTheme, TableSection, FPanelTheme,
              NameSection, PageControlSectionsConfig.Width, 90);
          end;
        end;

        _TabSheet := PageControlSectionsConfig.AddTabSheet;
        _TabSheet.Caption := CKPE_C_TABSHEET_CUSTOMTHEME_ASSETS;

        _Panel := TPanel.Create(_TabSheet);
        _Panel.Parent := _TabSheet;
        _Panel.Align := alClient;
        _Panel.BevelOuter := bvNone;

        with TLabel.Create(_Panel) do
        begin
          Parent := _Panel;
          Caption := '<< Back';
          SetBounds(30, 70, 70, 14);
          AutoSize := True;
          Tag := PtrInt(1);
          OnClick := @PanelCustomThemeBtnClick;
          OnMouseLeave := @LabelUrlMouseLeave;
          OnMouseEnter := @LabelUrlMouseEnter;
        end;

        if FCustomTheme.TryGetValue('Assets', _Value) then
        begin
          // No table type skips
          if _Value is TTOMLTable then
          begin
            // Add to ListBox section
            TableSection := _Value as TTOMLTable;
            // Parse
            ParseSection(FTOMLStrDataCustomTheme, TableSection, _Panel,
              NameSection, 0, 90);
          end;
        end;
      end
      else
      begin
        with TImage.Create(_ScrollBox) do
        begin
          Parent := _ScrollBox;
          SetBounds(30, 234, 192, 160);
          ImageListThemesDemo.GetBitmap(2, Picture.Bitmap);
          Tag := MAKELONG(2, 0);
          OnPaint := @ImageThemeBtnPaint;
          OnMouseEnter := @ImageThemeBtnMouseEnter;
          OnMouseLeave := @ImageThemeBtnMouseLeave;
          OnClick := @ImageSettingClick;
        end;
      end;
    end;

    // Classic
    if FHasClassicThemes then
    begin
      with TImage.Create(_ScrollBox) do
      begin
        Parent := _ScrollBox;
        SetBounds(240, 10, 192, 160);
        ImageListThemesDemo.GetBitmap(1, Picture.Bitmap);
        Tag := MAKELONG(1, 0);
        OnPaint := @ImageThemeBtnPaint;
        OnMouseEnter := @ImageThemeBtnMouseEnter;
        OnMouseLeave := @ImageThemeBtnMouseLeave;
        OnClick := @ImageSettingClick;
      end;
    end;
  end;
  // Bimbo page for ListBoxSections.ItemIndex = -1
  PageControlSectionsConfig.AddTabSheet.Caption := '[Empty]';
  // Sets first page
  if ListBoxSections.Items.Count > 0 then
  begin
    ListBoxSections.ItemIndex := -1;
    ListBoxSections.OnClick(ListBoxSections); // Fake call handler
  end;
end;

procedure TFormMainInstaller.ParseSection(AStrs: TStrings; ATable: TTOMLTable;
  AParent: TControl; const ANameOption: String; const AOffsetX,
  AOffsetY: Integer);
var
  NameSection, NameOption, STemp: String;
  IVal, _HeightLast, I, C, RetSize: Integer;
  _Value, VMoreTheme: TTOMLValue;
  _TabSheet: TTabSheet;
  _ScrollBox: TScrollBox;
begin
  NameSection := ANameOption;
  // Scroll window
  _ScrollBox := TScrollBox.Create(AParent);
  _ScrollBox.Parent := AParent as TWinControl;
  _ScrollBox.Align := alCustom;
  _ScrollBox.SetBounds(AOffsetX, AOffsetY,
    PageControlSectionsConfig.Width - 10,
    PageControlSectionsConfig.Height - AOffsetY - 8);
  _ScrollBox.BorderStyle := bsNone;
  // Start top
  _HeightLast := 20;
  with _ScrollBox do
  begin
    // Add controls
    for NameOption in ATable.Items.Keys do
    begin
      if not ATable.TryGetValue(NameOption, _Value) then
        Continue;

      case _Value.ValueType of
        TTOMLValueType.tvtBoolean: begin  // Boolean value type (true/false)
          STemp := LowerCase(NameOption) + ':' + LowerCase(NameSection);
          if STemp = 'buidarktheme:creationkit' then
             FHasDarkThemes := True;
          if STemp = 'buiclassictheme:creationkit' then
             FHasClassicThemes := True;
          with TCheckBox.Create(_ScrollBox) do
          begin
            Parent := _ScrollBox;
            Caption := GetCommentByKey(AStrs, NameOption, NameOption);
            Checked := (_Value as TTOMLBoolean).Value;
            RetSize := 24;
            Canvas.Font := PageControlSectionsConfig.Font;
            I := Round((Canvas.TextWidth(Caption) / 320) + 0.5);
            if I > 1 then
              RetSize := RetSize * I;
            SetBounds(30, _HeightLast, 320, RetSize);
            AutoSize := False;
            Tag := PtrInt(AStrs);
            Name := MakeNameForControl(NameSection, NameOption);
            OnChange := @CheckBoxSettingChange;
            // Multiline
            Windows.SetWindowLong(Handle, GWL_STYLE,
              Windows.GetWindowLong(Handle, GWL_STYLE) or BS_MULTILINE);
          end;
          _HeightLast := _HeightLast + RetSize;
        end;
        TTOMLValueType.tvtFloat: begin    // Float/decimal value type (including exponential notation)
          CreateControlLabel(_ScrollBox,
            GetCommentByKey(AStrs, NameOption, NameOption), 0, _HeightLast,
              RetSize);
          with TFloatSpinEdit.Create(_ScrollBox) do
          begin
            Parent := _ScrollBox;
            DecimalPlaces := 3;
            Value := (_Value as TTOMLFloat).Value;
            Increment := 0.1;
            SetBounds(30, _HeightLast + RetSize + 8, 80, 24);
            Tag := PtrInt(AStrs);
            Name := MakeNameForControl(NameSection, NameOption);
            OnChange := @FloatSpinEditSettingChange;
          end;
          _HeightLast := _HeightLast + RetSize + 32;
        end;
        TTOMLValueType.tvtInteger: begin  // Integer value type (decimal, hex, octal, binary)
          STemp := LowerCase(NameOption) + ':' + LowerCase(NameSection);
          case STemp of
            'ncharset:creationkit': begin
              STemp := GetCommentByKey(AStrs, NameOption, NameOption);
              I := Pos(#10, STemp);
              if I <> 0 then
                STemp := Copy(STemp, 1, I - 2);
              CreateControlLabel(_ScrollBox, STemp, 0,
                _HeightLast, RetSize);
              with TComboBox.Create(_ScrollBox) do
              begin
                Parent := _ScrollBox;
                SetBounds(30, _HeightLast + RetSize + 8, 180, 24);
                C := CharsetMap.Count - 1;
                for I := 0 to C do
                  Items.Add(CharsetMap.Names[I]);
                ItemIndex := 1;
                STemp := IntToStr((_Value as TTOMLInteger).Value);
                for I := 0 to C do
                  if CharsetMap.ValueFromIndex[I] = STemp then
                  begin
                    ItemIndex := I;
                    Break;
                  end;
                Tag := PtrInt(AStrs);
                Name := MakeNameForControl(NameSection, NameOption);
                Style := csDropDownList;
                OnChange := @ComboBoxCharsetChange;
              end;
              _HeightLast := _HeightLast + RetSize + 36;
            end;
            'utintmaskresolution:facegen': begin
              CreateControlLabel(_ScrollBox,
                GetCommentByKey(AStrs, NameOption, NameOption), 0, _HeightLast,
                  RetSize);
              with TComboBox.Create(_ScrollBox) do
              begin
                Parent := _ScrollBox;
                SetBounds(30, _HeightLast + RetSize + 8, 180, 24);
                Items.Add('256');
                Items.Add('512');
                Items.Add('1024');
                Items.Add('2048');
                Items.Add('4096');
                case (_Value as TTOMLInteger).Value of
                  256: ItemIndex := 0;
                  512: ItemIndex := 1;
                  1024: ItemIndex := 2;
                  2048: ItemIndex := 3;
                  4096: ItemIndex := 4;
                  else ItemIndex := 2;
                end;
                Tag := PtrInt(AStrs);
                Name := MakeNameForControl(NameSection, NameOption);
                Style := csDropDownList;
                OnChange := @ComboBoxSettingChange;
              end;
              _HeightLast := _HeightLast + RetSize + 36;
            end;
            'uuidarkthemeid:creationkit': begin
              FMoreThemes := True;
              VMoreTheme := _Value;
            end;
            else begin
              CreateControlLabel(_ScrollBox,
                GetCommentByKey(AStrs, NameOption, NameOption), 0, _HeightLast,
                  RetSize);
              with TSpinEdit.Create(_ScrollBox) do
              begin
                Parent := _ScrollBox;
                Value := (_Value as TTOMLInteger).Value;
                SetBounds(30, _HeightLast + RetSize + 8, 80, 24);
                Tag := PtrInt(AStrs);
                Name := MakeNameForControl(NameSection, NameOption);
                OnChange := @SpinEditSettingChange;
              end;
              _HeightLast := _HeightLast + RetSize + 36;
            end;
          end;
        end;
        TTOMLValueType.tvtArray: begin    // Array value type (ordered list of values)
          // so this color
          CreateControlLabel(_ScrollBox,
            GetCommentByKey(AStrs, NameOption, NameOption), 0, _HeightLast,
              RetSize);
          with TColorButton.Create(_ScrollBox) do
          begin
            Parent := _ScrollBox;
            with _Value as TTOMLArray do
            begin
              if Count <> 3 then
                ButtonColor := clBlack
              else
                ButtonColor :=
                  RGB(Items[0].AsInteger,
                      Items[1].AsInteger,
                      Items[2].AsInteger);
            end;
            SetBounds(30, _HeightLast + RetSize + 8, 80, 24);
            Tag := PtrInt(AStrs);
            Name := MakeNameForControl(NameSection, NameOption);
            OnColorChanged := @ColorButtonSettingColorChanged;
          end;
            _HeightLast := _HeightLast + RetSize + 44;
        end;
        TTOMLValueType.tvtString: begin   // String value type (basic and literal strings)
          STemp := LowerCase(NameOption) + ':' + LowerCase(NameSection);
          case STemp of
            'sfont:log': begin
              CreateControlLabel(_ScrollBox,
                GetCommentByKey(AStrs, NameOption, NameOption), 0, _HeightLast,
                  RetSize);
              with TComboBox.Create(_ScrollBox) do
              begin
                Parent := _ScrollBox;
                Items.AddStrings(Screen.Fonts);
                ItemIndex := 0;
                C := Items.Count - 1;
                STemp := (_Value as TTOMLString).Value;
                for I := 0 to C do
                begin
                  if AnsiCompareText(Items[I], STemp) = 0 then
                  begin
                    ItemIndex := I;
                    Break;
                  end;
                end;
                SetBounds(30, _HeightLast + RetSize + 8, 180, 24);
                Tag := PtrInt(AStrs);
                Name := MakeNameForControl(NameSection, NameOption);
                Style := csDropDownList;
                OnChange := @ComboBoxFontChange;
              end;
              _HeightLast := _HeightLast + RetSize + 36;
            end;
            else begin
              CreateControlLabel(_ScrollBox,
                GetCommentByKey(AStrs, NameOption, NameOption), 0, _HeightLast,
                  RetSize);
              with TEdit.Create(_ScrollBox) do
              begin
                Parent := _ScrollBox;
                Text := (_Value as TTOMLString).Value;
                SetBounds(30, _HeightLast + RetSize + 8, 180, 24);
                Tag := PtrInt(AStrs);
                Name := MakeNameForControl(NameSection, NameOption);
                OnChange := @EditSettingChange;
              end;
              _HeightLast := _HeightLast + RetSize + 36;
            end;
          end;
        end;
        else
        begin
          CreateControlLabel(_ScrollBox,
            Format('Unsupported format key "%s"', [NameOption + NameSection]),
              AOffsetX, _HeightLast, RetSize);
          _HeightLast := _HeightLast + RetSize + 12;
        end;
      end;
    end;
  end;
end;

procedure TFormMainInstaller.PanelButtonQuitMouseEnter(Sender: TObject);
begin
  // Made red button
  TPanel(Sender).Color := TColor($003535eb);
  TPanel(Sender).Font.Color := clWhite;
end;

procedure TFormMainInstaller.PanelButtonQuitClick(Sender: TObject);
begin
  // Quit
  Close;
end;

procedure TFormMainInstaller.FormCreate(Sender: TObject);
var
  RuntimeCore, RuntimeComm: TFileVersionInfo;
  Reg: TRegistry;
begin
  try
    FStart := False;
    ImageCKPE.Left := -500;
    LabelCKPEHi.Left := 1000;
    PanelCKPETalk.Left := 2500;
    PanelCKPEWelcome.Left := 13;
    PanelCKPEWelcome.Top := 16;

    FTOMLStrData := nil;
    FTOMLStrDataCustomTheme := nil;
    FDirectScrollTheme := 0;
    FMoreThemes := False;
    FHasDarkThemes := False;
    FHasClassicThemes := False;
    FPanelTheme := nil;

    if not FileExists(CKPE_C_FILENAME_CONFIG) then
      raise Exception.CreateFmt('File "%s" no found', [CKPE_C_FILENAME_CONFIG]);
    if not FileExists(CKPE_C_FILENAME_CUSTOMTHEME_CONFIG) then
      raise Exception.CreateFmt('File "%s" no found', [CKPE_C_FILENAME_CUSTOMTHEME_CONFIG]);

    try
      FNoHi := False;
      Reg := TRegistry.Create;
      Reg.RootKey := HKEY_CURRENT_USER;
      if Reg.OpenKey('\Software\CKPE\Installer\', True) then
      begin
        if Reg.ValueExists('NoHi') then
          FNoHi := Reg.ReadBool('NoHi');
        PanelCKPEWelcome.Visible := not FNoHi;
        Reg.WriteBool('NoHi', True);
      end;
    finally
      Reg.CloseKey;
      Reg.Free;
    end;

    // Charset list create
    CharsetMap := TStringList.Create;
    CharsetMap.Add('ANSI_CHARSET=0');
    CharsetMap.Add('DEFAULT_CHARSET=1');
    CharsetMap.Add('SYMBOL_CHARSET=2');
    CharsetMap.Add('SHIFTJIS_CHARSET=128');
    CharsetMap.Add('HANGUL_CHARSET=129');
    CharsetMap.Add('GB2312_CHARSET=134');
    CharsetMap.Add('CHINESEBIG5_CHARSET=136');
    CharsetMap.Add('OEM_CHARSET=255');
    CharsetMap.Add('JOHAB_CHARSET=130');
    CharsetMap.Add('HEBREW_CHARSET=177');
    CharsetMap.Add('ARABIC_CHARSET=178');
    CharsetMap.Add('GREEK_CHARSET=161');
    CharsetMap.Add('TURKISH_CHARSET=162');
    CharsetMap.Add('VIETNAMESE_CHARSET=163');
    CharsetMap.Add('THAI_CHARSET=222');
    CharsetMap.Add('EASTEUROPE_CHARSET=238');
    CharsetMap.Add('RUSSIAN_CHARSET=204');
    CharsetMap.Add('MAC_CHARSET=77');
    CharsetMap.Add('BALTIC_CHARSET=186');
    // Taskbar caption
    Application.Title := Caption;
    // Show version application
    LabelVersionApp.Caption := Format('%s by %s v%s',
      [AppVersion.ProductName, AppVersion.CompanyName, AppVersion.FileVersion]);
    // Show version os
    LabelVersionOs.Caption := GetVersionOsFromCom;
    // Show runtime version ckpe module
    if (not FileExists(CKPE_C_RUNTIME_CORE_DLL)) or
       (not FileExists(CKPE_C_RUNTIME_COMM_DLL)) then
      LabelRuntimeVersion.Caption := 'Runtime no found'
    else
    begin
      RuntimeCore := LGetFileVersion(CKPE_C_RUNTIME_CORE_DLL);
      RuntimeComm := LGetFileVersion(CKPE_C_RUNTIME_COMM_DLL);
      LabelRuntimeVersion.Caption := Format('Runtime v%d.%d.%d.%d, Common library v%d.%d.%d.%d',
        [RuntimeCore.Major, RuntimeCore.Minor, RuntimeCore.Revision, RuntimeCore.Build,
         RuntimeComm.Major, RuntimeComm.Minor, RuntimeComm.Revision, RuntimeComm.Build]);
    end;
    // Read config
    PageControlSectionsConfig.Visible := False;
    ReadingConfig;
    PageControlSectionsConfig.Visible := True;
  except
    on E: Exception do
    begin
      MessageDlg(E.Message, mtError, [mbOk], 0);
      Halt;
    end;
  end;
end;

procedure TFormMainInstaller.CheckBoxSettingChange(Sender: TObject);
var
  _This: TCheckBox;
  Range: TRange;
  STemp: String;
  Strs: TStringList;
begin
  _This := Sender as TCheckBox;
  Strs := TStringList(_This.Tag);
  Range := GetRangeForEdition(Strs, _This.Name);
  if (Range._Beg = -1) or (Range._End = -1) or (Range._Line = -1) then
  begin
    MessageDlg('Internal error save boolean', mtError, [mbOk], 0);
    Exit;
  end;

  STemp := Strs[Range._Line];
  System.Delete(STemp, Range._Beg, Range._End - Range._Beg);

  if _This.Checked then
    System.Insert('true', STemp, Range._Beg)
  else
    System.Insert('false', STemp, Range._Beg);

  Strs[Range._Line] := STemp;
end;

procedure TFormMainInstaller.PanelCustomThemeBtnClick(Sender: TObject);
var
  _TabSheet: TTabSheet;
  _NameSearch: String;
  I, C: Integer;
begin
  case (Sender as TControl).Tag of
    0: begin
      if not Assigned(FPanelTheme) then Exit;
      if FPanelTheme.Left = 0 then
        FDirectScrollTheme := 1
      else if FPanelTheme.Left <> 0 then
        FDirectScrollTheme := -1;

      if FHasDarkThemes then
      begin
        if FHasClassicThemes then
          WriteNewValueOption('CreationKit', 'bUIClassicTheme', 'false');
        WriteNewValueOption('CreationKit', 'bUIDarkTheme', 'true');
        WriteNewValueOption('CreationKit', 'uUIDarkThemeId', '3');
      end;
    end;
    2: begin
      C := PageControlSectionsConfig.PageCount - 1;
      for I := 0 to C do
      begin
        _TabSheet := PageControlSectionsConfig.Pages[I];
        if AnsiCompareText(_TabSheet.Caption,
          CKPE_C_TABSHEET_CUSTOMTHEME_ASSETS) = 0 then
        begin
          PageControlSectionsConfig.ActivePageIndex := I;
          Break;
        end;
      end;
    end;
    1: begin
      C := PageControlSectionsConfig.PageCount - 1;
      for I := 0 to C do
      begin
        _TabSheet := PageControlSectionsConfig.Pages[I];
        _NameSearch := ListBoxSections.Items[ListBoxSections.ItemIndex];
        if AnsiCompareText(_TabSheet.Caption, _NameSearch) = 0 then
        begin
          PageControlSectionsConfig.ActivePageIndex := I;
          Break;
        end;
      end;
    end;
  end;
end;

procedure TFormMainInstaller.ImageThemeBtnMouseLeave(Sender: TObject);
var
  _Img: TImage;
begin
  _Img := Sender as TImage;
  _Img.Tag := PtrInt(MAKELONG(LOWORD(_Img.Tag), 0));
  _Img.Repaint;
end;

procedure TFormMainInstaller.LabelUrlMouseLeave(Sender: TObject);
var
  _Label: TLabel;
begin
  _Label := Sender as TLabel;
  _Label.Font.Color := Font.Color;
  _Label.Font.Style := [];
  _Label.Cursor := crDefault;
end;

procedure TFormMainInstaller.LabelUrlMouseEnter(Sender: TObject);
var
  _Label: TLabel;
begin
  _Label := Sender as TLabel;
  _Label.Font.Color := clHighlight;
  _Label.Font.Style := [ fsUnderline ];
  _Label.Cursor := crHandPoint;
end;

procedure TFormMainInstaller.ImageThemeBtnMouseEnter(Sender: TObject);
var
  _Img: TImage;
begin
  _Img := Sender as TImage;
  _Img.Tag := PtrInt(MAKELONG(LOWORD(_Img.Tag), 1));
  _Img.Repaint;
end;

procedure TFormMainInstaller.ImageThemeBtnPaint(Sender: TObject);
var
  _Img: TImage;
begin
  if Sender is TImage then
  begin
    _Img := Sender as TImage;
    if HIWORD(LongInt(_Img.Tag)) = 1 then
      _Img.Canvas.Brush.Color := clHighlight
    else
      _Img.Canvas.Brush.Color := clBlack;
    _Img.Canvas.Brush.Style := bsSolid;
    _Img.Canvas.FrameRect(0, 0, 192, 160);
  end;
end;

procedure TFormMainInstaller.ImageSettingClick(Sender: TObject);
var
  _Img: TImage;
  Range: TRange;
  STemp: String;
begin
  _Img := Sender as TImage;
  case LOWORD(LongInt(_Img.Tag)) of
    0: begin // Vanilla
      if FHasDarkThemes then
      begin
        if FHasClassicThemes then
          WriteNewValueOption('CreationKit', 'bUIClassicTheme', 'false');
        WriteNewValueOption('CreationKit', 'bUIDarkTheme', 'false');
      end;
    end;
    1: begin // Classic
      if FHasClassicThemes then
      begin
        if FHasDarkThemes then
          WriteNewValueOption('CreationKit', 'bUIDarkTheme', 'false');
        WriteNewValueOption('CreationKit', 'bUIClassicTheme', 'true');
      end;
    end;
    2: begin // Light dark
      if FHasDarkThemes and FMoreThemes then
      begin
        if FHasClassicThemes then
          WriteNewValueOption('CreationKit', 'bUIClassicTheme', 'false');
        WriteNewValueOption('CreationKit', 'bUIDarkTheme', 'true');
        WriteNewValueOption('CreationKit', 'uUIDarkThemeId', '0');
      end;
    end;
    3: begin // Dark
      if FHasDarkThemes and FMoreThemes then
      begin
        if FHasClassicThemes then
          WriteNewValueOption('CreationKit', 'bUIClassicTheme', 'false');
        WriteNewValueOption('CreationKit', 'bUIDarkTheme', 'true');
        WriteNewValueOption('CreationKit', 'uUIDarkThemeId', '1');
      end;
    end;
    4: begin // Night blue
      if FHasDarkThemes and FMoreThemes then
      begin
        if FHasClassicThemes then
          WriteNewValueOption('CreationKit', 'bUIClassicTheme', 'false');
        WriteNewValueOption('CreationKit', 'bUIDarkTheme', 'true');
        WriteNewValueOption('CreationKit', 'uUIDarkThemeId', '2');
      end;
    end;
  end;
end;

procedure TFormMainInstaller.ColorButtonSettingColorChanged(Sender: TObject);
var
  _This: TColorButton;
  Range: TRange;
  STemp: String;
  Strs: TStringList;
  C: TColor;
begin
  _This := Sender as TColorButton;
  Strs := TStringList(_This.Tag);
  Range := GetRangeForEdition(Strs, _This.Name);
  if (Range._Beg = -1) or (Range._End = -1) or (Range._Line = -1) then
  begin
    MessageDlg('Internal error save color', mtError, [mbOk], 0);
    Exit;
  end;

  C := _This.ButtonColor;
  STemp := Strs[Range._Line];
  System.Delete(STemp, Range._Beg, Range._End - Range._Beg);
  System.Insert(Format('[%d,%d,%d]',
    [GetRValue(C), GetGValue(C), GetBValue(C)]), STemp, Range._Beg);
  Strs[Range._Line] := STemp;
end;

procedure TFormMainInstaller.Button1Click(Sender: TObject);
begin
  PanelCKPEWelcome.Visible := False;
end;

procedure TFormMainInstaller.FormShow(Sender: TObject);
begin
  if (not FNoHi) and PanelCKPEWelcome.Visible then
    FStart := True;
end;

procedure TFormMainInstaller.ComboBoxFontChange(Sender: TObject);
var
  _This: TComboBox;
  Range: TRange;
  STemp: String;
  Strs: TStringList;
begin
  _This := Sender as TComboBox;
  Strs := TStringList(_This.Tag);
  Range := GetRangeForEdition(Strs, _This.Name);
  if (Range._Beg = -1) or (Range._End = -1) or (Range._Line = -1) then
  begin
    MessageDlg('Internal error save font', mtError, [mbOk], 0);
    Exit;
  end;

  STemp := Strs[Range._Line];
  System.Delete(STemp, Range._Beg, Range._End - Range._Beg);
  System.Insert('''' + _This.Items[_This.ItemIndex] + '''', STemp, Range._Beg);
  Strs[Range._Line] := STemp;
end;

procedure TFormMainInstaller.ComboBoxCharsetChange(Sender: TObject);
var
  _This: TComboBox;
  Range: TRange;
  STemp: String;
  Strs: TStringList;
begin
  _This := Sender as TComboBox;
  Strs := TStringList(_This.Tag);
  Range := GetRangeForEdition(Strs, _This.Name);
  if (Range._Beg = -1) or (Range._End = -1) or (Range._Line = -1) then
  begin
    MessageDlg('Internal error save charset', mtError, [mbOk], 0);
    Exit;
  end;

  STemp := Strs[Range._Line];
  System.Delete(STemp, Range._Beg, Range._End - Range._Beg);
  System.Insert(CharsetMap.ValueFromIndex[_This.ItemIndex], STemp, Range._Beg);
  Strs[Range._Line] := STemp;
end;

procedure TFormMainInstaller.ComboBoxSettingChange(Sender: TObject);
var
  _This: TComboBox;
  Range: TRange;
  STemp: String;
  Strs: TStringList;
begin
  _This := Sender as TComboBox;
  Strs := TStringList(_This.Tag);
  Range := GetRangeForEdition(Strs, _This.Name);
  if (Range._Beg = -1) or (Range._End = -1) or (Range._Line = -1) then
  begin
    MessageDlg('Internal error save int', mtError, [mbOk], 0);
    Exit;
  end;

  STemp := Strs[Range._Line];
  System.Delete(STemp, Range._Beg, Range._End - Range._Beg);
  System.Insert(_This.Items[_This.ItemIndex], STemp, Range._Beg);
  Strs[Range._Line] := STemp;
end;

procedure TFormMainInstaller.EditSettingChange(Sender: TObject);
var
  _This: TEdit;
  Range: TRange;
  STemp: String;
  Strs: TStringList;
begin
  _This := Sender as TEdit;
  Strs := TStringList(_This.Tag);
  Range := GetRangeForEdition(Strs, _This.Name);
  if (Range._Beg = -1) or (Range._End = -1) or (Range._Line = -1) then
  begin
    MessageDlg('Internal error save text', mtError, [mbOk], 0);
    Exit;
  end;

  STemp := Strs[Range._Line];
  System.Delete(STemp, Range._Beg, Range._End - Range._Beg);
  System.Insert('''' + _This.Text + '''', STemp, Range._Beg);
  Strs[Range._Line] := STemp;
end;

procedure TFormMainInstaller.FloatSpinEditSettingChange(Sender: TObject);
var
  _This: TFloatSpinEdit;
  Range: TRange;
  STemp: String;
  Strs: TStringList;
begin
  _This := Sender as TFloatSpinEdit;
  Strs := TStringList(_This.Tag);
  Range := GetRangeForEdition(Strs, _This.Name);
  if (Range._Beg = -1) or (Range._End = -1) or (Range._Line = -1) then
  begin
    MessageDlg('Internal error save float', mtError, [mbOk], 0);
    Exit;
  end;

  STemp := Strs[Range._Line];
  System.Delete(STemp, Range._Beg, Range._End - Range._Beg);
  System.Insert(StringReplace(FloatToStr(_This.Value), ',', '.', []),
    STemp, Range._Beg);
  Strs[Range._Line] := STemp;
end;

procedure TFormMainInstaller.FormDestroy(Sender: TObject);
begin
  if Assigned(FConfig) then
    FConfig.Free;
  if Assigned(FTOMLStrData) then
    FTOMLStrData.Free;
  if Assigned(FCustomTheme) then
    FConfig.Free;
  if Assigned(FTOMLStrDataCustomTheme) then
    FTOMLStrData.Free;
end;

procedure TFormMainInstaller.FormResize(Sender: TObject);
var
  _rgn: HRGN;
begin
  // Round window
  _rgn := Windows.CreateRoundRectRgn(0, 0, Width, Height, 30, 30);
  Windows.SetWindowRgn(Handle, _rgn, true);
  Windows.DeleteObject(_rgn);
end;

procedure TFormMainInstaller.ListBoxSectionsClick(Sender: TObject);
var
  _TabSheet: TTabSheet;
  _NameSearch: String;
  I, C: Integer;
begin
  // Change tab
  try
    PageControlSectionsConfig.Visible := False;
    if ListBoxSections.ItemIndex = -1 then
    begin
      PageControlSectionsConfig.ActivePageIndex :=
        PageControlSectionsConfig.PageCount - 1;
      Exit;
    end
    else
    begin
      C := PageControlSectionsConfig.PageCount - 1;
      for I := 0 to C do
      begin
        _TabSheet := PageControlSectionsConfig.Pages[I];
        _NameSearch := ListBoxSections.Items[ListBoxSections.ItemIndex];
        if AnsiCompareText(_TabSheet.Caption, _NameSearch) = 0 then
        begin
          PageControlSectionsConfig.ActivePageIndex := I;
          Break;
        end;
      end;
    end;
  finally
    PageControlSectionsConfig.Visible := True;
  end;
end;

procedure TFormMainInstaller.PanelApplyBtnClick(Sender: TObject);
begin
  try
    if Assigned(FTOMLStrData) then
      FTOMLStrData.SaveToFile(CKPE_C_FILENAME_CONFIG);
    if Assigned(FTOMLStrDataCustomTheme) then
      FTOMLStrDataCustomTheme.SaveToFile(CKPE_C_FILENAME_CUSTOMTHEME_CONFIG);
  finally
    Close;
  end;
end;

end.

