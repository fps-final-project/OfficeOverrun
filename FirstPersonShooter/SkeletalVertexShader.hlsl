const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix inv_model;
	matrix view;
	matrix projection;
	matrix finalBonesMatricies[10];
};

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
	float4 total_position = float4(0.f, 0.f, 0.f, 0.f);
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
		if(input.boneIds[i] == -1)
            continue;
		if(input.boneIds[i] >= MAX_BONES)
        {
            total_position = float4(input.pos, 1.0f);
            break;
        }
		
        float4 localPosition = mul(float4(input.pos, 1.0f), finalBonesMatricies[input.boneIds[i]]);
        total_position += localPosition * input.weights[i];
		//float3 localNormal = finalBonesMatricies[input.boneIds[i]]
    }


    PixelShaderInput output;
	float4 pos = total_position;

	pos = mul(pos, model);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;
	output.model_pos = pos;

	output.texture_pos = input.texture_pos;

	output.normal = mul(input.normal, transpose(inv_model));

	return output;
}