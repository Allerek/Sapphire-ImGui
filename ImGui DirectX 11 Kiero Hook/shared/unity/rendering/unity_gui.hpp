#pragma once
#include "../../../rust/include.hpp"
#include "unity_gl.hpp"

namespace sapphire
{
	namespace rendering
	{
		inline std::uintptr_t label_style, skin, white_texture;

		inline auto init( ) -> void
		{
			SAPPHIRE_METHOD( get_skin_fn, "UnityEngine::GUI.get_skin()", 0, "", -1, std::uintptr_t( * )( ) );
			SAPPHIRE_METHOD( get_white_texture_fn, "UnityEngine::Texture2D.get_whiteTexture()", 0, "", -1, std::uintptr_t( * )( ) );

			if ( !skin || !label_style )
			{
				skin = get_skin_fn( );
				label_style = *reinterpret_cast< std::uintptr_t* >( skin + 0x38 );
			}

			white_texture = get_white_texture_fn( );

			SAPPHIRE_METHOD( set_font_size_fn, "UnityEngine::GUIStyle.set_fontSize()", -1, "", -1, void( * )( std::uintptr_t, int ) );
			SAPPHIRE_METHOD( set_color_fn, "UnityEngine::GUI.set_color()", -1, "", -1, void( * )( clr_t ) );
			SAPPHIRE_METHOD( set_alignment_fn, "UnityEngine::GUIStyle.set_alignment()", -1, "", -1, void( * )( std::uintptr_t, std::uintptr_t ) );
			SAPPHIRE_ICALL( load_asset_fn, "UnityEngine.AssetBundle::LoadAsset_Internal(System.String,System.Type)", std::uintptr_t( * )( std::uintptr_t, managed_system::string, std::uintptr_t ) );
			SAPPHIRE_ICALL( load_from_file_fn, "UnityEngine.AssetBundle::LoadFromFile_Internal(System.String,System.UInt32,System.UInt64)", std::uintptr_t( * )( managed_system::string, std::uint32_t, std::uint64_t ) );

			if ( !unity::asset_bundle ) // todo; use UnityWebRequestAssetBundle.GetAssetBundle to stream bundle from the github repo.
				unity::asset_bundle = load_from_file_fn( L"C:\\sapphire\\assets.saph", 0, 0 );

			const auto set_font = [ & ]( managed_system::string font_name, int size ) {
				static auto font = load_asset_fn( unity::asset_bundle, font_name, il2cpp_lib::type_object( "UnityEngine", "Font" ) );
				*reinterpret_cast< std::uintptr_t* >( skin + 0x18 ) = font; // lol.
				set_font_size_fn( label_style, size );
			};

			set_font( L"tahoma.ttf", 8 );

			if ( !sapphire::gl_rendering::draw_material )
			{
				sapphire::gl_rendering::draw_material = il2cpp_lib::object_new( il2cpp_lib::find_class( "Material", "UnityEngine" ) );

				SAPPHIRE_METHOD( create_with_shader_fn, "UnityEngine::Material.CreateWithShader()", 2, "", -1, void( * )( std::uintptr_t, std::uintptr_t ) );
				SAPPHIRE_METHOD( set_int_fn, "UnityEngine::Material.SetInt()", -1, "", -1, void( * )( std::uintptr_t, managed_system::string, int ) );
				SAPPHIRE_METHOD( find_shader_fn, "UnityEngine::Shader.Find()", -1, "", -1, std::uintptr_t( * )( managed_system::string ) );

				create_with_shader_fn( sapphire::gl_rendering::draw_material, find_shader_fn( L"Hidden/Internal-Colored" ) );

				set_int_fn( sapphire::gl_rendering::draw_material, L"_SrcBlend", 5 );
				set_int_fn( sapphire::gl_rendering::draw_material, L"_DstBlend", 10 );
				set_int_fn( sapphire::gl_rendering::draw_material, L"_Cull", 0 );
				set_int_fn( sapphire::gl_rendering::draw_material, L"_ZWrite", 0 );
			}

			set_alignment_fn( label_style, 0 );
			set_color_fn( { 1, 1, 1, 1 } );
		}

		inline auto draw_filled_rect( unity::rect_t pos, clr_t clr ) -> void
		{
			SAPPHIRE_METHOD( set_color_fn, "UnityEngine::GUI.set_color()", -1, "", -1, void( * )( clr_t ) );
			SAPPHIRE_METHOD( draw_texture_fn, "UnityEngine::GUI.DrawTexture()", 2, "", -1, void ( * )( unity::rect_t, std::uintptr_t ) );

			set_color_fn( clr );
			draw_texture_fn( pos, white_texture );
		}

		inline auto draw_string( unity::rect_t pos, managed_system::string text, clr_t clr, bool drop_shadow, bool outline, bool centered ) -> void
		{
			if ( !label_style ) {
				return;
			}

			SAPPHIRE_METHOD( gui_content_temp_fn, "UnityEngine::GUIContent.Temp()", 1, "t", 1, std::uintptr_t( * )( managed_system::string* ) );
			SAPPHIRE_METHOD( set_color_fn, "UnityEngine::GUI.set_color()", -1, "", -1, void( * )( clr_t ) );
			SAPPHIRE_METHOD( label_fn, "UnityEngine::GUI.Label()", 3, "content", 2, void( * )( unity::rect_t, std::uintptr_t, std::uintptr_t ) );
			SAPPHIRE_METHOD( set_alignment_fn, "UnityEngine::GUIStyle.set_alignment()", -1, "", -1, void( * )( std::uintptr_t, std::uintptr_t ) );

			const auto content = gui_content_temp_fn( &text );

			if ( centered )
				set_alignment_fn( label_style, 0x4 );
			else
				set_alignment_fn( label_style, 0x0 );

			if ( outline ) 
			{
				set_color_fn( { 0, 0, 0, ( clr.a * 255 ) } );
				label_fn( { pos.x, pos.y + 1, pos.w, pos.h }, content, label_style );
				label_fn( { pos.x, pos.y - 1, pos.w, pos.h }, content, label_style );
				label_fn( { pos.x - 1, pos.y, pos.w, pos.h }, content, label_style );
				label_fn( { pos.x + 1, pos.y, pos.w, pos.h }, content, label_style );
			}

			if ( drop_shadow ) 
			{
				set_color_fn( { 0, 0, 0, ( clr.a * 255 ) } );
				label_fn( { pos.x + 1, pos.y + 1, pos.w, pos.h }, content, label_style );
			}

			set_color_fn( clr );
			label_fn( pos, content, label_style );
		}
	}
}
