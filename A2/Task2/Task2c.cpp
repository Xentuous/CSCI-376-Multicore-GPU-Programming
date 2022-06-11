#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <vector>

// OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include "common.h"

int main() {

	// initialize OpenCL
	cl::Platform platform;
	cl::Device device;
	cl::Context context;
	cl::Program program;
	cl::Kernel kernel;
	cl::CommandQueue queue;
	
	try {
		if (select_one_device(&platform, &device)) {
			// Create a context
			context = cl::Context(device);

			// Create a queue
			queue = cl::CommandQueue(context, device);

			// If program builds successfully program will run encrpytion and decryption
			if (build_program(&program, &context, "MinZhanFoo_A2_T2c.cl")) {
				
				std::vector<char> initial = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
											  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

				std::vector<char> cipherLetters = { 'G', 'X', 'S', 'Q', 'F', 'A', 'R', 'O', 'W', 'B', 'L', 'M', 'T', 
					                                'H', 'C', 'V', 'P', 'N', 'Z', 'U', 'I', 'E', 'Y', 'D', 'K', 'J' };

				std::vector<std::string> fileName = { "plaintext.txt", "2C_cipher.txt", "2C_decrypted.txt" };
				
				// First iteration is ciphering, Second iteration is deciphering
				for (int i = 0; i < 2; i++) {
					std::vector<char> charLinesVec;

					// Read file
					std::ifstream textFile(fileName[i]);
					std::string line;

					while (!textFile.eof()) {
						std::getline(textFile, line);
						
						// Only uppercase when reading from plaintext.txt, the rest of fileName dont need to uppercase
						if (i == 0) {
							std::transform(line.begin(), line.end(), line.begin(), ::toupper);
						}
						
						// Add \n after reading each line
						line += '\n';

						// store each character inside plaintextLine into vector
						for (char c : line) {
							charLinesVec.push_back(c);
						}
					}

					std::vector<char> result(charLinesVec.size());

					// Create kernel base on iteration number
					kernel = (i == 0) ? cl::Kernel(program, "encrypt") : cl::Kernel(program, "decrypt");

					// Set cl::Buffer objects to be used in kernel
					cl::Buffer initialBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char) * 26, &initial[0]);
					cl::Buffer cipherLettersBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char) * 26, &cipherLetters[0]);
					cl::Buffer characterBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(char) * charLinesVec.size());
					cl::Buffer plaintextCharBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char) * charLinesVec.size(), &charLinesVec[0]);
					cl::Buffer resultBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(char) * charLinesVec.size());
				
					// Set kernel arguments
					kernel.setArg(0, initialBuffer);
					kernel.setArg(1, cipherLettersBuffer);
					kernel.setArg(2, plaintextCharBuffer);
					kernel.setArg(3, characterBuffer);
					kernel.setArg(4, resultBuffer);	

					// Utilise max gpu work-items size
					cl::NDRange offset(0);
					cl::NDRange globalSize(1024);

					// Enqueue kernel with offset = 0 and globalSize = 1024 (max gpu working size)
					queue.enqueueNDRangeKernel(kernel, offset, globalSize);
					std::cout << "Kernel enqueued." << std::endl;
					std::cout << "---------------------" << std::endl;

					// Read result from device to host
					queue.enqueueReadBuffer(resultBuffer, CL_TRUE, 0, sizeof(char) * charLinesVec.size(), &result[0]);

					// Remove last element ('\n') in result vector
					result.pop_back();

					// Prepare output file 2C_cipher.txt or 2C_decrypted.txt
					std::ofstream textfileCL(fileName[i + 1]);
					
					// Output to 2C_cipher.txt or 2C_decrypted.txt
					for (char c : result) {
						textfileCL << c;
					}
					textfileCL.close();

					// Clear result vector
					result.clear();
				}
			}
		}
	} catch (cl::Error e) {
		handle_error(e);
	}

#ifdef _WIN32
	std::cout << "\n\npress a key to quit...";
	std::cin.ignore();
#endif

	return 0;
}
