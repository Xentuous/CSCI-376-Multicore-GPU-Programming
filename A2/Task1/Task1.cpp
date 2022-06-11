//#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
//#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions
//
//// C++ standard library and STL headers
//#include <algorithm>
//#include <fstream>
//#include <iostream>
//#include <iomanip>
//#include <sstream>
//#include <random>
//#include <vector>
//
//// OpenCL header, depending on OS
//#ifdef __APPLE__
//#include <OpenCL/cl.hpp>
//#else
//#include <CL/cl.hpp>
//#endif
//
//#include "common.h"
//
//bool validate(int vec1[], int vec2[], std::vector<cl_int>& outputVec);
//
//int main() {
//
//	// initialize OpenCL
//	cl::Platform platform;
//	cl::Device device;
//	cl::Context context;
//	cl::Program program;
//	cl::Kernel kernel;
//	cl::CommandQueue queue;
//
//	cl::Buffer vec1Buffer, vec2Buffer, outputBuffer;
//	
//	std::vector<int> outputVec(32);
//	
//	try {
//		// User to select 1 device
//		if (select_one_device(&platform, &device)) {
//
//			// An 8-element array of ints that contains random values between 10 and 20.
//			int vec1[8]{};
//
//			// A 16-element array of ints. Initialise the first half of the array with values from
//			// 1 to 8 (i.e. 1, 2, 3, …, 8) and the second half with values from - 9 to - 2 (i.e. - 9, -8, -7, …, -2).
//			int vec2[16]{};
//
//			std::random_device rd;
//			std::mt19937 gen(rd());
//			std::uniform_int_distribution<> dis(10, 20);
//
//			for (int i = 0; i < 8; i++) {
//				vec1[i] = dis(gen);
//			}
//
//			for (int i = 0; i < 16; i++) {
//				vec2[i] = (i < 8) ? i + 1 : i - 17;
//			}
//
//			// Create a context
//			context = cl::Context(device);
//
//			// Create a queue
//			queue = cl::CommandQueue(context, device);
//
//			// Build the program
//			if (build_program(&program, &context, "MinZhanFoo_A2_T1.cl")) {
//
//				// Create a kernel
//				kernel = cl::Kernel(program, "MinZhanFoo_A2_T1");
//
//				// Create buffers to send as arguments
//				vec1Buffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * 8, &vec1[0]);
//				vec2Buffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * 16, &vec2[0]);
//				outputBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(int) * 32, NULL);
//
//				// Contents of vec1 and vec2 are to be passed to the kernel as input1 and input2, outputBuffer is to be passed as output
//				kernel.setArg(0, vec1Buffer);
//				kernel.setArg(1, vec2Buffer);
//				kernel.setArg(2, outputBuffer);
//
//				// Kernel is enqueued by the host
//				queue.enqueueTask(kernel);
//				std::cout << "Kernel enqueued." << std::endl;
//				std::cout << "--------------------" << std::endl;
//
//				// Read the output buffer back to the host
//				queue.enqueueReadBuffer(outputBuffer, CL_TRUE, 0, sizeof(int) * 32, &outputVec[0]);
//
//				// Calls validate() to process if output is correct
//				if (validate(vec1, vec2, outputVec)) {
//
//					// Display the output if correct
//					std::cout << "Results are correct.\n\nResults:" << std::endl;
//
//					for (int i = 0; i < 32; i++) {
//						if (i == 0) {
//							std::cout << "v" << std::setw(6) << " " << ":" << std::right << std::setw(3) << outputVec[i];
//						}
//						else if (i > 0 && i < 8) {
//							std::cout << std::right << std::setw(3) << " " << std::right << std::setw(3) << outputVec[i];
//						}
//						else if (i == 8) {
//							std::cout << "\nv1" << std::setw(5) << " " << ":" << std::right << std::setw(3) << outputVec[i];
//						}
//						else if (i > 8 && i < 16) {
//							std::cout << std::right << std::setw(3) << " " << std::right << std::setw(3) << outputVec[i];
//						}
//						else if (i == 16) {
//							std::cout << "\nv2" << std::setw(5) << " " << ":" << std::right << std::setw(3) << outputVec[i];
//						}
//						else if (i > 16 && i < 24) {
//							std::cout << std::right << std::setw(3) << " " << std::right << std::setw(3) << outputVec[i];
//						}
//						else if (i == 24) {
//							std::cout << "\nresults:" << std::right << std::setw(3) << outputVec[i];
//						}
//						else if (i > 24 && i < 32) {
//							std::cout << std::right << std::setw(3) << " " << std::right << std::setw(3) << outputVec[i];
//						}
//					}
//				} else {
//					quit_program("Results are incorrect.");
//				}
//			}
//		}
//	}
//	catch (cl::Error e) {
//		handle_error(e);
//	}
//	
//#ifdef _WIN32
//	std::cout << "\n\npress a key to quit...";
//	std::cin.ignore();
//#endif
//
//	return 0;
//}
//
//bool validate(int vec1[], int vec2[], std::vector<cl_int>& outputVec) {
//	bool outputVec_found = false, vec1_found = false;
//	
//	for (int i = 0; i < 8; i++) {
//		if (vec1[i] > 15 && outputVec[i] > 15) {
//			vec1_found = true;
//			outputVec_found = true;
//			break;
//		}
//	}
//
//	std::vector<int> combArr(&vec1[0], &vec1[8]);
//	std::copy(&vec2[0], &vec2[16], std::back_inserter(combArr));
//
//	if (std::equal(combArr.begin(), combArr.end(), outputVec.begin(), outputVec.begin() + 24)) {
//		combArr.clear();
//		
//		if (vec1_found && outputVec_found) {
//			for (int i = 0; i < 8; i++) {
//				if (vec1[i] > 15) {
//					combArr.push_back(vec2[i]);
//				} else if (vec1[i] <= 15) {
//					combArr.push_back(vec2[i + 8]);
//				}
//			}
//		} else if (!vec1_found && !outputVec_found) {
//			for (int i = 0; i < 4; i++) {
//				combArr.push_back(vec2[i]);
//				combArr.push_back(vec2[i + 8]);
//			}
//		}
//		return std::equal(combArr.begin(), combArr.end(), outputVec.begin() + 24, outputVec.begin() + 32);
//	}
//	return false;
//}