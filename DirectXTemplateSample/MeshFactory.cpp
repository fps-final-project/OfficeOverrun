#include "pch.h"
#include "MeshFactory.h"
#include "Common/DirectXHelper.h"

Mesh MeshFactory::createMesh(
	const std::vector<DirectXTemplateSample::VertexData>& verticies,
	const std::vector<unsigned short>& indicies,
	const std::vector<std::shared_ptr<Texture>>& textures,
	std::shared_ptr<DX::DeviceResources> deviceResources)
{
	Mesh mesh;
	for (const auto& t : textures)
	{
		mesh.textures.push_back(Texture(*t));
	}

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = verticies.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(verticies.size() * sizeof(DirectXTemplateSample::VertexData), D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			&mesh.vertexBuffer
		)
	);

	// Load mesh indices. Each trio of indices represents
	// a triangle to be rendered on the screen.
	// For example: 0,2,1 means that the vertices with indexes
	// 0, 2 and 1 from the vertex buffer compose the 
	// first triangle of this mesh.
	mesh.indexCount = indicies.size();

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indicies.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(indicies.size() * sizeof(unsigned short), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		deviceResources->GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&mesh.indexBuffer
		)
	);

	return mesh;
}
