{ TOML.Types.pas
  This unit defines the core data types and classes used to represent TOML data structures.
  It provides a type-safe, object-oriented representation of TOML values including:
  - Basic types (string, integer, float, boolean, datetime)
  - Complex types (array, table)
  - Type conversion and validation
  - Memory management for TOML data structures
  
  The type system follows the TOML v1.0.0 specification and ensures type safety through
  runtime checks and explicit type conversions.
}
unit TOML.Types;

{$mode objfpc}{$H+}{$J-}
{$modeswitch typehelpers}
{$modeswitch advancedrecords}

interface

uses
  SysUtils, Generics.Collections;

type
  { TOML value types - represents all possible TOML data types
    These types correspond directly to the TOML specification types }
  TTOMLValueType = (
    tvtString,      // String value type (basic and literal strings)
    tvtInteger,     // Integer value type (decimal, hex, octal, binary)
    tvtFloat,       // Float/decimal value type (including exponential notation)
    tvtBoolean,     // Boolean value type (true/false)
    tvtDateTime,    // Date/time value type (RFC 3339)
    tvtArray,       // Array value type (ordered list of values)
    tvtTable,       // Table value type (collection of key/value pairs)
    tvtInlineTable  // Inline table value type (compact table representation)
  );

  { Forward declarations for interdependent types }
  TTOMLValue = class;
  TTOMLArray = class;
  TTOMLTable = class;

  { Exception types for TOML parsing and handling }
  
  { Base exception type for all TOML-related errors }
  ETOMLException = class(Exception);
  
  { Exception type for TOML parsing errors }
  ETOMLParserException = class(ETOMLException);
  
  { Exception type for TOML serialization errors }
  ETOMLSerializerException = class(ETOMLException);

  { Generic dictionary type for TOML tables
    Maps string keys to TOML values with case-sensitive comparison }
  TTOMLTableDict = specialize TDictionary<string, TTOMLValue>;
  
  { Generic list type for TOML arrays
    Stores ordered list of TOML values }
  TTOMLValueList = specialize TList<TTOMLValue>;

  { Base TOML value class - abstract base class for all TOML value types
    Provides common functionality and type conversion methods }
  TTOMLValue = class
  private
    FValueType: TTOMLValueType;  // Type of the TOML value
  protected
    { Protected type conversion methods - override in derived classes
      Each method raises ETOMLException if conversion is not supported }
    function GetAsString: string; virtual;
    function GetAsInteger: Int64; virtual;
    function GetAsFloat: Double; virtual;
    function GetAsBoolean: Boolean; virtual;
    function GetAsDateTime: TDateTime; virtual;
    function GetAsArray: TTOMLArray; virtual;
    function GetAsTable: TTOMLTable; virtual;
  public
    { Creates a new TOML value with the specified type
      @param AType The type of TOML value to create }
    constructor Create(AType: TTOMLValueType);
    destructor Destroy; override;
    
    { Properties for accessing the value in different formats
      Each property will raise ETOMLException if conversion is not supported }
    property ValueType: TTOMLValueType read FValueType;
    property AsString: string read GetAsString;
    property AsInteger: Int64 read GetAsInteger;
    property AsFloat: Double read GetAsFloat;
    property AsBoolean: Boolean read GetAsBoolean;
    property AsDateTime: TDateTime read GetAsDateTime;
    property AsArray: TTOMLArray read GetAsArray;
    property AsTable: TTOMLTable read GetAsTable;
  end;

  { String value - represents a TOML string (basic or literal) }
  TTOMLString = class(TTOMLValue)
  private
    FValue: string;  // The string value
  protected
    function GetAsString: string; override;
  public
    { Creates a new TOML string value
      @param AValue The string value to store }
    constructor Create(const AValue: string);
    property Value: string read FValue write FValue;
  end;

  { Integer value - represents a TOML integer (decimal, hex, octal, binary) }
  TTOMLInteger = class(TTOMLValue)
  private
    FValue: Int64;  // The 64-bit integer value
  protected
    function GetAsInteger: Int64; override;
    function GetAsFloat: Double; override;  // Allows conversion to float
  public
    { Creates a new TOML integer value
      @param AValue The 64-bit integer value to store }
    constructor Create(const AValue: Int64);
    property Value: Int64 read FValue write FValue;
  end;

  { Float value - represents a TOML float (including exponential notation) }
  TTOMLFloat = class(TTOMLValue)
  private
    FValue: Double;  // The double-precision floating point value
  protected
    function GetAsFloat: Double; override;
  public
    { Creates a new TOML float value
      @param AValue The double-precision float value to store }
    constructor Create(const AValue: Double);
    property Value: Double read FValue write FValue;
  end;

  { Boolean value - represents a TOML boolean (true/false) }
  TTOMLBoolean = class(TTOMLValue)
  private
    FValue: Boolean;  // The boolean value
  protected
    function GetAsBoolean: Boolean; override;
  public
    { Creates a new TOML boolean value
      @param AValue The boolean value to store }
    constructor Create(const AValue: Boolean);
    property Value: Boolean read FValue write FValue;
  end;

  { DateTime value - represents a TOML datetime (RFC 3339 format) }
  TTOMLDateTime = class(TTOMLValue)
  private
    FValue: TDateTime;  // The datetime value
  protected
    function GetAsDateTime: TDateTime; override;
  public
    { Creates a new TOML datetime value
      @param AValue The TDateTime value to store }
    constructor Create(const AValue: TDateTime);
    property Value: TDateTime read FValue write FValue;
  end;

  { Array value - represents a TOML array (ordered list of values) }
  TTOMLArray = class(TTOMLValue)
  private
    FItems: TTOMLValueList;  // List of TOML values
  protected
    function GetAsArray: TTOMLArray; override;
  public
    { Creates a new empty TOML array }
    constructor Create;
    destructor Destroy; override;
    
    { Adds a value to the array
      @param AValue The value to add
      @note Takes ownership of the value }
    procedure Add(AValue: TTOMLValue);
    
    { Gets an item at the specified index
      @param Index The zero-based index
      @returns The TOML value at the index
      @raises EListError if index is out of bounds }
    function GetItem(Index: Integer): TTOMLValue;
    
    { Gets the number of items in the array
      @returns The count of items }
    function GetCount: Integer;
    
    { Properties for accessing array data }
    property Items: TTOMLValueList read FItems;
    property Count: Integer read GetCount;
  end;

  { Table value - represents a TOML table (collection of key/value pairs) }
  TTOMLTable = class(TTOMLValue)
  private
    FItems: TTOMLTableDict;  // Dictionary of key/value pairs
  protected
    function GetAsTable: TTOMLTable; override;
  public
    { Creates a new empty TOML table }
    constructor Create;
    destructor Destroy; override;
    
    { Adds a key-value pair to the table
      @param AKey The key for the value
      @param AValue The value to add
      @raises ETOMLParserException if the key already exists
      @note Takes ownership of the value }
    procedure Add(const AKey: string; AValue: TTOMLValue);
    
    { Tries to get a value by key
      @param AKey The key to look up
      @param AValue The found value (if successful)
      @returns True if the key exists, False otherwise }
    function TryGetValue(const AKey: string; out AValue: TTOMLValue): Boolean;
    
    { Property for accessing the underlying dictionary }
    property Items: TTOMLTableDict read FItems;
  end;

