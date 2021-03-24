#pragma once
#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#include <CL/cl.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cassert>
#include "SFML/Main.hpp"


// Use (void) to silent unused warnings.
#define assertm(exp, msg) assert(((void)msg, exp))

class OpenCL
{
public:
    OpenCL();
   // ~OpenCL();

	//Fitness calculation via OpenCL
	std::vector<float>  FitnessCalculation(const sf::Uint8* pixelData, const sf::Uint8* originalData, int width,int height, int channels = 4);

private:
private:
	cl::Device device;
	cl::Program program;
	cl::Context context;
	cl::CommandQueue queue;
};
