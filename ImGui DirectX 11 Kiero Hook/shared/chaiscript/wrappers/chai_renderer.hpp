#pragma once
#include "../../unity/rendering/unity_gl.hpp"
#include "../../unity/rendering/unity_gui.hpp"

class chai_wrapper_renderer
{
public:
	chai_wrapper_renderer( ) { }

	static void draw_text( float x, float y, const std::string str, clr_t color, bool drop_shadow, bool outline, bool centered )
	{
		const auto wstring = std::wstring( str.begin( ), str.end( ) );
		sapphire::rendering::draw_string( { x , y, 300, 50 }, wstring.c_str( ), color, drop_shadow, outline, centered );
	}

	static void draw_rect( float x, float y, float w, float h, clr_t color )
	{
		sapphire::gl_rendering::box( { x, y }, { w, h }, color );
	}

	static void draw_filled_rect( float x, float y, float w, float h, clr_t color )
	{
		sapphire::rendering::draw_filled_rect( { x, y, w, h }, color );
	}

	static void draw_line( float x, float y, float x1, float y1, clr_t color )
	{
		sapphire::gl_rendering::line( { x, y }, { x1, y1 }, color );
	}

	static math::vector_t world_to_screen( math::vector_t position ) {

		auto vec = unity::screen_transform( position );
		if ( !vec.is_empty( ) )
			return vec;

		return math::vector_t( -9999, -9999, -9999 );
	}
};