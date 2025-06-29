{**
** File: CKPE.Installer.ApplicationVersion.pas
**
** LICENSE:
**
** This Source Code Form is subject to the terms of the
** Mozilla Public License, v. 2.0. If a copy of the MPL
** was not distributed with this file,
** You can obtain one at https://mozilla.org/MPL/2.0/.
**
** Author: Shabalin Vlad, 2019
** Email: vlad.shabalin.1992@mail.ru
** Support: Windows, Linux.
**}

unit ApplicationVersion;

{$mode objfpc}{$H+}
{$WARN 5093 off : function result variable of a managed type does not seem to be initialized}
interface

{$IFDEF WINDOWS}

type
  TFileVersionInfo = record
    FileType,
    CompanyName,
    FileDescription,
    FileVersion,
    InternalName,
    LegalCopyRight,
    LegalTradeMarks,
    OriginalFileName,
    ProductName,
    ProductVersion,
    Comments,
    SpecialBuildStr,
    PrivateBuildStr,
    FileFunction,
    TargetCPU: String;
    DebugBuild,
    PreRelease,
    SpecialBuild,
    PrivateBuild,
    Patched,
    InfoInferred: Boolean;
  end;

function FileVersionInfo(const AAppFilName: String): TFileVersionInfo;

var
  AppVersion: TFileVersionInfo;

{$ELSE}

// adding from http://wiki.freepascal.org/Show_Application_Title,_Version,_and_Company

uses
  Classes,
  SysUtils
  // FPC 3.0 fileinfo reads exe resources as long as you register the appropriate units
  , fileinfo
  , winpeimagereader {need this for reading exe info}
  , elfreader {needed for reading ELF executables}
  , machoreader {needed for reading MACH-O executables}
  ;

type
  TAppFileVersionInfo = record
    FileType,
    CompanyName,
    FileDescription,
    FileVersion,
    InternalName,
    LegalCopyRight,
    LegalTradeMarks,
    OriginalFileName,
    ProductName,
    ProductVersion,
    Comments,
    SpecialBuildStr,
    PrivateBuildStr,
    TargetCPU,
    FileFunction: String;
    DebugBuild,
    PreRelease,
    SpecialBuild,
    PrivateBuild,
    Patched,
    InfoInferred: Boolean;
  end;

function FileVersionInfo(const AAppFilName: String): TAppFileVersionInfo;

var
  AppVersion: TAppFileVersionInfo;

{$ENDIF} // WINDOWS

implementation

{$IFDEF WINDOWS}

uses
  Windows,
  ShellApi,
  LazUTF8,
  SysUtils,
  Classes,
  Controls,
  Dialogs,
  Forms;

{$HINTS OFF}

function FileVersionInfo(const AAppFilName: String): TFileVersionInfo;
var FName: WideString;
    rSHFI: TSHFileInfoW;
    iRet: Integer;
    VerSize: Integer;
    VerBuf: PChar;
    VerBufValue: Pointer;
    VerHandle: Cardinal;
    VerBufLen: Cardinal;
    VerKey: String;
    FixedFileInfo: PVSFixedFileInfo;
// dwFileType, dwFileSubtype
function GetFileSubType(FixedFileInfo: PVSFixedFileInfo): String;
begin
  case FixedFileInfo^.dwFileType of
    VFT_UNKNOWN:    Result := 'Unknown';
    VFT_APP:        Result := 'Application';
    VFT_DLL:        Result := 'DLL';
    VFT_STATIC_LIB: Result := 'Static-link Library';
    // ---
    VFT_DRV:
      case FixedFileInfo^.dwFileSubtype of
        VFT2_UNKNOWN:         Result := 'Unknown Driver';
        VFT2_DRV_COMM:        Result := 'Communications Driver';
        VFT2_DRV_PRINTER:     Result := 'Printer Driver';
        VFT2_DRV_KEYBOARD:    Result := 'Keyboard Driver';
        VFT2_DRV_LANGUAGE:    Result := 'Language Driver';
        VFT2_DRV_DISPLAY:     Result := 'Display Driver';
        VFT2_DRV_MOUSE:       Result := 'Mouse Driver';
        VFT2_DRV_NETWORK:     Result := 'Network Driver';
        VFT2_DRV_SYSTEM:      Result := 'System Driver';
        VFT2_DRV_INSTALLABLE: Result := 'InstallableDriver';
        VFT2_DRV_SOUND:       Result := 'Sound Driver';
      end;
    // ---
    VFT_FONT:
      case FixedFileInfo^.dwFileSubtype of
        VFT2_UNKNOWN:       Result := 'Unknown Font';
        VFT2_FONT_RASTER:   Result := 'Raster Font';
        VFT2_FONT_VECTOR:   Result := 'Vector Font';
        VFT2_FONT_TRUETYPE: Result := 'Truetype Font';
      end;
    // ---
    VFT_VXD: Result := 'Virtual Defice Identifier = ' +
      IntToHex(FixedFileInfo^.dwFileSubtype, 8);
  end;
