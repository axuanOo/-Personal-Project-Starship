#pragma once

// Raw string of HLSL code
const char* shaderSource = 
R"(
struct vs_input_t
{
float3 localPosition: POSITION;
float4 color: COLOR;
float2 uv : TEXCOORD;
};

Texture2D diffuseTexture : register(t0);
SamplerState diffuseSampler : register(s0);

cbuffer CameraConstants : register (b2)
{
float4x4 ProjectionMatrix;
float4x4 ViewMatrix;
};

cbuffer ModelConstants : register (b3)
{
float4x4 ModelMatrix;
float4 ModelColor;
};

float RangeMap(float value, float inMin, float inMax, float outMin, float outMax)
{
return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

struct v2p_t
{
float4 position: SV_Position;
float4 color : COLOR;
float2 uv: TEXCOORD;
};

v2p_t VertexMain(vs_input_t input)
{
float4 localPosition = float4(input.localPosition,1);

float4 clipPosition;

clipPosition = mul(ModelMatrix,localPosition);
clipPosition = mul(ViewMatrix,clipPosition);
clipPosition = mul(ProjectionMatrix,clipPosition);

v2p_t v2p;
v2p.position = clipPosition;
v2p.color = input.color*ModelColor;
v2p.uv = input.uv;
return v2p;
}

float4 PixelMain(v2p_t input) : SV_Target0
{
float4 textureColor = diffuseTexture.Sample(diffuseSampler,input.uv);
float4 color = textureColor * input.color;
clip(color.a - 0.01f);
return color;
}

)";


/*R"(
struct vs_input_t
{
	float3 localPosition: POSITION;
	float4 color: COLOR;
	float2 uv : TEXCOORD;
};

struct v2p_t
{
	float4 position: SV_Position;
	float4 color : COLOR;
	float2 uv: TEXCOORD;
};

v2p_t VertexMain(vs_input_t input)
{
	v2p_t v2p;
	v2p.position = float4(input.localPosition,1);
	v2p.color = input.color;
	v2p.uv = input.uv;
	return v2p;
}

float4 PixelMain(v2p_t input) : SV_Target0
{
	return float4(input.color);
}

)";
*/
