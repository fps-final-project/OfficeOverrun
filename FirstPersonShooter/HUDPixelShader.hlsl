struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float2 texture_pos : UV;
};

Texture2D my_texture;
SamplerState my_sampler;

float4 main(PixelShaderInput input) : SV_TARGET
{
	return my_texture.Sample(my_sampler, input.texture_pos);
}
