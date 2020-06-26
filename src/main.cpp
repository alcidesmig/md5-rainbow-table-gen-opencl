#include "md5_opencl.cpp"

int main(int argc, char *argv[])
{
	generate(5);		// size of the bruteforce
	md5_hash(false);	// change here to true for write the hashes into a file
	return EXIT_SUCCESS;
}
