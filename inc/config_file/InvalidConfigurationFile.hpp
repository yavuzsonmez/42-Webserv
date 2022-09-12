#ifndef INVALIDCONFIGURATIONFILE
# define INVALIDCONFIGURATIONFILE
#include <exception>
#include <string>

class InvalidConfigurationFile : public std::exception {
			public:
				InvalidConfigurationFile() {};
				InvalidConfigurationFile( const InvalidConfigurationFile &src ) { *this = src; };
				InvalidConfigurationFile & operator = (const InvalidConfigurationFile &src) { (void) src; return *this; };
				virtual const char* what() const throw();
};

#endif