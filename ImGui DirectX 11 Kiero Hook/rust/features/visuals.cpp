#include "visuals.hpp"

#pragma warning( disable : 4267 )

auto sapphire::features::c_visuals::run( ) -> void
{
	auto get_client_entities = [ & ]( ) { client_entities = il2cpp_lib::field( "BaseNetworkable", "clientEntities", false ); };

	if ( !client_entities )
		get_client_entities( );

	const auto entity_list = reinterpret_cast< managed_system::list_dictionary* >( client_entities );

	if ( !entity_list )
	{
		get_client_entities( );
		return;
	}

	const auto buffer = entity_list->buffer< std::uintptr_t >( );

	for ( size_t idx{ 0 }; idx <= entity_list->size( ); idx++ )
	{
		auto current_object = *reinterpret_cast< std::uintptr_t* >( buffer + 0x20 + ( idx * 0x8 ) );
		if ( !current_object )
			continue;

		auto base_object = *reinterpret_cast< std::uintptr_t* >( current_object + 0x10 );
		if ( !base_object )
			continue;

		auto object = *reinterpret_cast< std::uintptr_t* >( base_object + 0x30 );
		if ( !object )
			continue;

		const auto tag = *reinterpret_cast< std::uint16_t* >( object + 0x54 );
		if ( tag != 6 )
			continue;

		const auto ent = *reinterpret_cast< std::uintptr_t* >( base_object + 0x28 );
		if ( !ent )
			continue;

		const auto player = reinterpret_cast< rust::classes::c_base_player* >( ent );
		if ( player->sleeping( ) )
			continue;


		chams( player );
		render_player( player );
	}

	hand_chams( );
	draw_tracers( );
}

auto sapphire::features::c_visuals::draw_tracers( ) -> void 
{
	if ( m_shots.empty( ) )
		return;

	SAPPHIRE_METHOD( ddraw_line_fn, "UnityEngine::DDraw.Line()", 6, "", -1, void( * )( math::vector_t, math::vector_t, clr_t, float, bool, bool ) );

	for ( size_t idx{ }; idx < m_shots.size( ) && !m_shots.empty( ); idx++ )
	{
		auto& tr = m_shots[ idx ];

		float delta = unity::c_time::get_time( ) - tr.m_time;
		if ( delta > 1.0f || std::abs( delta ) > 5.f )
			m_shots.erase( m_shots.begin( ) + idx );
	}

	if ( !m_shots.empty( ) )
	{
		for ( auto& shot : m_shots )
		{
			auto delta = unity::c_time::get_time( ) - shot.m_time;
			auto col = clr_t::from_hsb( delta, 1.0f, 1.0f );
			col.a = 1;

			ddraw_line_fn( shot.m_start, shot.m_end, col, 0.5f, false, true );
		}
	}
}

