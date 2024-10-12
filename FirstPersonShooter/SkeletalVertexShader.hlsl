static const int MAX_BONE_INFLUENCE = 4;
static const int MAX_BONES = 50;

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix inv_model;
	matrix view;
	matrix projection;
};

cbuffer AnimationTransformBuffer1 : register(b2)
{
	matrix finalBonesMatricies1[MAX_BONES];
}

cbuffer AnimationTransformBuffer2 : register(b3)
{
	matrix finalBonesMatricies2[MAX_BONES];
}

cbuffer AnimationTransformBuffer3 : register(b4)
{
	matrix finalBonesMatricies3[MAX_BONES];
}

cbuffer AnimationInverseTransformBuffer1 : register(b5)
{
	matrix finalBonesMatriciesInverses1[MAX_BONES];
}

cbuffer AnimationInverseTransformBuffer2 : register(b6)
{
	matrix finalBonesMatriciesInverses2[MAX_BONES];
}

cbuffer AnimationInverseTransformBuffer3 : register(b7)
{
	matrix finalBonesMatriciesInverses3[MAX_BONES];
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
	matrix finalBoneTransform;
	matrix finalBoneTransformInverse;

	if (input.finalTransformId < MAX_BONES)
	{
		finalBoneTransform = finalBonesMatricies1[input.finalTransformId];
		finalBoneTransformInverse = finalBonesMatriciesInverses1[input.finalTransformId];
	}
	else if (input.finalTransformId < 2 * MAX_BONES)
	{
		finalBoneTransform = finalBonesMatricies2[input.finalTransformId - MAX_BONES];
		finalBoneTransformInverse = finalBonesMatriciesInverses2[input.finalTransformId - MAX_BONES];
	}
	else
	{
		finalBoneTransform = finalBonesMatricies3[input.finalTransformId - 2 * MAX_BONES];
		finalBoneTransformInverse = finalBonesMatriciesInverses3[input.finalTransformId - 2 * MAX_BONES];
	}
	
	// dobrze
	matrix model_final = mul(finalBoneTransform, model);

	PixelShaderInput output;
	float4 pos = mul(float4(input.pos, 1.0f), model_final);
	output.model_pos = pos;

	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;

	output.texture_pos = input.texture_pos;

	// (AB)^(-1) = B^(-1) * A^(-1)
	output.normal = mul(input.normal, transpose(mul(inv_model, finalBoneTransformInverse)));

	return output;
}