#pragma once
#include <cstdint>
#include <intrin.h>
#undef min
#undef max
#define max( a, b ) ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#define min( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )

namespace utl // small crt replacements. maybe use minicrt? not sure if worth it for 3 functions.
{
	namespace crt 
	{
		namespace string 
		{
			inline unsigned int wcslen( const wchar_t* str )
			{
				int counter = 0;
				if ( !str )
					return 0;
				for ( ; *str != '\0'; ++str )
					++counter;
				return counter;
			}

			inline int wcsicmp_insensitive( const wchar_t* cs, const wchar_t* ct )
			{
				const auto len = wcslen( cs );
				if ( len != wcslen( ct ) )
					return false;

				for ( size_t i = 0; i < len; i++ )
					if ( ( cs[ i ] | L' ' ) != ( ct[ i ] | L' ' ) )
						return false;

				return true;
			}

			inline bool strcmp( const char* a, const char* b ) {
				if ( ( uintptr_t )a == 0x00000000ffffffff || ( uintptr_t )b == 0x00000000ffffffff )
					return false;
				if ( ( uintptr_t )a == 0x000000000000007d || ( uintptr_t )b == 0x000000000000007d )
					return false;

				if ( !a || !b ) 
					return !a && !b;

				int ret = 0;
				unsigned char* p1 = ( unsigned char* )a;
				unsigned char* p2 = ( unsigned char* )b;
				while ( !( ret = *p1 - *p2 ) && *p2 )
					++p1, ++p2;

				return ret == 0;
			}
		}
	}
}