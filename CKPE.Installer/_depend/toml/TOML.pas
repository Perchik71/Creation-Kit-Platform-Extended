{ TOML.pas
  This is the main unit of the TOML library that provides high-level functionality for working with TOML data.
  It re-exports the main types and provides helper functions for parsing and serializing TOML data.
  
  The unit follows the TOML v1.0.0 specification and provides a clean, type-safe interface for:
  - Parsing TOML from strings and files
  - Serializing TOML data structures to strings and files
  - Creating and manipulating TOML data structures
  - Type-safe access to TOML values
  
  Example usage:
    var
      Table: TTOMLTable;
    begin
      // Parse TOML from string
      Table := ParseTOML('key = "value"');
      try
        // Use the table...
      finally
        Table.Free;
      end;
    end;
}
unit TOML;

{$mode objfpc}{$H+}{$J-}

interface

uses
  SysUtils, Classes, TOML.Types, TOML.Parser, TOML.Serializer;

type
  { Re-export main types for easier access }
  TTOMLValue = TOML.Types.TTOMLValue;
  TTOMLString = TOML.Types.TTOMLString;
  TTOMLInteger = TOML.Types.TTOMLInteger;
  TTOMLFloat = TOML.Types.TTOMLFloat;
  TTOMLBoolean = TOML.Types.TTOMLBoolean;
  TTOMLDateTime = TOML.Types.TTOMLDateTime;
  TTOMLArray = TOML.Types.TTOMLArray;
  TTOMLTable = TOML.Types.TTOMLTable;
  
  { Re-export exception types }
  ETOMLException = TOML.Types.ETOMLException;
  ETOMLParserException = TOML.Types.ETOMLParserException;
  ETOMLSerializerException = TOML.Types.ETOMLSerializerException;

{ Helper functions for creating TOML values }

{ Creates a new TOML string value
  @param AValue The string value to store
  @returns A new TTOMLString instance that must be freed by the caller
  @note The caller is responsible for freeing the returned instance }
function TOMLString(const AValue: string): TTOMLString;

{ Creates a new TOML integer value
  @param AValue The 64-bit integer value to store
  @returns A new TTOMLInteger instance that must be freed by the caller
  @note The caller is responsible for freeing the returned instance }
function TOMLInteger(const AValue: Int64): TTOMLInteger;

{ Creates a new TOML float value
  @param AValue The double-precision floating point value to store
  @returns A new TTOMLFloat instance that must be freed by the caller
  @note The caller is responsible for freeing the returned instance }
function TOMLFloat(const AValue: Double): TTOMLFloat;

{ Creates a new TOML boolean value
  @param AValue The boolean value to store
  @returns A new TTOMLBoolean instance that must be freed by the caller
  @note The caller is responsible for freeing the returned instance }
function TOMLBoolean(const AValue: Boolean): TTOMLBoolean;

{ Creates a new TOML datetime value
  @param AValue The TDateTime value to store
  @returns A new TTOMLDateTime instance that must be freed by the caller
  @note The caller is responsible for freeing the returned instance }
function TOMLDateTime(const AValue: TDateTime): TTOMLDateTime;

{ Creates a new empty TOML array
  @returns A new empty TTOMLArray instance that must be freed by the caller
  @note The caller is responsible for freeing the returned instance and any values added to it }
function TOMLArray: TTOMLArray;

{ Creates a new empty TOML table
  @returns A new empty TTOMLTable instance that must be freed by the caller
  @note The caller is responsible for freeing the returned instance and any values added to it }
function TOMLTable: TTOMLTable;

{ Parsing functions }

{ Parses a TOML string into a table
  @param ATOML The TOML-formatted string to parse
  @returns A new TTOMLTable containing the parsed data
  @raises ETOMLParserException if the input is invalid TOML
  @note The caller is responsible for freeing the returned table and all its contents }
function ParseTOML(const ATOML: string): TTOMLTable;

{ Parses a TOML file into a table
  @param AFileName The path to the TOML file to parse
  @returns A new TTOMLTable containing the parsed data
  @raises ETOMLParserException if the input is invalid TOML
  @raises EFileStreamError if the file cannot be opened or read
  @note The caller is responsible for freeing the returned table and all its contents }
function ParseTOMLFromFile(const AFileName: string): TTOMLTable;

{ Serialization functions }

{ Serializes a TOML value to a string
  @param AValue The TOML value to serialize
  @returns A string containing the serialized TOML data
  @raises ETOMLSerializerException if the value cannot be serialized }
function SerializeTOML(const AValue: TTOMLValue): string;

{ Serializes a TOML value to a file
  @param AValue The TOML value to serialize
  @param AFileName The path to the output file
  @returns True if successful, False otherwise
  @raises ETOMLSerializerException if the value cannot be serialized
  @raises EFileStreamError if the file cannot be written }
function SerializeTOMLToFile(const AValue: TTOMLValue; const AFileName: string): Boolean;

implementation

{ Helper functions implementation }

function TOMLString(const AValue: string): TTOMLString;
begin
  Result := TTOMLString.Create(AValue);
end;

function TOMLInteger(const AValue: Int64): TTOMLInteger;
begin
  Result := TTOMLInteger.Create(AValue);
end;

function TOMLFloat(const AValue: Double): TTOMLFloat;
begin
  Result := TTOMLFloat.Create(AValue);
end;

function TOMLBoolean(const AValue: Boolean): TTOMLBoolean;
begin
  Result := TTOMLBoolean.Create(AValue);
end;

function TOMLDateTime(const AValue: TDateTime): TTOMLDateTime;
begin
  Result := TTOMLDateTime.Create(AValue);
end;

function TOMLArray: TTOMLArray;
begin
  Result := TTOMLArray.Create;
end;

function TOMLTable: TTOMLTable;
begin
  Result := TTOMLTable.Create;
end;

{ Parsing functions implementation }

function ParseTOML(const ATOML: string): TTOMLTable;
begin
  Result := TOML.Parser.ParseTOMLString(ATOML);
end;

function ParseTOMLFromFile(const AFileName: string): TTOMLTable;
begin
  Result := TOML.Parser.ParseTOMLFile(AFileName);
end;

{ Serialization functions implementation }

function SerializeTOML(const AValue: TTOMLValue): string;
begin
  Result := TOML.Serializer.SerializeTOML(AValue);
end;

function SerializeTOMLToFile(const AValue: TTOMLValue; const AFileName: string): Boolean;
begin
  Result := TOML.Serializer.SerializeTOMLToFile(AValue, AFileName);
end;

end. 