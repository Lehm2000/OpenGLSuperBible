#ifndef GESETTING_H
#define GESETTING_H

/**
	GESetting
	Class to hold a controllable setting.  Similar to GEController but intended to cycle through a list of specified values
*/

#include <vector>

template <class T>
class GESetting
{
private:
	T value;						// current value
	std::vector<T> valueList;		// list of possible values that will be cycled through

public:
};

#endif /* GESETTING_H */