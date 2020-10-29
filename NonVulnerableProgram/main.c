#include <stdlib.h>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <share.h>
#include <errno.h>
#include <stdio.h>
#include <inttypes.h>

__declspec(dllexport) int fuzz_target(char* filename);

int fuzz_target(char* filename) {
	//int fd;
	//int file_size;
	int bytes_read;

	/*
	_sopen_s(&fd, filename, _O_RDONLY, _SH_DENYRW, _S_IREAD);
	if (fd == -1) {
		printf("Error opening file.");
		return 1;
	}

	file_size = _filelength(fd);
	if (file_size == -1) {
		printf("Error getting file size.");
		return 2;
	}
	*/
	// open file 
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, filename, "rb");
	if (err != 0) {
		printf("Error reading file.");
		return 0;
	}

	// determine no of bytes 
	fseek(fp, 0, SEEK_END);
	int bytes_count = ftell(fp);
	rewind(fp);

	// dynamically allocate memory for file data
	unsigned char* buf = malloc(sizeof(unsigned char) * (bytes_count + 1));
	if (buf == NULL) {
		printf("Memory error occured.");
		return 3;
	}

	memset(buf, 0, sizeof(unsigned char) * (bytes_count + 1));
	if ((bytes_read = fread(buf, 1, bytes_count, fp)) <= 0) {
		printf("Problem reading file.");
		free(buf);
		return 4;
	}

	fclose(fp);

	if (bytes_read < 10) {
		printf("Fewer than 10 bytes were read.");
	}
	else if (bytes_read < 50) {
		printf("More than 10 bytes but fewer than 50 bytes were read.");
	}
	else if (bytes_read < 100) {
		printf("More than 50 bytes but fewer than 100 bytes were read.");
	}
	else if (bytes_read < 500) {
		printf("More than 100 bytes but fewer than 500 bytes were read.");
	}
	else {
		printf("More than 500 bytes were read.");
	}
	free(buf);
	return 5;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		printf("Usage: %s <input file>\n", argv[0]);
		return 0;
	}
	int code = fuzz_target(argv[1]);

	return code;
}