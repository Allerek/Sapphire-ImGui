#pragma once
#include "../shared/utl/il2cpp_lib.hpp"
#include "../math/vector/vector.hpp"
#include "../math/matrix/matrix.hpp"
#include "../shared/unity/rendering/color.hpp"
#include "../shared/utl/system.hpp"

#include "../shared/unity/unity_sdk.hpp"

#define DECLARE_MEMBER( type, klass, name ) type & name( ) { SAPPHIRE_FIELD( klass, ##name ) return *reinterpret_cast< type* >( this + name ); }

#include "rust.hpp"

#include "classes/c_base_projectile.hpp"
#include "classes/c_item_mod_projectile.hpp"
#include "classes/c_projectile.hpp"
#include "classes/c_base_weapon.hpp"
#include "classes/c_base_player.hpp"
#include "classes/c_hit_info.hpp"
#include "classes/c_item_definition.hpp"
#include "classes/c_model_state.hpp"
#include "classes/c_networkable.hpp"
#include "classes/c_player_eyes.hpp"
#include "classes/c_transform.hpp"
#include "classes/c_walk_movement.hpp"

#include "features/silent_aim.hpp"
#include "features/visuals.hpp"
#include "features/notifications.hpp"
#include "features/hitmarkers.hpp"