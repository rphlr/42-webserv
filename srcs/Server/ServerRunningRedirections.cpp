#include "../../includes/Server/ServerRunning.hpp"

std::string ServerRunning::generateDirectoryListing(const std::string& directoryPath)
{
	std::string completeDirectoryPath = _rootPath + directoryPath;
	std::stringstream ss;
	ss << "<html><head><title>Directory Listing</title></head><body><h1>Directory Listing: " << completeDirectoryPath << "</h1><ul>";

	DIR* dir = opendir(completeDirectoryPath.c_str());
	if (dir == NULL) {
		ss << "<p>Error: Unable to open directory</p></body></html>";
		return ss.str();
	}

	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL) {
		std::string name = entry->d_name;
		if (name != "." && name != "..") {
			name = directoryPath + "/" + name;
			ss << "<li><a href=\"" << name << "\">" << name << "</a></li>";
		}
	}
	closedir(dir);
	ss << "</ul></body></html>";
	return ss.str();
}

void ServerRunning::checkRedirection(int response_socket, std::string &path_to_check, std::string &method)
{
	// if (path_to_check.find(".") != std::string::npos) {
	// 	handleFilePath(response_socket, path_to_check);
	// 	return;
	// }
	for (std::vector<Location>::iterator it = _locations.begin(); it != _locations.end(); it++)
	{
		if (path_to_check == (*it).getPath())
		{
			if ((*it).getRedirect().empty())
			{
				std::string inLocDefPath(path_to_check + _default_file);
				if (!pathExists(inLocDefPath))
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
					handleFilePath(response_socket, inLocDefPath);
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