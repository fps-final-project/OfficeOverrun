static const int MAX_BONE_INFLUENCE = 4;

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix inv_model;
	matrix view;
	matrix projection;
};

cbuffer AnimationTransformBuffer : register(b2)
{
	static const int MAX_BONES = 50;
	matrix finalBonesMatricies[MAX_BONES];
}

cbuffer AnimationInverseTransformBuffer : register(b3)
{
	matrix finalBonesMatriciesInverses[MAX_BONES];
}

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

	PixelShaderInput output;
	float4 pos = mul(float4(input.pos, 1.0f), model_final);

	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;
	output.model_pos = pos;

	output.texture_pos = input.texture_pos;

	// (AB)^(-1) = B^(-1) * A^(-1)
	output.normal = mul(input.normal, transpose(finalBonesMatriciesInverses[input.finalTransformId] * inv_model));
	//output.normal = mul(input.normal, transpose(inv_model * finalBonesMatriciesInverses[input.finalTransformId]));

	output.normal = input.normal;

	return output;
}