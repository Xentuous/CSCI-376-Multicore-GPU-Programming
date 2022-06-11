//#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
//#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions
//
//// C++ standard library and STL headers
//#include <algorithm>
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <string>
//#include <regex>
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
//std::vector<char> initCipher(signed int key_int, const std::vector<char>& initial);
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
//	std::string key = "";
//	
//	try {
//		if (select_one_device(&platform, &device)) {
//			// Create a context
//			context = cl::Context(device);
//
//			// Create a queue
//			queue = cl::CommandQueue(context, device);
//
//			// Get key from users
//			std::cout << "Enter an encryption key: ";
//			std::cin >> key;
//
//			// If key matches unsigned and signed int regex, and program builds successfully
//			if (std::regex_match(key, std::regex("-?\\d*"))) {
//				std::vector<char> initial = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
//											  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
//
//				std::vector<std::string> fileName = { "plaintext.txt", "2B_cipher.txt", "2B_decrypted.txt" };
//				
//				// Initialize cipher letters
//				signed int key_int = std::stoi(key);
//
//				// If key is 0 or 26, no shifting is needed, else shift cipher letters
//				std::vector<char> cipherLetters = (key_int == 0 || key_int == 26) ? initial : initCipher(key_int, initial);
//				
//				// Build program
//				if (build_program(&program, &context, "MinZhanFoo_A2_T2b.cl")) {
//					
//					// First iteration is to encrypt plaintext.txt, second iteration is to decrypt cipherCL.txt
//					for (int i = 0; i < 2; i++) {
//						std::vector<char> charLinesVec;
//						
//						std::ifstream textFile(fileName[i]);
//						std::string line;
//
//						while (!textFile.eof()) {
//							std::getline(textFile, line);
//
//							// Only uppercase when reading from plaintext.txt, the rest of fileName dont need to uppercase
//							if (i == 0) {
//								std::transform(line.begin(), line.end(), line.begin(), ::toupper);
//							}
//							
//							// Add \n after reading each line
//							line += '\n';
//
//							// store each character inside plaintextLine into vector
//							for (char c : line) {
//								charLinesVec.push_back(c);
//							}
//						}
//
//						std::vector<char> result(charLinesVec.size());
//
//						// Create kernel base on iteration number
//						kernel = (i == 0) ? cl::Kernel(program, "encrypt") : cl::Kernel(program, "decrypt");
//
//						// Set cl::Buffer objects to be used in kernel
//						cl::Buffer initialBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char) * 26, &initial[0]);
//						cl::Buffer cipherLettersBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char) * 26, &cipherLetters[0]);
//						cl::Buffer characterBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(char) * charLinesVec.size());
//						cl::Buffer plaintextCharBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(char) * charLinesVec.size(), &charLinesVec[0]);
//						cl::Buffer resultBuffer = cl::Buffer(context, CL_MEM_READ_WRITE, sizeof(char) * charLinesVec.size());
//
//						// Set kernel arguments
//						kernel.setArg(0, initialBuffer);
//						kernel.setArg(1, cipherLettersBuffer);
//						kernel.setArg(2, plaintextCharBuffer);
//						kernel.setArg(3, characterBuffer);
//						kernel.setArg(4, resultBuffer);
//
//						// Utilise max gpu work-items size
//						cl::NDRange offset (0);
//						cl::NDRange globalSize(1024);
//
//						// Enqueue kernel with offset = 0 and globalSize = 1024 (max gpu working size)
//						queue.enqueueNDRangeKernel(kernel, offset, globalSize);
//						std::cout << "Kernel enqueued." << std::endl;
//						std::cout << "---------------------" << std::endl;
//
//						// Read result from device to host
//						queue.enqueueReadBuffer(resultBuffer, CL_TRUE, 0, sizeof(char) * charLinesVec.size(), &result[0]);
//
//						// Remove last element ('\n') in result vector
//						result.pop_back();
//
//						// output result to fileName[]
//						std::ofstream textfileCL(fileName[i + 1]);
//
//						// Output to 2B_cipher.txt or 2B_decrypted.txt
//						for (char c : result) {
//							textfileCL << c;
//						}
//						textfileCL.close();
//						
//						// Clear result vector
//						result.clear();
//					}
//				}
//			}
//		}
//	} catch (cl::Error e) {
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
//// Initialize cipher letters
//std::vector<char> initCipher(signed int key_int, const std::vector<char>& initial) {
//	signed int diff;
//	std::vector<char> cipher;
//
//	// If key_int is negative, get difference by += 26 then add from key_int to end of vector, then add from beginning to key_int position
//	// If key_int is positive, add from beginning to key_int position, then add from key_int to end of vector
//	if (key_int < 0 || key_int < -26) {
//		while (key_int < -26) {
//			key_int %= 26;
//		}
//		key_int += 26;
//	}
//	else if (key_int > 26) {
//		while (key_int > 26) {
//			key_int %= 26;
//		}
//	}
//
//	std::copy(initial.begin() + key_int, initial.end(), std::back_inserter(cipher));
//	std::copy(initial.begin(), initial.begin() + key_int, std::back_inserter(cipher));
//	
//	return cipher;
//}