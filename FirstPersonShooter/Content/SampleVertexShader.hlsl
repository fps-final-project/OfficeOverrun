cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix inv_model;
	matrix view;
	matrix projection;
};

struct VertexShaderInput
{
	float3 pos : POSITION;
	float2 texture_pos : UV;
	float3 normal : NORMAL;
};

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 model_pos : POSITION;
	float2 texture_pos: UV;
	float3 normal : NORMAL;
};

PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.pos, 1.0f);

	pos = mul(pos, model);
	output.model_pos = pos;

	pos = mul(pos, view);
	pos = mul(pos, projection);

	output.pos = pos;

	output.texture_pos = input.texture_pos;

	output.normal = normalize(mul(input.normal, transpose(inv_model)));

	return output;
}
