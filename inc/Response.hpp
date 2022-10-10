#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "Webserv.hpp"
# include "HttpRequest.hpp"


/* 
    Takes a string object that contain the whole request message and parse it into 3 Variables 
    _request_line, _request_headers, _request_body. And build the response message.
*/
class Response
{
    public:
        Response();
        Response(HttpRequest&);
        ~Response();
        
        void    buildResponse();
        std::string    getContent();
        char*    getBody();
        size_t   getBodyLength();
        int      getErrorCode();


    private:
        HttpRequest _request;
        std::string _request_line[3];
        std::string _request_headers; // This is temp only, Later to be changed to something like map<Header_name, Header details>
        // std::map<std::string, std::string> _requset_heaeders;
        size_t _body_length;
        char* _request_body;
        std::string _response_content;
        std::string _response_body;
        int         _error_code;

        int    buildBody();
        void    addStatus();
        void    addHeaders();
        int    readFile();
        // void    tokenize(std::string&, std::string&, std::string del = "\n");

};

#endif // RESPONSE_HPP