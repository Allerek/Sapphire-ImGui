#pragma once
#include "include.hpp"

class rust::classes::c_walk_movement
{
public:
	DECLARE_MEMBER( bool, "PlayerWalkMovement", flying )
	DECLARE_MEMBER( bool, "PlayerWalkMovement", swimming )
	DECLARE_MEMBER( float, "PlayerWalkMovement", landTime )
	DECLARE_MEMBER( float, "PlayerWalkMovement", jumpTime )
	DECLARE_MEMBER( float, "PlayerWalkMovement", groundTime )
	DECLARE_MEMBER( float, "PlayerWalkMovement", groundAngleNew )
	DECLARE_MEMBER( float, "PlayerWalkMovement", gravityMultiplier )
};