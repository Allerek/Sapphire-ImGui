#include <windows.h>
#include <string>

#include "../hooks.hpp"

auto impl::hooks::hk_on_attacked( void* instance, rust::classes::c_hit_info* hit_info ) -> void
{
	sapphire::features::c_visuals::shot_record_t shot_record_t;

	shot_record_t.m_time = unity::c_time::get_time( );
	shot_record_t.m_start = hit_info->PointStart( );
	shot_record_t.m_end = hit_info->PointEnd( );
	shot_record_t.m_damage = hit_info->damage( );

	sapphire::features::visuals::object.m_shots.emplace_back( shot_record_t );

	const auto string = std::wstring( L"[saph] hit " + std::wstring( hit_info->name( ) ) + L" for " + std::to_wstring( hit_info->damage( ) ) + L" damage" );
	sapphire::features::notifcations::object.push( string.c_str( ), unity::c_time::get_time( ) );

	impl::hooks::on_attacked.get_original< decltype( &impl::hooks::hk_on_attacked ) >( )( instance, hit_info );
}