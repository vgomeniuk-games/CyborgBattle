#ifndef RES_PATH_H
#define RES_PATH_H

#include <iostream>
#include <string>
#include <SDL.h>

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
std::string getResourcePath(const std::string &assetsDir = "", const std::string &subDir = "");

#endif