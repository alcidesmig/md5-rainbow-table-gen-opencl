#include "md5_opencl.hpp"

//g++ -Wno-deprecated md5_opencl.cpp -o program -I${OCL_INC} -Iinclude -lOpenCL -L${OCL_LIB}

void write_to_file(std::vector<data> & plain, std::vector<hashed_data> & hashed) {

    FILE * fp = fopen(OUTPUT_FILE, "w");

    for (int i = 0; i < plain.size(); i++) {
        fprintf(fp, "%s:", (char *) plain[i].value);
        for (int j = 0; j < 16; j++) {
            fprintf(fp, "%2.2x", hashed[i].value[j]);
        }
        fprintf(fp, "\n");
    }

}

int md5_hash()
{

    /* Prepare data */
    std::vector<data> plain; // Host memory for plain PINs
    std::string z = "";
    combine(&plain, 1, z);
    int size = plain.size();
    std::vector<hashed_data> hashed(size); // Host memory for store hashed PINs

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

    /* Create buffers */
    cl::Buffer d_plain = cl::Buffer(context, plain.begin(), plain.end(), true);
    cl::Buffer d_hash = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(hashed_data) * size);

    /* Read kernel and create program */
    cl::Program program(context, loadProgram(KERNEL_FILE), true);
    cl::make_kernel<cl::Buffer, cl::Buffer> md5_hash(program, "md5_hash");

    /* Enqueue tasks */
    md5_hash(
        cl::EnqueueArgs(
            queue,
            cl::NDRange(size)),
        d_plain, d_hash);

    queue.finish();

    /* Copy from device to cpu */
    cl::copy(queue, d_hash, hashed.begin(), hashed.end());

    /* Write results in OUTPUT_FILE */
    write_to_file(plain, hashed);

    return EXIT_SUCCESS;
}
