#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\SampleFpsTextRenderer.h"
#include <Keyboard.h>
#include <Mouse.h>

#include <queue>
#include "World.h"
#include "GunRig.h"
#include "Camera.hpp"
#include "AnimatedModelRenderer.hpp"
#include "ModelRenderer.hpp"
#include "SpriteRenderer.hpp"
#include <CommonStates.h>

// Renders Direct2D and 3D content on the screen.
namespace FirstPersonShooter
{
	class FirstPersonShooterMain : public DX::IDeviceNotify
	{
	public:
		FirstPersonShooterMain(
			const std::shared_ptr<DX::DeviceResources>& deviceResources,
			const std::shared_ptr<DirectX::Mouse>& mouse,
			const std::shared_ptr<DirectX::Keyboard>& keyboard);

		~FirstPersonShooterMain();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::unique_ptr<ModelRenderer> m_modelRenderer;
		std::unique_ptr<AnimatedModelRenderer> m_animatedRenderer;
		std::unique_ptr<SpriteRenderer> m_spriteRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		std::unique_ptr<Camera> m_camera;
		std::unique_ptr<World> m_world;
		std::unique_ptr<GunRig> m_gunRig;
		std::unique_ptr<DirectX::CommonStates> m_states;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		std::shared_ptr<DirectX::Keyboard> m_keyboard;
		std::shared_ptr<DirectX::Mouse> m_mouse;

		const float FOV = 95.0f;

	};
}