#pragma once

enum EKeyState
{
	KeyState_Down,
	KeyState_Push,
	KeyState_Up,
	KeyState_Max
};

enum class ETexture_Type
{
	Atlas,
	Frame
};

enum class ECollider_Type
{
	Box,
	Sphere,
	Point
};

enum ECollision_Channel
{
	Channel_Object,
	Channel_Player,
	Channel_Monster,
	Channel_PlayerAttack,
	Channel_MonsterAttack,
	Channel_Door,
	Channel_Max
};

enum class ECollision_State
{
	Ignore,
	Collision
};

enum class EButton_State
{
	Normal,
	MouseOn,
	Click,
	Disable,
	End
};

enum class EObject_Type
{
	GameObject,
	Character,
	Bullet,
	Effect
};

enum class EMonsterType
{
	NormalGround,
	NormalFly,
	Boss
};
enum class EEditMode
{
	TileMap,
	Spawner,
	Door,
	End
};

enum class ESpawnerEditMode
{
	Create,
	Edit,
	Delete,
	End
};

enum class ESpawnerEditDir
{
	Left,
	Right,
	End
};

enum class ESpawnerEditOBJ
{
	Zombie,
	Boss,
	End
};

enum class ETileOption
{
	Normal,
	Wall,
	Slow,
	End
};

enum class ETileEditMode
{
	Option,
	Image,
	Line,
	End
};

enum class EMonsterAI
{
	Idle,
	Detect,
	Trace,
	Attack,
	Death
};

enum EPlayerAnimState
{
	Idle,
	MoveStart,
	Move,
	Down,
	JumpStart,
	Jump,
	JumpMax,
	DownJump,
	DoubleJump,
	SuperJump,
	Falling,
	JumpEnd,
	Attack,
	AttackEnd,
	DownAttack,
	DownAttackEnd,
	JumpAttack,
	JumpAttackEnd,
	Up,
	SoulAttack,
	SoulAttackEnd,
	Damaged,
	JumpDamaged,
	Dash,
	Ceilling
};

enum class EDoorStage
{
	Stage1,
	Stage2,
	Stage3,
	Ending,
	End
};

enum class EDoorForm
{
	Left,
	Right,
	End
};
