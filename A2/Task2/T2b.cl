__kernel void encrypt(__global char* initial,
                      __global char* cipherLetters,
					  __global char* plaintextChar,
					  __global char* character,
					  __global char* result
					  ) {
	
	int gid = get_global_id(0);

	__local char16 plaintext1;

	for (int i = gid * 16; i < 16 * (gid + 1); i++) {
		if (
			(int) plaintextChar[i] >= 8   && (int) plaintextChar[i] <= 15  ||
			(int) plaintextChar[i] >= 32  && (int) plaintextChar[i] <= 64  ||
			(int) plaintextChar[i] >= 91  && (int) plaintextChar[i] <= 96  ||
			(int) plaintextChar[i] >= 123 && (int) plaintextChar[i] <= 126 ||
			plaintextChar[i] == ' '
			) {
			character[i] = plaintextChar[i];
		}
		if (plaintextChar[i] >= 65 && plaintextChar[i] <= 90) {
			character[i] = cipherLetters[(26 + plaintextChar[i] - 65) % 26];
		}
	}
	
plaintext1 = vload16(gid, character);

vstore16(plaintext1, gid, result);
}

__kernel void decrypt(__global char* initial,
                      __global char* cipherLetters,
					  __global char* plaintextChar,
					  __global char* character,
					  __global char* result
					  ) {
	int gid = get_global_id(0);
	
	__local char16 plaintext1;

		for (int i = gid * 16; i < 16 * (gid + 1); i++) {
		if (
			(int) plaintextChar[i] >= 8   && (int) plaintextChar[i] <= 15  ||
			(int) plaintextChar[i] >= 32  && (int) plaintextChar[i] <= 64  ||
			(int) plaintextChar[i] >= 91  && (int) plaintextChar[i] <= 96  ||
			(int) plaintextChar[i] >= 123 && (int) plaintextChar[i] <= 126 ||
			plaintextChar[i] == ' '
			) {
			character[i] = plaintextChar[i];
		}
		if (plaintextChar[i] >= 65 && plaintextChar[i] <= 90) {
			character[i] = initial[(26 + plaintextChar[i] - cipherLetters[0]) % 26];
		}
	}
	
plaintext1 = vload16(gid, character);

vstore16(plaintext1, gid, result);

}
