__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | 
                               CLK_ADDRESS_CLAMP | 
							   CLK_FILTER_NEAREST; 


__kernel void luminate (read_only image2d_t src,
						write_only image2d_t dst) {
		// Get X, Y axis
		int2 coord = (int2)(get_global_id(0), get_global_id(1));
		
		// Read image and get pixel
		float4 pixel = read_imagef(src, sampler, coord);
		
		// Convert to luminance of the pixel xyz = RGB
		float luminance = (0.299f * pixel.x) + (0.7152f * pixel.y) + (0.0722f * pixel.z);
		
		// write the luminance to the output image
		write_imagef(dst, coord, luminance);		
}