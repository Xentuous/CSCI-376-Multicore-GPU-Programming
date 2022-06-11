#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

// OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include "common.h"

#define SIZE 1024

int main() {
	cl::Platform platform;
	cl::Device device;
	cl::Context context;
	cl::Program program;
	cl::Kernel kernel;
	cl::CommandQueue queue;

	cl::Buffer resultBuffer;
	
	std::vector<cl_int> result(SIZE);
	unsigned int option;

	// Let user select a device, if user selects a device, build program
	if (select_one_device(&platform, &device)) {
		context = cl::Context(device);
		queue = cl::CommandQueue(context, device);
		
		std::cout << "--------------------" << std::endl;
		
		// If build successfully, prompt user for input
		if (build_program(&program, &context, "MinZhanFoo_A1_Task3_Kernel.cl")) {

			// Prompt user for input
			std::cout << "Enter a number between 1 and 89 (inclusive): ";
			std::cin >> option;

			// If user enters a value between 1 to 89 inclusive, proceed with program 
			if (option >= 1 || option <= 89 && isdigit(option)) {

				// Initialise resultBuffer
				resultBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_int) * SIZE);
			
				// Create kernel
				kernel = cl::Kernel(program, "task3");

				// Set kernal arguments
				kernel.setArg(0, option);
				kernel.setArg(1, resultBuffer);
				
				// Set NDRange to prepare for enqueueNDRange
				// Start from index 0th
				cl::NDRange offset(0);

				// Utilise 1024 work-items 
				cl::NDRange globalSize(SIZE);

				// Enqueue kernel with offset and globalSize
				queue.enqueueNDRangeKernel(kernel, offset, globalSize);

				std::cout << std::endl << "Kernel enqueued." << std::endl;
				std::cout << "--------------------" << std::endl;
				std::cout << "Contents of the buffer after kernel execution:" << std::endl;	 

				// Display output from deive to host 
				queue.enqueueReadBuffer(resultBuffer, CL_TRUE, 0, sizeof(cl_int) * SIZE, &result[0]);

				for (auto &value : result) {
					std::cout << value << " ";
				}
				
			} else {
				quit_program("Invalid option");
			}
		}
	}

}