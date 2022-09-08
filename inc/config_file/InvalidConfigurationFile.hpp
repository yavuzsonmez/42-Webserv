#ifndef INVALIDCONFIGURATIONFILE
# define INVALIDCONFIGURATIONFILE
#include <exception>

class InvalidConfigurationFile : public std::exception {
			public:
				virtual const char* what() const throw();
				virtual const char* what(char *message) const throw();
};

#endif