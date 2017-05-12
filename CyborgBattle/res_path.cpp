#include "res_path.h"

/*
* Get the resource path for resources located in assetsDir/subDir
* It's assumed the project directory is structured like:
* $(ProjectDir)/
*	  the executable
*	  assetsDir/
*		subDir/
*		subDir/
*
* Paths returned will be $(ProjectDir)/assetsDir/subDir
*/
std::string getResourcePath(const std::string &assetsDir, const std::string &subDir){
	// We need to choose the path separator properly based on which
	// platform we're running on, since Windows uses a different
	// separator than most systems
#ifdef _WIN32
	const char PATH_SEP = '\\';
#else
	const char PATH_SEP = '/';
#endif

	// This will hold the base resource path: $(ProjectDir)/assetsDir/
	// We give it static lifetime so that we'll only need to call
	// SDL_GetBasePath once to get the executable path
	static std::string baseRes;
	if (baseRes.empty()){
		//SDL_GetBasePath will return NULL if something went wrong in retrieving the path
		char *basePath = SDL_GetBasePath();
		if (basePath){
			baseRes = basePath + assetsDir + PATH_SEP;
			SDL_free(basePath);
		}
		else {
			std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
			return "";
		}
	}
	// If we want a specific subdirectory path in the resource directory
	// append it to the base path. This would be something like $(ProjectDir)/assetsDir/subDir
	return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
}
