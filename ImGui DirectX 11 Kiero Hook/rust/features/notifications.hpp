#pragma once
#include <vector>
#include <memory>

#include "../include.hpp"
#include "../../shared/unity/rendering/unity_gui.hpp"

namespace sapphire
{
	namespace features
	{
		class c_notify_text
		{
		public:
			managed_system::string m_text;
			clr_t m_color;
			float m_time;

		public:
			c_notify_text( const managed_system::string text, clr_t color, float time ) : m_text( text ), m_color( color ), m_time( time ) { }
		};

		class c_notify
		{
		private:
			std::vector< std::shared_ptr< sapphire::features::c_notify_text > > m_notify_text;
		public:
			c_notify( ) : m_notify_text{} {}

			auto push( managed_system::string text, float timestamp, clr_t color = clr_t( 255, 255, 255, 255 ) ) -> void {
				m_notify_text.push_back( std::make_shared< sapphire::features::c_notify_text >( text, color, timestamp ) );
			}

			auto run( ) -> void {
				float x{ 8 }, y{ 5 }, size{ 11 };

				for ( size_t idx{ }; idx < m_notify_text.size( ) && !m_notify_text.empty( ); idx++ )
				{
					auto& notify = m_notify_text[ idx ];

					if ( fabs( unity::c_time::get_time( ) - notify->m_time ) > 5.f )
					{
						m_notify_text.erase( m_notify_text.begin( ) + idx );

						if ( !m_notify_text.size( ) )
							return;
					}
				}

				if ( m_notify_text.empty( ) )
					return;

				for ( size_t i{}; i < m_notify_text.size( ); ++i ) {
					auto& notify = m_notify_text[ i ];

					float time_delta = fabs( unity::c_time::get_time( ) - notify->m_time );

					clr_t color = notify->m_color;

					float alpha = 255.f;
					float height = 5 + ( 16 * i );

					if ( time_delta > 5.f - 0.2f )
						alpha = ( 255 - ( ( ( time_delta - ( 5.f - 0.2f ) ) / 0.2f ) * 255.f ) );

					if ( time_delta < 0.3f )
						alpha = ( ( time_delta / 0.3f ) * 255.f );

					float width = 8;

					if ( time_delta < 0.3f )
						width = ( time_delta / 0.3f ) * static_cast< float >( 8 );

					if ( time_delta > 5.f - 0.2f )
						width = -( ( time_delta / 0.3f ) * static_cast< float >( 8 ) );

					color.a = alpha / 255.f;

					sapphire::rendering::draw_string( { width, height, 300, 20 }, notify->m_text, color, true, false, false );
				}
			}
		};

		namespace notifcations
		{
			inline c_notify object;
		}
	}
}