#pragma once
#include "rust.hpp"

class rust::classes::c_player_eyes 
{
public:
	DECLARE_MEMBER( math::vector_t, "PlayerEyes", viewOffset )

	auto position( ) -> math::vector_t {
		SAPPHIRE_METHOD( get_position_fn, "PlayerEyes.get_position()", 0, "", -1, math::vector_t( * )( void* ) );
		return get_position_fn( this );
	}
};