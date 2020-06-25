#ifndef __AUX
#define __AUX

void combine(std::vector<data> * vec, int num, std::string palavra) {
    data aux;
    sprintf(aux.value, "%s", (palavra + '\0').c_str());
    vec->push_back(aux);
    if (num == SIZE_RAW_TEXT) {
        return;
    }
    for (int i = 0; i < 10; i++) {
        combine(vec, num + 1, palavra + alphabet[i]);
    }
}

/*
func loadProgram
unknown author, provided by UFSCar
*/
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
#endif