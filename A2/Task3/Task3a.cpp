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
//			// Build the program
//			if (build_program(&program, &context, "MinZhanFoo_A2_T3a.cl")) {
//				// Create a kernel
//				kernel = cl::Kernel(program, "luminate");
//
//				// Read 24-bit input image to RGBA format with calculated imgWidth and imgHeight values 
//				inputImage = read_BMP_RGB_to_RGBA("peppers.bmp", &imgWidth, &imgHeight);
//
//				// Allocate memory for output image, * 4 for RGBA values, 1 byte 8 bits
//				imageSize = imgWidth * imgHeight * 4;
//				outputImage = new unsigned char[imageSize];
//
//				// Image format for input and output images, CL_UNORM_INT 8 = H (0 to 255) D (0.0 to 1.0)
//				imgFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);
//
//				// Create image objects
//				inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
//				outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);
//
//				// Set kernel arguments
//				kernel.setArg(0, inputImgBuffer);
//				kernel.setArg(1, outputImgBuffer);
//
//				// Enqueue kernel
//				cl::NDRange offset(0, 0);
//				cl::NDRange globalSize(imgWidth, imgHeight);
//
//				queue.enqueueNDRangeKernel(kernel, offset, globalSize);
//
//				std::cout << "Kernel enqueued." << std::endl;
//				std::cout << "--------------------" << std::endl;
//
//				// Enqueue command to read image from device to host memory
//				cl::size_t<3> origin, region;
//				origin[0] = origin[1] = origin[2] = 0;
//				region[0] = imgWidth;
//				region[1] = imgHeight;
//				region[2] = 1;
//
//				queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);
//
//				// Output results to image file to 24-bit RGB bitmap file
//				write_BMP_RGBA_to_RGB("3A_output.bmp", outputImage, imgWidth, imgHeight);
//
//				std::cout << "Done." << std::endl;
//
//				// Deallocate memory
//				free(inputImage);
//				
//				// OutputImage uses new, need use delete
//				delete[] outputImage;
//			}
//		}
//	}
//	catch (cl::Error e) {
//		handle_error(e);
//	}
//
//#ifdef _WIN32
//	std::cout << "\npress a key to quit...";
//	std::cin.ignore();
//#endif
//	return 0;
//}
