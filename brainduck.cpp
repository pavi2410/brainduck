#include <iostream>
#include <string>

#define MEMSIZE 96

#define ESC "\x1b["

using namespace std;


void showmem(char array[]) {
	for (int i = 0; i < MEMSIZE / 16; i++) {
		printf("%08x : ", i);
		for (int j = 0; j < 16; j++)
			printf("%02x ", (unsigned char) array[16*i+j]);
		printf("\n");
	}
}


int main(int argc, char* argv[]) {

	string input;
	cin >> input;
	cout << input << endl;

	char array[MEMSIZE] = {0};
	char *ptr = array;
	cout << "---\n";
	for (int pos = 0; pos < input.size(); pos++) {
		char c = input[pos];

		//showmem(array);
		//printf("pos = %d\n", pos);
		//printf("ptr = %ld (%d)\n", ptr - array, *ptr);
		//printf(ESC "8A");
		

		switch (c) {
			case '>': ++ptr; break;
			case '<': --ptr; break;
			case '+': ++*ptr; break;
			case '-': --*ptr; break;
			case '.': putchar(*ptr); break;
			case ',': *ptr = getchar(); break;
			case '[': if (*ptr) ; else {
				int balance = 1;
				while (balance !=0) {
					pos++;
					c = input[pos];
					if (c == '[') balance++;
					if (c == ']') balance--;
				}
				pos++;
			}; break;
			case ']': if (*ptr) {
				int b = 1;
				while (b != 0) {
					pos--;
					c = input[pos];
					if (c == '[') b--;
					if (c == ']') b++;
				}
			}; break; 
			default:;
			//cout << "unrecognized char " << c << endl; exit(1);
		}
	}
	cout << "\n---\n";

	showmem(array);

	return 0;
}
