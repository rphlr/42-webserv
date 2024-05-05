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
    }
    
}

std::string HandleCGI::execute() {
    int stdout_pipefd[2]; // Pipe pour la sortie standard
    int stdin_pipefd[2]; // Pipe pour les données POST

    if (pipe(stdout_pipefd) == -1 || pipe(stdin_pipefd) == -1) {
        perror("pipe");
        return "";
    }

    bool codeExecution = false;

    if (scriptPath.find("?") != std::string::npos) {
        scriptPath = scriptPath.substr(scriptPath.find("?") + 1);
        char *cwd = getcwd(NULL, 0);
        std::string rootPath = std::string(cwd) + "/" + "webpages/cgi-bin/";
        free(cwd);
        scriptPath = rootPath + scriptPath;
        codeExecution = true;
    }
    // Exécute le script CGI
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

		if (scriptPath.find(".php") != std::string::npos)
			execl("/usr/bin/php", "php", scriptPath.c_str(), (char *)NULL);
		else if (scriptPath.find(".py") != std::string::npos)
			execl("/usr/bin/python3", "python3", scriptPath.c_str(), (char *)NULL);
		else if (scriptPath.find(".pl") != std::string::npos)
			execl("/usr/bin/perl", "perl", scriptPath.c_str(), (char *)NULL);
        else if (scriptPath.find(".sh") != std::string::npos)
            execl("/bin/sh", "sh", scriptPath.c_str(), (char *)NULL);
        else
            std::cerr << "Unsupported script type" << std::endl;

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
            // std::cout << YELLOW << "Output: " << output << std::endl << RESET;
        }
        close(stdout_pipefd[0]);

        int status;
        waitpid(pid, &status, 0);

        if (codeExecution) {
            std::string rmCommand = "rm -f '" + scriptPath + "'";
            if (system(rmCommand.c_str()) != 0) {
                std::cerr << "Failed to delete script: " << scriptPath << std::endl;
            }
        }
        return output;
    }
}
