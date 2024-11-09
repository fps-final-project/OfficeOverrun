#pragma once

#include "pch.h" 
#include <vector>
#include "Common\DeviceResources.h"
#include "Content/ShaderStructures.h"
#include "Texture.h"
#include "Mesh.h"
#include "Common/DirectXHelper.h"


template<typename T>
class MeshFactory {
public:
	static Mesh createMesh(
		const std::vector<T>& verticies,
		const std::vector<unsigned short>& indicies,
		const std::vector<std::shared_ptr<Texture>>& textures,
		std::shared_ptr<DX::DeviceResources> deviceResources);
};

template<typename T>
inline Mesh MeshFactory<T>::createMesh(const std::vector<T>& verticies, const std::vector<unsigned short>& indicies, const std::vector<std::shared_ptr<Texture>>& textures, std::shared_ptr<DX::DeviceResources> deviceResources)
{
	Mesh m_mesh;
	for (const auto& t : textures)
	{
		m_mesh.textures.push_back(Texture(*t));
	}

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = verticies.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(verticies.size() * sizeof(T), D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&m_mesh.vertexBuffer
		)
	);

	m_mesh.indexCount = indicies.size();

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indicies.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(indicies.size() * sizeof(unsigned short), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		deviceResources->GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&m_mesh.indexBuffer
		)
	);

	return m_mesh;
}
