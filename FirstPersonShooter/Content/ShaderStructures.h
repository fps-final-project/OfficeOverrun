#pragma once
#define MAX_BONE_INFLUENCE 4

#pragma region 2D
struct VertexData2D
{
	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 texture_pos;
};

struct VertexShaderData2D
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 projection;
};

#pragma endregion

#pragma region 3D

struct VertexData
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 texture_pos;
	DirectX::XMFLOAT3 normal;
	VertexData() {}
	VertexData(DirectX::XMFLOAT3 _pos, DirectX::XMFLOAT2 _tx, DirectX::XMFLOAT3 _norm)
		: pos(_pos), texture_pos(_tx), normal(_norm)
	{}
};

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
	int fully_visible;              // 4 bytes
	float flashlight_cutoffAngle;    // 4 bytes
	int pad1;
	int pad11;
	DirectX::XMFLOAT3 flashlight_dir;// 12 bytes
	int pad2;
	DirectX::XMFLOAT3 camera_pos;    // 12 bytes
	int pad3;
	DirectX::XMFLOAT4 light_pos; 
};

#pragma endregion

#pragma region 3DAnimated

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

struct AnimationConstantBuffer 
{
	static const int MAX_BONES = 50;
	DirectX::XMFLOAT4X4 pose[MAX_BONES];
};

#pragma endregion






