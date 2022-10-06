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

You can ignore following:   
INVALID   
SERVERSTARTSEGMENT   


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