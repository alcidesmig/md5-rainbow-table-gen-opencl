#ifndef __MD5HPP
#define __MD5HPP


#define __CL_ENABLE_EXCEPTIONS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <fstream>
#include <CL/cl.hpp>
#include <time.h>

// SIZE_RAW_TEXT is the size + 1 for allowed words
#define SIZE_RAW_TEXT 7
// output file to write the rainbow table
#define OUTPUT_FILE "./raint.md5"
#define INPUT_FILE "./wordlist.txt"
// kernel file
#define KERNEL_FILE "./kernel/md5.cl"

const char * alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

typedef struct {
    char value[SIZE_RAW_TEXT];
    size_t size;
} data;

typedef struct {
    uint8_t value[16];
} hashed_data;

#include "device_picker.hpp"
#include "aux.cpp"


int md5_hash();
void write_to_file(std::vector<data> & plain, std::vector<hashed_data> & hashed);

#endif
