#pragma once
#include "include.hpp"

class rust::classes::c_base_weapon 
{
public:
	DECLARE_MEMBER( rust::classes::c_base_projectile*, "Item", heldEntity );
	DECLARE_MEMBER( rust::classes::c_item_definition*, "Item", info );

	auto is_weapon( ) -> bool 
	{
		SAPPHIRE_FIELD( "Item", info );
		const auto item_definition = *reinterpret_cast< std::uintptr_t* >( this + info );
		if ( !item_definition )
			return false;


		SAPPHIRE_FIELD( "ItemDefinition", category );
		return *reinterpret_cast< std::uint32_t* >( item_definition + category ) == 0;
	}

	auto item_id( ) -> std::int32_t 
	{
		SAPPHIRE_FIELD( "Item", info );
		const auto item_definition = *reinterpret_cast< std::uintptr_t* >( this + info );
		if ( !item_definition )
			return false;

		SAPPHIRE_FIELD( "ItemDefinition", itemid );
		return *reinterpret_cast< std::int32_t* >( item_definition + itemid );
	}

	auto name( ) -> const wchar_t* 
	{
		SAPPHIRE_FIELD( "Item", info );
		const auto item_def = *reinterpret_cast< std::uintptr_t* >( this + info );
		if ( !item_def )
			return {};

		SAPPHIRE_FIELD( "ItemDefinition", displayName );
		const auto display_name = *reinterpret_cast< std::uintptr_t* >( item_def + displayName );
		if ( !display_name )
			return {};

		const auto weapon_name = reinterpret_cast< managed_system::string* >( *reinterpret_cast< std::uintptr_t* >( display_name + 0x18 ) );

		return weapon_name->buffer;
	}
};