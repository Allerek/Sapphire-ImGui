#pragma once
#include <vector>

#include "../include.hpp"

namespace sapphire
{
	namespace features
	{
		class c_visuals
		{
		public:
			auto run( ) -> void;

			struct shot_record_t
			{
				float m_time;
				math::vector_t m_start, m_end;
				int m_damage;
			};

			std::vector< shot_record_t > m_shots;
		private:
			std::uintptr_t client_entities, shader;
			struct box_t
			{
				math::vector_2d_t pos;
				math::vector_2d_t size;
			};

			auto chams( rust::classes::c_base_player* player ) -> void;
			auto hand_chams( ) -> void;
			auto draw_tracers( ) -> void;
			auto render_player( rust::classes::c_base_player* player ) -> void;
		};

		namespace visuals
		{
			inline c_visuals object;
		}
	}
}