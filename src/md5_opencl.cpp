#include "md5_opencl.hpp"

//g++ -Wno-deprecated md5_opencl.cpp -o program -I${OCL_INC} -Iinclude -lOpenCL -L${OCL_LIB}

void write_to_file(std::vector<data> & plain, std::vector<hashed_data> & hashed) {

    FILE * fp = fopen(OUTPUT_FILE, "a");

    for (int i = 0; i < plain.size(); i++) {
        fprintf(fp, "%s:", (char *) plain[i].value);
        for (int j = 0; j < 16; j++) {
            fprintf(fp, "%2.2x", hashed[i].value[j]);
        }
        fprintf(fp, "\n");
    }

}

int md5_hash(bool write)
{

    /* Select device */
    cl_uint deviceIndex = 0; // change here your device (cpu, gpu, ...)
    std::vector<cl::Device> devices;
    unsigned numDevices = getDeviceList(devices);
    std::cout << "Number of devices: " << devices.size() << "\n";
    if (deviceIndex >= numDevices)
    {
        return EXIT_FAILURE;
    }
    cl::Device device = devices[deviceIndex];
    std::string name;
    getDeviceName(device, name);
    std::cout << "Using OpenCL device: " << name << std::endl;
    std::vector<cl::Device> chosen_device;
    chosen_device.push_back(device);

    /* Create context and task queue*/
    cl::Context context(chosen_device);
    cl::CommandQueue queue(context, device);
    
    /* Read kernel and create program */
    cl::Program program(context, loadProgram(KERNEL_FILE), true);
    cl::make_kernel<cl::Buffer, cl::Buffer> md5_hash(program, "md5_hash");


    /* Prepare data */
    /*
        std::vector<data> plain; // Host memory for plain PINs
        std::string z = "";
        combine(&plain, 1, z);
    */
    /* Prepare data */

    FILE * fp = fopen(INPUT_FILE, "r");
    fseek(fp, 0, SEEK_END);
    size_t max = ftell(fp);
    fclose(fp);

    double sum_time = 0;
    clock_t start, end;

    /* Split file */
    for (size_t file_start_read = 0; file_start_read < max; file_start_read += 1e8 - 10) {
        std::vector<data> plain;
        read_from_file(plain, file_start_read, 1e8);
        int size = plain.size();
        std::vector<hashed_data> hashed(size); // Host memory for store hashed PINs



        /* Create buffers */
        cl::Buffer d_plain = cl::Buffer(context, plain.begin(), plain.end(), true);
        cl::Buffer d_hash = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(hashed_data) * size);


        start = clock();

        /* Enqueue tasks */
        md5_hash(
            cl::EnqueueArgs(
                queue,
                cl::NDRange(size)),
            d_plain, d_hash);

        queue.finish();

        /* Copy from device to cpu */
        cl::copy(queue, d_hash, hashed.begin(), hashed.end());

        end = clock();

        sum_time += (double) end - start;

        /* Write results in OUTPUT_FILE */
        if (write) write_to_file(plain, hashed);
    }
    printf("%.4lf seconds\n", sum_time / CLOCKS_PER_SEC);

    return EXIT_SUCCESS;
}
