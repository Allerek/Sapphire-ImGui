#pragma once
#include "../include.hpp"
#pragma warning( disable : 26451 )

class rust::classes::c_base_player
{
public:
	DECLARE_MEMBER( float, "BaseCombatEntity", _health )
	DECLARE_MEMBER( float, "BaseCombatEntity", _maxHealth )
	DECLARE_MEMBER( std::uintptr_t, "BasePlayer", playerModel )
	DECLARE_MEMBER( std::uintptr_t, "BasePlayer", playerFlags )
	DECLARE_MEMBER( rust::classes::c_walk_movement*, "BasePlayer", movement )
	DECLARE_MEMBER( rust::classes::c_player_eyes*, "BasePlayer", eyes )
	DECLARE_MEMBER( rust::classes::c_model_state*, "BasePlayer", modelState )
	DECLARE_MEMBER( float, "BasePlayer", clientTickInterval )

	auto name( ) -> const wchar_t* 
	{
		SAPPHIRE_FIELD( "BasePlayer", _displayName );
		auto player_name = reinterpret_cast< managed_system::string* >( *reinterpret_cast< std::uintptr_t* >( this + _displayName ) );
		return player_name->buffer;
	}

	auto npc( ) -> bool {
		SAPPHIRE_METHOD( get_is_npc_fn, "PlayerModel.get_IsNpc()", -1, "", -1, bool( * )( std::uintptr_t ) );
		return get_is_npc_fn( this->playerModel( ) );
	}

	auto networkable( ) -> rust::classes::c_networkable* 
	{
		return *reinterpret_cast< rust::classes::c_networkable** >( this + 0x50 );
	}

	auto sleeping( ) -> bool 
	{
		return this->playerFlags( ) & rust::enums::e_player_flags::sleeping;
	}

	auto is_local_player( ) -> bool 
	{
		if ( !this )
			return false;

		auto player_model = this->playerModel( );
		if ( !player_model )
			return false;

		SAPPHIRE_FIELD( "PlayerModel", isLocalPlayer );
		return *reinterpret_cast< bool* >( player_model + isLocalPlayer );
	}

	auto velocity( ) -> math::vector_t 
	{
		SAPPHIRE_FIELD( "PlayerModel", newVelocity );
		return *reinterpret_cast< math::vector_t* >( this->playerModel( ) + newVelocity );
	}

	auto weapon( ) -> rust::classes::c_base_weapon* // crashing on death. need to fix that.
	{
		SAPPHIRE_FIELD( "BasePlayer", clActiveItem );
		const auto active_uid = *reinterpret_cast< unsigned int* >( this + clActiveItem );
		if ( !active_uid )
			return 0;

		SAPPHIRE_FIELD( "BasePlayer", inventory );
		const auto inv = *reinterpret_cast< std::uint64_t* >( this + inventory );
		const auto belt = *reinterpret_cast< std::uint64_t* >( inv + 0x28 );
		const auto item_list = *reinterpret_cast< std::uint64_t* >( belt + 0x38 );

		const auto items = *reinterpret_cast< std::uint64_t* >( item_list + 0x10 );
		if ( !items )
			return {};

		for ( int i = 0; i < 6; i++ )
		{
			const auto weapon_info = *reinterpret_cast< rust::classes::c_base_weapon** >( items + 0x20 + ( i * 0x8 ) );
			if ( !weapon_info )
				return {};

			const auto weapon_uid = *reinterpret_cast< unsigned int* >( weapon_info + 0x28 );
			if ( !weapon_uid )
				return {};

			if ( active_uid == weapon_uid )
				return weapon_info;
		}
		return {};
	}

	auto alive( ) -> bool 
	{
		if ( !this )
			return false;

		SAPPHIRE_FIELD( "BaseCombatEntity", lifestate );
		return !( *reinterpret_cast< bool* >( this + lifestate ) );
	}

	auto visible( math::vector_t source, math::vector_t destination ) -> bool 
	{
		return unity::line_of_sight( source, destination );
	}

	auto get_bone_transform( int id ) -> rust::classes::c_transform* 
	{
		SAPPHIRE_FIELD( "BaseEntity", model );
		const auto entity_model = *reinterpret_cast< std::uintptr_t* >( this + model );
		if ( !entity_model )
			return { };

		const auto bone_dict = *reinterpret_cast< std::uintptr_t* >( entity_model + 0x48 );
		if ( !bone_dict )
			return { };

		return *reinterpret_cast< rust::classes::c_transform** >( bone_dict + 0x20 + id * 0x8 );
	}
};