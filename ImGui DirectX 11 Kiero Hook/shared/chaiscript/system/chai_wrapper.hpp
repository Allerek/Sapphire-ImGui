#pragma once
#include "../include/chaiscript.hpp"
#include "../wrappers/chai_base_player.hpp"
#include "../wrappers/chai_renderer.hpp"
#include "../../../rust/include.hpp"

class c_chai_wrapper
{
public:
	chaiscript::ChaiScript* chai;

	auto chai_load( const std::string& str ) -> void;
	auto chai_loadscript( const std::string& str ) -> void;
	auto chai_init( ) -> void;

	bool script_on_client_input{ false };
	bool script_on_gui{ false };

	auto chai_on_client_input_callback( rust::classes::c_base_player* base_player, std::uintptr_t* input_state ) -> void;
	auto chai_on_gui_callback( ) -> void;
};

namespace chai_instance_manager {
	inline std::vector<c_chai_wrapper> instances;
}