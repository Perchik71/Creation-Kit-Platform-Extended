/***************************************************************************************************

  FXAA Fast approXimate Anti-Aliasing

  Author : perchik71 (Alexey Tim)
  Original see : https://github.com/bartwronski/CSharpRenderer/blob/master/shaders/FXAA.hlsl

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

// FXAA_EDGE_THRESHOLD - The minimum amount of local contrast required 
//                       to apply algorithm.
//                       1.0/3.0  - too little
//                       1.0/4.0  - good start
//                       1.0/8.0  - applies to more edges
//                       1.0/16.0 - overkill
#define FXAA_EDGE_THRESHOLD (1.0/16.0)

// FXAA_EDGE_THRESHOLD_MIN - Trims the algorithm from processing darks.
//                           Perf optimization.
//                           1.0/32.0 - visible limit (smaller isn't visible)
//                           1.0/16.0 - good compromise
//                           1.0/12.0 - upper limit (seeing artifacts)
#define FXAA_EDGE_THRESHOLD_MIN (1.0/16.0)

// FXAA_SEARCH_STEPS - Maximum number of search steps for end of span.
#define FXAA_SEARCH_STEPS (16)

// FXAA_SEARCH_ACCELERATION - How much to accelerate search,
//                            1 - no acceleration
//                            2 - skip by 2 pixels
//                            3 - skip by 3 pixels
//                            4 - skip by 4 pixels
#define FXAA_SEARCH_ACCELERATION (1)
   
// FXAA_SEARCH_THRESHOLD - Controls when to stop searching.
//                         1.0/4.0 - seems to be the best quality wise
#define FXAA_SEARCH_THRESHOLD (1.0/4.0)

// FXAA_SUBPIX - Toggle subpix filtering.
//               0 - turn off
//               1 - turn on
//               2 - turn on full (ignores FXAA_SUBPIX_TRIM and CAP)
#define FXAA_SUBPIX 1

// FXAA_SUBPIX_TRIM - Controls sub-pixel aliasing removal.
//                    1.0/2.0 - low removal
//                    1.0/3.0 - medium removal
//                    1.0/4.0 - default removal
//                    1.0/8.0 - high removal
//                    0.0 - complete removal
#define FXAA_SUBPIX_TRIM (1.0/4.0)

// FXAA_SUBPIX_CAP - Insures fine detail is not completely removed.
//                   This is important for the transition of sub-pixel detail,
//                   like fences and wires.
//                   3.0/4.0 - default (medium amount of filtering)
//                   7.0/8.0 - high amount of filtering
//                   1.0 - no capping of sub-pixel aliasing removal
#define FXAA_SUBPIX_CAP (3.0/4.0) 

#define FXAA_SUBPIX_TRIM_SCALE (1.0/(1.0 - FXAA_SUBPIX_TRIM))
#define FXAA_DOT_LUMA 1

/////////////////////////////

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

float3 helper_TexOff(float2 uv, int2 off)
{
    return ColorBuffer.SampleLevel(linearSampler, uv, 0.0, off).rgb;
}

float3 helper_TexGrad(float2 uv, float2 grad)
{
    return ColorBuffer.SampleGrad(linearSampler, uv, grad, grad).rgb;
}

float helper_Luma(float3 rgb)
{
#if FXAA_DOT_LUMA
    float3 luma = float3(0.299, 0.587, 0.114);  
    return dot(rgb, luma);
#else
    // will range 0.0 to 2.963210702.
    return rgb.g * (0.587 / 0.299) + rgb.r;
#endif // FXAA_DOT_LUMA
}

/////////////////////////////

float3 fxaa(InputData Data)
{
    // Given the following neighborhood: 
    //   N
    // W M E
    //   S
   
    float3 rgbM = helper_TexOff(Data.UV, int2(0,  0));
    float3 rgbN = helper_TexOff(Data.UV, int2(0, -1));
    float3 rgbE = helper_TexOff(Data.UV, int2(1,  0));
    float3 rgbW = helper_TexOff(Data.UV, int2(-1, 0));
    float3 rgbS = helper_TexOff(Data.UV, int2(0,  1)); 
    
    // Given luminance
    
    float lumaM = helper_Luma(rgbM);
    float lumaN = helper_Luma(rgbN);
    float lumaE = helper_Luma(rgbE);
    float lumaW = helper_Luma(rgbW);
    float lumaS = helper_Luma(rgbS);
    
    float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
    float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));
    float range = rangeMax - rangeMin;
    
    if (range < max(FXAA_EDGE_THRESHOLD_MIN, rangeMax * FXAA_EDGE_THRESHOLD))
        return rgbM;
    
#if FXAA_SUBPIX > 0
    float3 rgbL = rgbN + rgbW + rgbM + rgbE + rgbS;
#endif // FXAA_SUBPIX > 0
   
    // FXAA computes a local neighborhood lowpass value as follows:
    // (N + W + E + S)/4
  
#if FXAA_SUBPIX != 0
    float lumaL = (lumaN + lumaW + lumaE + lumaS) * 0.25;
    float rangeL = abs(lumaL - lumaM);
#endif // FXAA_SUBPIX != 0
    
#if FXAA_SUBPIX == 1
    float blendL = max(0.0,
            (rangeL / range) - FXAA_SUBPIX_TRIM) * FXAA_SUBPIX_TRIM_SCALE;
    blendL = min(FXAA_SUBPIX_CAP, blendL);
#endif // FXAA_SUBPIX == 1
    
#if FXAA_SUBPIX == 2
    float blendL = rangeL / range; 
#endif // FXAA_SUBPIX == 2
    
    // FXAA uses the following local neighborhood:
    // NW N NE
    // W  M  E
    // SW S SE
    
    float3 rgbNW = helper_TexOff(Data.UV, int2(-1, -1));
    float3 rgbNE = helper_TexOff(Data.UV, int2(1, -1));
    float3 rgbSW = helper_TexOff(Data.UV, int2(-1, 1));
    float3 rgbSE = helper_TexOff(Data.UV, int2(1, 1));
    
#if FXAA_SUBPIX > 0
    rgbL += rgbNW + rgbNE + rgbSW + rgbSE;
    rgbL /= 9.0;
#endif
    
    float lumaNW = helper_Luma(rgbNW);
    float lumaNE = helper_Luma(rgbNE);
    float lumaSW = helper_Luma(rgbSW);
    float lumaSE = helper_Luma(rgbSE);
    
    float edgeVert =
        abs((0.25 * lumaNW) + (-0.5 * lumaN) + (0.25 * lumaNE)) +
        abs((0.50 *  lumaW) + (-1.0 * lumaM) + (0.50 *  lumaE)) +
        abs((0.25 * lumaSW) + (-0.5 * lumaS) + (0.25 * lumaSE));
    float edgeHorz =
        abs((0.25 * lumaNW) + (-0.5 * lumaW) + (0.25 * lumaSW)) +
        abs((0.50 *  lumaN) + (-1.0 * lumaM) + (0.50 *  lumaS)) +
        abs((0.25 * lumaNE) + (-0.5 * lumaE) + (0.25 * lumaSE));
    
    bool horzSpan = edgeHorz >= edgeVert;
    float lengthSign = horzSpan ? -tilesize.y : -tilesize.x;
    
    if (!horzSpan)
    {
        lumaN = lumaW;
        lumaS = lumaE;
    }
    
    float gradientN = abs(lumaN - lumaM);
    float gradientS = abs(lumaS - lumaM);
    lumaN = (lumaN + lumaM) * 0.5;
    lumaS = (lumaS + lumaM) * 0.5;

    // This chooses a pixel pair. 
    // For "horzSpan == true" this will be a vertical pair,
    // [N]     N
    // [M] or [M]
    //  S     [S]
    
    if (gradientN < gradientS)
    {
        lumaN = lumaS;
        gradientN = gradientS;
        lengthSign *= -1.0;
    }
        
    float2 posN;
    posN.x = Data.UV.x + (horzSpan ? 0.0 : lengthSign * 0.5);
    posN.y = Data.UV.y + (horzSpan ? lengthSign * 0.5 : 0.0);
    
    // SEARCH LIMITING VALUES
    
    gradientN *= FXAA_SEARCH_THRESHOLD;
    
    float2 posP = posN;
    float2 offNP = horzSpan ? float2(tilesize.x, 0.0) : float2(0.0f, tilesize.y);
    float lumaEndN = lumaN;
    float lumaEndP = lumaN;
    bool doneN = false;
    bool doneP = false;
    
#if FXAA_SEARCH_ACCELERATION == 1
    posN += offNP * float2(-1.0, -1.0);
    posP += offNP * float2(1.0, 1.0);
#endif // FXAA_SEARCH_ACCELERATION == 1
    
#if FXAA_SEARCH_ACCELERATION == 2
    posN += offNP * float2(-1.5, -1.5);
    posP += offNP * float2( 1.5,  1.5);
    offNP *= float2(2.0, 2.0);
#endif // FXAA_SEARCH_ACCELERATION == 2
    
#if FXAA_SEARCH_ACCELERATION == 3
    posN += offNP * float2(-2.0, -2.0);
    posP += offNP * float2( 2.0,  2.0);
    offNP *= float2(3.0, 3.0);
#endif // FXAA_SEARCH_ACCELERATION == 3
    
#if FXAA_SEARCH_ACCELERATION == 4
    posN += offNP * float2(-2.5, -2.5);
    posP += offNP * float2( 2.5,  2.5);
    offNP *= float2(4.0, 4.0);
#endif // FXAA_SEARCH_ACCELERATION == 4

    for (int i = 0; i < FXAA_SEARCH_STEPS; i++)
    {
#if FXAA_SEARCH_ACCELERATION == 1
        if (!doneN) lumaEndN = helper_Luma(helper_Tex(posN.xy));
        if (!doneP) lumaEndP = helper_Luma(helper_Tex(posP.xy));
#else
        if(!doneN) lumaEndN = helper_Luma(helper_TexGrad(posN.xy, offNP).xyz);
        if(!doneP) lumaEndP = helper_Luma(helper_TexGrad(posP.xy, offNP).xyz);
#endif // FXAA_SEARCH_ACCELERATION == 1
        doneN = doneN || (abs(lumaEndN - lumaN) >= gradientN);
        doneP = doneP || (abs(lumaEndP - lumaN) >= gradientN);
        if (doneN && doneP) break;
        if (!doneN) posN -= offNP;
        if (!doneP) posP += offNP;
    }
    
    // FXAA uses the pixel's position in the span 
    // in combination with the values (lumaEnd*) at the ends of the span,
    // to determine filtering.
    
    // This step computes which side of the span the pixel is on. 
    // On negative side if dstN < dstP,
    
    //      posN        pos                      posP
    //       |-----------|------|------------------|
    //       |           |      |                  | 
    //       |<--dstN--->|<---------dstP---------->|
    //                          |
    //                     span center
    
    float dstN = horzSpan ? Data.UV.x - posN.x : Data.UV.y - posN.y;
    float dstP = horzSpan ? posP.x - Data.UV.x : posP.y - Data.UV.y;
    bool directionN = dstN < dstP;
    lumaEndN = directionN ? lumaEndN : lumaEndP;
    
    // If both the pair luma at the end of the span (lumaEndN) 
    // and middle pixel luma (lumaM)
    // are on the same side of the middle pair average luma (lumaN),
    // then don't filter.
    
    if (((lumaM - lumaN) < 0.0) == ((lumaEndN - lumaN) < 0.0)) 
        lengthSign = 0.0;
    
    // FXAA filters using a bilinear texture fetch offset 
    // from the middle pixel M towards the center of the pair (NM below).
    // Maximum filtering will be half way between pair.
    
    float spanLength = (dstP + dstN);
    dstN = directionN ? dstN : dstP;
    float subPixelOffset = (0.5 + (dstN * (-1.0 / spanLength))) * lengthSign;

    float3 rgbF = helper_Tex(float2(
        Data.UV.x + (horzSpan ? 0.0 : subPixelOffset),
        Data.UV.y + (horzSpan ? subPixelOffset : 0.0)));
    
#if FXAA_SUBPIX == 0
    return rgbF; 
#else   
    return lerp(rgbF, rgbL, blendL);
#endif // FXAA_SUBPIX == 0
}

float4 main(InputData Data) : SV_Target
{
    return float4(fxaa(Data), 1.0);
}