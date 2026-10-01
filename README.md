# BattleBlaster

An Unreal Engine C++ combat game featuring tank-and-tower gameplay, modular gameplay modifiers, procedural arena generation, and wave-based encounter management[cite: 1].

---

## 📸 Development Showcase

| AI Navigation & NavMesh Bounds | Procedural Obstacle Layout | Modular Kitchen Environment |
| :---: | :---: | :---: |
| ![AI Navigation & NavMesh Bounds](https://github.com/user-attachments/assets/5e675fac-79e4-4d7e-a332-59d41bc49220) | ![Procedural Obstacle Layout](https://github.com/user-attachments/assets/f7bc65a8-ac63-4967-bf79-2e57e9b8a6d8) | ![Modular Kitchen Environment](https://github.com/user-attachments/assets/1897804b-959e-4e6f-a766-19e801682993) |
| *RecastNavMesh generation configured for melee and tower enemy AI pathfinding[cite: 3]* | *Procedural grid testing environment with dynamic shadow-casting pillar geometry[cite: 2]* | *Custom-textured arena showcasing modular floor tiles, wall materials, and window meshes[cite: 1, 4]* |

---

## Architecture & Implemented Systems

### 1. Character & Combat Hierarchy
* **Base & Player Characters:** Built around `BaseCharacter` with custom derivations for `PlayerCharacter` and `ComputerCharacter`[cite: 1].
* **Enemy Variants:** Implemented AI combat entities including stationary `TowerEnemyTest` and mobile `MeleeEnemyTest` actors[cite: 1].
* **Health & Damage Handling:** Decoupled combat health mechanics into an actor-attachable `HealthComponent`[cite: 1].
* **Combat Static Meshes:** Integrated base and turret meshes for player and enemy tanks (`SM_TankBase`, `SM_TankTurret`, `SM_TowerBase`, `SM_TowerTurret`)[cite: 1].

### 2. Projectile & Modifier Framework
* **Projectile Mechanics:** Dedicated `Projectile` C++ class paired with `BP_BasicProjectile` for projectile lifecycle, impacts, and directional movement[cite: 1].
* **Projectile Modifiers:** Extensible modifier layer (`ProjectileModifier`) with implemented dynamic behaviors such as `BouncingModifier` and `BP_BouncingModifier`[cite: 1].
* **Character Modifiers:** Extensible character state modifier system (`CharacterModifier`) featuring defensive capabilities like `ShieldModifier` and `BP_ShieldModifier`[cite: 1].

### 3. Spawning & Match Progression
* **Core Game Loop:** Managed through `BattleBlasterGameMode`, `BattleBlasterGameInstance`, and `BP_BattleBlasterPlayerController`[cite: 1].
* **Timeline-Driven Spawns:** Automated combat encounter system driven by `SpawnManager` and data-driven configuration assets via `SpawnTimeLineDataAsset` (`DA_SpawnTimelineDataAsset`)[cite: 1].

### 4. World Creation, AI Navigation & Level Design
* **Procedural Generation:** Automated grid layout logic driven by `MapGenerator` and `BP_MapGenerator`[cite: 1].
* **Pathfinding Infrastructure:** Integrated dynamic `RecastNavMesh` runtime generation across arena boundaries to support enemy tracking and navigation[cite: 3].
* **Environment Modules:** Modular arena prefabs including `BP_BasicFloor`, `BP_BoundaryWall`, and `BP_BarrelsFloor1`[cite: 1].
* **Level Setups:** Maps configured for testing and gameplay including `Main`, `Level_1`, `Lvl_Procedural_Test`, and `KitchenMap`[cite: 1].
* **Environmental Dressing:** Full modular kitchen asset suite containing custom meshes, floor tile sets, wallpapers, glass, and multi-channel PBR textures[cite: 1, 4].

### 5. Input, UI & Audio-Visual Feedback
* **Enhanced Input System:** Configured `IMC_Default` input mapping context with discrete input actions for `IA_Move`, `IA_Look`, `IA_Fire`, and `IA_JUMP`[cite: 1].
* **UI Feedback:** HUD communication implemented via `ScreenMessage` and `WBP_ScreenMessage`[cite: 1].
* **Particle Systems & FX:** Combat visual effects covering projectile trails (`P_ProjectileTrail`), surface impacts (`P_HitEffect`), and destruction bursts (`P_DeathEffect`)[cite: 1].
* **Audio Cues:** Audio assets hooked into combat events, including hit feedback (`Thud_Audio`) and impact detonations (`Explode_Audio`)[cite: 1].

---

To be continued...
