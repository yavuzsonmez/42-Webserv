## How to use the ConfigFileParsing Class:

After startup, it parses. Do not change how the configuration file is being handled in the main.   
When it was successful and it threw no exceptions, you can now use the config object.   

it is structured like that:   
ConfigFileParsing  (class)   
-- ServerBlock (in a vector)   
---- ConfigurationKeys   


The ConfigFileParsing holds all the ServerBlocks.
A server block is written like this in the configuration file

```
server {

}
```

Each ServerBlock can hold configuration keys.
The configuration keys have different types:

Relevant for you:   
INDEX   
SERVER_NAME   
LISTEN   
LOCATION   
ROOT   
METHODS   
GENERAL_ERROR_PAGE
NOT_FOUND_ERROR_PAGE
CGI_PATH
CGI_EXECUTABLE_PATH
CGI_FILEENDING
POST_MAX_SIZE
REDIRECTION

You can ignore following:   
INVALID   
SERVERSTARTSEGMENT   

**These are the according keys**   
 	KEY_LOCATION				"location"   
 	KEY_INDEX					"index"   
 	KEY_SERVER_START_SEGMENT	"server"   
 	KEY_ROOT					"root"   
 	KEY_SERVER_NAMES			"server_name"   
 	KEY_LISTEN					"listen"   
 	KEY_INVALID					"INVALID"   
 	KEY_METHODS					"methods"   
 	KEY_EXECUTABLE_PATH			"cgi_path"   
 	KEY_FILEENDING				"cgi_fileending"   
 	KEY_NOT_FOUND_PAGE			"not_found_error_page"
 	KEY_GENERAL_ERROR_PAGE		"general_error_page"   
 	KEY_POST_MAX_SIZE			"post_max_size"   
 	KEY_REDIRECTION				"redirection"   


Each configuration key has a key and a value, and furthermore the corresponding attributes like indexes or root.
Also, there is one special key. It is called LOCATION.

A location looks like this in the configuration file:  (LOCATION BLOCK)

```
location {

}
```

It can also hold additional configuration keys. Currently supported are:   

ROOT   
INDEX   
METHODS   

All the nested configuration keys will be saved in the configuration keys within the LOCATION typed configuration type.
The attributes of the location configuration key holds the information found in the location block.


## How to add a new key   
Go to ConfigurationKey.hpp and add a new key to the enum.   
In the same file, add the KEY_CONFIGURATIONKEY to the defines. This is what the parser will look for in the configuration file.   
Then go to detectConfigurationType (or detectLocationKeyConfiguration) function in ConfigurationKey.cpp and add a new case to the if statement.
Think of a function name which would be fitting to check for the new key.
It should look like this:
```
if (this->isServerNameKeyType(raw))
{
		debugger.info("Detected server name key type in server block.");
		return SERVER_NAME;
}
```

Write the function you just created which checks for the new key.
If necesssary, add a property to the class which saves the value of the key.
Make sure to add it in the copy constructor.
If you added it to the location before, make sure to add it to addConfigurationKeyToLocation function in ConfigFileParsing.cpp as well. It takes care of adding the configuration key to the location and combining all the location attached configuration keys into one configuration key.