implementation

{ TTOMLValue }

constructor TTOMLValue.Create(AType: TTOMLValueType);
begin
  inherited Create;
  FValueType := AType;
end;

destructor TTOMLValue.Destroy;
begin
  inherited Destroy;
end;

function TTOMLValue.GetAsString: string;
begin
  Result := '';
  raise ETOMLException.Create('Cannot convert this TOML value to string');
end;

function TTOMLValue.GetAsInteger: Int64;
begin
  Result := 0;
  raise ETOMLException.Create('Cannot convert this TOML value to integer');
end;

function TTOMLValue.GetAsFloat: Double;
begin
  Result := 0.0;
  raise ETOMLException.Create('Cannot convert this TOML value to float');
end;

function TTOMLValue.GetAsBoolean: Boolean;
begin
  Result := False;
  raise ETOMLException.Create('Cannot convert this TOML value to boolean');
end;

function TTOMLValue.GetAsDateTime: TDateTime;
begin
  Result := 0;
  raise ETOMLException.Create('Cannot convert this TOML value to datetime');
end;

function TTOMLValue.GetAsArray: TTOMLArray;
begin
  Result := nil;
  raise ETOMLException.Create('Cannot convert this TOML value to array');
end;

function TTOMLValue.GetAsTable: TTOMLTable;
begin
  Result := nil;
  raise ETOMLException.Create('Cannot convert this TOML value to table');
