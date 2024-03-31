#include "../../includes/Server/HandleCGI.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

HandleCGI::HandleCGI(const std::string& scriptPath,
                     const std::map<std::string, std::string>& env,
                     const std::string& postData)
    : scriptPath(scriptPath), env(env), postData(postData) {}


HandleCGI::~HandleCGI() {}

void HandleCGI::prepareEnvironment() {
    for (const auto& pair : env) {
        setenv(pair.first.c_str(), pair.second.c_str(), 1);
    }
}

std::string HandleCGI::execute() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return "";
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    } else if (pid == 0) {
        // Child process
        close(pipefd[0]); // Close unused read end

        // Redirect stdout to pipe
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[1]); // Close the write end of the stdout pipe

        // Redirect stdin from a pipe if there is POST data
        if (!postData.empty()) {
            close(STDIN_FILENO); // Close existing stdin
            dup(pipefd[0]); // Duplicate the read end of the pipe to stdin
            close(pipefd[0]); // Close the read end of the pipe
        }

        prepareEnvironment();

        // Using execl to call the PHP interpreter. 
        // Note: The path to the PHP interpreter might need to be changed according to your system setup.
        execl("/usr/bin/php", "php", scriptPath.c_str(), (char *)NULL);

        // execl only returns on error
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        close(pipefd[1]); // Close the write end of the stdout pipe
        
        // Reading the output from the CGI script
        std::string output;
        char buffer[1024];
        ssize_t count;
        while ((count = read(pipefd[0], buffer, sizeof(buffer)-1)) > 0) {
            buffer[count] = '\0';
            output += buffer;
        }
        close(pipefd[0]); // Close the read end of the stdout pipe

        int status;
        waitpid(pid, &status, 0);

        return output;
    }
}
