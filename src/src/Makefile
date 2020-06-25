PATH=.
FILENAME=main.cpp

docker:
	g++ $(PATH)/$(FILENAME) -o program -I${OCL_INC} -Iinclude -lOpenCL -L${OCL_LIB}

devcloud:
	g++ $(PATH)/$(FILENAME) -o program -lOpenCL
