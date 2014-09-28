#ifndef GERESOURCECONTAINER_H
#define GERESOURCECONTAINER_H

/**
	GEResourceContainer
	Templated class to hold the various resource types for the graphics engine.
	Allows resources to be accessed in a standard way and have common error 
	handling.  Also allows internal container to be changed without the engine
	having to know about it.

	Author:  Jeff Adams
*/

#include <map>
#include <string>

template <class T>
class GEResourceContainer
{
private:
	std::map< std::string, T > resourceMap;			// Container for the resources

public:
	GEResourceContainer();
	// Copy constructor here.
	~GEResourceContainer();

	// setters
	bool AddResource( const std::string resourceName, const T resource );

	// getters
	T GetResource( const std::string resourceName ) const;

	// Functions
	bool ContainsResource( const std::string resourceName ) const;
	void RemoveResource( const std::string resourceName );
};

template <class T>
GEResourceContainer<T>::GEResourceContainer()
{
}

template <class T>
GEResourceContainer<T>::~GEResourceContainer()
{
}

// setters

template <class T>
bool GEResourceContainer<T>::AddResource( const std::string resourceName, const T resource )
{
	bool success = false;

	if( !ContainsResource( resourceName ) )
	{
		std::pair< std::map< std::string, T>::iterator, bool> ret;
		ret = resourceMap.insert( std::pair< std::string, T>( resourceName, resource ) );
		success = ret.second;
	}

	return success;
}

// getters

template <class T>
T GEResourceContainer<T>::GetResource( const std::string resourceName ) const
{
	T returnResource = T();

	std::map< std::string, T>::const_iterator resI = resourceMap.find( resourceName );

	if( resI != resourceMap.end() )
	{
		returnResource = resI->second;
	}
	else
	{
		std::map< std::string, T>::const_iterator missingI = resourceMap.find( GE_MISSING );
			
		if( missingI != resourceMap.end() )
		{
			returnResource = missingI->second;
		}
		// TODO: else we're screwed... something really went wrong... what do we do?

	}

	return returnResource;
}

// Functions

template <class T>
bool GEResourceContainer<T>::ContainsResource( const std::string resourceName ) const
{
	bool found = false;

	std::map< std::string, T>::const_iterator resI = resourceMap.find( resourceName );
	
	if( resI != resourceMap.end() )
		found = true;

	return found;
}

template <class T>
void GEResourceContainer<T>::RemoveResource( const std::string resourceName )
{
}


#endif /* GERESOURCECONTAINER_H */