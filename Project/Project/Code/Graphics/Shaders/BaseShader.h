#pragma once

#include <vector>

class BaseShader
{
public:
	//Register all uniform locations
	virtual void RegisterUniforms() {} 
	//Get uniform from shaderProgram once a frame to avoid doing find operations too many times a frame which will cost much time
	virtual void GetUniforms() {} 

	std::vector<int> GetVector4Locations() { return mVector4Locations; }
	std::vector<int> GetColorLocation() { return mColorLocations; }
protected:
	std::vector<int> mVector4Locations;
	std::vector<int> mColorLocations;
};