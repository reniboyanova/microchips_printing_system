#ifndef ORDER_MESSAGES_H
#define ORDER_MESSAGES_H
#include <string>

#define HARDWARE_POSITION_IN_MAP 2
#define EXIT_OHM 6
#define PRIORITY_POSITION_IN_MAP  1
#define CLIENT_NAME_IN_MAP  0

// const std::string HARDWARE_COMPONENTS_LIBRARY_FILE_PATH =  "../../lib/HardwareComponentsLib.txt";
// const std::string ELECTRONIC_COMPONENTS_LIBRARY_FILE_PATH = "../../lib/ElectronicComponentsLib.txt";
const std::string INCOME_ORDERS = "income_orders.txt";

const std::string WELCOME_MENU_MSG = "Make your choise of using Ohm:\n0 - adding orders by file;\n1 - adding order by CIN;\n2 - for show orders in order they ware made;\n3 - show orders by HIGH priority first;\n4 - show orders by NORMAL priority first;\n5 - cancel order by it's ID;\n6 - EXIT;";
const std::string GET_NEW_ORDER_MSG = "Use blueprint below:";
const std::string GET_CLIENT_NAME_MSG = ">>Insert Client Name:";
const std::string GET_PRIORITY_MSG = ">>Priority (0 for NORMAL; 1 for HIGH):";
const std::string GET_HARDWARE_ID_MSG = ">>Insert HARDWARE ID:";
const std::string INVALID_MENU_OPTIONS_MSG = "Invalid option. Please try again.";

const std::string FOUND_COMPONENT = "Searched component is found in: ";
const std::string NOT_FOUND_HARDWARE_COMPONENT = "Hardware component is NOT found in HardwareComponentsLib.txt!";
const std::string SUCCESS_ORDER = "Your order is successfully set!";
const std::string NOT_SUCCESS_ORDER = "Your order is NOT successfully set!";

const std::string UNABLE_OPEN_FILE = "Unable to open file!";
const std::string CANSEL_ORDER_MSG = "Which order do you want to cancel? Please insert the number of order: ";
const std::string CANT_FIND_ORDER_BY_ID = "Sorry! Order with this ID number is not find! Try again!";
const std::string SUCCESS_CANSEL_ORDER = "You successfuly cansel order from orders!";
const std::string SUCCESS_DESERIALIZE_ORDERS = "Now all the orders from order.txt is successfuly pushed in orders!";


const std::string GET_HARDWARE_WIDTH = "Please, insert the widht of hardware component you want: ";
const std::string GET_HARDWARE_HIGHT = "Please, insert the hight of hardware component you want: ";
const std::string GET_NUMBER_OF_COMPONENTS = "Please, insert number of components: ";
const std::string GET_ELECTRONIC_COMPONENT_ID = "Please, insert electronic components ID: ";
const std::string GET_COORDINATES_WITH_ROTATION = "Please, insert the current electronic element coordinats with roation in this format\nx,y,rotation (from 0 to 3 inclusive for rotation): ";
const std::string GET_CONECTIONS = "Please, insert connections in following format\n(el_1.el_1_number_of_pin, el_2.el_2_number_of_pin), (el_1.el_1_number_of_pin, el_2.el_2_number_of_pin), ...: ";
const std::string GET_EL_ELEMENT_W = "Please insert width of el element:";
const std::string GET_EL_ELEMENT_H = "Please insert hight of el element:";
const std::string GET_PINS_POSITIONS = "Please insert the PINS position in el element in following format: For example if pins are 3: (x,y),(x,y),(x,y)";

const std::string MAKE_ORDER_OR_NOT = "Your choose in uncorrect! Do you want to make new order? (y/n):";
const std::string GOOD_BYE_MASSAGE = "Thank you for using ASAPML 1.0\nWelcome back!";

const std::string SEMICOLON_WITH_SPACE = "; ";
const char SEMICOLON = ';';
const std::string COLON_WITH_SPACE = ": ";
const char COLON = ':';
const std::string DOUBLE_CURLLY_BRACKET_OPEN = "{{";
const std::string DOUBLE_CURLLY_BRACKET_CLOSE = "}}";

const char YES_ANSWER = 'y';
const std::string READ_THIS_ORDER = "0";
const std::string SKIP_THIS_ORDER = "3";

const std::string FOUND_ELECTRONIC_COMPONENT_IN_LIBRARY = "Found the elsectronic components in library with ID: ";
const std::string INSERT_INFO_FOR_EL_COMP_WITH_ID = "Insert the info about EL COMPONENT with ID: ";

const std::string DELIMITER_CONNECTIONS = "connections: ";
const std::string FIND_COMPONENTS = "components:";
const std::string NOT_FULL_ORDER_INFO = "Order DOES NOT contains full info!";

#endif