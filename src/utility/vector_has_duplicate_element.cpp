#include <vector>
#include <set>
#include "../../inc/utility/utility.hpp"

/**
 * @brief Template function which accepts any vector andd returns true if there are duplicate elements.
 * 
 * @param v vector
 * @return true, there are duplicate elements
 * @return false, all elements are unique.
 */
template<typename X>
bool vector_has_duplicate_element(const std::vector<X>& v)
{
	std::set<X> set;
	for (typename std::vector<X>::const_iterator it = v.begin(); it != v.end(); ++it)
	{
		if (set.find(*it) != set.end())
			return true;
		set.insert(*it);
	}
	return false;
}

//int main() {
//	std::vector<int> v;
//	v.push_back(1);
//	v.push_back(2);
//	v.push_back(2);

//	std::vector<int> v1;
//	v1.push_back(1);
//	v1.push_back(2);
//	v1.push_back(3);

//	std::cout << vector_has_duplicate_element(v) << std::endl;
//	std::cout << vector_has_duplicate_element(v1) << std::endl;
//}