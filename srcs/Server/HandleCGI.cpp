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
    for (std::map<std::string, std::string>::const_iterator it = env.begin(); it != env.end(); ++it) {
        setenv(it->first.c_str(), it->second.c_str(), 1);
        std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
		// std::cout
    }
}

std::string HandleCGI::execute() {
    int stdout_pipefd[2]; // Pipe pour la sortie standard
    int stdin_pipefd[2]; // Pipe pour les données POST

    if (pipe(stdout_pipefd) == -1 || pipe(stdin_pipefd) == -1) {
        perror("pipe");
        return "";
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        close(stdout_pipefd[0]);
        close(stdout_pipefd[1]);
        close(stdin_pipefd[0]);
        close(stdin_pipefd[1]);
        return "";
    } else if (pid == 0) {
        // Child process
        close(stdout_pipefd[0]); // Ferme le côté lecture du pipe de sortie
        close(stdin_pipefd[1]); // Ferme le côté écriture du pipe d'entrée

        // Redirige stdout
        dup2(stdout_pipefd[1], STDOUT_FILENO);
        close(stdout_pipefd[1]);

        // Redirige stdin pour lire à partir du pipe
        dup2(stdin_pipefd[0], STDIN_FILENO);
        close(stdin_pipefd[0]);

        prepareEnvironment();
        execl("/usr/bin/php", "php", scriptPath.c_str(), (char *)NULL);
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        close(stdout_pipefd[1]); // Ferme le côté écriture du pipe de sortie
        close(stdin_pipefd[0]); // Ferme le côté lecture du pipe d'entrée

        // Écrit les données POST dans le pipe d'entrée, puis ferme-le
        if (!postData.empty()) {
            write(stdin_pipefd[1], postData.c_str(), postData.size());
        }
        close(stdin_pipefd[1]);

        // Lit la sortie du script CGI
        std::string output;
        char buffer[1024];
        ssize_t count;
        while ((count = read(stdout_pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[count] = '\0';
            output += buffer;
        }
        close(stdout_pipefd[0]);

        int status;
        waitpid(pid, &status, 0);

        return output;
    }
}


// std::string HandleCGI::execute() {
//     int pipefd[2];
//     if (!pipe(pipefd)) {
//         perror("pipe");
//         return "";
//     }

//     pid_t pid = fork();
//     if (pid == -1) {
//         perror("fork");
//         close(pipefd[0]);
//         close(pipefd[1]);
//         return "";
//     } else if (pid == 0) {
//         // Child process
//         close(pipefd[0]); // Close unused read end

//         // Redirect stdout to pipe
//         dup2(pipefd[1], STDOUT_FILENO); 
//         close(pipefd[1]); // Close the write end of the stdout pipe

//         // Redirect stdin from a pipe if there is POST data
//         if (!postData.empty()) {
//             close(STDIN_FILENO); // Close existing stdin
//             dup(pipefd[0]); // Duplicate the read end of the pipe to stdin
//             close(pipefd[0]); // Close the read end of the pipe
//         }

//         prepareEnvironment();

//         // Using execl to call the PHP interpreter. 
//         // Note: The path to the PHP interpreter might need to be changed according to your system setup.
//         execl("/usr/bin/php", "php", scriptPath.c_str(), (char *)NULL);

//         // execl only returns on error
//         perror("execl");
//         exit(EXIT_FAILURE);
//     } else {
//         // Parent process
//         close(pipefd[1]); // Close the write end of the stdout pipe
        
//         // Reading the output from the CGI script
//         std::string output;
//         char buffer[1024];
//         ssize_t count;
//         while ((count = read(pipefd[0], buffer, sizeof(buffer)-1)) > 0) {
//             buffer[count] = '\0';
//             output += buffer;
//         }
//         close(pipefd[0]); // Close the read end of the stdout pipe

//         int status;
//         waitpid(pid, &status, 0);

//         return output;
//     }
// }
