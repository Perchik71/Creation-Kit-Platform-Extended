program offset_transfer;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}
  cthreads,
  {$ENDIF}
  Classes, SysUtils, CustApp, Contnrs, FileUtil, StrUtils
  { you can add units after this };

type
  { TOffset }
  TOffset = class
  private
    FRvaOld, FRvaNew: LongWord;
  public
    constructor Create; virtual; overload;
    constructor Create(RvaOld, RvaNew: LongWord); virtual; overload;
    destructor Destroy; override;

    property OldRVA: LongWord read FRvaOld write FRvaOld;
    property NewRVA: LongWord read FRvaNew write FRvaNew;
  end;
  { TOffsetMap }
  TOffsetMap = class
  private
    FItems: TObjectList;

    function Find(RVA: LongWord): TOffset;
    function GetItemAt(I: Integer): TOffset;
  public
    constructor Create; virtual;
    destructor Destroy; override;

    function Has(RVA: LongWord): Boolean;
    function GetNewRva(RVA: LongWord): LongWord;

    procedure Add(RvaOld, RvaNew: LongWord);
    procedure Remove(RVA: LongWord);
    procedure Clear;

    property Items[I: Integer]: TOffset read GetItemAt;
  end;
  { TOffsetTransfer }
  TOffsetTransfer = class(TCustomApplication)
  private
    FOffsetMap: TOffsetMap;
  protected
    procedure DoRun; override;
  public
    procedure Clear;
    function OpenBase(const AFileName: String): Boolean;
    procedure UpdateFilesInDir(const APath, ADestPath: String);

    constructor Create(TheOwner: TComponent); override;
    destructor Destroy; override;
    procedure WriteHelp; virtual;
  end;

function TOffsetMap.Find(RVA: LongWord): TOffset;
var
  I: Integer;
  T: TOffset;
begin
  Result := nil;
  for I := 0 to FItems.Count - 1 do
  begin
    T := TOffset(FItems[I]);
    if T.OldRVA = RVA then
    begin
      Result := T;
      Exit;
    end;
  end;
end;

function TOffsetMap.GetItemAt(I: Integer): TOffset;
begin
  Result := TOffset(FItems.Items[I]);
end;

constructor TOffsetMap.Create;
begin
  FItems := TObjectList.Create(True);
end;

destructor TOffsetMap.Destroy;
begin
  if Assigned(FItems) then
    FItems.Free;

  inherited Destroy;
end;

function TOffsetMap.Has(RVA: LongWord): Boolean;
begin
  Result := Assigned(Find(RVA));
end;

function TOffsetMap.GetNewRva(RVA: LongWord): LongWord;
var T: TOffset;
begin
  Result := 0;
  T := Find(RVA);
  if Assigned(T) then
    Result := T.NewRVA;
end;

procedure TOffsetMap.Add(RvaOld, RvaNew: LongWord);
begin
  if not Has(RvaOld) then
    FItems.Add(TOffset.Create(RvaOld, RvaNew));
end;

procedure TOffsetMap.Remove(RVA: LongWord);
var T: TOffset;
begin
  T := Find(RVA);
  if Assigned(T) then
    FItems.Remove(T);
end;

procedure TOffsetMap.Clear;
begin
  FItems.Clear;
end;

constructor TOffset.Create;
begin
  FRvaOld := 0;
  FRvaNew := 0;
end;

constructor TOffset.Create(RvaOld, RvaNew: LongWord);
begin
  FRvaOld := RvaOld;
  FRvaNew := RvaNew;
end;

destructor TOffset.Destroy;
begin
  inherited Destroy;
end;

{ TOffsetTransfer }

procedure TOffsetTransfer.DoRun;
begin
  if paramCount = 3 then
  begin
    if not OpenBase(paramStr(1)) then
    begin
      WriteLn('No open base file: ', paramStr(1));
      Exit;
    end;
    UpdateFilesInDir(paramStr(2), paramStr(3));
  end;
  // stop program loop
  Terminate;
end;

procedure TOffsetTransfer.Clear;
begin
  FOffsetMap.Clear;
end;

function TOffsetTransfer.OpenBase(const AFileName: String): Boolean;
var
  Strs: TStringList;
  Str: String;
  I, IPos, IPosEnd: Integer;
  RavOld, RavNew: LongWord;
