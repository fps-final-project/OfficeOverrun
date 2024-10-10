static const int MAX_LIGHTS = 20;

cbuffer LightingConstantBuffer : register(b1)
{
    int nlights;
    float3 camera_pos;
    float3 light_pos[MAX_LIGHTS];
};

struct PixelShaderInput
{
    float4 pos : SV_POSITION;
    float4 model_pos : POSITION;
    float2 texture_pos : UV;
    float3 normal : NORMAL;
};

Texture2D my_texture;
SamplerState my_sampler;

float4 main(PixelShaderInput input) : SV_TARGET
{
    float3 final_light = float3(0.f, 0.f, 0.f);

    for (int i = 0; i < nlights; i++)
    {
        // ambient 
        float ambientStrength = 0.1;
        float3 lightColor = float3(1.f, 1.f, 1.f);
        float3 ambient = ambientStrength * lightColor;

        // diffuse
        float3 norm = normalize(input.normal);
        float3 lightDir = normalize(light_pos[i] - input.model_pos);
        float diff = max(dot(norm, lightDir), 0.0);
        float3 diffuse = diff * lightColor;

        // specular
        float specularStrength = 0.5;
        float3 viewDir = normalize(camera_pos - input.model_pos);
        float3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        float3 specular = (specularStrength * spec * lightColor);

        final_light += (ambient + diffuse + specular);
    }
    
    return float4(final_light, 0.f) * my_texture.Sample(my_sampler, input.texture_pos);
}