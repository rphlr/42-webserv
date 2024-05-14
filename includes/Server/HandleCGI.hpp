#ifndef HANDLECGI_HPP
#define HANDLECGI_HPP

#include <string>
#include <map>
#include "../42webserv.hpp"

class HandleCGI {
    public:
        HandleCGI(const std::string& scriptPath,
                const std::map<std::string, std::string>& env,
                const std::string& postData);
        ~HandleCGI();

        std::string execute(std::string serverRoot);

    private:
        std::string scriptPath;
        std::map<std::string, std::string> env;
        std::string postData;

        void prepareEnvironment();
};

#endif
