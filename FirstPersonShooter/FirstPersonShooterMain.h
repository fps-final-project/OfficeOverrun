#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Base3DRenderer.h"
#include "Content\SampleFpsTextRenderer.h"

#include <queue>
#include <AnimatedModelDrawRequest.hpp>
#include "World.h"

// Renders Direct2D and 3D content on the screen.
namespace FirstPersonShooter
{
	class FirstPersonShooterMain : public DX::IDeviceNotify
	{
	public:
		FirstPersonShooterMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
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
		std::unique_ptr<Base3DRenderer> m_sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		std::queue<AnimatedModelDrawRequest> m_drawQueue;
		World world;
	};
}