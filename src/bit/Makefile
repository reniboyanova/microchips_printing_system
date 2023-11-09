CC=g++
CCFLAGS = -std=c++14 -Wall -Wextra

TARGET = bit.exe
SRC = main.cpp PrintManager.cpp PrintJob.cpp ./../shared/HardwareComponent.cpp ./../shared/ElectronicComponent.cpp ./../shared/Board.cpp ./../shared/sparser.cpp ./../shared/Utils.cpp 

$(TARGET): $(SRC)
	$(CC) $(CCFLAGS) $(SRC) -o $(TARGET)
