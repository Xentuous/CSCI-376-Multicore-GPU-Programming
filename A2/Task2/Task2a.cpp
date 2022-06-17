#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <vector>

std::vector<char> initCipher(signed int key_int, const std::vector<char>& initial);
void dencryptor(std::vector<char> initial, std::vector<char> cipherLetters, signed int key_int);

int main() {
	std::string key = "";
	
	// Get key from users
	std::cout << "Enter an encryption key: ";
	std::cin >> key;
	
	// If key matches unsigned and signed int regex
	if (std::regex_match(key, std::regex("-?\\d*"))) {
		std::vector<char> cipherLetters;
		std::vector<char> initial = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 
									  'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };
		
		// Initialize cipher letters
		signed int key_int = std::stoi(key);
		
		// If key is 0 or 26, no shifting is needed, else shift cipher letters
		cipherLetters = (key_int == 0 || key_int == 26 || key_int == -26) ? initial : initCipher(key_int, initial);
		
		// Encrypt plaintext.txt -> cipher.txt then decrypt cipher.txt -> decrpyted.txt
		dencryptor(initial, cipherLetters, key_int);
	}
}

// Initialize cipher letters
std::vector<char> initCipher(signed int key_int, const std::vector<char>& initial) {
	std::vector<char> cipher;

	// If key_int is negative, get difference by += 26 then add from key_int to end of vector, then add from beginning to key_int position
	// If key_int is positive, add from beginning to key_int position, then add from key_int to end of vector
	if (key_int < 0 || key_int < -26) {
		while (key_int < -26) {
			key_int %= 26;
		}
		key_int += 26;
	}
	else if (key_int > 26) {
		while (key_int > 26) {
			key_int %= 26;
		}
	}

	std::copy(initial.begin() + key_int, initial.end(), std::back_inserter(cipher));
	std::copy(initial.begin(), initial.begin() + key_int, std::back_inserter(cipher));
	
	return cipher;
}

// Encrypt then decrypt plaintext -> cipher.txt -> decrypted.txt
void dencryptor(std::vector<char> initial, std::vector<char> cipherLetters, signed int key_int) {
	// Create a vector of filenames
	std::vector<std::string> fileNames = { "plaintext.txt", "2A_cipher.txt", "2A_decrypted.txt" };
		
	// Create a vector to hold strings, to be used for output of textfile
	std::vector<std::string> stringsVec;
	
	// First iteration to encrypt, second iteration to decrypt
	for (int i = 0; i < 2; i++) {
		std::ifstream textfile(fileNames[i]);
		std::string str;

		while (!textfile.eof()) {
			// Get each line from the file
			std::getline(textfile, str);

			// If fileName[i] is "plaintext.txt" (i equals to index 0), then we want to encrypt the string
			if (i == 0) {
				// Convert all to uppercase
				std::transform(str.begin(), str.end(), str.begin(), ::toupper);
				
				// Iterate each char
				for (int i = 0; i < str.length(); i++) {
					// If specialChar does not contain str[i] continue
					if (
						(int)str[i] >= 8 && (int)str[i] <= 15  || (int)str[i] >= 32 && (int)str[i] <= 64   ||
						(int)str[i] >= 91 && (int)str[i] <= 96 || (int)str[i] >= 123 && (int)str[i] <= 126 ||
						str[i] == ' ') {
						str[i] = str[i];
					}
					if (str[i] >= 65 && str[i] <= 90) {
						str[i] = cipherLetters[(26 + str[i] - 65) % 26];
					}
				}
			}
			// If fileName[i] is "cipher.txt" (i equals to index 1), then we want to decrypt the string
			else if (i == 1) {
				for (int i = 0; i < str.length(); i++) {
					if (
						(int)str[i] >= 8 && (int)str[i] <= 15  || (int)str[i] >= 32 && (int)str[i] <= 64   ||
						(int)str[i] >= 91 && (int)str[i] <= 96 || (int)str[i] >= 123 && (int)str[i] <= 126 ||
						str[i] == ' ' ) {
						str[i] = str[i];
					}
					if (str[i] >= 65 && str[i] <= 90) {
						str[i] = initial[(26 + str[i] - cipherLetters[0]) % 26];
					}
				}
			}
			// Pushback to stringsVec
			stringsVec.push_back(str);
		}

		// output encryptedVec into a textfile called "cipher.txt", "decrypted.txt"
		std::ofstream outputFile(fileNames[i + 1]);

		for (int i = 0; i < stringsVec.size(); i++) {
			outputFile << stringsVec[i];

			if (i < stringsVec.size() - 1) {
				outputFile << std::endl;
			}
		}
		outputFile.close();
		stringsVec.clear();
		
		std::cout << (i == 0 ? "Encrypted" : "Decrypted") << " successfully!" << std::endl;
	}
}
