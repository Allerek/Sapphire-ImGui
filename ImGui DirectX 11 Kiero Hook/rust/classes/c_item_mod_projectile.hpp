#pragma once
#include "include.hpp"

class rust::classes::c_item_mod_projectile {
public:
	auto get_random_velocity( ) -> float {
		SAPPHIRE_METHOD( get_random_velocity_fn, "ItemModProjectile.GetRandomVelocity()", -1, "", -1, float( * )( void* ) );
		return get_random_velocity_fn( this );
	}
};