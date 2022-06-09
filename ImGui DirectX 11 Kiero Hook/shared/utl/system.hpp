#pragma once
#include "sapphire_crt.hpp"

namespace managed_system 
{
	class string 
	{
	public:
		char zpad[ 0x10 ]{ };
		int size{ };
		wchar_t buffer[ 128 + 1 ];
	public:
		string( const wchar_t* st ) 
		{
			size = min( utl::crt::string::wcslen( st ), 128 );
			for ( int idx = 0; idx < size; idx++ ) 
			{
				buffer[ idx ] = st[ idx ];
			}
			buffer[ size ] = 0;
		}
	};

	template<typename type>
	class list 
	{
	public:
		type get( std::uint32_t idx ) 
		{
			const auto internal_list = reinterpret_cast< std::uintptr_t >( this + 0x20 );
			return *reinterpret_cast< type* >( internal_list + idx * sizeof( type ) );
		}

		type value( std::uint32_t idx ) 
		{
			const auto list = *reinterpret_cast< std::uintptr_t* >( this + 0x10 );
			const auto internal_list = list + 0x20;
			return *reinterpret_cast< type* >( internal_list + idx * sizeof( type ) );
		}

		auto size( ) -> const std::uint32_t { return *reinterpret_cast< std::uint32_t* >( this + 0x18 ); }
	};

	class list_dictionary 
	{
	public:
		template <typename type>
		auto value( ) -> type
		{
			auto list = *reinterpret_cast< std::uintptr_t* >( this + 0x10 );
			if ( !list )
				return {};

			auto value = *reinterpret_cast< type* >( list + 0x28 );
			if ( !value )
				return {};

			return value;
		}

		auto size( ) -> int
		{
			auto val = value< std::uintptr_t >( );
			if ( !val )
				return {};

			auto size = *reinterpret_cast< int* >( val + 0x10 );
			if ( !size )
				return {};

			return size;
		}

		template <typename type>
		auto buffer( ) -> type
		{
			auto val = value< std::uintptr_t >( );
			return *reinterpret_cast< std::uintptr_t* >( val + 0x18 );
		}
	};
}