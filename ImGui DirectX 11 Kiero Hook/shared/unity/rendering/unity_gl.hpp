#pragma once
#include "../../../rust/include.hpp"

// better performance over draw texture and such, also can be rendered in 3d space. 
// can be called in update as well instead of ongui. 
// maybe run different parts of esp across two hooks? 

namespace sapphire
{
	namespace gl_rendering
	{
		class c_gl {
		public:
			static auto push( ) -> void {
				SAPPHIRE_METHOD( push_fn, "UnityEngine::GL.PushMatrix()", 0, "", -1, void( * )( void ) );
				return push_fn( );
			}

			static auto pop( ) -> void {
				SAPPHIRE_METHOD( pop_fn, "UnityEngine::GL.PopMatrix()", 0, "", -1, void( * )( void ) );
				return pop_fn( );
			}

			static auto begin( int mode ) -> void {
				SAPPHIRE_METHOD( begin_fn, "UnityEngine::GL.Begin()", 1, "", -1, void( * )( int ) );
				return begin_fn( mode );
			}

			static auto end( ) -> void {
				SAPPHIRE_METHOD( end_fn, "UnityEngine::GL.End()", 0, "", -1, void( * )( void ) );
				return end_fn( );
			}

			static auto color( clr_t clr ) -> void {
				SAPPHIRE_METHOD( color_fn, "UnityEngine::GL.Color()", 1, "", -1, void( * )( clr_t ) );
				return color_fn( clr );
			}

			static auto vertex( float x, float y, float z ) -> void {
				SAPPHIRE_METHOD( vertex_fn, "UnityEngine::GL.Vertex3()", -1, "", -1, void( * )( float, float, float ) );
				return vertex_fn( x, y, z );
			}
		};

		inline std::uintptr_t draw_material;

		enum primitive_mode : int {
			lines = 1,
			line_strip,
			triangles = 4,
			triangle_strip,
			quads = 7
		};

		inline auto gl_start( int mode ) -> void
		{
			if ( !draw_material )
				return;

			SAPPHIRE_METHOD( set_pass_fn, "UnityEngine::Material.SetPass()", 1, "", -1, bool( * )( std::uintptr_t, int ) );

			set_pass_fn( draw_material, 0 );
			c_gl::push( );
			c_gl::begin( mode );
		}

		inline auto gl_end( ) -> void
		{
			c_gl::end( );
			c_gl::pop( );
		}


		inline auto line( math::vector_2d_t start, math::vector_2d_t end, clr_t col ) -> void
		{
			gl_start( sapphire::gl_rendering::primitive_mode::lines );

			c_gl::color( col );
			c_gl::vertex( start.x, start.y, 0 );
			c_gl::vertex( end.x, end.y, 0 );

			gl_end( );
		}

		inline auto box( math::vector_2d_t pos, math::vector_2d_t size, clr_t col ) -> void
		{
			gl_start( sapphire::gl_rendering::primitive_mode::line_strip );

			// note - chloe; my attempt at trying to fix the opengl diamond exit rule. seems to work alright someone smarter than me can fix.
			c_gl::color( col );
			c_gl::vertex( pos.x + 0.5f, pos.y + 0.5f, 0 );
			c_gl::vertex( pos.x + size.x + 0.5f, pos.y + 0.5f, 0 );
			c_gl::vertex( pos.x + size.x + 0.5f, pos.y + size.y + 0.5f, 0 );
			c_gl::vertex( pos.x + 0.5f, pos.y + size.y + 0.5f, 0 );
			c_gl::vertex( pos.x + 0.5f, pos.y + 0.5f, 0 );

			gl_end( );
		}
	}
}