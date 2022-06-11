//#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
//#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions
//
//// C++ standard library and STL headers
//#include <iostream>
//#include <vector>
//#include <fstream>
//
//// OpenCL header, depending on OS
//#ifdef __APPLE__
//#include <OpenCL/cl.hpp>
//#else
//#include <CL/cl.hpp>
//#endif
//
//#include "common.h"
//#include "bmpfuncs.h"
//
//int main(void)
//{
//	cl::Platform platform;			// device's platform
//	cl::Device device;				// device used
//	cl::Context context;			// context for the device
//	cl::Program program;			// OpenCL program object
//	cl::Kernel kernel;				// a single kernel object
//	cl::CommandQueue queue;			// commandqueue for a context and device
//
//	// Declare data and memory objects
//	unsigned char* inputImage;
//	unsigned char* outputImage;
//	int imgWidth, imgHeight, imageSize;
//
//	// Image objects
//	cl::ImageFormat imgFormat;
//	cl::Image2D inputImgBuffer, outputImgBuffer;
//
//	try {
//		// Select an OpenCL device
//		if (select_one_device(&platform, &device)) {
//			// Create a context from device
//			context = cl::Context(device);
//
//			// Create command queue
//			queue = cl::CommandQueue(context, device);
//
//			std::vector<std::string> kernalNames = { "MinZhanFoo_A2_T3b.cl", "MinZhanFoo_A2_T3bii.cl" };
//			
//			// Build the program
//			for (int tasksNumber = 0; tasksNumber < 2; tasksNumber++) {			
//				std::cout << "--------------------" << std::endl;
//				std::cout << "Task 3b (" << (tasksNumber == 0 ? "i)" : "ii)") << std::endl;
//				std::cout << "--------------------" << std::endl;
//
//				// Proceeds to try building the program
//				if (build_program(&program, &context, kernalNames[tasksNumber])) {
//					const char* inputImageName = " ", * outputImageName = " ";
//					
//					// Since there are 2 subtask in 3bii, has 2 kernels functions -> 2 iterations				
//					for (int kernelFunction = 0; kernelFunction < 2; kernelFunction++) {
//						if (tasksNumber == 0) {
//							kernel = cl::Kernel(program, "gaussian");
//							inputImageName = "peppers.bmp";
//							outputImageName = "3B_Gaussian.bmp";
//							
//						} else if (tasksNumber == 1 && kernelFunction == 0) {
//							kernel = cl::Kernel(program, "gaussianH");
//							inputImageName = "peppers.bmp";
//							outputImageName = "3B_2Pass.bmp";
//
//						} else if (tasksNumber == 1 && kernelFunction == 1) {
//							kernel = cl::Kernel(program, "gaussianV");
//							inputImageName = "3B_2Pass.bmp";
//							outputImageName = "3B_2Pass.bmp";
//						}				
//						
//						// Read 24-bit input image to RGBA format with calculated imgWidth and imgHeight values 
//						inputImage = read_BMP_RGB_to_RGBA(inputImageName, &imgWidth, &imgHeight);
//
//						// Allocate memory for output image, * 4 for RGBA values, 1 byte 8 bits
//						imageSize = imgWidth * imgHeight * 4;
//						outputImage = new unsigned char[imageSize];
//
//						// Image format for input and output images, CL_UNORM_INT 8 = H (0 to 255) D (0.0 to 1.0)
//						imgFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);
//
//						// Create image objects
//						inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
//						outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);
//
//						// Set kernel arguments
//						kernel.setArg(0, inputImgBuffer);
//						kernel.setArg(1, outputImgBuffer);
//
//						// Enqueue kernel
//						cl::NDRange offset(0, 0);
//						cl::NDRange globalSize(imgWidth, imgHeight);
//
//						queue.enqueueNDRangeKernel(kernel, offset, globalSize);
//
//						std::cout << "Kernel enqueued." << std::endl;
//						std::cout << "--------------------" << std::endl;
//
//						// Enqueue command to read image from device to host memory
//						cl::size_t<3> origin, region;
//						origin[0] = origin[1] = origin[2] = 0;
//						region[0] = imgWidth;
//						region[1] = imgHeight;
//						region[2] = 1;
//
//						queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);
//
//						// Output results to image file to 24-bit RGB bitmap file
//						write_BMP_RGBA_to_RGB(outputImageName, outputImage, imgWidth, imgHeight);
//
//						if (tasksNumber == 1 && kernelFunction == 0) {
//							std::cout << "Horizontal Pass done." << std::endl;
//						} else if (tasksNumber == 1 && kernelFunction == 1) {
//							std::cout << "Vertical Pass done." << std::endl;
//						}
//						
//						std::cout << "Done." << std::endl;
//						
//						// Deallocate memory
//						free(inputImage);
//
//						// Deallocate memory using delete
//						delete[] outputImage;
//
//						// If tasknumber is Task 3b (i) break loop to move on to Task 3b (ii)
//						if (tasksNumber == 0) {
//							break;
//						}
//					}	
//				}
//			}
//		}
//	} catch (cl::Error e) {
//		handle_error(e);
//	}
//
//#ifdef _WIN32
//	std::cout << "\npress a key to quit...";
//	std::cin.ignore();
//#endif
//	return 0;
//}
