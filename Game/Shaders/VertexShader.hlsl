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
};

struct VertexIn
{
	float3 PosL    : POSITION;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);

    return vout;
}
