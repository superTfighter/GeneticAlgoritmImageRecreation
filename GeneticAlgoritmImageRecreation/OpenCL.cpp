#include "OpenCL.h"

OpenCL::OpenCL()
{
    
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);

	auto platform = platforms.front();

	std::vector<cl::Device> devices;

	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

	assert(devices.size() >= 1);

	std::cout << "Available devices: " << std::endl;

	for (auto device : devices)
	{
		auto vendor = device.getInfo<CL_DEVICE_VENDOR>();
		auto version = device.getInfo<CL_DEVICE_VERSION>();

		std::cout << "Name: " << vendor << " version: " << version << std::endl;
	}

	device = devices.front();

	std::cout << "Default device choosen: " << device.getInfo<CL_DEVICE_VENDOR>() << std::endl;

	std::ifstream invertFile("kernel.cl");

	std::string src(std::istreambuf_iterator<char>(invertFile), (std::istreambuf_iterator<char>()));

	cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

	context = cl::Context(device);
	program = cl::Program(context, sources);

	auto err = program.build("-cl-std=CL2.0");

	if (err)
		std::cout << "Error: " << err << std::endl;

	queue = cl::CommandQueue(context, device);

	auto maxBlockNumber = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

	std::cout << "Maximum block number: " << maxBlockNumber << std::endl;
}

std::vector<float>  OpenCL::FitnessCalculation(const sf::Uint8* pixelData, const sf::Uint8* originalData, int width, int height, int channels)
{
	auto maxBlockNumber = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();

	if (width < maxBlockNumber)
		maxBlockNumber = width;

	std::vector<float> fitness = std::vector<float>(width * height * channels);
	cl_int err = 0;

	cl::Buffer memBuf1(context, 0, width * height * channels * sizeof(uint8_t));
	cl::Buffer memBuf2(context, 0, width * height * channels * sizeof(uint8_t));
	cl::Buffer fitnessBuf(context, 0, width * height * channels * sizeof(float));

	queue.enqueueWriteBuffer(memBuf1, true, 0, width * height * channels * sizeof(uint8_t), pixelData);
	queue.enqueueWriteBuffer(memBuf2, true, 0, width * height * channels * sizeof(uint8_t), originalData);

	cl::Kernel kernel(program, "fitness", &err);
	kernel.setArg(0, memBuf1);
	kernel.setArg(1, memBuf2);
	kernel.setArg(2, fitnessBuf);

	queue.enqueueNDRangeKernel(kernel, cl::NullRange, width * height * channels, maxBlockNumber);
	queue.enqueueReadBuffer(fitnessBuf, true, 0, width * height * channels * sizeof(float), &fitness[0]);
	
	if (err)
		std::cout << err << std::endl;

	return fitness;
}

