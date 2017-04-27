//***************************************************************************************
// File:           PixelShader.hlsl
// Author:         Geoffrey Douglas
// Date Created:   04/27/17
// Last Modified:  04/27/17
// Description:    Most basic PS. Outputs the color white.
//***************************************************************************************

#include "LightHelper.hlsl"

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
};

float4 PS(VertexOut pin) : SV_Target
{
    float4 litColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return litColor; 
}