#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <cctype>
#include <cstdio>

using namespace std;

void showmem(const vector<unsigned char>& tape) {
	cout << "\nMemory dump:\n";
	
	const int chunk_size = 8;
	
	for (size_t i = 0; i < tape.size(); i += chunk_size) {
		// offset
		printf("%08lx  ", i);
		
		// hex bytes
		for (size_t j = i; j < i + chunk_size && j < tape.size(); j++) {
			printf("%02x ", tape[j]);
		}
		
		// padding for shorter last line
		for (size_t j = tape.size() - i; j < chunk_size; j++) {
			printf("   ");
		}
		
		printf("  ");
		
		// ASCII (safe)
		for (size_t j = i; j < i + chunk_size && j < tape.size(); j++) {
			unsigned char b = tape[j];
			if (isprint(b) || b == ' ') {
				printf("%c", (char)b);
			} else {
				printf(".");
			}
		}
		
		printf("\n");
	}
}

void bf(const string& code) {
	auto start = chrono::high_resolution_clock::now();
	
	// Initialize tape and pointers
	vector<unsigned char> tape(64, 0);
	size_t ptr = 0;
	size_t max_ptr = 0;
	
	// Build jump table
	vector<size_t> jump(code.length(), 0);
	vector<size_t> stack;
	for (size_t i = 0; i < code.length(); i++) {
		if (code[i] == '[') {
			stack.push_back(i);
		}
		if (code[i] == ']') {
			size_t j = stack.back();
			stack.pop_back();
			jump[i] = j;
			jump[j] = i;
		}
	}
	
	// Execute
	size_t ip = 0;
	while (ip < code.length()) {
		switch (code[ip]) {
			case '>':
				ptr++;
				max_ptr = max(max_ptr, ptr);
				// Grow tape if needed
				if (ptr >= tape.size()) {
					tape.resize(ptr + 1, 0);
				}
				break;
			case '<':
				ptr--;
				break;
			case '+':
				tape[ptr]++;
				break;
			case '-':
				tape[ptr]--;
				break;
			case '.':
				putchar(tape[ptr]);
				break;
			case ',':
				tape[ptr] = getchar();
				break;
			case '[':
				if (tape[ptr] == 0) {
					ip = jump[ip];
				}
				break;
			case ']':
				if (tape[ptr] != 0) {
					ip = jump[ip];
				}
				break;
			default:
				break;
		}
		ip++;
	}
	
	auto elapsed = chrono::high_resolution_clock::now() - start;
	
	cout << "\n---\n";
	cout << "execution time = " << chrono::duration_cast<chrono::microseconds>(elapsed).count() << " us\n";
	cout << "max memory usage = " << hex << (max_ptr + 1) << " bytes\n";
	
	showmem(tape);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cerr << "Usage: " << argv[0] << " <file_path>" << endl;
		return 1;
	}
	
	// Read file
	ifstream file(argv[1]);
	if (!file) {
		cerr << "Could not read file" << endl;
		return 1;
	}
	
	string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	file.close();
	
	bf(code);
	
	return 0;
}