end;
// --- ---
function HasdwFileFlags(FixedFileInfo: PVSFixedFileInfo; bFlag: Word): Boolean;
begin
  Result := (FixedFileInfo^.dwFileFlagsMask and FixedFileInfo^.dwFileFlags and
             bFlag) = bFlag;
end;
// --- ---
function GetFixedFileInfo: PVSFixedFileInfo;
begin
  if not VerQueryValue(VerBuf, '', Pointer(Result), VerBufLen) then
    Result := nil;
end;
// --- ---
function GetInfo(const AKey: String): String;
var KeyString: WideString;
begin
  Result := '';
  VerKey := Format('\StringFileInfo\%.4x%.4x\%s',
                   [LoWord(Integer(VerBufValue^)),
                    HiWord(Integer(VerBufValue^)), AKey]);
  /// Перекодировка в понятный для Windows OS
  KeyString := UTF8Decode(VerKey);

  if VerQueryValueW(VerBuf, PWideChar(KeyString), VerBufValue, VerBufLen) then
    Result := UTF8Encode(StrPas(PWideChar(VerBufValue)));
end;
// --- ---
function QueryValue(const AValue: String): String;
var TrString: WideString;
begin
  Result := '';
  /// Перекодировка в понятный для Windows OS
  TrString := UTF8Decode('\VarFileInfo\Translation');
  /// получение специфичной информации об версии файла
  if GetFileVersionInfoW(PWideChar(FName), VerHandle, VerSize, VerBuf) and
      // return selected version information
      VerQueryValueW(VerBuf, PWideChar(TrString), VerBufValue, VerBufLen) then
    Result := GetInfo(AValue);
