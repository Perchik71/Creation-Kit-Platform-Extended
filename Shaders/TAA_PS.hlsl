/***************************************************************************************************

 * TAA (Temporal Anti-Aliasing)
 * Original Author : perchik71 (Alexey Tim)

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.

***************************************************************************************************/

struct InputData
{
    float4 ClipPosition : SV_POSITION;
    float2 UV : TEXCOORD0;
};

Texture2D ColorBuffer : register(t0);
SamplerState linearSampler : register(s0);
cbuffer dataBuffer : register(b0)
{
    float2 tilesize;
    float2 jitter;
};

/////////////////////////////
// HELPER FUNCTION
/////////////////////////////

float3 helper_Tex(float2 uv)
{
    return ColorBuffer.SampleLevel(linearSampler, uv, 0.0).rgb;
}

/////////////////////////////

float3 taa(InputData Data)
{
    // implementation of the simplest method, 
    // shifting the current frame by half of the textile, 
    // creates a motion effect that is invisible to the human eye.
    
    return helper_Tex(Data.UV + (jitter * tilesize));
}

float4 main(InputData Data) : SV_Target
{
    return float4(taa(Data), 1.0);
}