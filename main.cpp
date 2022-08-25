#include "inc/configuration.hpp"

int	main(int argc, char **argv)
{
	(void)argc;
	Configuration	petroulette = Configuration(argv[1]);	//constructs a Configuration object, taking the configfile as parameter
	petroulette.launch();									//launches the the initialized Configuration object

	return 0;
}