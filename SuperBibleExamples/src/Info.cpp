

#include "Info.h"

// Structors

Info::Info()
	:GEObject()
{
	this->setVisible( false );
}

Info::Info( const Info& info )
	:GEObject( info )
{
}


// Functions

Info* Info::clone() const
{
	return new Info( *this );
}


std::string Info::getClassName() const
{
	return "Info";
}
