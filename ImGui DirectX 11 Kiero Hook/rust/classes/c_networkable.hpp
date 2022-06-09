#pragma once

class rust::classes::c_networkable 
{
public:
	unsigned int id( ) {
		return *reinterpret_cast< unsigned int* >( this + 0x10 );
	}
};