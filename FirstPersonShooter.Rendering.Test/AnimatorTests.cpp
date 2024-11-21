#include "pch.h"
#include "CppUnitTest.h"

#include "Animator.h"
#include <memory>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FirstPersonShooter_Rendering_Test
{
	TEST_CLASS(AnimatorTests)
	{
		const float ANIMATION_DURATION = 1000.f;
		const float ANIMATION_TICKSPERSECOND = 60.f;
		const float ANIMATION_UPDATE_TICK = 1.f / 60.f;
		const std::string BONE_NAME = "root";

		const DirectX::XMFLOAT3 DEFAULT_POSITION = {0.f, 0.f, 0.f};
		const DirectX::XMFLOAT4 DEFAULT_ORIENTATION = {0.f, 0.f, 0.f, 0.f};
		const DirectX::XMFLOAT3 DEFAULT_SCALE = {1.f, 1.f, 1.f};

		Joint rootJoint;
		std::map<std::string, BoneInfo> boneInfoMap;

		Animation GetAnimation(
			DirectX::XMFLOAT3 targetPosition,
			DirectX::XMFLOAT4 targetOrientationQuat,
			DirectX::XMFLOAT3 targetScale)
		{
			Animation animation;

			animation.m_Duration = ANIMATION_DURATION;
			animation.m_TicksPerSecond = ANIMATION_TICKSPERSECOND;

			// set up two frames that change the root bone by some amount
			KeyFrame frame1;
			JointTransform transform1;
			transform1.orientation = DEFAULT_ORIENTATION;
			transform1.position = DEFAULT_POSITION;
			transform1.scale = DEFAULT_SCALE;

			frame1.timestamp = 0.f;
			frame1.pose.insert(std::make_pair(BONE_NAME, transform1));

			KeyFrame frame2;
			JointTransform transform2;
			transform2.orientation = targetOrientationQuat;
			transform2.position = targetPosition;
			transform2.scale = targetScale;

			frame2.timestamp = ANIMATION_DURATION;
			frame2.pose.insert(std::make_pair(BONE_NAME, transform2));

			animation.frames.push_back(frame1);
			animation.frames.push_back(frame2);

			return animation;
		}

		DirectX::XMFLOAT4X4 getTransform(DirectX::XMFLOAT3 targetPosition,
			DirectX::XMFLOAT4 targetOrientationQuat,
			DirectX::XMFLOAT3 targetScale,
			DirectX::XMFLOAT4X4 boneOffsetMatrix)
		{
			auto matrix = DirectX::XMMatrixScaling(targetScale.x, targetScale.y, targetScale.z)
				* DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&targetOrientationQuat))
				* DirectX::XMMatrixTranslation(targetPosition.x, targetPosition.y, targetPosition.z);

			matrix = matrix * DirectX::XMLoadFloat4x4(&boneOffsetMatrix);

			DirectX::XMFLOAT4X4 matrix_asf4x4;
			DirectX::XMStoreFloat4x4(&matrix_asf4x4, matrix);

			return matrix_asf4x4;
		}

		double matrixNorm2(DirectX::XMFLOAT4X4 m1, DirectX::XMFLOAT4X4 m2)
		{
			double result = 0.0;

			for(int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
				{
					result += (m1(i, j) - m2(i, j)) * (m1(i, j) - m2(i, j));
				}

			return result;
		}

	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			rootJoint.name = BONE_NAME;
			rootJoint.id = 1;
			rootJoint.transformation = DirectX::XMFLOAT4X4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);

			BoneInfo info;
			info.id = 1;
			info.offsetMatrix = DirectX::XMFLOAT4X4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);

			boneInfoMap.insert(std::make_pair(BONE_NAME, info));
		}

		TEST_METHOD(UpdateAnimation_correctly_applies_transformation)
		{
			// arrange
			DirectX::XMFLOAT3 positionOffset = { 1.f, 1.f, 1.f };

			Animation animation = GetAnimation(positionOffset, DEFAULT_ORIENTATION, DEFAULT_SCALE);
			Animator animator;

			// act
			animator.playAnimation(std::make_shared<Animation>(animation));
			animator.updateAnimation(rootJoint, boneInfoMap, ANIMATION_UPDATE_TICK);

			auto rootTransformMatrix = animator.m_finalBoneMatrices[rootJoint.id];
			DirectX::XMFLOAT4X4 rootTransformMatrix_asf4x4;
			DirectX::XMStoreFloat4x4(&rootTransformMatrix_asf4x4, rootTransformMatrix);


			// assert
			positionOffset = {
				(DEFAULT_POSITION.x + (positionOffset.x - DEFAULT_POSITION.x) / ((ANIMATION_UPDATE_TICK * ANIMATION_TICKSPERSECOND)) / ANIMATION_DURATION),
				(DEFAULT_POSITION.y + (positionOffset.y - DEFAULT_POSITION.y) / ((ANIMATION_UPDATE_TICK * ANIMATION_TICKSPERSECOND)) / ANIMATION_DURATION),
				(DEFAULT_POSITION.z + (positionOffset.z - DEFAULT_POSITION.z) / ((ANIMATION_UPDATE_TICK * ANIMATION_TICKSPERSECOND)) / ANIMATION_DURATION) };

			auto matNorm = matrixNorm2(
				rootTransformMatrix_asf4x4,
				getTransform(positionOffset, DEFAULT_ORIENTATION, DEFAULT_SCALE, boneInfoMap[BONE_NAME].offsetMatrix));

			Assert::IsTrue(matNorm < 1e-9);
		}

		TEST_METHOD(UpdateAnimation_correctly_applies_rotation)
		{
			// arrange
			DirectX::XMFLOAT4 rotationOffset = { 1.f, 1.f, 1.f, 1.f};
			
			Animation animation = GetAnimation(DEFAULT_POSITION, rotationOffset, DEFAULT_SCALE);
			Animator animator;

			// act
			animator.playAnimation(std::make_shared<Animation>(animation));
			animator.updateAnimation(rootJoint, boneInfoMap, ANIMATION_UPDATE_TICK);

			auto rootTransformMatrix = animator.m_finalBoneMatrices[rootJoint.id];
			DirectX::XMFLOAT4X4 rootTransformMatrix_asf4x4;
			DirectX::XMStoreFloat4x4(&rootTransformMatrix_asf4x4, rootTransformMatrix);

			// assert
			DirectX::XMStoreFloat4(&rotationOffset, DirectX::XMVector4Normalize(DirectX::XMQuaternionSlerp(
				DirectX::XMLoadFloat4(&DEFAULT_ORIENTATION), 
				DirectX::XMLoadFloat4(&rotationOffset), 
				(ANIMATION_UPDATE_TICK * ANIMATION_TICKSPERSECOND) / ANIMATION_DURATION)));

			auto matNorm = matrixNorm2(
				rootTransformMatrix_asf4x4,
				getTransform(DEFAULT_POSITION, rotationOffset, DEFAULT_SCALE, boneInfoMap[BONE_NAME].offsetMatrix));

			Assert::IsTrue(matNorm < 1e-9);
		}

		TEST_METHOD(UpdateAnimation_correctly_applies_scale)
		{
			// arrange
			DirectX::XMFLOAT3 scaleOffset = { 2.f, 2.f, 2.f };

			Animation animation = GetAnimation(DEFAULT_POSITION, DEFAULT_ORIENTATION, scaleOffset);
			Animator animator;

			// act
			animator.playAnimation(std::make_shared<Animation>(animation));
			animator.updateAnimation(rootJoint, boneInfoMap, ANIMATION_UPDATE_TICK);

			auto rootTransformMatrix = animator.m_finalBoneMatrices[rootJoint.id];
			DirectX::XMFLOAT4X4 rootTransformMatrix_asf4x4;
			DirectX::XMStoreFloat4x4(&rootTransformMatrix_asf4x4, rootTransformMatrix);


			// assert
			scaleOffset = {
				(DEFAULT_SCALE.x + (scaleOffset.x - DEFAULT_SCALE.x) / ((ANIMATION_UPDATE_TICK * ANIMATION_TICKSPERSECOND)) / ANIMATION_DURATION),
				(DEFAULT_SCALE.y + (scaleOffset.y - DEFAULT_SCALE.y) / ((ANIMATION_UPDATE_TICK * ANIMATION_TICKSPERSECOND)) / ANIMATION_DURATION),
				(DEFAULT_SCALE.z + (scaleOffset.z - DEFAULT_SCALE.z) / ((ANIMATION_UPDATE_TICK * ANIMATION_TICKSPERSECOND)) / ANIMATION_DURATION) };

			auto matNorm = matrixNorm2(
				rootTransformMatrix_asf4x4,
				getTransform(DEFAULT_POSITION, DEFAULT_ORIENTATION, scaleOffset, boneInfoMap[BONE_NAME].offsetMatrix));

			Assert::IsTrue(matNorm < 1e-9);
		}
	};
}