# Office Overrun

**Office Overrun** is a first-person shooter video game with procedurally generated levels, developed as part of a diploma thesis. The project includes:

- A fully developed rendering pipeline using **DirectX 11**
- Custom world generation algorithms based on **grammars**, **graph algorithms**, and **Binary Space Partitioning (BSP)**
- Advanced **zombie AI**
- Shooting mechanics using the **stencil buffer**
- Animated **skeletal models**
- Spatialized **3D audio**

<p align="center">
  <img src="https://github.com/user-attachments/assets/0824fb02-b73a-41d7-86ac-121ee530471c" alt="gameplay">
</p>


## Architecture

The application is divided into three components, each compiled into a separate DLL and subjected to isolated unit testing:

- **Rendering Module** – Responsible for loading models, rendering the game scene, and applying shading.
- **World Generation Module** – Handles procedural generation of game levels.
- **Core Module** – Manages game logic, state processing, and communication between modules.

<p align="center">
  <img src="https://github.com/user-attachments/assets/7a916451-a298-494d-91a5-3ec4ba0cd345" alt="architecture" width="80%">
</p>


## Procedural World Generation

Every level in the game is unique! The application uses advanced procedural generation algorithms to create the building layout, place enemies, and populate rooms with props.

The generation process consists of several steps:

1. Generate a 3D building and divide it into multiple floors and sections
2. Use graph manipulation algorithms to create structured connections between rooms, ensuring a maze-like experience
3. Apply grammar-based graph rewriting rules to assign room types
4. Based on room types and geometry, generate dynamic and static props within each room

<p align="center">
  <img src="https://github.com/user-attachments/assets/18a35ea3-53b9-4779-b773-a54dfbedb1b4" alt="rooms gif">
</p>

## Zombie AI

The game features advanced pathfinding AI for zombie NPCs.  
The implemented algorithms are designed to make enemies actively follow and attempt to surround the player.

<p align="center">
  <img src="https://github.com/user-attachments/assets/028f0d61-a4d8-4fe3-b637-85fb7bc02c0b" alt="zombie AI">
</p>

## Shooting

The game features a complex shooting mechanism.  
Each shot checks for collisions with zombies using dynamic hitboxes stored in the stencil buffer, which is integrated into the rendering pipeline.

<p align="center">
  <img src="https://github.com/user-attachments/assets/9d7a39c8-69c6-4d17-b0a5-d3cbdc4c1113" alt="shooting">
</p>

## Dynamic Lighting

The game features dynamic lighting based on the **Blinn-Phong** reflection model.  
The two primary light sources are:

- A **flashlight** held by the player
- A **muzzle flash** that appears when firing a weapon

<p align="center">
  <img src="https://github.com/user-attachments/assets/88df073d-2216-42bf-801d-366f3a4e206f" alt="Image 1" width="45%" />
  <img src="https://github.com/user-attachments/assets/b09e5acc-02bf-4685-92c6-f15d7c3ecb00" alt="Image 2" width="45%" />
</p>
