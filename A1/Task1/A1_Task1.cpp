#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <fstream>
#include <regex>

#include "common.h"

void getProcessingUnit(unsigned int* selection, std::string* puType);
void setPlatformDevice(cl::Platform* platform, cl::Device* device, std::vector<cl::Platform>& platformVec, const std::string& pu);

int main() {
	cl::Platform platform;
	cl::Device device;	
	cl::Context context;
	cl::Program program;
	cl::Kernel kernel;	
	cl::CommandQueue queue;

	std::vector<cl::Platform> platformVec;
	std::vector<cl::Kernel> allKernels;
	std::string puType;
	
	unsigned int selection;

	try {
		getProcessingUnit(&selection, &puType); 
		setPlatformDevice(&platform, &device, platformVec, puType);
		
		// Create a context using that device
		context = cl::Context(device);

		// Command a queue
		queue = cl::CommandQueue(context, device);

		// Build the program
		if (build_program(&program, &context, "task1.cl")) {
			// Create kernels
			program.createKernels(&allKernels);

			std::cout << "Device - " << context.getInfo<CL_CONTEXT_DEVICES>()[0].getInfo<CL_DEVICE_NAME>() << ", build log: " << std::endl;

			// Display build log
			std::cout << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
			std::cout << "---------------------" << std::endl;			
			
			// Display number of kernels in the program
			std::cout << "Number of kernels - " << allKernels.size() << std::endl;

			// Display all kernal names
			for (size_t i = 0; i < allKernels.size(); i++) {
				std::cout << "Kernel " << i << ": " << allKernels[i].getInfo<CL_KERNEL_FUNCTION_NAME>() << std::endl;
			}

			std::cout << "---------------------" << std::endl;
		} else {
			quit_program("Build failed.");
		}
	} catch (cl::Error e) {
		handle_error(e);
	}

#ifdef _WIN32
	std::cout << "\npress a key to quit...";
	std::cin.ignore();
#endif

	return 0;
}

// Allows user to choose either 0. CPU or 1. GPU
// If user enters anything outside the given options, the program quits
void getProcessingUnit(unsigned int* selection, std::string* puType) {
	std::cout << "Select a device type (CPU or GPU) " << std::endl;
	std::cout << "---------------------" << std::endl;
	std::cout << "0. CPU" << std::endl;
	std::cout << "1. GPU" << std::endl;
	std::cout << "---------------------" << std::endl;
	std::cout << "Enter a device type: ";
	std::cin >> *selection;
	
	if (*selection >= 0 || *selection <= 1 && isdigit(*selection)) {
		*puType = (*selection == 0) ? "CPU" : "GPU";
	} else {
		quit_program("Invalid selection");
	}
}

void setPlatformDevice(cl::Platform* platform, cl::Device* device, std::vector<cl::Platform>& platformVec, const std::string& puType) {
	std::vector<cl::Device> deviceVec;
	std::string deviceType;
	cl_device_type clDeviceType;
	unsigned int option;
	
	try {
		cl::Platform::get(&platformVec);

		// Store all CPU/GPU devices for each platform into platformVec, depending on the user's selection 
		for (auto &plat : platformVec) {
			std::vector<cl::Device> tempDevVec;
			(puType == "CPU") ? plat.getDevices(CL_DEVICE_TYPE_CPU, &tempDevVec) : plat.getDevices(CL_DEVICE_TYPE_GPU, &tempDevVec);
			deviceVec.insert(deviceVec.end(), tempDevVec.begin(), tempDevVec.end());
		}
		std::cout << std::endl;
		
		// Displaying information for each CPU/GPU devices, depending on the user's selection that is available
		for (size_t i = 0; i < deviceVec.size(); i++) {
			// Get device type
			deviceVec[i].getInfo(CL_DEVICE_TYPE, &clDeviceType);
			
			// Convert device type to string value of CPU or GPU
			deviceType = (clDeviceType == CL_DEVICE_TYPE_CPU) ? "CPU" : "GPU";

			// Get max work item sizes for each device, store into vector and display its string value 
			std::vector<size_t> maxWorkItem = deviceVec[i].getInfo<CL_DEVICE_MAX_WORK_ITEM_SIZES>();
			
			std::string maxWorkItemString = "[";

			for (auto &value : maxWorkItem) {
				maxWorkItemString += std::to_string(value) + ", ";
			}

			maxWorkItemString = maxWorkItemString.substr(0, maxWorkItemString.size() - 2) + "]";

			// Device #
			std::cout << "Device " << i << std::endl;
			
			// Name of platform that supports the device
			std::cout << "Platform - " << platformVec[i].getInfo<CL_PLATFORM_VENDOR>() << std::endl;
			
			// Device type
			std::cout << "Device type - " << deviceType << std::endl;
			
			// Device name
			std::cout << "Device name - " << deviceVec[i].getInfo<CL_DEVICE_NAME>() << std::endl;

			// Number of compute units
			std::cout << "Number of compute units - " << deviceVec[i].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << std::endl;

			// Max work group size
			std::cout << "Maximum work group size - " << deviceVec[i].getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << std::endl;

			// Max num of work item dimensions
			std::cout << "Maxinum number of work item dimensions - " << deviceVec[i].getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>() << std::endl;

			// Max work item sizes
			std::cout << "Maximum work item sizes - " << maxWorkItemString << std::endl;

			// Global mem size
			std::cout << "Global memory size - " << deviceVec[i].getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() << std::endl;

			// Local mem size
			std::cout << "Local memory size - " << deviceVec[i].getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << std::endl << std::endl;
		}

		// Display all the devices in deviceVec vector
		std::cout << "---------------------" << std::endl;
		std::cout << "Device options:" << std::endl;
		std::cout << "---------------------" << std::endl;

		for (size_t i = 0; i < deviceVec.size(); i++) {
			std::cout << i << ": Platform - " << platformVec[i].getInfo<CL_PLATFORM_VENDOR>() << ", " << deviceVec[i].getInfo<CL_DEVICE_NAME>() << std::endl;
		}
		
		// Allow user to select one of the devices, if user enters anything outside the given options, the program quits
		std::cout << "---------------------" << std::endl;
		std::cout << "Select a device: ";
		std::cin >> option;
		std::cout << std::endl;
		std::cout << "---------------------" << std::endl;

		if (option < deviceVec.size() || option >= 0 && isdigit(option)) {
			*platform = platformVec[option];
			*device = deviceVec[option];
		}
		else {
			quit_program("Invalid choice");
		}
	} catch (cl::Error e) {
		handle_error(e);
	}
}
