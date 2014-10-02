#ifndef GEUTILITIES_H
#define GEUTILITIES_H

/**
	GEUtilites
	Purpose: Misc Utility functions
	Author: Jeff Adams
*/

#include <string>
#include <sstream>


std::string LTrim( std::string trimString );
std::string RTrim( std::string trimString );
std::string LRTrim( std::string trimString );

/**
	http://www.cplusplus.com/forum/articles/9645/
*/
template <typename T>
T StringToNumber ( const std::string &Text )//Text not by const reference so that the function can be used with a 
{                               //character array as argument
	std::stringstream ss(Text);
	T result;
	return ss >> result ? result : 0;
}

#endif /* GEUTILITIES_H */