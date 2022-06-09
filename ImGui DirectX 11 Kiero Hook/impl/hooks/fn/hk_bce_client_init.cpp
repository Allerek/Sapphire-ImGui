#include "../hooks.hpp"
#include "../../../shared/utl/sapphire_crt.hpp"

auto impl::hooks::hk_bce_client_init( rust::classes::c_base_player* instance, void* entity ) -> void
{
	return impl::hooks::bce_client_init.get_original< decltype( &impl::hooks::hk_bce_client_init ) >( )( instance, entity );
}