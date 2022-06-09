#pragma once
#include "../../shared/utl/il2cpp_lib.hpp"

namespace impl
{
	// note - chloe; we cannot call the original of these functions however most of them OnEnable/Start/Update/OnGUI are empty so we do not need to add saving of original.
	class swap_ptr_c
	{
	private:
		std::uintptr_t m_original{ 0 };
	public:
		auto setup( std::string name, void* our_function, int arg_count = -1 ) -> void {
			auto il2cpp_method = il2cpp_lib::method( name, arg_count, "" );
			m_original = *reinterpret_cast< std::uintptr_t* >( il2cpp_method );
			*reinterpret_cast< void** >( il2cpp_method ) = our_function;
		}

		auto restore( void* our_function ) -> void {
			*reinterpret_cast< void** >( our_function ) = &m_original;
		}
	};

	// note - chloe; virtual functions inside class will still have same index between restarts. Maybe i will add hook by specific index.
	class virt_hook_c
	{
	private:
		std::uintptr_t m_original{ 0 };
	public:
		auto setup( std::string name, void* our_function ) -> void
		{
			const auto st = il2cpp_lib::sanitize_method_name( name );

			const auto search_method = *reinterpret_cast< std::uintptr_t* >( il2cpp_lib::method( name, -1, "" ) );
			const auto class_base = il2cpp_lib::find_class( st.kl.c_str( ), st.ns.c_str( ) );

			if ( search_method == std::uintptr_t( our_function ) )
				return;

			for ( auto idx{ class_base }; idx <= class_base + 0x1500; idx += 0x1 ) {
				const auto addr = *reinterpret_cast< std::uintptr_t* >( idx );
				if ( addr == search_method ) {
					m_original = search_method;
					*reinterpret_cast< std::uintptr_t* >( idx ) = std::uintptr_t( our_function );
				}
			}
		}

		template< typename type >
		auto get_original( ) -> type { return reinterpret_cast< type >( m_original ); }

		auto restore( void* our_function ) -> void {
			*reinterpret_cast< void** >( our_function ) = &m_original;
		}
	};
}