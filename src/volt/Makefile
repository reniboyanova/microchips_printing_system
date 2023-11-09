CC=g++
CCFLAGS = -std=c++14 -Wall -Wextra

TARGET = volt.exe
SRC = main.cpp ProcessManagement.cpp UpdatedesignLibrary.cpp ./../shared/HardwareComponent.cpp ./../shared/ElectronicComponent.cpp ./../shared/Board.cpp ./../shared/sparser.cpp ./../shared/Utils.cpp 

$(TARGET): $(SRC)
	$(CC) $(CCFLAGS) $(SRC) -o $(TARGET)