
#include <string>

#include "GEObject.h"
#include "GEObjectContainer.h"

GEObjectContainer::GEObjectContainer()
{
	objects.clear();
}

GEObjectContainer::GEObjectContainer( GEObjectContainer& source )
{
}

GEObjectContainer::~GEObjectContainer()
{
	// TODO: destroy here or as part of engine cleanup?
}

bool GEObjectContainer::AddObject( const std::string entityName, GEObject* entity)
{
	bool success = false;

	if ( !this->ContainsObject( entityName ) )
	{
		objects.insert( std::pair< std::string, GEObject* >( entityName, entity ) );
		success = true;
	}

	return success;
}

void GEObjectContainer::RemoveObject( const std::string entityName)
{
	objects.erase ( entityName );
}

bool GEObjectContainer::ContainsObject( const std::string entityName )
{
	return objects.find( entityName ) != objects.end();
}

GEObject* GEObjectContainer::GetObject( const std::string entityName )
{
	GEObject* returnObject = nullptr;

	std::map< std::string, GEObject*>::const_iterator objectI = objects.find( entityName );

	if( objectI != objects.end() )
	{
		returnObject = objectI->second;
	}

	return returnObject;
}

const GEObject* GEObjectContainer::GetObjectConst( const std::string entityName ) const
{
	GEObject* returnObject = nullptr;

	std::map< std::string, GEObject*>::const_iterator objectI = objects.find( entityName );

	if( objectI != objects.end() )
	{
		returnObject = objectI->second;
	}

	return returnObject;
}

void GEObjectContainer::UpdateObjects( double gameTime, double deltaTime )
{
	for ( std::map< std::string, GEObject* >::const_iterator it = objects.begin(); it != objects.end(); it++ )
	{
		it->second->Update( gameTime, deltaTime );
	}
}