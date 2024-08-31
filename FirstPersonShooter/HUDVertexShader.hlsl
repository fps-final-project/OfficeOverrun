cbuffer VertexShaderData2D : register(b0)
{
	matrix model;
	matrix projection;
};

struct VertexShaderInput
{
	float2 pos : POSITION;
	float2 texture_pos : UV;
};

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 texture_pos : UV;
};

PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.pos, 1.0f, 1.0f);

	pos = mul(pos, model);
	pos = mul(pos, projection);

	pos.w = 1.f;

	output.pos = pos;
	output.texture_pos = input.texture_pos;

	return output;
}
