cbuffer LightingConstantBuffer : register(b1)
{
	int fully_visible;               // 4 bytes
	float flashlight_cutoffAngle;    // 4 bytes
	int pad1;
	int pad11;
	float3 flashlight_dir;           // 12 bytes
	int pad2;
	float3 camera_pos;               // 12 bytes
	int pad3;
	float4 light_pos;
}

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 model_pos : POSITION;
	float2 texture_pos : UV;
	float3 normal : NORMAL;
};

Texture2D my_texture;
SamplerState my_sampler;

float phong_lighting(float3 light_position, float3 model_pos, float3 normal, float k1, float k2)
{
	// diffuse
	float3 norm = normalize(normal);
	float3 lightDir = normalize(light_position - model_pos);
	float diffuse = max(dot(norm, lightDir), 0.0);

	// specular - phong-blinn model
	float specularStrength = 0.5;
	float specular = 0.f;
	if (diffuse > 0)
	{
		float3 viewDir = normalize(camera_pos - model_pos);
		float3 reflectDir = reflect(-lightDir, norm);
		float3 halfwayVec = normalize(viewDir + lightDir);
		specular = specularStrength * pow(max(dot(normal, halfwayVec), 0.0), 32);
	}

	// attenuation (light is dimmer the farther the object is from the light)
	float distance = length(light_position - model_pos);
	float attenuation = 1.0 / (1.0 + k1 * distance + k2 * (distance * distance));

	return (diffuse + specular) * attenuation;
}

float flashlight(float3 player_pos, float3 model_pos, float3 normal)
{
	float3 lightDir = normalize(player_pos - model_pos);
	float cos_theta = dot(lightDir, normalize(-flashlight_dir));

	if (cos_theta > flashlight_cutoffAngle)
	{
		return phong_lighting(player_pos, model_pos, normal, 0.09, 0.09) * (1 - 50 * (1 - cos_theta) * (1 - cos_theta));
	}
	else return 0.f;
}

float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 final_light = float3(0.f, 0.f, 0.f);

// ambient 
float ambientStrength = 0.1;
float3 lightColor = float3(1.f, 1.f, 1.f);
float3 ambient = ambientStrength * lightColor;

if (fully_visible)
{
	final_light = lightColor;
}
else
{
	if (light_pos.w != 0)
	{
		final_light += float3(1.f, 1.f, 1.f) * phong_lighting(light_pos.xyz, input.model_pos, input.normal, 0.7, 1.8);
	}
	final_light += float3(1.f, 1.f, 1.f) * flashlight(camera_pos, input.model_pos, input.normal);
}

return float4(final_light, 0.f) * my_texture.Sample(my_sampler, input.texture_pos);
}
