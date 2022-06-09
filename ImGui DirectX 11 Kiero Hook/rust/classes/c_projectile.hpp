#pragma once
#include "include.hpp"

class rust::classes::c_projectile {
public:
	DECLARE_MEMBER( math::vector_t, "Projectile", currentVelocity )
	DECLARE_MEMBER( float, "Projectile", thickness ) 
	DECLARE_MEMBER( float, "Projectile", gravityModifier )
	DECLARE_MEMBER( float, "Projectile", drag )
};