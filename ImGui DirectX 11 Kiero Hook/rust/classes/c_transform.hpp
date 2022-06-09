#pragma once
#include "../include.hpp"
#include "../../shared/unity/unity_sdk.hpp"

class rust::classes::c_transform
{
public:
	auto forward( ) -> math::vector_t
	{
		SAPPHIRE_METHOD( get_forward_fn, "UnityEngine::Transform.get_forward()", 0, "", -1, math::vector_t( * )( c_transform* ) );
		return get_forward_fn( this );
	}

	auto right( ) -> math::vector_t
	{
		SAPPHIRE_METHOD( get_right_fn, "UnityEngine::Transform.get_right()", 0, "", -1, math::vector_t( * )( c_transform* ) );
		return get_right_fn( this );
	}

	auto up( ) -> math::vector_t
	{
		SAPPHIRE_METHOD( get_up, "UnityEngine::Transform.get_up()", 0, "", -1, math::vector_t( * )( c_transform* ) );
		return get_up( this );
	}

	auto position( ) -> math::vector_t
	{
		SAPPHIRE_METHOD( get_position_fn, "UnityEngine::Transform.get_position()", 0, "", -1, math::vector_t( * )( c_transform* ) );
		return get_position_fn( this );
	}

	auto set_position( math::vector_t pos ) -> math::vector_t
	{
		SAPPHIRE_METHOD( set_position_fn, "UnityEngine::Transform.set_position()", 1, "", -1, void( * )( c_transform*, math::vector_t ) );
		set_position_fn( this, pos );
	}
};