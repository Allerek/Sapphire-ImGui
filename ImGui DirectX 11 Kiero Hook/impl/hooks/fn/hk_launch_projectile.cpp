#include "../hooks.hpp"

auto impl::hooks::hk_launch_projectile( void* instance ) -> void 
{
	if ( sapphire::globals::local ) 
	{
		const auto player_eyes = sapphire::globals::local->eyes( );
		const auto original_pos = player_eyes->viewOffset( );

		if ( sapphire::features::aimbot::object.m_view_offset == original_pos )
			return impl::hooks::launch_projectile.get_original< decltype( &impl::hooks::hk_launch_projectile ) >( )( instance );

		if ( !sapphire::features::aimbot::object.m_view_offset.is_empty( ) )
			player_eyes->viewOffset( ) = sapphire::features::aimbot::object.m_view_offset;

		impl::hooks::launch_projectile.get_original< decltype( &impl::hooks::hk_launch_projectile ) >( )( instance );

		player_eyes->viewOffset( ) = original_pos;
	}
}