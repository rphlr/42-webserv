#include "../../includes/Server/ServerRunning.hpp"

std::string ServerRunning::generateDirectoryListing(const std::string& directoryPath)
{
	std::string completeDirectoryPath = _rootPath + directoryPath;
	std::stringstream ss;
	ss << "<html><head>";
	ss << "<title>DirListing</title>";
	ss << "<link rel=\"stylesheet\" type=\"text/css\" href=\"/default_webpages/styles.css\">";
	ss << "</head><body>";
	ss << "</header><h1>Directory Listing: " << directoryPath << "</h1></header>";
	ss << "<main>";

	DIR* dir = opendir(completeDirectoryPath.c_str());
	if (dir == NULL) {
		ss << "<p>Error: Unable to open directory</p></body></html>";
	}
	else {
		ss << "<section class=\"file-list\">";
		struct dirent* entry;
		while ((entry = readdir(dir)) != NULL) {
			std::string name = entry->d_name;
			if (name != "." && name != ".." && name.find(".") != std::string::npos && name.find("styles.css") == std::string::npos) {
				name = directoryPath + "/" + name;
				ss << "<li><a href=\"" << name << "\">" << name << "</a></li>";
			}
		}
		ss << "</section>";
		closedir(dir);
	}

	ss << "</main>";

	ss << "<footer><p>&copy; 2024 website magnifique</p></footer>";
	ss << "</body></html>";
	return ss.str();
}

void ServerRunning::checkIfRedirectionNeeded(int response_socket, std::string &path_to_check, std::string &method)
{
	if (path_to_check.find(".") != std::string::npos) {
		handleFilePath(response_socket, path_to_check);
		return;
	}

	for (std::vector<Location>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		if (path_to_check == (*it).getPath())
		{
			if ((*it).getRedirect().empty())
			{
				std::string _inLocDefPath(path_to_check + _default_file);
				if (!pathExists(_inLocDefPath))
				{
					if ((*it).getDirList() == true)
					{
						std::string dirListHTML = generateDirectoryListing(path_to_check);
						sendResponse(response_socket, dirListHTML, 200, "text/html");
					}
					else
						handleErrorFilePath(response_socket, 404);
				}
				else
					handleFilePath(response_socket, _inLocDefPath);
				return;
			}

			if ((*it).checkMethod(method) == false)
				handleErrorFilePath(response_socket, 405);
			else
				handleFilePath(response_socket, (*it).getRedirect());
			return;
		}
	}
	std::string noLocDefPath(path_to_check + _default_file);
	if (!pathExists(noLocDefPath))
		handleErrorFilePath(response_socket, 404);
	else
		handleFilePath(response_socket, noLocDefPath);
	return;
}