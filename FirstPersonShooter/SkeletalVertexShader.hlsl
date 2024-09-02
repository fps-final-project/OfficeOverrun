static const int MAX_BONE_INFLUENCE = 4;

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix inv_model;
	matrix view;
	matrix projection;

	static const int MAX_BONES = 55;
	matrix finalBonesMatricies[MAX_BONES];
};

struct VertexShaderInput
{
	float3 pos : POSITION;
	float2 texture_pos : UV;
	float3 normal : NORMAL;
	int finalTransformId : FINAL_ID;
};

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 model_pos : POSITION;
	float2 texture_pos : UV;
	float3 normal : NORMAL;
};

PixelShaderInput main(VertexShaderInput input)
{
	// dobrze
	matrix model_final = mul(finalBonesMatricies[input.finalTransformId], model);


	// DUPA
	// kiedys cos sie pomysli
	//float3x3 normalMatrix = (float3x3)transpose(inv(model_final));


    PixelShaderInput output;
	float4 pos = mul(float4(input.pos, 1.0f), model_final);
	//float4 pos = total_position;

	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;
	output.model_pos = pos;

	output.texture_pos = input.texture_pos;

	//output.normal = mul(input.normal, input.inv_model);
	output.normal = input.normal;

	return output;
}