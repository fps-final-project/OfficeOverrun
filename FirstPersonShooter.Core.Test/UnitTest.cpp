#include "pch.h"
#include "CppUnitTest.h"

#include "World.h"
#include "MapGeneratorAdapter.h"
#include "EnemyBuilder.hpp"
#include "ActionHandler.hpp"
#include "Player.h"
#include "Pathfinder.h"
#include "ResourceManager.h"
#include "SimpleCollisionDetector.hpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace FirstPersonShooter_Core_Test
{
	bool operator==(const XMFLOAT3& lhs, const XMFLOAT3& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
	}


    TEST_CLASS(EnemyTest)
    {
		EnemyBuilder enemyBuilder;
        World world;
		MapGeneratorAdapter mapGeneratorAdapter;
        std::shared_ptr<Pathfinder> pathfinder;
    public:
        EnemyTest()
        {
			world.m_rooms = mapGeneratorAdapter.GenerateRooms();
            pathfinder = std::make_shared<Pathfinder>(world.m_rooms, DirectX::XMFLOAT3(0.f, 0.f, 0.f));
        }
        TEST_METHOD(SameRoomShouldMove)
        {            
			XMFLOAT3 start = { 1.f, 1.f, 1.f };
			XMFLOAT3 playerPos = { 0.f, 1.f, 0.f };
            pathfinder->UpdatePlayerNode(playerPos);

            std::shared_ptr<Enemy> e = enemyBuilder
                .WithNewEnemy(std::make_shared<AnimatedModel>())
                .WithSpeed(1.f)
                .WithPosition(start)
                .WithPath(pathfinder)
                .Build();

            
            e->Update(pathfinder, playerPos);

			e->getPosition() == start;
            Assert::IsTrue(!(e->getPosition() == start));
        }
        TEST_METHOD(AdjacentRoomShouldMove)
        {

            XMFLOAT3 playerPos = { 0.f, 1.f, 0.f };
            pathfinder->UpdatePlayerNode(playerPos);
            int adj = world.GetCurrentRoom().getAdjacentRooms()[0];
            XMFLOAT3 roomPos = world.m_rooms[adj].getPosition();
            XMFLOAT3 roomSize = world.m_rooms[adj].getSize();
            XMFLOAT3 start = { roomPos.x + (roomSize.x / 2.f), roomPos.y, roomPos.z + (roomSize.z / 2.f) };
            std::shared_ptr<Enemy> e = enemyBuilder
                .WithNewEnemy(std::make_shared<AnimatedModel>())
                .WithSpeed(1.f)
                .WithPosition(start)
                .WithPath(pathfinder)
                .Build();

            e->Update(pathfinder, playerPos);

            Assert::IsTrue(!(e->getPosition() == start));
        }
		TEST_METHOD(NotAdjacentRoomShouldNotMove)
        {
			XMFLOAT3 playerPos = { 0.f, 1.f, 0.f };
            pathfinder->UpdatePlayerNode(playerPos);

            std::vector adj = world.GetCurrentRoom().getAdjacentRooms();

            for (int i = 0; i < world.m_rooms.size(); i++)
            {
				if (std::find(adj.begin(), adj.end(), i) == adj.end() && i != world.m_currentRoomIndex)
                {
					XMFLOAT3 roomPos = world.m_rooms[i].getPosition();
					XMFLOAT3 roomSize = world.m_rooms[i].getSize();
					XMFLOAT3 start = { roomPos.x + (roomSize.x / 2.f), roomPos.y + (roomSize.y / 2.f), roomPos.z + (roomSize.z / 2.f) };
                    std::shared_ptr<Enemy> e = enemyBuilder
                        .WithNewEnemy(std::make_shared<AnimatedModel>())
                        .WithSpeed(1.f)
                        .WithPosition(start)
                        .WithPath(pathfinder)
                        .Build();

                    e->Update(pathfinder, playerPos);

					Assert::IsTrue(e->getPosition() == start);

                    break;
				}
            }
		}
        TEST_METHOD(InRadiusShouldAttack)
        {
            XMFLOAT3 start = { 1.f, 1.f, 1.f };
            XMFLOAT3 playerPos = { 0.f, 1.f, 1.f };
            pathfinder->UpdatePlayerNode(playerPos);
            std::shared_ptr<Enemy> e = enemyBuilder
                .WithNewEnemy(std::make_shared<AnimatedModel>())
                .WithSpeed(1.f)
				.WithAttackRadius(1.1f)
                .WithPosition(start)
                .WithPath(pathfinder)
                .Build();


            Action a = e->Update(pathfinder, playerPos);
            Assert::IsTrue(a.type == ActionType::ATTACK);
        }
    };

	TEST_CLASS(WorldTest)
	{
		MapGeneratorAdapter mapGeneratorAdapter;
        EnemyBuilder enemyBuilder;

		TEST_METHOD(RoomShouldChange)
        {
            World world;
			world.m_rooms = mapGeneratorAdapter.GenerateRooms();  
			int adj = world.GetCurrentRoom().getAdjacentRooms()[0];
            XMFLOAT3 roomPos = world.m_rooms[adj].getPosition();
            XMFLOAT3 roomSize = world.m_rooms[adj].getSize();
            XMFLOAT3 start = { roomPos.x + (roomSize.x / 2.f), roomPos.y + (roomSize.y / 2.f), roomPos.z + (roomSize.z / 2.f) };
            world.UpdateCurrentRoom(start);

			Assert::IsTrue(world.m_currentRoomIndex == adj);
		}

		TEST_METHOD(ShotEnemyShouldDelete)
		{
            
			std::shared_ptr<std::queue <Action>> actionQueue = std::make_shared<std::queue<Action>>();
			ActionHandler actionHandler(actionQueue);
			World world;
            Player player{ nullptr };

			std::shared_ptr<Enemy> e = enemyBuilder
                .WithNewEnemy(std::make_shared<AnimatedModel>())
				.Build();

			GUID guid = e->id; 

            world.AddEnemy(e);

			actionHandler.SetLastHitEntity(guid);
            actionQueue->push(Action{ ActionType::SHOOT, {} });
			actionHandler.HandleActions(&player, &world, nullptr, nullptr);
            
            Assert::IsTrue(world.m_enemies.find(guid) == world.m_enemies.end());
            Assert::IsTrue(world.m_animatedEntities.find(guid) == world.m_animatedEntities.end());
		}

        TEST_METHOD(AttackPlayerShouldTakeDamage) 
        {
            std::shared_ptr<std::queue <Action>> actionQueue = std::make_shared<std::queue<Action>>();
            ActionHandler actionHandler(actionQueue);
            World world;
            Player player{ nullptr };

			ActionArgs args;
			args.attack = { 10 };
            actionQueue->push(Action{ ActionType::ATTACK, args });
			actionHandler.HandleActions(&player, &world, nullptr, nullptr);

			Assert::IsTrue(player.getHealth() == 90);
        }
        
        TEST_METHOD(WallCollisionPlayerShouldMove) 
        {
            Player player{ nullptr };
            RoomCollision collisionData;
            collisionData.collision[0] = true;
            collisionData.collision[1] = true;
            collisionData.collision[2] = true;
            collisionData.correction[0] = 1.f;
            collisionData.correction[1] = 1.f;
            collisionData.correction[2] = 1.f;
            collisionData.isOnGround = true;

			player.handleRoomCollision(collisionData);

			Assert::IsTrue(player.getPostition() == XMFLOAT3{ 1.f, 1.f, 1.f });
        }

        TEST_METHOD(ObjectsCollidingShouldRegisterCollision) 
        {
			EnemyBuilder enemyBuilder;
			World world;
			SimpleCollisionDetector collisionDetector;

            std::shared_ptr<Enemy> e = enemyBuilder
                .WithNewEnemy(std::make_shared<AnimatedModel>())
				.WithSpeed(1.f)
                .WithSize({ 1.f, 1.f, 1.f })
				.WithPosition({ 1.f, 1.f, 1.f })
				.Build();

                std::shared_ptr<Enemy> e2 = enemyBuilder
                .WithNewEnemy(std::make_shared<AnimatedModel>())
                .WithSpeed(1.f)
                .WithSize({ 1.f, 1.f, 1.f })
				.WithPosition({ 1.f, 1.f, 1.f })
				.Build();

			world.AddEnemy(e);
			world.AddEnemy(e2);

			auto result = collisionDetector.GetCollisions(world.GetHittableEntities());

			Assert::IsTrue(result[0].first.get() == (Hittable*)e.get() && result[0].second.get() == (Hittable*)e2.get() || 
                result[0].first.get() == (Hittable*)e2.get() && result[0].second.get() == (Hittable*)e.get());
        }

        TEST_METHOD(ObjectsNotCollidingShouldNotRegisterCollision)
        {
            EnemyBuilder enemyBuilder;
            World world;
            SimpleCollisionDetector collisionDetector;

            std::shared_ptr<Enemy> e = enemyBuilder
                .WithNewEnemy(std::make_shared<AnimatedModel>())
                .WithSpeed(1.f)
                .WithSize({ 1.f, 1.f, 1.f })
                .WithPosition({ 1.f, 1.f, 1.f })
                .Build();

            std::shared_ptr<Enemy> e2 = enemyBuilder
                .WithNewEnemy(std::make_shared<AnimatedModel>())
                .WithSpeed(1.f)
                .WithSize({1.f, 1.f, 1.f})
                .WithPosition({ 1.f, 10.f, 1.f })
                .Build();

            world.AddEnemy(e);
            world.AddEnemy(e2);

            auto result = collisionDetector.GetCollisions(world.GetHittableEntities());

            Assert::IsTrue(result.size() == 0);
        }
		
	};
}