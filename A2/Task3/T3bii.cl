__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | 
                               CLK_ADDRESS_CLAMP | 
                               CLK_FILTER_NEAREST; 


__constant float blurring2[7] = { 0.00598,  0.060626, 0.241843, 
                                  0.383103, 0.241843, 0.060626, 
                                  0.00598 
                                };

__kernel void gaussianH(read_only image2d_t src,
						write_only image2d_t dst) {
    int col = get_global_id(0); 
    int row = get_global_id(1);
    int filter =  0;
	
    // Accumulated pixel value
    float4 sum = (float4)(0.0);
    
    // x,y coord
    int2 coord;
	
    // Given fixed row value based on each work-item
    coord.y = row;
	
    // Iterate each col with given row coordinate
    for (int i = -3; i <= 3; i++) {
	    // Get col coordinate of the pixel to be filtered
        coord.x = col + i;
		
		// Read image from src, sampler, cord
		float4 pixel = read_imagef(src, sampler, coord);
		
        // Add RGB + blurring[filterIndex] to the accumulator
        sum.xyz += pixel.xyz * blurring2[filter++];
    }
   
    // Store final coord
    coord = (int2)(col, row); 
	
	// Output image
    write_imagef(dst, coord, sum);
}


__kernel void gaussianV(read_only image2d_t src,
						write_only image2d_t dst) {
    int col = get_global_id(0); 
    int row = get_global_id(1);
    int filter =  0;
	
    // Accumulated pixel value
    float4 sum = (float4)(0.0);
    
    // x,y coord
    int2 coord;
	
    // Given fixed col value based on each work-item
    coord.x = col;
	
	// Iterate each row with given col coordinate
    for (int i = -3; i <= 3; i++) {
	    // Get row coordinate of the pixel to be filtered
        coord.y = row + i;

		// Read image from src, sampler, cord
		float4 pixel = read_imagef(src, sampler, coord);

		// Add RGB + blurring[filterIndex] to the accumulator
		sum.xyz += pixel.xyz * blurring2[filter++];
    }

    // Store final coord
    coord = (int2)(col, row); 

	// Output image
    write_imagef(dst, coord, sum);
}
