__kernel void task3 (int option, 
					 __global int* result) {

	int i  = get_global_id(0);
	result[i] = option * i + 1;
}
