__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | 
							   CLK_ADDRESS_CLAMP | 
							   CLK_FILTER_NEAREST; 

__constant float blurring[49] = {
	0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036,
	0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363,
	0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446,
	0.002291, 0.023226, 0.092651, 0.146768, 0.092651, 0.023226, 0.002291,
	0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446,
	0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363,
	0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036 
	};			
	
__kernel void gaussian(read_only image2d_t src, 
					   write_only image2d_t dst) {
					
	int col = get_global_id(0);
	int row = get_global_id(1);
	int filter = 0;

	// Accumulated pixel value
	float4 sum = (float4)(0.0);

	// Pixel value to be added to the accumulator
	float4 pixel;

	// x,y coord
	int2 coord;

	// Iterate inside the 7x7 size, and apply the filter to the pixel
	for (int i = -3; i <= 3; i++) {
		
		// Get the row coordinate of the pixel to be filtered
		coord.y = row + i;
		
		// Iterate each column with given row coordinate
		for (int j = -3; j <= 3; j++) {
			// Get col coordinate of the pixel to be filtered
			coord.x = col + j;
			
			// Read image from src, sampler, cord
			pixel = read_imagef(src, sampler, coord);

			// Add RGB + blurring[filterIndex] to the accumulator
			sum.xyz += pixel.xyz * blurring[filter++];
		}
	}

	// Store final coord
	coord = (int2)(col, row); 

	// Output image
	write_imagef(dst, coord, sum);
}