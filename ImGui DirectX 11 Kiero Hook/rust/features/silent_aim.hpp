#pragma once
#include "../include.hpp"
#include "../../sapphire/globals.hpp"

namespace sapphire
{
	namespace features
	{
		class c_silent_aim
		{
		public:
			auto find_aimbot_target( ) -> bool;
			auto do_manipulation( ) -> void;
			auto on_projectile_shoot_rpc( std::uint64_t instance ) -> void;
			math::vector_t m_view_offset;
			rust::classes::c_base_player* m_aim_target{ 0 };
		private:
			auto view_offset_visible( math::vector_t pos, math::vector_t offset ) -> void;
			std::uintptr_t client_entities{ 0 };
			math::vector_t m_aim_angle{ };
		};

		namespace aimbot
		{
			inline c_silent_aim object;
		}
	}
}