#include "pch.h"
#include "ResourceManager.h"

#include "AudioReader.hpp"
#include "AssimpModelLoader.h"
#include "TextureFactory.h"
#include <filesystem>

ResourceManager ResourceManager::Instance;

void ResourceManager::loadModel(const std::string& path, 
	const std::shared_ptr<DX::DeviceResources>& deviceResources,
	const std::string& nameOverride)
{
	std::string name = nameOverride.empty() ? std::filesystem::path(path).stem().string() : nameOverride;

	this->m_models.insert(std::make_pair(
		name,
		std::make_shared<Model>(AssimpModelLoader::createModelFromFile(path, deviceResources))));
}

void ResourceManager::addModel(Model& model, const std::string& name)
{
	this->m_models.insert(std::make_pair(
		name,
		std::make_shared<Model>(model)));
}

void ResourceManager::loadAnimatedModel(const std::string& path, 
	const std::shared_ptr<DX::DeviceResources>& deviceResources, 
	const std::vector<std::string>& missingTextures,
	const std::string& nameOverride)
{
	std::string name = nameOverride.empty() ? std::filesystem::path(path).stem().string() : nameOverride;

	this->m_animatedModels.insert(std::make_pair(
		name,
		std::make_shared<AnimatedModel>(AssimpModelLoader::createAnimatedModelFromFile(path, deviceResources))));

	for (const auto& texturePath : missingTextures)
	{
		AssimpModelLoader::appendTextureToMesh(texturePath, this->m_animatedModels[name]->meshes[0], deviceResources);
	}
}

void ResourceManager::loadTexture(const std::string& path, 
	const std::shared_ptr<DX::DeviceResources>& deviceResources,
	const std::string& nameOverride)
{
	std::string name = nameOverride.empty() ? std::filesystem::path(path).stem().string() : nameOverride;

	this->m_textures.insert(std::make_pair(
		name,
		std::make_shared<Texture>(
			TextureFactory::CreateTextureFromFile(std::wstring(path.begin(), path.end()).c_str(),
			deviceResources
		))));
}

void ResourceManager::loadAudioFile(
	const std::string& path, 
	const UINT32 loop,
	const std::shared_ptr<DX::DeviceResources>& deviceResources, 
	const std::string& nameOverride
)
{
	std::string name = nameOverride.empty() ? std::filesystem::path(path).stem().string() : nameOverride;

	AudioFile audioFile = AudioReader::ReadWAVFile(path, loop);

	DX::ThrowIfFailed(
		deviceResources
		->GetXAudio()
		->CreateSourceVoice(
			&audioFile.pXAudio2SourceVoice,
			(WAVEFORMATEX*)(&audioFile.wfx)
		));

	DX::ThrowIfFailed(audioFile.pXAudio2SourceVoice->SubmitSourceBuffer(&audioFile.buffer));

	this->m_audioFiles.insert(std::make_pair(
		name,
		std::make_shared<AudioFile>(audioFile)));
}

void ResourceManager::loadGunRigMetadata(const std::string& path)
{
	auto data = std::make_shared<GunRigMetadata>(GunRigMetadata::loadFromFile(path));
	m_gunRigMetadata.insert(std::make_pair(data->name, data));
}

std::shared_ptr<Model> ResourceManager::getModel(std::string name)
{
	if (m_models.find(name) != m_models.end())
	{
		return m_models[name];
	}

	return nullptr;
}

std::shared_ptr<AnimatedModel> ResourceManager::getAnimatedModel(std::string name)
{
	if (m_animatedModels.find(name) != m_animatedModels.end())
	{
		return m_animatedModels[name];
	}

	return nullptr;
}

std::shared_ptr<Texture> ResourceManager::getTexture(std::string name)
{
	if (m_textures.find(name) != m_textures.end())
	{
		return m_textures[name];
	}
	
	return nullptr;
}

std::shared_ptr<GunRigMetadata> ResourceManager::getGunRigMetadata(std::string name)
{
	if (m_gunRigMetadata.find(name) != m_gunRigMetadata.end())
	{
		return m_gunRigMetadata[name];
	}

	return nullptr;
}

std::shared_ptr<AudioFile> ResourceManager::getAudioFile(std::string name)
{
	if (m_audioFiles.find(name) != m_audioFiles.end())
	{
		return m_audioFiles[name];
	}

	return nullptr;
}
