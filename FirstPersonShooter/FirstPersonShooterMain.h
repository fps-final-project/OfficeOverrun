#pragma once
#include <CommonStates.h>
#include <Keyboard.h>
#include <Mouse.h>
#include <queue>

#include "Content\SampleFpsTextRenderer.h"
#include "SimpleCollisionDetector.hpp"
#include "AnimatedModelRenderer.hpp"
#include "Common\DeviceResources.h"
#include "Common\StepTimer.h"
#include "SpriteRenderer.hpp"
#include "ModelRenderer.hpp"
#include "RenderMaster.hpp"
#include "InputHandler.hpp"
#include "GameState.hpp"
#include "Camera.hpp"
#include "World.h"
#include "GunRig.h"
#include "Menu.hpp"

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
		bool ShouldClose();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		std::shared_ptr<SpriteRenderer> m_spriteRenderer;
		std::shared_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		std::shared_ptr<RenderMaster> m_renderMaster;
		std::shared_ptr<Menu> m_menu;

		std::unique_ptr<DirectX::CommonStates> m_states;
		std::unique_ptr<GameState> m_gameState;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		std::shared_ptr<DirectX::Keyboard> m_keyboard;
		std::shared_ptr<DirectX::Mouse> m_mouse;

		MenuResponse m_lastMenuResponse;
	};
}