#include "../hooks.hpp"

auto impl::hooks::hk_projectile_shoot_rpc( void* _rcx, void* _rdx, void* a1, std::uint64_t instance, void* a3 ) -> void 
{
	static auto o_func = reinterpret_cast< decltype( &impl::hooks::hk_projectile_shoot_rpc ) >( impl::hooks::o_projectile_shoot_rpc );

	sapphire::features::aimbot::object.on_projectile_shoot_rpc( instance );

	return o_func( _rcx, _rdx, a1, instance, a3 );
}