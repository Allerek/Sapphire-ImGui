#pragma once
#include "../include.hpp"
#include "../../sapphire/globals.hpp"

namespace sapphire 
{
	namespace features 
	{
		class c_hitmarkers 
		{
		public:
			auto run( ) -> void;
			float m_hit_start{ 0 }, m_hit_end{ 0 };
		};

		namespace hitmarkers 
		{
			inline c_hitmarkers object;
		}
	}
}