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

struct VertexShaderInput
{
	float3 pos : POSITION;
	float2 texture_pos : UV;
	float3 normal : NORMAL;
	int4 boneIds : BONE_IDS;
	float4 weights : WEIGHTS;
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
	matrix BoneTransform =
	{
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 }
	};

	for (int i = 0; i < 4; i++)
	{
		if (input.boneIds[i] == -1)
			continue;

		if (input.boneIds[i] >= 2 * MAX_BONES)
		{
			break;
		}

		if (input.boneIds[i] < 50)
		{
			BoneTransform += finalBonesMatricies1[input.boneIds[i]] * input.weights[i];
		}
		else
		{
			BoneTransform += finalBonesMatricies2[input.boneIds[i] - 50] * input.weights[i];
		}
	}

	// dobrze
	matrix model_final = mul(BoneTransform, model);
	
	PixelShaderInput output;
	float4 pos = mul(float4(input.pos, 1.0f), model_final);
	output.model_pos = pos;

	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;

	output.texture_pos = input.texture_pos;

	output.normal = normalize(mul(input.normal, model_final));

	return output;
}