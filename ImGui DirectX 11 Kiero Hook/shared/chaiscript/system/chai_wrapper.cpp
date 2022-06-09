#include "chai_wrapper.hpp"

void c_chai_wrapper::chai_load( const std::string& str ) 
{
	std::fstream file( str.c_str( ), std::fstream::in );
	std::string content = "";
	std::ifstream input_file = std::ifstream( str );

	if ( input_file.good( ) ) 
	{
		try 
		{
			std::getline( input_file, content, '\0' );
			chai_loadscript( content );
			input_file.close( );
		}
		catch ( const std::exception & ex ) 
		{
			(void)ex;
			input_file.close( );
			sapphire::features::notifcations::object.push( L"[chaiscript] failed to load script", unity::c_time::get_time( ) );
		}
	}
	else 
	{
		sapphire::features::notifcations::object.push( L"[chaiscript] failed to load script", unity::c_time::get_time( ) );
	}
}

void c_chai_wrapper::chai_loadscript( const std::string& str )
{
	if ( str.find( "on_client_input(base_player, input_state)" ) != std::string::npos ) 
		script_on_client_input = true; 

	if ( str.find( "on_gui()" ) != std::string::npos ) 
		script_on_gui = true; 

	try 
	{
		chai->eval( str );
	}
	catch ( const std::exception & e ) 
	{
		auto string = std::string( e.what( ) );
		sapphire::features::notifcations::object.push( std::wstring( string.begin( ), string.end( ) ).c_str( ), unity::c_time::get_time( ) );
	}
}

void c_chai_wrapper::chai_init( ) 
{
	chai = new chaiscript::ChaiScript( );

	chai->add( chaiscript::user_type<c_chai_wrapper>( ), "main" );
	chai->add( chaiscript::fun( &c_chai_wrapper::chai_load ), "load" );

	chai->add( chaiscript::user_type<chai_wrapper_renderer>( ), "renderer" );
	chai->add( chaiscript::fun( &chai_wrapper_renderer::draw_text ), "draw_text" );
	chai->add( chaiscript::fun( &chai_wrapper_renderer::draw_rect ), "draw_rect" );
	chai->add( chaiscript::fun( &chai_wrapper_renderer::draw_filled_rect ), "draw_filled_rect" );
	chai->add( chaiscript::fun( &chai_wrapper_renderer::draw_line ), "draw_line" );
	chai->add( chaiscript::fun( &chai_wrapper_renderer::world_to_screen ), "world_to_screen" );

	chai->add( chaiscript::user_type<c_chai_entity_list>( ), "entitylist" );
	chai->add( chaiscript::fun( &c_chai_entity_list::get_local_player ), "get_local_player" );
	chai->add( chaiscript::fun( &c_chai_entity_list::get_size ), "get_size" );
	chai->add( chaiscript::fun( &c_chai_entity_list::get_player ), "get_player" );
	chai->add( chaiscript::fun( &c_chai_entity_list::is_player ), "is_player" );

	chai->add( chaiscript::fun( &c_chai_entity_list::get_entity_list ), "get_entity_list" );

	chai->add( chaiscript::user_type<c_chai_base_player>( ), "player" );
	chai->add( chaiscript::fun( &c_chai_base_player::get_health ), "get_health" );
	chai->add( chaiscript::fun( &c_chai_base_player::is_alive ), "is_alive" );
	chai->add( chaiscript::fun( &c_chai_base_player::get_name ), "get_name" );
	chai->add( chaiscript::fun( &c_chai_base_player::set_flags ), "set_flags" );
	chai->add( chaiscript::fun( &c_chai_base_player::bone_position ), "get_bone_position" );

	chai->add( chaiscript::user_type<clr_t>( ), "color" );
	chai->add( chaiscript::constructor<clr_t( float, float, float )>( ), "color" );
	chai->add( chaiscript::constructor<clr_t( float, float, float, float )>( ), "color" );

	chai->add( chaiscript::user_type<math::vector_t>( ), "vec3_t" );
	chai->add( chaiscript::fun( &math::vector_t::x ), "x" );
	chai->add( chaiscript::fun( &math::vector_t::y ), "y" );
	chai->add( chaiscript::fun( &math::vector_t::z ), "z" );
}

void c_chai_wrapper::chai_on_client_input_callback( rust::classes::c_base_player* base_player, std::uintptr_t* input_state ) 
{
	try 
	{
		auto func = chai->eval< std::function< void( rust::classes::c_base_player*, std::uintptr_t* ) > >( "on_client_input" );
		return func( base_player, input_state );
	}
	catch ( const std::exception & e ) 
	{
		auto string = std::string( e.what( ) );
		sapphire::features::notifcations::object.push( std::wstring( string.begin( ), string.end( ) ).c_str( ), unity::c_time::get_time( ) );

		script_on_client_input = false;
	}
}

void c_chai_wrapper::chai_on_gui_callback( )
{
	try
	{
		auto func = chai->eval< std::function< void( void ) > >( "on_gui" );
		return func( );
	}
	catch ( const std::exception & e )
	{
		auto string = std::string( e.what( ) );
		sapphire::features::notifcations::object.push( std::wstring( string.begin( ), string.end( ) ).c_str( ), unity::c_time::get_time( ) );

		script_on_gui = false;
	}
}