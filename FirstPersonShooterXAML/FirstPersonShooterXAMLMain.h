#pragma once

#include <CommonStates.h>
#include "Content\SampleFpsTextRenderer.h"
#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "SpriteRenderer.hpp"
#include "ModelRenderer.hpp"
#include "RenderMaster.hpp"
#include "InputHandler.hpp"
#include "GameState.hpp"
#include "Camera.hpp"
#include "World.h"
#include "GunRig.h"

#include <Keyboard.h>
#include <Mouse.h>

// Renders Direct2D and 3D content on the screen.
namespace FirstPersonShooterXAML
{
	class FirstPersonShooterXAMLMain : public DX::IDeviceNotify
	{
	public:
		FirstPersonShooterXAMLMain(
			const std::shared_ptr<DX::DeviceResources>& deviceResources,
			const std::shared_ptr<DirectX::Mouse>& mouse,
			const std::shared_ptr<DirectX::Keyboard>& keyboard);
		~FirstPersonShooterXAMLMain();
		void CreateWindowSizeDependentResources();
		void StartRenderLoop();
		void StopRenderLoop();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();
		bool ShouldClose();

	private:
		void ProcessInput();
		void Update();
		bool Render();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::unique_ptr<SpriteRenderer> m_spriteRenderer;
		std::unique_ptr<FirstPersonShooter::SampleFpsTextRenderer> m_fpsTextRenderer;

		std::shared_ptr<RenderMaster> m_renderMaster;

		std::unique_ptr<DirectX::CommonStates> m_states;
		std::unique_ptr<GameState> m_gameState;

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		// Track current input pointer position.
		float m_pointerLocationX;

		std::shared_ptr<DirectX::Keyboard> m_keyboard;
		std::shared_ptr<DirectX::Mouse> m_mouse;
	};
}