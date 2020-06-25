#include "md5_opencl.hpp"
#include "device_picker.hpp"
#include <string>
#include <fstream>
typedef struct {
    char value[5];
} data;

typedef struct {
    uint8_t value[16];
} hashed_data;

std::string loadProgram(std::string input)
{
    std::ifstream stream(input.c_str());
    if (!stream.is_open()) {
        std::cout << "Cannot open file: " << input << std::endl;
        exit(1);
    }

    return std::string(
               std::istreambuf_iterator<char>(stream),
               (std::istreambuf_iterator<char>()));
}

int main(int argc, char *argv[])
{

    int size = 4;      // Number of elements to be hashed


    std::vector<data> plain(size); // Host memory for plain PINs
    std::vector<hashed_data> hashed(size); // Host memory for store hashed PINs

    for (int i = 0; i < size; i++) {
        strcpy(plain[i].value, "TEST\0");
        printf("%s\n", plain[i].value);
   }

    cl_uint deviceIndex = 0;

    // Get list of devices
    std::vector<cl::Device> devices;
    unsigned numDevices = getDeviceList(devices);
    std::cout << "Devices " << devices.size();
    if (deviceIndex >= numDevices)
    {
        std::cout << "Invalid device index (try '--list')\n";
        return EXIT_FAILURE;
    }

    cl::Device device = devices[deviceIndex];

    std::string name;
    getDeviceName(device, name);
    std::cout << "\nUsing OpenCL device: " << name << "\n";
    printf("CPU\n");

    std::vector<cl::Device> chosen_device;
    chosen_device.push_back(device);
    cl::Context context(chosen_device);
    cl::CommandQueue queue(context, device);

    cl::Buffer d_plain = cl::Buffer(context, plain.begin(), plain.end(), true);
    cl::Buffer d_hash = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(hashed_data) * size);

    cl::Program program(context, loadProgram("kernel/md5.cl"), true);
    cl::make_kernel<cl::Buffer, cl::Buffer> md5_hash(program, "md5_hash");
    md5_hash(
        cl::EnqueueArgs(
            queue,
            cl::NDRange(size)),
        d_plain, d_hash);

    queue.finish();

    cl::copy(queue, d_hash, hashed.begin(), hashed.end());
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 16; j++) {
            printf("%2.2x", hashed[i].value[j]);
        }
        printf("\n");
    }



    return EXIT_SUCCESS;
}
