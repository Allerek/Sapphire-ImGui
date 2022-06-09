#pragma once
#include "../../../rust/include.hpp"

class c_chai_base_player
{
public:
	rust::classes::c_base_player* player;

	c_chai_base_player( rust::classes::c_base_player* _player )
	{
		player = _player;
	}

	int get_health( )
	{
		return player->_health( );
	}

	void set_flags( int flags ) 
	{
		player->playerFlags( ) |= flags;
	}

	math::vector_t bone_position( int bone_id ) 
	{
		return player->get_bone_transform( bone_id )->position( );
	}

	bool is_alive( )
	{
		return player->alive( );
	}

	std::string get_name( )
	{
		const auto str = std::wstring( player->name( ) );
		return std::string( str.begin( ), str.end( ) );
	}
};

class c_chai_entity_list
{
public:
	managed_system::list_dictionary* list;
	c_chai_entity_list( managed_system::list_dictionary* _list ) 
	{
		list = _list;
	}

	static c_chai_entity_list get_entity_list( )
	{
		const auto client_entities = il2cpp_lib::field( "BaseNetworkable", "clientEntities", false );
		const auto entity_list = reinterpret_cast< managed_system::list_dictionary* >( client_entities );

		return c_chai_entity_list( entity_list );
	}

	int get_size( )
	{
		return list->size( );
	}

	bool is_player( int index )
	{
		const auto buffer = list->buffer< std::uintptr_t >( );
		const auto current_object = *reinterpret_cast< std::uintptr_t* >( buffer + 0x20 + ( index * 0x8 ) );
		const auto base_object = *reinterpret_cast< std::uintptr_t* >( current_object + 0x10 );
		const auto object = *reinterpret_cast< std::uintptr_t* >( base_object + 0x30 );
		const auto tag = *reinterpret_cast< std::uint16_t* >( object + 0x54 );

		if ( tag == 6 )
			return true;
		else
			return false;
	}

	c_chai_base_player get_player( int index )
	{
		const auto buffer = list->buffer< std::uintptr_t >( );
		const auto current_object = *reinterpret_cast< std::uintptr_t* >( buffer + 0x20 + ( index * 0x8 ) );
		const auto player = reinterpret_cast< rust::classes::c_base_player* >( current_object );

		if ( player )
			return c_chai_base_player( player );

		else
			return nullptr;
	}

	c_chai_base_player get_local_player( )
	{
		return c_chai_base_player( sapphire::globals::local );
	}
};