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

int main() {
	cl::Platform platform;
	cl::Device device;	
	cl::Context context;
	cl::Program program;
	cl::Kernel kernel;	
	cl::CommandQueue queue;

	std::vector<cl_int> intOuput(512);
	std::vector<cl_char> alpOuput(52);

	// Vector of unsigned chars for alphabets
	std::vector<unsigned char> alphabets;
	
	// Vector of of unsigned int 
	std::vector<unsigned int> numbers;

	// Assign lower case alphabets to vector alphabets
	for (size_t i = 97; i <= 122; i++) {
		alphabets.push_back(i);
	}
	// Assign upper case alphabets to vector alphabets
	for (size_t i = 65; i <= 90; i++) {
		alphabets.push_back(i);
	}
	
	// Assign unsigned int to vector numbers
	for (size_t i = 1; i <= 512; i++) {
		numbers.push_back(i);
	}
	
	try {
		// Create three OpenCL memory objects
		cl::Buffer firstBuffer;
		cl::Buffer secondBuffer;
		cl::Buffer thirdBuffer;		

		if (select_one_device(&platform, &device)) {
			std::cout << "--------------------" << std::endl;
			
			// Create context
			context = cl::Context(device);

			// Command queue for the device
			queue = cl::CommandQueue(context, device);
			
			// The first buffer is read-only and initialised with the contents of the alphabet vector
			firstBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, alphabets.size() * sizeof(unsigned char), &alphabets[0]);

			// The second buffer is write-only and created to store 52 unsigned chars.
			secondBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, alphabets.size() * sizeof(unsigned char), NULL);

			// The third buffer is read-and-write and created to store 512 unsigned ints.
			thirdBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, numbers.size() * sizeof(unsigned int), NULL);

			// Enqueue two OpenCL commands 
			// To copy the contents from the first buffer into the second buffer.
			queue.enqueueCopyBuffer(firstBuffer, secondBuffer, 0, 0, alphabets.size() * sizeof(unsigned char));

			// To write the contents from the vector of 512 integers into the third buffer.
			queue.enqueueWriteBuffer(thirdBuffer, CL_TRUE, 0, numbers.size() * sizeof(unsigned int), &numbers[0]);

			// Build the program
			if (build_program(&program, &context, "task2.cl")) {

				// Create kernel
				kernel = cl::Kernel(program, "task2");
				
				// Set the arguments for the kernel
				kernel.setArg(0, 21.43f);
				kernel.setArg(1, secondBuffer);
				kernel.setArg(2, thirdBuffer);

				// Enqueue the kernel
				queue.enqueueTask(kernel);

				std::cout << "Kernel enqueued." << std::endl;
				std::cout << "--------------------" << std::endl;
				
				// Read contents from two buffers and display results on screen
				queue.enqueueReadBuffer(secondBuffer, CL_TRUE, 0, alphabets.size() * sizeof(unsigned char), &alpOuput[0]);
				queue.enqueueReadBuffer(thirdBuffer, CL_TRUE, 0, numbers.size() * sizeof(unsigned int), &intOuput[0]);
				
				std::cout << "Contents of the second buffer after kernel execution:" << std::endl;
				for (int i = 0; i < alpOuput.size(); i++) {
					std::cout << alpOuput[i] << " ";
				}
				std::cout << std::endl << std::endl;

				std::cout << "Contents of the third buffer after kernel execution:" << std::endl;
				for (int i = 0; i < intOuput.size(); i++) {
					std::cout << intOuput[i] << " ";
				}
				std::cout << std::endl;
			}
		}
	}
	catch (cl::Error e) {
		handle_error(e);
	}
}