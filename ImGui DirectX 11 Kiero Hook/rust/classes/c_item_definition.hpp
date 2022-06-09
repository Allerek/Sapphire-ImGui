#pragma once
#include "include.hpp"

class rust::classes::c_item_definition : public unity::c_component
{
public:
	DECLARE_MEMBER( std::uint32_t, "ItemDefinition", category );
	DECLARE_MEMBER( managed_system::string*, "ItemDefinition", displayName );
	DECLARE_MEMBER( int, "ItemDefinition", itemid );
	

};