end;
begin
  // Иницмализация структуры результата
  with Result do
  begin
    FileType         := '';
    CompanyName      := '';
    FileDescription  := '';
    FileVersion      := '';
    InternalName     := '';
    LegalCopyRight   := '';
    LegalTradeMarks  := '';
    OriginalFileName := '';
    ProductName      := '';
    ProductVersion   := '';
    Comments         := '';
    SpecialBuildStr  := '';
    PrivateBuildStr  := '';
    {$IFDEF CPU64}
    TargetCPU        := 'x64';
    {$ENDIF}
    {$IFDEF CPU32}
    TargetCPU        := 'x86';
    {$ENDIF}
    FileFunction     := '';
    DebugBuild       := False;
    Patched          := False;
    PreRelease       := False;
    SpecialBuild     := False;
    PrivateBuild     := False;
    InfoInferred     := False;
  end;
  /// проверка существования файла
  if not FileExists(AAppFilName) then Exit;
  /// Перекодировка в понятный для Windows OS
  FName := UTF8Decode(AAppFilName);
  /// Получение типа файла
  if SHGetFileInfoW(PWideChar(FName), 0, rSHFI, SizeOf(rSHFI),
                    SHGFI_TYPENAME) <> 0 then
     Result.FileType := rSHFI.szTypeName;
  /// Получение информации об исполняемом файле
  iRet := SHGetFileInfoW(PWideChar(FName), 0, rSHFI, SizeOf(rSHFI),
                    SHGFI_EXETYPE);
  if iRet <> 0 then
  begin
    // Определяем по версии размер структуры данных
    VerSize := GetFileVersionInfoSizeW(PWideChar(FName), VerHandle);
    if VerSize > 0 then
    begin
      VerBuf := AllocMem(VerSize);
      try
        with Result do
        begin
          CompanyName      := QueryValue('CompanyName');
          FileDescription  := QueryValue('FileDescription');
          FileVersion      := QueryValue('FileVersion');
          InternalName     := QueryValue('InternalName');
          LegalCopyRight   := QueryValue('LegalCopyRight');
          LegalTradeMarks  := QueryValue('LegalTradeMarks');
          OriginalFileName := QueryValue('OriginalFileName');
          ProductName      := QueryValue('ProductName');
          ProductVersion   := QueryValue('ProductVersion');
          Comments         := QueryValue('Comments');
          SpecialBuildStr  := QueryValue('SpecialBuild');
          PrivateBuildStr  := QueryValue('PrivateBuild');
          FixedFileInfo    := GetFixedFileInfo;
          DebugBuild       := HasdwFileFlags(FixedFileInfo,VS_FF_DEBUG);
          PreRelease       := HasdwFileFlags(FixedFileInfo,VS_FF_PRERELEASE);
          PrivateBuild     := HasdwFileFlags(FixedFileInfo,VS_FF_PRIVATEBUILD);
          SpecialBuild     := HasdwFileFlags(FixedFileInfo,VS_FF_SPECIALBUILD);
          Patched          := HasdwFileFlags(FixedFileInfo,VS_FF_PATCHED);
          InfoInferred     := HasdwFileFlags(FixedFileInfo,VS_FF_INFOINFERRED);
          FileFunction     := GetFileSubType(FixedFileInfo);
        end;
      finally
        FreeMem(VerBuf, VerSize);
      end
    end;
  end
end;

{$HINTS ON}

{$ELSE}

uses
  Forms;

function FileVersionInfo(const AAppFilName: String): TAppFileVersionInfo;
var FileVerInfo: TFileVersionInfo;
begin
  FileVerInfo := TFileVersionInfo.Create(nil);
  try
    FileVerInfo.ReadFileInfo;
    with Result do
    begin
      CompanyName      := FileVerInfo.VersionStrings.Values['CompanyName'];
      FileDescription  := FileVerInfo.VersionStrings.Values['FileDescription'];
      FileVersion      := FileVerInfo.VersionStrings.Values['FileVersion'];
      InternalName     := FileVerInfo.VersionStrings.Values['InternalName'];
      LegalCopyRight   := FileVerInfo.VersionStrings.Values['LegalCopyright'];
      LegalTradeMarks  := FileVerInfo.VersionStrings.Values['LegalTrademarks'];
      OriginalFileName := FileVerInfo.VersionStrings.Values['OriginalFilename'];
      ProductName      := FileVerInfo.VersionStrings.Values['ProductName'];
      ProductVersion   := FileVerInfo.VersionStrings.Values['ProductVersion'];
      Comments         := FileVerInfo.VersionStrings.Values['Comments'];
      SpecialBuildStr  := FileVerInfo.VersionStrings.Values['SpecialBuild'];
      PrivateBuildStr  := FileVerInfo.VersionStrings.Values['PrivateBuild'];
      {$IFDEF CPU64}
      TargetCPU        := 'x64';
      {$ENDIF}
      {$IFDEF CPU32}
      TargetCPU        := 'x86';
      {$ENDIF}
      //FixedFileInfo    := GetFixedFileInfo;
      DebugBuild       := False; {no supported}
      PreRelease       := False; {no supported}
      PrivateBuild     := False; {no supported}
      SpecialBuild     := False; {no supported}
      Patched          := False; {no supported}
      InfoInferred     := False; {no supported}
      FileFunction     := '';    {no supported}
    end;
  finally
    if Assigned(FileVerInfo) then
      FreeAndNil(FileVerInfo);
  end;
end;

{$ENDIF} // WINDOWS

initialization
  AppVersion := FileVersionInfo(Application.ExeName);

end.
