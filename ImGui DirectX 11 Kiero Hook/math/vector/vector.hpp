#pragma once
#include <cmath>
#include <cfloat>
#undef sqrtf

namespace math 
{
	struct vector_2d_t
	{
		float x, y;

		vector_2d_t( ) {}
		vector_2d_t( float x, float y ) : x( x ), y( y ) {}

		vector_2d_t operator-( const vector_2d_t& vector ) const
		{
			return vector_2d_t( x - vector.x, y - vector.y );
		}

		vector_2d_t& operator+=( const vector_2d_t& vector )
		{
			x += vector.x;
			y += vector.y;

			return *this;
		}
		vector_2d_t& operator-=( const vector_2d_t& vector )
		{
			x -= vector.x;
			y -= vector.y;

			return *this;
		}

		static float sqrtf( float number )
		{
			long        i;
			float       x2, y;
			const float threehalfs = 1.5F;

			x2 = number * 0.5F;
			y = number;
			i = *( long* )&y; // floating point bit level hacking [sic]
			i = 0x5f3759df - ( i >> 1 ); // Newton's approximation
			y = *( float* )&i;
			y = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
			y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration

			return 1 / y;
		}

		inline float length( )
		{
			return sqrtf( ( x * x ) + ( y * y ) );
		}
	};

	struct vector_t
	{
		float x;
		float y;
		float z;

		vector_t( )
		{}

		vector_t( float x, float y, float z )
			: x( x ), y( y ), z( z )
		{}

		bool is_empty( ) { return x == 0 && y == 0 && z == 0; }

		static float sqrtf( float number )
		{
			long        i;
			float       x2, y;
			const float threehalfs = 1.5F;

			x2 = number * 0.5F;
			y = number;
			i = *( long* )&y; // floating point bit level hacking [sic]
			i = 0x5f3759df - ( i >> 1 ); // Newton's approximation
			y = *( float* )&i;
			y = y * ( threehalfs - ( x2 * y * y ) ); // 1st iteration
			y = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration

			return 1 / y;
		}

		auto length( ) -> float { return sqrtf( ( x * x ) + ( y * y ) + ( z * z ) ); }
		auto length_2d( ) -> float { return sqrtf( ( x * x ) + ( z * z ) ); }

		template<typename T>
		T get_remainder( T val, T min, T max )
		{
			while ( val > max )
				val -= max * 2;
			while ( val < min )
				val += max * 2;
			return val;
		}

		auto clamp( ) -> vector_t
		{
			constexpr auto yaw_limit = static_cast< float >( 180 );
			constexpr auto pitch_limit = static_cast< float >( 90 );

			this->y = get_remainder( this->y, -yaw_limit, yaw_limit );
			this->x = get_remainder( this->x, -pitch_limit, pitch_limit );

			if ( this->x != this->x )
				this->x = 0;
			if ( this->y != this->y )
				this->y = 0;
			if ( this->z != this->z )
				this->z = 0;

			return *this;
		}

		auto dot( const vector_t& vector ) -> float { return x * vector.x + y * vector.y + z * vector.z; }

//#undef powf
#define _powf(n) (n)*(n)

		auto distance3d( const vector_t& vector ) -> float { return sqrtf( _powf( x - vector.x ) + _powf( y - vector.y ) + _powf( z - vector.z ) ); }
		auto distance( const vector_t& vector ) -> float { return sqrtf( ( x - vector.x ) * ( x - vector.x ) + ( y - vector.y ) * ( y - vector.y ) + ( z - vector.z ) * ( z - vector.z ) ); }

		vector_t normalized( ) {
			float len = length( );
			return vector_t( x / len, y / len, z / len );
		}

		vector_t& operator+=( const vector_t& vector )
		{
			x += vector.x;
			y += vector.y;
			z += vector.z;

			return *this;
		}

		vector_t& operator-=( const vector_t& vector )
		{
			x -= vector.x;
			y -= vector.y;
			z -= vector.z;

			return *this;
		}

		vector_t& operator*=( float number )
		{
			x *= number;
			y *= number;
			z *= number;

			return *this;
		}

		vector_t& operator/=( float number )
		{
			x /= number;
			y /= number;
			z /= number;

			return *this;
		}

		bool operator==( const vector_t& vector ) const
		{
			return x == vector.x && y == vector.y && z == vector.z;
		}

		bool operator!=( const vector_t& vector ) const
		{
			return x != vector.x || y != vector.y || z != vector.z;
		}

		vector_t operator+( const vector_t& vector ) const
		{
			return vector_t( x + vector.x, y + vector.y, z + vector.z );
		}

		vector_t operator-( const vector_t& vector ) const
		{
			return vector_t( x - vector.x, y - vector.y, z - vector.z );
		}

		vector_t operator-( ) const
		{
			return vector_t( -x, -y, -z );
		}

		vector_t operator*( float number ) const
		{
			return vector_t( x * number, y * number, z * number );
		}

		vector_t operator/( float number ) const
		{
			return vector_t( x / number, y / number, z / number );
		}
	};

	class vec4_t
	{
	public:
		float x;
		float y;
		float z;
		float w;

		vec4_t( ) {}
		vec4_t( float x, float y, float z, float w ) : x( x ), y( y ), z( z ), w( w ) {}

		auto dot( const vec4_t& vector ) -> float { return x * vector.x + y * vector.y + z * vector.z + w * vector.w; }
		auto distance( const vec4_t& vector ) -> float { return sqrtf( ( x - vector.x ) * ( x - vector.x ) + ( y - vector.y ) * ( y - vector.y ) + ( z - vector.z ) * ( z - vector.z ) + ( w - vector.w ) * ( w - vector.w ) ); }

		bool operator==( const vec4_t& vector ) const
		{
			return x == vector.x && y == vector.y && z == vector.z && w == vector.w;
		}

		bool operator!=( const vec4_t& vector ) const
		{
			return x != vector.x || y != vector.y || z != vector.z || w != vector.w;
		}

		vec4_t operator+( const vec4_t& vector ) const
		{
			return vec4_t( x + vector.x, y + vector.y, z + vector.z, w + vector.w );
		}

		vec4_t operator-( const vec4_t& vector ) const
		{
			return vec4_t( x - vector.x, y - vector.y, z - vector.z, w - vector.w );
		}

		vec4_t operator-( ) const
		{
			return vec4_t( -x, -y, -z, -w );
		}

		vec4_t operator*( float number ) const
		{
			return vec4_t( x * number, y * number, z * number, w * number );
		}

		vec4_t operator/( float number ) const
		{
			return vec4_t( x / number, y / number, z / number, w / number );
		}

		vec4_t& operator+=( const vec4_t& vector )
		{
			x += vector.x;
			y += vector.y;
			z += vector.z;
			w += vector.w;
			return *this;
		}

		vec4_t& operator-=( const vec4_t& vector )
		{
			x -= vector.x;
			y -= vector.y;
			z -= vector.z;
			w -= vector.w;
			return *this;
		}

		vec4_t& operator*=( float number )
		{
			x *= number;
			y *= number;
			z *= number;
			w *= number;
			return *this;
		}

		vec4_t& operator/=( float number )
		{
			x /= number;
			y /= number;
			z /= number;
			w /= number;
			return *this;
		}
	};
}