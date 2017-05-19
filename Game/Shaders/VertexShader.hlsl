//***************************************************************************************
// File:           VertexShader.hlsl
// Author:         Geoffrey Douglas
// Date Created:   04/27/17
// Last Modified:  04/27/17
// Description:    Most basic VS. Transforms vertex position from object space (PosL) 
//                 to world space (PosW) and homogenous clip space (PosH)
//***************************************************************************************

#include "LightHelper.hlsl"

cbuffer cbPerObject : register(b1)
{
	float4x4 gWorld;
    float4x4 gWorldViewProj;
    float4x4 gWorldInvTranspose;
    float4x4 gTexTransform;
    Material gMaterial;
};

struct VertexIn
{
	float3 PosL    : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex     : TEXCOORD;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex     : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

    vout.NormalW = mul(float4(vin.NormalL, 1.0f), gWorldInvTranspose).xyz;

    // TODO Work out this math
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

    return vout;
}
