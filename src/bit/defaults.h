#define DEFAULT_PRINT_FILE "print_jobs.txt"
#define TEMP_PRINT_FILE "tempPrint_Jobs.txt"
#define PRINT_ERROR_FILE "print_jobs_canceled.txt"
#define DEFAULT_DATA_PATH "../../data/"
#define DEFAULT_PRODUCTION_FILE "production.txt"
#define EL_COMP_LIB "../../lib/ElectronicComponentsLib.txt"
#define HARD_COMP_LIB "../../lib/HardwareComponentsLib.txt"
#define CFG_FILE "cfg_bit.txt"
#define SLEEP_TIME "sleepStep"
#define NO_ROTATION 0
#define ROTATE_90d 1
#define ROTATE_180d 2
#define ROTATE_270d 3

using FILENAME_PATH = std::string;
using MATRIX = std::vector<std::vector<std::string>>;
using MATRIX_INT = std::vector<std::vector<int>>;
