#include "../hooks.hpp"
#include "../../../shared/chaiscript/system/chai_wrapper.hpp"
#include <thread>
#pragma warning( disable : 4267 )

static bool has_initialized{ false };

auto impl::hooks::hk_bp_client_input( rust::classes::c_base_player* base_player, std::uintptr_t* input_state ) -> void
{
	if ( !has_initialized )
	{
		static auto initialize_method = utl::pattern::find( L"GameAssembly.dll", "48 83 EC 48 48 8B 05 ? ? ? ? 48 63 90 ? ? ? ?" );
		static auto il2cpp_codegen_initialize_method = reinterpret_cast< void ( * )( unsigned int ) >( initialize_method );

		for ( size_t idx{ 0 }; idx < 56229; idx++ )
		{
			il2cpp_codegen_initialize_method( idx );
		}

		static auto projectile_shoot_sig = utl::pattern::find( L"GameAssembly.dll", "4C 8B 0D ? ? ? ? 48 8B 75 28" );
		const auto relative_projectile_shoot = *reinterpret_cast< std::uintptr_t* >( projectile_shoot_sig + *reinterpret_cast< std::int32_t* >( projectile_shoot_sig + 3 ) + 7 );

		if ( relative_projectile_shoot )
		{
			const auto projectile_shoot_rpc = **reinterpret_cast< std::uintptr_t*** >( relative_projectile_shoot + 0x30 );
			HOOK_CUSTOM( projectile_shoot_rpc, impl::hooks::o_projectile_shoot_rpc, &impl::hooks::hk_projectile_shoot_rpc );
		}

		std::thread( [ ]( )
			{
				while ( true )
				{
					if ( unity::get_key( unity::key_code::UpArrow ) )
					{
						auto wrapper = c_chai_wrapper( );

						wrapper.chai_init( );
						wrapper.chai_load( "C:\\sapphire\\scripts\\main.chai" ); // todo; make instances identifiable so we can reload one instance instead of loading a new one

						chai_instance_manager::instances.push_back( wrapper );

						sapphire::features::notifcations::object.push( L"[saph] loaded script!", unity::c_time::get_time( ) );
					}
				}
			}
		).detach( );

		has_initialized = true;
	}

	for ( auto instance : chai_instance_manager::instances )
	{
		if ( instance.script_on_client_input )
			instance.chai_on_client_input_callback( base_player, input_state );
	}

	if ( base_player )
	{
		if ( !sapphire::globals::local )
			sapphire::globals::local = base_player;

		sapphire::features::aimbot::object.find_aimbot_target( );

		base_player->playerFlags( ) |= rust::enums::e_player_flags::is_admin; // beware i dont remove flying from your modelstate flags so if you noclip you will get a cheat detected! ban msg.
		if ( const auto movement = base_player->movement( ) )
		{
			base_player->modelState( )->set_flag( rust::enums::e_model_state_flag::on_ground );
			*reinterpret_cast< float* >( movement + 0x4c ) = 1.f;
		}

		if ( unity::get_key( unity::key_code::Mouse3 ) )
			base_player->clientTickInterval( ) = 0.99f;
		else
			base_player->clientTickInterval( ) = 0.03f;

		if ( unity::get_key( unity::key_code::DownArrow ) ) {
			chai_instance_manager::instances.clear( );
		}

		SAPPHIRE_METHOD( draw_get, "UnityEngine::DDraw.Get()", -1, "", -1, void ( * )( ) );
		draw_get( );
	}

	impl::hooks::bp_client_input.get_original< decltype( &impl::hooks::hk_bp_client_input ) >( )( base_player, input_state );
}