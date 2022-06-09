#pragma once
#include "include.hpp"

class rust::classes::c_hit_info
{
public:
	DECLARE_MEMBER( math::vector_t, "HitInfo", PointStart );
	DECLARE_MEMBER( math::vector_t, "HitInfo", PointEnd );
	DECLARE_MEMBER( std::uintptr_t, "HitInfo", damageTypes );
	DECLARE_MEMBER( rust::classes::c_base_player*, "HitInfo", HitEntity );

	auto name( ) -> const wchar_t*
	{
		auto ent = this->HitEntity( );

		if ( ent )
			return ent->name( );

		return L"unknown";
	}

	auto damage( ) -> int { // this is wrong. always returns same value.
		SAPPHIRE_METHOD( get_total_damage_fn, "Rust::DamageTypeList.Total()", -1, "", -1, float( * )( std::uintptr_t ) );
		return static_cast< int >( get_total_damage_fn( this->damageTypes( ) ) );
	}
};