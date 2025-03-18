// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

struct InputData
{
    float2 Position : POS;
    float2 UV : TEX;
};

struct OutputData
{
    float4 ClipPosition : SV_POSITION;
    float2 UV : TEXCOORD0;
};

OutputData main(InputData Input)
{
    // zero the memory first
    OutputData Output = (OutputData)0;
    Output.ClipPosition = float4(Input.Position, 0.0, 1.0);
    Output.UV = Input.UV;
    return Output;
}