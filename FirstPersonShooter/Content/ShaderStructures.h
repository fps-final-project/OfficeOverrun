#pragma once
#define MAX_BONE_INFLUENCE 4


namespace FirstPersonShooter
{
	// Constant buffer used to send MVP matrices to the vertex shader.
	struct ModelViewProjectionConstantBuffer
	{
		DirectX::XMFLOAT4X4 model;
		DirectX::XMFLOAT4X4 inv_model;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	__declspec(align(16))
	struct LightingConstantBuffer
	{
		DirectX::XMFLOAT3 camera_pos;
		DirectX::XMFLOAT3 light_pos;
	};

	// Used to send per-vertex data to the vertex shader.
	struct VertexData
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 texture_pos;
		DirectX::XMFLOAT3 normal;
	};

	struct AnimatedVertexData : public VertexData
	{
		int boneIds[MAX_BONE_INFLUENCE];
		float weights[MAX_BONE_INFLUENCE];

		AnimatedVertexData()
		{
			for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
			{
				this->boneIds[i] = -1;
				this->weights[i] = 0.0f;
			}
		}
	};
}