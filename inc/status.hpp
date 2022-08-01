
#ifndef STATUS_HPP
# define STATUS_HPP

// Information responses
#define Continue "100 Continue"
#define Switching "101 Switching Protocols"
#define Processing "102 Processing"
#define Early "103 Early Hints"

//Successful responses
#define OK "200 OK"
#define Created "201 Created"
#define Accepted "202 Accepted"
#define Non_Authoritative "203 Non-Authoritative Information"
#define No_Content "204 No Content"
#define Reset_Content "205 Reset Content"
#define Partial_Content "206 Partial Content"
#define Multi_Status "207 Multi-Status"
#define Already_Reported "208 Already Reported"
#define IM_Used "226 IM Used"

//Redirection messages
#define Multiple_Choices "300 Multiple Choices"
#define Moved_Permanently "301 Moved Permanently"
#define Found "302 Found"
#define See_Other "303 See Other"
#define Not_Modified "304 Not Modified"
#define Use_Proxy "305 Use Proxy"
#define unused "306 unused"
#define Temporary_Redirect "307 Temporary Redirect"
#define Permanent_Redirect "308 Permanent Redirect"

//Client error responses
#define Bad_Request "400 Bad Request"
#define Unauthorized "401 Unauthorized"
#define Payment_Required "402 Payment Required"
#define Forbidden "403 Forbidden"
#define Not_Found "404 Not Found"
#define Method_Not_Allowed "405 Method Not Allowed"
#define Not_Acceptable "406 Not Acceptable"
#define Proxy Authentication "407 Proxy Authentication Required"
#define Request_Timeout "408 Request Timeout"
#define Conflict "409 Conflict"
#define Gone "410 Gone"
#define Length_Required "411 Length Required"
#define Precondition_Failed "412 Precondition Failed"
#define Payload_Too_Large "413 Payload Too Large"
#define URI_Too_Long "414 URI Too Long"
#define Unsupported_Media_Type "415 Unsupported Media Type"
#define Range_Not_Satisfiable "416 Range Not Satisfiable"
#define Expectation_Failed "417 Expectation Failed"
#define Teapot "418 I'm a teapot"
#define Misdirected_Request "421 Misdirected Request"
#define Unprocessable_Entity "422 Unprocessable Entity"
#define Locked "423 Locked"
#define Failed_Dependency "424 Failed Dependency"
#define Too_Early "425 Too Early"
#define Upgrade_Required "426 Upgrade Required"
#define Precondition_Required "428 Precondition Required"
#define Too_Many_Requests "429 Too Many Requests"
#define Request_Header_Too_Large "431 Request Header Fields Too Large"
#define Unavailabe_For_Legal "451 Unavailable For Legal Reasons"

//Server error responses
#define Internal_Server_Error "500 Internal Server Error"
#define Not_Implemented "501 Not Implemented"
#define Bad_Gateway "502 Bad Gateway"
#define Service_Unavailable "503 Service Unavailable"
#define Gateway_Timeout "504 Gateway Timeout"
#define HTTP_Version_Not_Supported "505 HTTP Version Not Supported"
#define Variant_Negotiates "506 Variant Also Negotiates"
#define Insufficient_Storage "507 Insufficient Storage"
#define Loop_Detected "508 Loop Detected"
#define Not_Extended "510 Not Extended"
#define Network_Auth_Required "511 Network Authentication Required"

#endif