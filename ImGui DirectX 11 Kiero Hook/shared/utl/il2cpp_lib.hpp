#pragma once
#include "sapphire_importer.hpp"
#include "pattern.hpp"

#include <string>

#pragma warning( disable : 26451 )

#define CREATE_TYPE( name, args ) using il2cpp_##name = args; inline il2cpp_##name name;
#define ASSIGN_TYPE( name ) name = SAPPHIRE_TYPE( L"GameAssembly.dll", il2cpp_##name );

#define SAPPHIRE_FIELD( klass, name ) static std::uintptr_t name = il2cpp_lib::field( klass, #name, true );
#define SAPPHIRE_TYPE_OBJECT( ns, name ) il2cpp_lib::type_object( ns, name )
#define SAPPHIRE_ICALL( fn_name, name, args ) static auto fn_name = reinterpret_cast< args >( il2cpp_lib::resolve_icall( name ) );

#define SAPPHIRE_METHOD( fn_name, name, arg_count, arg_name, selected_arg, args ) \
	static auto fn_name = reinterpret_cast< args >( *reinterpret_cast< std::uintptr_t* >( il2cpp_lib::method( name, arg_count, arg_name, selected_arg ) ) )

namespace il2cpp_lib
{
	struct sanitized_method_t {
		std::string ns, kl, mt;
	};

	CREATE_TYPE( domain_get, std::uintptr_t( * )( ) );
	CREATE_TYPE( class_get_methods, std::uintptr_t( * )( std::uintptr_t, std::uintptr_t* ) );
	CREATE_TYPE( method_get_param_count, int ( * )( std::uintptr_t ) );
	CREATE_TYPE( assembly_get_image, std::uintptr_t( * )( std::uintptr_t ) );
	CREATE_TYPE( domain_get_assemblies, std::uintptr_t* ( * )( void* domain, std::uintptr_t* size ) );
	CREATE_TYPE( object_new, std::uintptr_t( * )( std::uintptr_t ) );
	CREATE_TYPE( class_from_name, std::uintptr_t( * )( std::uintptr_t, const char*, const char* ) );
	CREATE_TYPE( resolve_icall, std::uintptr_t( * )( const char* ) );
	CREATE_TYPE( field_static_get_value, std::uintptr_t( * )( std::uintptr_t, std::uintptr_t* ) );
	CREATE_TYPE( class_get_fields, std::uintptr_t( * )( std::uintptr_t, std::uintptr_t* ) );
	CREATE_TYPE( field_get_offset, std::uintptr_t( * )( std::uintptr_t ) );
	CREATE_TYPE( string_new_wrapper, std::uintptr_t( * )( const char* ) );
	CREATE_TYPE( class_get_type, std::uintptr_t( * )( std::uintptr_t ) );
	CREATE_TYPE( type_get_object, std::uintptr_t( * )( std::uintptr_t ) );
	CREATE_TYPE( method_get_return_type, const char* ( * )( std::uintptr_t ) );

	inline void init( ) {
		ASSIGN_TYPE( domain_get );
		ASSIGN_TYPE( class_get_methods );
		ASSIGN_TYPE( method_get_param_count );
		ASSIGN_TYPE( assembly_get_image );
		ASSIGN_TYPE( domain_get_assemblies );
		ASSIGN_TYPE( object_new );
		ASSIGN_TYPE( class_from_name );
		ASSIGN_TYPE( resolve_icall );
		ASSIGN_TYPE( field_static_get_value );
		ASSIGN_TYPE( class_get_fields );
		ASSIGN_TYPE( field_get_offset );
		ASSIGN_TYPE( string_new_wrapper );
		ASSIGN_TYPE( class_get_type );
		ASSIGN_TYPE( type_get_object );
		ASSIGN_TYPE( method_get_return_type );
	}

	inline auto find_class( const char* name, const char* name_space = "" ) -> const std::uintptr_t {

		const auto domain = domain_get( );

		std::uintptr_t assembly_count{ 0 };
		std::uintptr_t* assemblies;
		assemblies = domain_get_assemblies( ( void* )domain, &assembly_count );

		for ( size_t idx{ 0 }; idx < assembly_count; idx++ ) {
			const auto img = assembly_get_image( assemblies[ idx ] );
			const auto kl = class_from_name( img, name_space, name );
			if ( !kl )
				continue;

			return kl;
		}

		return 0;
	}

	inline auto type_object( const char* name_space, const char* name ) -> const uintptr_t
	{
		auto klass = find_class( name, name_space );
		return type_get_object( class_get_type( klass ) );
	}

	// im sorry this is messy but its a lot cleaner in the end.
	inline auto sanitize_method_name( const std::string name ) -> sanitized_method_t {
		std::string ns, kl, method_name;

		if ( auto ns_break = name.find( "::" ) )
			ns = name.substr( 0, ns_break );

		if ( name.find( "::" ) == std::string::npos )
			ns = "";

		if ( name.find( "::" ) ) {
			kl = name.substr( name.find( "::", 2 ) + 2 );
			kl = kl.substr( 0, kl.find( ".", 0 ) );
		}

		if ( name.find( "::" ) == std::string::npos )
			kl = name.substr( 0, name.find( ".", 0 ) );

		method_name = name.substr( name.find( ".", 0 ) + 1 );

		if ( method_name.find( "()" ) )
			method_name.erase( method_name.find( "()" ), 2 );

		return { ns, kl, method_name };
	}

	inline auto method( const std::string name, int argument_number = -1, const char* arg_name = "", int selected_argument = -1 ) -> const std::uintptr_t {
		std::uintptr_t iter{};

		const auto st = sanitize_method_name( name );
		const auto klass = find_class( st.kl.c_str( ), st.ns.c_str( ) );

		while ( auto method = class_get_methods( klass, &iter ) ) {

			const auto fn_name = *reinterpret_cast< const char** >( method + 0x10 );

			if ( utl::crt::string::strcmp( fn_name, st.mt.c_str( ) ) ) {
				auto method_count = method_get_param_count( method );
				if ( argument_number >= 0 && method_count != argument_number )
					continue;

				if ( selected_argument >= 0 && arg_name ) {
					auto args = *reinterpret_cast< std::uintptr_t* >( method + 0x28 );

					if ( selected_argument > method_count )
						continue;

					char* argname;

					if ( method_count > 0 )
						argname = *reinterpret_cast< char** >( args + ( selected_argument - 1 ) * 0x18 );
					else
						argname = ( char* )( "-" );

					if ( !argname || !utl::crt::string::strcmp( argname, arg_name ) )
						continue;
				}

				return method;
			}
		}

		return {};
	}

	inline auto field_internal( std::uintptr_t klass, const char* field_name, bool offset = true ) -> const uintptr_t {
		auto iterator = std::uintptr_t( );

		while ( auto il2cpp_field = class_get_fields( klass, &iterator ) ) {

			const auto name = reinterpret_cast< const char* >( *reinterpret_cast< std::uintptr_t* >( il2cpp_field ) );
			if ( !name ) {
				break;
			}

			if ( !utl::crt::string::strcmp( name, field_name ) ) {
				continue;
			}

			if ( !offset ) {
				return il2cpp_field;
			}

			return field_get_offset( il2cpp_field );
		}
		return 0;
	}

	inline auto field( const char* kl, const char* name, bool offset = true ) -> const std::uintptr_t {

		const auto klass = find_class( kl );

		if ( offset ) {
			return field_internal( klass, name );
		}

		const auto _field = field_internal( klass, name, false );

		auto ret = std::uintptr_t( );
		field_static_get_value( _field, &ret );
		return ret;
	}
}