#include "md5_opencl.cpp"

int main(int argc, char *argv[])
{
	//md5_hash();
	generate(5);
	md5_hash(false);
	return EXIT_SUCCESS;
}
