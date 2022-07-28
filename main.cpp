#include "inc/configuration.hpp"

int	main(int argc, char **argv)
{
	(void)argc;
	configuration	petroulette = configuration(argv[1]);
	petroulette.launch();

	return 0;
}