end;

{ TTOMLString }

constructor TTOMLString.Create(const AValue: string);
begin
  inherited Create(tvtString);
  FValue := AValue;
end;

function TTOMLString.GetAsString: string;
begin
  Result := FValue;
end;

{ TTOMLInteger }

constructor TTOMLInteger.Create(const AValue: Int64);
begin
  inherited Create(tvtInteger);
  FValue := AValue;
end;

function TTOMLInteger.GetAsInteger: Int64;
begin
  Result := FValue;
end;

function TTOMLInteger.GetAsFloat: Double;
begin
  Result := FValue;
end;

{ TTOMLFloat }

constructor TTOMLFloat.Create(const AValue: Double);
begin
  inherited Create(tvtFloat);
  FValue := AValue;
end;

function TTOMLFloat.GetAsFloat: Double;
begin
  Result := FValue;
end;

{ TTOMLBoolean }

constructor TTOMLBoolean.Create(const AValue: Boolean);
begin
  inherited Create(tvtBoolean);
  FValue := AValue;
end;

function TTOMLBoolean.GetAsBoolean: Boolean;
begin
  Result := FValue;
end;

{ TTOMLDateTime }

constructor TTOMLDateTime.Create(const AValue: TDateTime);
begin
  inherited Create(tvtDateTime);
  FValue := AValue;
end;

function TTOMLDateTime.GetAsDateTime: TDateTime;
begin
  Result := FValue;
end;

{ TTOMLArray }

constructor TTOMLArray.Create;
begin
  inherited Create(tvtArray);
  FItems := TTOMLValueList.Create;
end;

destructor TTOMLArray.Destroy;
var
  Item: TTOMLValue;
begin
  // Free all items in the array
  for Item in FItems do
    Item.Free;
  FItems.Free;
  inherited Destroy;
end;

procedure TTOMLArray.Add(AValue: TTOMLValue);
begin
  FItems.Add(AValue);
end;

function TTOMLArray.GetItem(Index: Integer): TTOMLValue;
begin
  Result := FItems[Index];
end;

function TTOMLArray.GetCount: Integer;
begin
  Result := FItems.Count;
end;

function TTOMLArray.GetAsArray: TTOMLArray;
begin
  Result := Self;
end;

{ TTOMLTable }

constructor TTOMLTable.Create;
begin
  inherited Create(tvtTable);
  FItems := TTOMLTableDict.Create;
end;

destructor TTOMLTable.Destroy;
var
  Item: TTOMLValue;
begin
  // Free all values in the table
  for Item in FItems.Values do  // Iterates through all values in the dictionary
    Item.Free;                  // Frees each value (including nested tables)
  FItems.Free;                  // Frees the dictionary itself
  inherited Destroy;            // Calls parent destructor
end;

procedure TTOMLTable.Add(const AKey: string; AValue: TTOMLValue);
var
  ExistingValue: TTOMLValue;
begin
  if FItems = nil then
    FItems := TTOMLTableDict.Create;
    
  // Check for duplicate keys
  if FItems.TryGetValue(AKey, ExistingValue) then
    raise ETOMLParserException.CreateFmt('Duplicate key "%s" found', [AKey]);
    
  FItems.AddOrSetValue(AKey, AValue);
end;

function TTOMLTable.TryGetValue(const AKey: string; out AValue: TTOMLValue): Boolean;
begin
  Result := FItems.TryGetValue(AKey, AValue);
end;

function TTOMLTable.GetAsTable: TTOMLTable;
begin
  Result := Self;
end;

end. 