//***************************************************************************************
// File:           PixelShader.hlsl
// Author:         Geoffrey Douglas
// Date Created:   04/27/17
// Last Modified:  04/27/17
// Description:    Most basic PS. Outputs the color white.
//***************************************************************************************

#include "LightHelper.hlsl"

cbuffer cbPerFrame : register(b0)
{
    DirectionalLight gLight;
    float3 gEyePosW;
    float pad;
};

cbuffer cbPerObject : register(b1)
{
    float4x4 gWorld;
    float4x4 gWorldViewProj;
    float4x4 gWorldInvTranspose;
    float4x4 gTexTransform;
    SurfaceProperties gSurface;
};

Texture2D gDiffuseMap;

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
    float2 TexC    : TEXCOORD;
};

SamplerState ssLinear : register(s0);

float4 PS(VertexOut pin) : SV_Target
{
    pin.NormalW = normalize(pin.NormalW);

    float3 toEye = normalize(gEyePosW - pin.PosW);

    float4 texColor = gDiffuseMap.Sample(ssLinear, pin.TexC);

    float4 ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    ComputeDirectionalLight(gSurface, gLight, pin.NormalW, toEye, ambient, diffuse, specular);
    
    float4 litColor = texColor*(ambient + diffuse) + specular;
    litColor.a = gSurface.Diffuse.a;
	
    return litColor; 
}