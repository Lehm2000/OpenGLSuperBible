


#include <cctype>

#include "GEUtilities.h"


std::string LTrim( std::string trimString )
{
	while ( !trimString.empty() && std::isspace( trimString.at(0) ) )
	{
		trimString = trimString.substr( 1, std::string::npos );
	}

	return trimString;
}

std::string RTrim( std::string trimString )
{
	while ( !trimString.empty() && std::isspace( trimString.at( trimString.length() - 1 ) ) )
	{
		trimString = trimString.substr( 0, trimString.length() - 1 );
	}

	return trimString;
}

std::string LRTrim( std::string trimString )
{
	return LTrim( RTrim( trimString ) );
}