auto sapphire::features::c_visuals::render_player( rust::classes::c_base_player* player ) -> void
{
	// hacky way to get our root transform for box
	auto clr = clr_t{ 153, 148, 171, 240 };

	if ( player == sapphire::features::aimbot::object.m_aim_target )
	{
		clr = { 253, 117, 135, 255 };
	}

	SAPPHIRE_METHOD( get_transform_fn, "UnityEngine::Component.get_transform()", 0, "", -1, rust::classes::c_transform * ( * )( void* ) );


	auto base_pos = unity::screen_transform( get_transform_fn( player )->position( ) );
	auto head_pos = unity::screen_transform( player->get_bone_transform( 48 )->position( ) );
	auto standing_pos = unity::screen_transform( get_transform_fn( player )->position( ) + math::vector_t{ 0, 1.9f, 0, } );

	if ( base_pos.is_empty( ) || head_pos.is_empty( ) || standing_pos.is_empty( ) )
		return;

	const auto box_width = ( base_pos.y - standing_pos.y ) * 0.45f;
	const auto box_height = ( base_pos.y - head_pos.y ) * 1.15f;

	const auto box = box_t{ { base_pos.x - box_width * 0.5f, base_pos.y - box_height  }, { box_width, box_height } };

	sapphire::gl_rendering::box( { box.pos.x - 1, box.pos.y - 1 }, { box.size.x + 2, box.size.y + 2 }, { 0, 0, 0, 255 } );
	sapphire::gl_rendering::box( { box.pos.x, box.pos.y }, { box.size.x, box.size.y }, clr );
	sapphire::gl_rendering::box( { box.pos.x + 1, box.pos.y + 1 }, { box.size.x - 2, box.size.y - 2 }, { 0, 0, 0, 255 } );

	const auto half = ( box.size.x ) / 2;

	const auto player_name = player->name( );

	if ( player_name )
		sapphire::rendering::draw_string( { box.pos.x - 100.f, box.pos.y - 18.f, ( half * 2 ) + 200.f, 20 }, player_name, { 255, 255, 255, 255 }, true, false, true );

	if ( const auto player_weapon = player->weapon( ) )
	{
		if ( const auto weapon_name = player->weapon( )->name( ) )
			sapphire::rendering::draw_string( { box.pos.x - 100.f, box.pos.y + box.size.y - 1.f, ( half * 2 ) + 200.f, 20 }, weapon_name, { 255, 255, 255, 255 }, true, false, true );
	}

	const auto npc = player->npc( );
	const auto health = box_t{ { box.pos.x - 5, box.pos.y + ( box.size.y - box.size.y * ( player->_health( ) ) / ( npc ? player->_maxHealth( ) : 100 ) ) }, { 2, box.size.y * ( player->_health( ) ) / ( npc ? player->_maxHealth( ) : 100 ) } };
	const auto health_bg = box_t{ { box.pos.x - 5, box.pos.y }, { 2, box.size.y } };

	sapphire::rendering::draw_filled_rect( { health_bg.pos.x, health_bg.pos.y - 1,  health_bg.size.x + 1, health_bg.size.y + 2 }, clr_t( 0, 0, 0, 240 ) );
	sapphire::rendering::draw_filled_rect( { health.pos.x + 1, health.pos.y, health.size.x - 1, health.size.y }, clr_t( 135, 186, 49, 240 ) );
}

bool wants_to_apply_chams = true;

auto sapphire::features::c_visuals::hand_chams( ) -> void 
{

}

auto sapphire::features::c_visuals::chams( rust::classes::c_base_player* player ) -> void
{
	SAPPHIRE_FIELD( "PlayerModel", _multiMesh );
	const auto multiMesh = *reinterpret_cast< std::uintptr_t* >( player->playerModel( ) + _multiMesh );

	if ( !multiMesh )
		return;

	SAPPHIRE_METHOD( get_renderers_fn, "SkinnedMultiMesh.get_Renderers()", 0, "", -1, managed_system::list<uintptr_t> * ( * )( std::uintptr_t ) );
	auto renderers = get_renderers_fn( multiMesh );

	for ( size_t idx{ 0 }; idx < renderers->size( ); idx++ )
	{
		auto renderer = renderers->value( idx );

		if ( renderer )
		{
			SAPPHIRE_ICALL( set_shader_fn, "UnityEngine.Material::set_shader()", void( * )( std::uintptr_t, std::uintptr_t ) );
			SAPPHIRE_ICALL( load_asset_fn, "UnityEngine.AssetBundle::LoadAsset_Internal(System.String,System.Type)", std::uintptr_t( * )( std::uintptr_t, managed_system::string, std::uintptr_t ) );
			SAPPHIRE_METHOD( set_color_fn, "UnityEngine::Material.SetColor()", 2, "", -1, void( * )( std::uintptr_t, managed_system::string, clr_t ) );
			SAPPHIRE_METHOD( get_material_fn, "UnityEngine::Renderer.get_material()", -1, "", -1, std::uintptr_t( * )( std::uintptr_t ) );
			SAPPHIRE_METHOD( set_float_fn, "UnityEngine::Material.SetFloat()", 2, "", -1, void( * )( std::uintptr_t, managed_system::string, float ) );

			const auto material = get_material_fn( renderer );
			if ( !material )
				return;

			if ( !shader )
				shader = load_asset_fn( unity::asset_bundle, L"chams.shader", il2cpp_lib::type_object( "UnityEngine", "Shader" ) );

			set_shader_fn( material, shader );
			set_color_fn( material, L"_ColorBehind", { 167, 98, 209, 255 } );
			set_color_fn( material, L"_ColorVisible", { 182, 3, 252, 255 } );
		}
	}
}