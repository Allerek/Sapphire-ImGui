#include "silent_aim.hpp"

auto sapphire::features::c_silent_aim::view_offset_visible( math::vector_t pos, math::vector_t offset ) -> void
{
	// brocken.
}

auto sapphire::features::c_silent_aim::do_manipulation( ) -> void 
{
	// brocken.
}

auto sapphire::features::c_silent_aim::find_aimbot_target( ) -> bool
{
	if ( !sapphire::globals::local )
		return false;

	const auto get_client_entities = [ & ]( ) { client_entities = il2cpp_lib::field( "BaseNetworkable", "clientEntities", false ); };

	if ( !client_entities )
		get_client_entities( );

	const auto entity_list = reinterpret_cast< managed_system::list_dictionary* >( client_entities );
	if ( !entity_list )
		return false;

	const auto buffer = entity_list->buffer< std::uintptr_t >( );

	for ( size_t idx{ 0 }; idx <= entity_list->size( ); idx++ )
	{
		const auto current_object = *reinterpret_cast< std::uintptr_t* >( buffer + 0x20 + ( idx * 0x8 ) );
		if ( !current_object )
			continue;

		const auto base_object = *reinterpret_cast< std::uintptr_t* >( current_object + 0x10 );
		if ( !base_object )
			continue;

		const auto object = *reinterpret_cast< std::uintptr_t* >( base_object + 0x30 );
		if ( !object )
			continue;

		const auto tag = *reinterpret_cast< std::uint16_t* >( object + 0x54 );
		if ( tag != 6 )
			continue;

		const auto player = reinterpret_cast< rust::classes::c_base_player* >( current_object );
		if ( !player )
			continue;

		if ( player->is_local_player( ) )
			continue;

		if ( !player->alive( ) )
			continue;

		if ( player->sleeping( ) )
			continue;

		const auto pos = player->get_bone_transform( 48 )->position( );
		//const auto source = sapphire::globals::local->eyes( )->position( );
		
		//const auto visible = player->visible( source, pos );
		//if ( !visible )
		//	continue;

		const auto fov = unity::calculate_fov( pos );

		if ( fov >= 100 )
			continue;

		m_aim_target = player;
		return true;
	}

	return false;
}

auto sapphire::features::c_silent_aim::on_projectile_shoot_rpc( std::uint64_t instance ) -> void
{
	if ( !sapphire::globals::local )
		return;

	if ( !m_aim_target )
		return;

	if ( !m_aim_target->alive( ) )
		return;

	const auto weapon = sapphire::globals::local->weapon( );
	if ( !weapon )
		return;

	const auto base_projectile = weapon->heldEntity( );
	if ( !base_projectile )
		return;

	const auto projectile_list = *reinterpret_cast< rust::classes::c_base_projectile** >( *reinterpret_cast< std::uintptr_t* >( base_projectile + 0x358 ) + 0x10 );

	// maybe use system::list 
	const auto shoot_list = *reinterpret_cast< std::uintptr_t* >( *reinterpret_cast< std::uintptr_t* >( instance + 0x18 ) + 0x10 );
	const auto shoot_list_size = *reinterpret_cast< int* >( *reinterpret_cast< std::uintptr_t* >( instance + 0x18 ) + 0x18 );

	for ( size_t idx{ 0 }; idx < shoot_list_size; idx++ )
	{
		const auto projectile_shoot = *reinterpret_cast< uintptr_t* >( shoot_list + 0x20 + idx * 0x8 );

		auto start_position = *reinterpret_cast< math::vector_t* >( projectile_shoot + 0x18 );
		auto original_velocity = *reinterpret_cast< math::vector_t* >( projectile_shoot + 0x24 );

		auto target_head = m_aim_target->get_bone_transform( 48 )->position( );

		auto direction = target_head - start_position;
		m_aim_angle = ( direction.normalized( ) * original_velocity.length( ) );

		*reinterpret_cast< math::vector_t* >( projectile_shoot + 0x24 ) = m_aim_angle;
	}

	for ( size_t idx{ 0 }; idx < projectile_list->list_size( ); idx++ )
	{
		const auto projectile = *reinterpret_cast< rust::classes::c_projectile** >( projectile_list + 0x20 + ( idx * 0x8 ) );

		if ( !projectile )
			continue;

		if ( m_aim_target )
			projectile->currentVelocity( ) = m_aim_angle;

		projectile->thickness( ) = 0.85f;
	}
}
