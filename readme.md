## webserv

Webserv project at 42.

TODO:

Implement error pages. Error pages can be defined in the configuration already but are not handled yet.
Error pages need to be able to be defined for every server block.


webserv fork() (a)
	- cgi request (b) fork
			- runs index.php (b)
			- fork: monitor index.php -> sleep(2) ....... still there? kill(parent) (c) (c kills b)