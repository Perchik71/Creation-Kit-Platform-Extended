library UnicodePlugin;

{$mode objfpc}{$H+}

uses
  SysUtils,
  Classes,
  lazUtf8;

function is_utf8(const astring: pansichar): boolean; stdcall; export;
begin
  Result := False;
  if Assigned(AString) then
     Result := FindInvalidUTF8Codepoint(AString, StrLen(AString)) = -1;
end;

function ansi_to_utf8(const asource: pansichar; adest: pansichar): integer;
  stdcall; export;
var Utf8Str: Utf8String;
begin
  Result := -1;
  Utf8Str := WinCPToUTF8(RawByteString(ASource));
  Result := Length(Utf8Str);
  if Result = 0 then Exit;
  if not Assigned(ADest) then
    Result := Length(Utf8Str) + 1
  else
    StrLCopy(ADest, PAnsiChar(Utf8Str), Result);
end;

function utf8_to_ansi(const asource: pansichar; adest: pansichar;
  const atestoninvalid: boolean): integer;
  stdcall; export;
var AnsiStr: RawByteString;
begin
  Result := -1;
  if ATestOnInvalid and
    (FindInvalidUTF8Codepoint(ASource, Length(ASource)) <> -1) then Exit;
  AnsiStr := UTF8ToWinCP(RawByteString(ASource));
  Result := Length(AnsiStr);
  if Result = 0 then Exit;
  if not Assigned(ADest) then
    Result := Length(AnsiStr) + 1
  else
    StrLCopy(ADest, PAnsiChar(AnsiStr), Result);
end;

function utf8_to_wide(const asource: pansiChar; adest: pwidechar;
  const atestoninvalid: boolean): integer;
  stdcall; export;
var WideStr: WideString;
begin
  Result := -1;
  if ATestOnInvalid and
    (FindInvalidUTF8Codepoint(ASource, Length(ASource)) <> -1) then Exit;
  WideStr := UTF8ToUTF16(RawByteString(ASource));
  Result := Length(WideStr);
  if Result = 0 then Exit;
  if not Assigned(ADest) then
    Result := Length(WideStr) + 1
  else
    StrLCopy(ADest, PWideChar(WideStr), Result);
end;

function wide_to_utf8(const asource: pwideChar; adest: pansichar): integer;
  stdcall; export;
var Utf8Str: Utf8String;
begin
  Result := -1;
  Utf8Str := UTF16ToUTF8(WideString(ASource));
  Result := Length(Utf8Str);
  if Result = 0 then Exit;
  if not Assigned(ADest) then
    Result := Length(Utf8Str) + 1
  else
    StrLCopy(ADest, PAnsiChar(Utf8Str), Result);
end;

{ exports }

exports
  utf8_to_ansi index 1,
  ansi_to_utf8 index 2,
  utf8_to_wide index 3,
  wide_to_utf8 index 4,
  is_utf8 index 5;

{$R *.res}

begin
end.

