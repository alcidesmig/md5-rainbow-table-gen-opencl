#ifndef __MD5HPP
#define __MD5HPP


#define __CL_ENABLE_EXCEPTIONS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <CL/cl.hpp>
#include <time.h>

// SIZE_RAW_TEXT is the size + 1 of the PIN
#define SIZE_RAW_TEXT 5
// output file to write the rainbow table
#define OUTPUT_FILE "./raint.md5"
// kernel file
#define KERNEL_FILE "./kernel/md5.cl"

char alphabet[10] = {'0', '1', '2', '3', '4', '5', '6',
                 '7', '8', '9'};

typedef struct {
    char value[SIZE_RAW_TEXT];
} data;

typedef struct {
    uint8_t value[16];
} hashed_data;

#include "device_picker.hpp"
#include "aux.cpp"


int md5_hash();
void write_to_file(std::vector<data> & plain, std::vector<hashed_data> & hashed);

#endif