begin
  Result := False;
  if not FileExists(AFileName) then
    Exit;

  try
    Strs := TStringList.Create;
    Strs.LoadFromFile(AFileName);
    Clear;
    for I := 0 to Strs.Count - 1 do
    begin
      Str := Strs[I];
      IPos := Pos('0x', Str);
      if IPos = 0 then
        Continue;
      IPosEnd := PosEx(',', Str, IPos);
      if IPosEnd = 0 then
        Continue;

      if not TryStrToUInt(Trim(Copy(Str, IPos, IPosEnd - IPos)), RavOld) then
      begin
        WriteLn('Error Old RVA; index: ', I);
        Continue;
      end;

      IPos := PosEx('0x', Str, IPosEnd);
      if IPos = 0 then
        Continue;
      IPosEnd := PosEx(')', Str, IPos);
      if IPosEnd = 0 then
        Continue;

      if not TryStrToUInt(Trim(Copy(Str, IPos, IPosEnd - IPos)), RavNew) then
      begin
        WriteLn('Error New RVA; index: ', I);
        Continue;
      end;

      if (RavOld = 0) or (RavNew = 0) then
      begin
        WriteLn('RavOld = 0 or RavNew = 0; index: ', I);
        Continue;
      end;

      FOffsetMap.Add(RavOld, RavNew);
      WriteLn('RavOld = 0x', IntToHex(RavOld),' or RavNew = 0x', IntToHex(RavNew),
        '; index: ', I);
    end;
    Result := True;
  finally
    if Assigned(Strs) then
      Strs.Free;
  end;
end;

procedure TOffsetTransfer.UpdateFilesInDir(const APath, ADestPath: String);
var
  Strs, StrsData: TStringList;
  FileName: String;
  RvaOld: LongWord;
  I, IR: Integer;
begin
  try
    Strs := TStringList.Create;
    StrsData := TStringList.Create;
    FileUtil.FindAllFiles(
      Strs,
      IncludeTrailingPathDelimiter(APath),
      '*.relb',
      True);

    ForceDirectories(ExcludeTrailingPathDelimiter(ADestPath));

    for I := 0 to Strs.Count - 1 do
    begin
      FileName := Strs[I];
      if not FileExists(FileName) then
      begin
        WriteLn('File ''', FileName, ''' no found');
        Continue;
      end;
      try
        StrsData.Clear;
        StrsData.LoadFromFile(FileName);

        if StrsData.Count < 3 then
        begin
          WriteLn('File ''', FileName, ''' empty');
          Continue;
        end;

        for IR := 2 to StrsData.Count - 1 do
        begin
          if not TryStrToUInt('0x' + StrsData[IR], RvaOld) then
          begin
            WriteLn('Failed to file ''', FileName, ''': Invalid number in line ',
              IR);
            Continue;
          end;

          if not FOffsetMap.Has(RvaOld) then
          begin
            WriteLn('Failed to file ''', FileName, ''': Rva no found 0x',
              IntToHex(RvaOld),
              ' in line ', IR);
            Continue;
          end;

          StrsData[IR] := IntToHex(FOffsetMap.GetNewRva(RvaOld));
        end;

        try
          StrsData.SaveToFile(
            IncludeTrailingPathDelimiter(ADestPath) + ExtractFileName(FileName));
        except
          WriteLn('File ''', IncludeTrailingPathDelimiter(ADestPath) +
            ExtractFileName(FileName), ''' can''t create');
        end;
      except
        on E: Exception do
          WriteLn('Exception ''', FileName, ''': ', E.Message);
      end;
    end;
  finally
    if Assigned(StrsData) then
      StrsData.Free;
    if Assigned(Strs) then
      Strs.Free;
  end;
end;

constructor TOffsetTransfer.Create(TheOwner: TComponent);
begin
  FOffsetMap := TOffsetMap.Create;
  inherited Create(TheOwner);
  StopOnException := True;
end;

destructor TOffsetTransfer.Destroy;
begin
  if Assigned(FOffsetMap) then
    FOffsetMap.Free;
  inherited Destroy;
end;

procedure TOffsetTransfer.WriteHelp;
begin
  { add your help code here }
  writeln('Usage: ', ExeName, ' -h');
end;

var
  Application: TOffsetTransfer;

{$R *.res}

begin
  Application := TOffsetTransfer.Create(nil);
  Application.StopOnException := True;
  Application.Run;
  Application.Free;
end.

