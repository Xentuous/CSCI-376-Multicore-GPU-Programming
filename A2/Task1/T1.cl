__kernel void MinZhanFoo_A2_T1 (__global int4* input1, 
								__global int* input2, 
								__global int* output) {
	__local int8 v, v1, v2;
	__private int8 results;
	
	v = (int8)(input1[0], input1[1]);
	v1 = vload8(0, input2);
	v2 = vload8(1, input2);
	
	results = (any(v > 15)) ? select(v1, v2, (v <= 15)) : (int8)(v1.lo, v2.lo);
	
	vstore8(v, 0, output);
	vstore8(v1, 1, output);
	vstore8(v2, 2, output);
	vstore8(results, 3, output);
}
