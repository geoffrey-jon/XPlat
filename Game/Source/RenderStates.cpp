#include "RenderStates.h"

ID3D11RasterizerState* RenderStates::DefaultRS = 0;
ID3D11DepthStencilState* RenderStates::DefaultDSS = 0;
ID3D11BlendState* RenderStates::DefaultBS = 0;
ID3D11SamplerState* RenderStates::DefaultSS = 0;

void RenderStates::InitAll(ID3D11Device* device)
{
	// Default Rasterizer State
	{
		D3D11_RASTERIZER_DESC DefaultRSDesc;
		DefaultRSDesc.FillMode = D3D11_FILL_SOLID;
		DefaultRSDesc.CullMode = D3D11_CULL_BACK;
		DefaultRSDesc.FrontCounterClockwise = false;
		DefaultRSDesc.DepthBias = 0;
		DefaultRSDesc.DepthBiasClamp = 0.0f;
		DefaultRSDesc.SlopeScaledDepthBias = 0.0f;
		DefaultRSDesc.DepthClipEnable = true;
		DefaultRSDesc.ScissorEnable = false;
		DefaultRSDesc.MultisampleEnable = false;
		DefaultRSDesc.AntialiasedLineEnable = false;
		HR(device->CreateRasterizerState(&DefaultRSDesc, &DefaultRS));
	}

	// Default Depth/Stencil State
	{
		D3D11_DEPTH_STENCIL_DESC DefaultDSSDesc;
		DefaultDSSDesc.DepthEnable = true;
		DefaultDSSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		DefaultDSSDesc.DepthFunc = D3D11_COMPARISON_LESS;
		DefaultDSSDesc.StencilEnable = false;
		DefaultDSSDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		DefaultDSSDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		DefaultDSSDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		DefaultDSSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		DefaultDSSDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		DefaultDSSDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

		DefaultDSSDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		DefaultDSSDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		DefaultDSSDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		DefaultDSSDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

		HR(device->CreateDepthStencilState(&DefaultDSSDesc, &DefaultDSS));
	}

	// Default Blend State
	{
		D3D11_BLEND_DESC DefaultBSDesc;
		DefaultBSDesc.AlphaToCoverageEnable = false;
		DefaultBSDesc.IndependentBlendEnable = false;
		DefaultBSDesc.RenderTarget[0].BlendEnable = false;
		DefaultBSDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		DefaultBSDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		DefaultBSDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		DefaultBSDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		DefaultBSDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		DefaultBSDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		DefaultBSDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HR(device->CreateBlendState(&DefaultBSDesc, &DefaultBS));
	}

	// Default Sampler State
	{
		D3D11_SAMPLER_DESC DefaultSSDesc;
		DefaultSSDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		DefaultSSDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		DefaultSSDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		DefaultSSDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		DefaultSSDesc.MipLODBias = 0.0f;
		DefaultSSDesc.MaxAnisotropy = 1;
		DefaultSSDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		DefaultSSDesc.BorderColor[0] = 1.0f;
		DefaultSSDesc.BorderColor[1] = 1.0f;
		DefaultSSDesc.BorderColor[2] = 1.0f;
		DefaultSSDesc.BorderColor[3] = 1.0f;
		DefaultSSDesc.MinLOD = -FLT_MAX;
		DefaultSSDesc.MaxLOD = FLT_MAX;

		HR(device->CreateSamplerState(&DefaultSSDesc, &DefaultSS));
	}
}

void RenderStates::DestroyAll()
{
	ReleaseCOM(DefaultRS);
	ReleaseCOM(DefaultDSS);
	ReleaseCOM(DefaultBS);
	ReleaseCOM(DefaultSS);
}
