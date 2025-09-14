#include <iostream>
#include <fstream>
#include <concepts>
#include <sstream>
#include <iomanip>
#include <string>
#include <cctype>
#include <string_view>
#include <algorithm>
#include <stdexcept>
#include <cmath>


/////////////////// CONSTANTS ///////////////////

constexpr std::string_view STR_FIRST  = "first";
constexpr std::string_view STR_SECOND = "second";
constexpr std::string_view STR_NUMBER = "number";

/////////////////// MATH ///////////////////

enum class Operation 
{
    Add = 1, Sub, Div, Mul, Pow, Integral, Min, Max, Unknown
};

template <typename T>
auto min(const T& a, const T& b) 
{
    return !(b < a) ? a : b;
}

template <typename T>
auto max(const T& a, const T& b) 
{
    return (b < a) ? a : b;
}

double f(double x) 
{
    return 3*(x*x)+2*x+5;
}

// There a simpsons implementation of integral
// This is so bad realization. I need to improve it, but later. 
float integral(double lower_end, double high_end) 
{
    double a = lower_end;
    double b = high_end;
    double h = (b - a)/3;
    return ((3*h)/8)*(f(a) + 3*f((2*a+b)/3) + 3*f((a+2*b)/3) + f(b));
}

float pow(float x, int n) 
{
    if (n < 0) 
    {
        throw std::invalid_argument("Exponent must be positive");
    }
    switch (n) 
    {
        case 0: return 1;
        case 1: return x;
        default: return std::pow(x, n);
    }
}

/////////////////// OPERATION PARSING ///////////////////

std::string getOperationName(Operation operation) 
{
    switch (operation)
    {
    case Operation::Add: return "Addition";
    case Operation::Sub: return "Substraction";
    case Operation::Div: return "Division";
    case Operation::Mul: return "Multiplication";
    case Operation::Pow: return "Power";
    case Operation::Integral: return "Integral";
    case Operation::Min: return "Minimum";
    case Operation::Max: return "Maximum";
    default: return "Unknown";
    }
}

Operation parseOperation(const std::string& input) 
{
    std::string lower_input = input;
    std::transform(lower_input.begin(), lower_input.end(), lower_input.begin(), ::tolower);

    try 
    {
        int opNum = std::stoi(input);
        switch (opNum)
        {
            case 1: return Operation::Add;
            case 2: return Operation::Sub;
            case 3: return Operation::Div;
            case 4: return Operation::Mul;
            case 5: return Operation::Pow;
            case 6: return Operation::Integral;
            case 7: return Operation::Min;
            case 8: return Operation::Max;
            default: return Operation::Unknown;
        }
    }
    catch (...) 
    {
        if(lower_input == "add" || lower_input == "+") return Operation::Add;
        if(lower_input == "sub" || lower_input == "substract" || lower_input == "-") return Operation::Sub;
        if(lower_input == "div" || lower_input == "divide" || lower_input == "/") return Operation::Div;
        if(lower_input == "mul" || lower_input == "multiply" || lower_input == "*") return Operation::Mul;
        if(lower_input == "pow" || lower_input == "power" || lower_input == "^") return Operation::Pow;
        if(lower_input == "integral" || lower_input == "int") return Operation::Integral;
        if(lower_input == "min" || lower_input == "minimum") return Operation::Min;
        if(lower_input == "max" || lower_input == "maximum") return Operation::Max;

        return Operation::Unknown;
    }
}

Operation getOperationFromLine(const std::string& line) 
{
    std::istringstream iss(line);
    std::string tmp;
    int opNum = 0;

    iss >> tmp >> tmp >> tmp >> opNum;

    switch (opNum)
    {
    case 1: return Operation::Add;
    case 2: return Operation::Sub; 
    case 3: return Operation::Div;
    case 4: return Operation::Mul; 
    case 5: return Operation::Pow;
    case 6: return Operation::Integral; 
    case 7: return Operation::Min;
    case 8: return Operation::Max; 
    default: return Operation::Unknown;
    }
}


double calculate(double first_number, double second_number, Operation operation) 
{
    switch(operation) 
    {
        case Operation::Add: return first_number + second_number;
        case Operation::Sub: return first_number - second_number;
        case Operation::Div: if(second_number == 0) throw std::invalid_argument("Division by 0"); 
                             return first_number / second_number;
        case Operation::Mul: return first_number * second_number;
        case Operation::Pow: return pow(first_number, second_number); 
        case Operation::Integral: return integral(first_number, second_number);
        case Operation::Min: return min(first_number, second_number); 
        case Operation::Max: return max(first_number, second_number);
        default: throw std::invalid_argument("Unknown operation");
    }
}

/////////////////// FIO ///////////////////

void writeToFile(std::string pathFile, std::string str) 
{
    std::fstream fs;
    fs.open(pathFile, std::ios::app);

    if(fs.is_open()) 
    {
        fs << str << std::endl;
    }
    fs.close();
}

void writeResultToFile(double result, double first_number, double second_number, Operation operation) 
{
    std::cout << "Starting..." << std::endl;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    oss << "result of calculating: " << result << "\n";
    oss << "used: first number: " << first_number << ", second number: " << second_number << ", name of operation: " << getOperationName(operation);
    std::cout << oss.str() << std::endl;
    writeToFile("result.txt", oss.str());
}

void writeResultToFile(double result, std::string& file) 
{
    std::cout << "Starting..." << std::endl;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    oss << std::endl << "result of calculating: " << result << "\n";
    std::cout << oss.str() << std::endl;
    writeToFile(file, oss.str());
}

std::string readFromFile(const std::string& filePath) 
{
    std::ostringstream buffer;

    std::ifstream in(filePath);
    if(in.is_open()) 
    {
        buffer << in.rdbuf();
    }
    else if(!in) 
    {
        std::cerr << "Error: file is not found";
    }
    in.close();
    return buffer.str();
}

std::string readStringFromFile(const std::string& filePath, const std::string& prefix) 
{

    std::ifstream in(filePath);
    if(!in) 
    {
        std::cerr << "Error: file is not found";
    }

    std::string line;
    while(std::getline(in, line)) 
    {
        if(line.rfind(prefix, 0) == 0) 
        {
            return line;
        }
    }
    
    in.close();
    return "Error: string is not found";
}

double getNumberFromLine(const std::string& line) 
{
    std::istringstream iss(line);
    std::string tmp;
    double value = 0;

    iss >> tmp >> tmp >> value;

    return value;
}

class Calculator
{
    private:
        bool started;

        void PrintWelcome() 
        {
            std::cout   << "        === CALCULATOR ==="        << std::endl
                        << "Type 'help' for getting comands"   << std::endl
                        << "Type 'quit' or 'exit' to close"    << std::endl
                        << "===============================\n" << std::endl;
        }

        void printHelp() 
        {            
            std::cout     << "\n     === ALL OPERATIONS ==="                  << std::endl
                          << "1 or add/+         : Addition"                  << std::endl
                          << "2 or sub/-         : Substraction"              << std::endl
                          << "3 or div//         : Dividing"                  << std::endl
                          << "4 or mul/*         : Multiplication"            << std::endl
                          << "5 or pow/^         : Power"                     << std::endl
                          << "6 or integral/int  : Integral Calculation"      << std::endl
                          << "7 or min           : Minimum of two numbers"    << std::endl
                          << "8 or max           : Maximum of two numbers"    << std::endl
                          << "\n"
                          << "       ===   COMMANDS    ==="                   << std::endl
                          << "calc               : Start calculation"         << std::endl
                          << "file               : Calculate from file"       << std::endl
                          << "help               : Show this help"            << std::endl
                          << "quit/exit          : Close Calculator"          << std::endl
                          << "=======================================\n"      << std::endl;
        }

        bool isQuitCommand(const std::string& input) 
        {
            return (input == "quit" || input == "exit" || input == "close");
        }

        double getNumber(const std::string& prompt) 
        {
            std::string input;
            while(true) 
            {
                std::cout << prompt << " (or 'quit' to exit): ";
                std::cin >> input;

                if(isQuitCommand(input)) 
                {
                    started = false;
                    return 0;
                }

                try 
                {
                    return std::stod(input);
                } 
                catch(...) 
                {
                    std::cerr << "Invalid input. Please try again." << std::endl;
                }
            }
        }

        Operation getOperation() 
        {
            std::string input;
            while(true) 
            {
                std::cout << "Enter operation (number 1-8 or operation name): ";
                std::cin >> input;

                if(isQuitCommand(input)) 
                {
                    started = false;
                    return Operation::Unknown;
                }

                Operation operation = parseOperation(input);
                if(operation != Operation::Unknown) 
                {
                    return operation;
                }

                std::cout << "Unknown operation. Type 'help' for available operations." << std::endl;
            }
        }

        bool askUser(const std::string& question) 
        {
            std::string answer;
            std::cout << question << " (y/n, yes/no, true/false): " << std::endl;
            std::cin >> answer;
            return (answer == "y" || answer == "yes" || answer == "true");
        }

        void doCalculation() 
        {
            try 
            {
                double first = getNumber("Enter first number");
                if(!started) return;

                double second = getNumber("Enter second number");
                if(!started) return;

                Operation operation = getOperation();
                if(!started || operation == Operation::Unknown) return;

                std::cout << "\nCalculating..." << std::endl;
                double result = calculate(first, second, operation);

                if(askUser("Save result to file?")) 
                {
                    writeResultToFile(result, first, second, operation);
                }
            }
            catch(const std::exception& e) 
            {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }

        void calculateFromFile() 
        {
            std::cout << "Enter file name: ";
            std::string file_name;
            std::cin >> file_name;

            std::string first_variable  = readStringFromFile(file_name, (std::string)STR_FIRST);
            std::string second_variable = readStringFromFile(file_name, (std::string)STR_SECOND);
            std::string operation_str   = readStringFromFile(file_name, (std::string)STR_NUMBER);

            double first_var         = getNumberFromLine(first_variable);
            double second_var        = getNumberFromLine(second_variable);
            Operation operation      = getOperationFromLine(operation_str);

            if(operation == Operation::Unknown) 
            {
                std::cerr << "Unknown operation in file" << std::endl;
                return;
            }

            std::cout << "\nCalculatin...";

            double d_result = calculate(first_var, second_var, operation);
            
            std::cout << "The result is: " << d_result << std::endl;

            if(askUser("Save result to file?")) 
            {
                writeResultToFile(d_result, file_name);
            }
        }

        void processCommand(const std::string& command) 
        {
            if(command == "calc" || command == "calculate") 
            {
                doCalculation();
            } 
            else if(command == "file")
            {
                calculateFromFile();
            }
            else if(command == "help") 
            {
                printHelp();
            }
            else if(isQuitCommand(command)) 
            {
                std::cout << std::endl << "Closing calculator." << std::endl;
                started = false;
            }
            else
            {
                std::cout << "Unknown command. Type 'help' for available commands." << std::endl;
            }
        }
    
    public:
        Calculator() : started(true) {}

        void run() 
        {
            PrintWelcome();

            std::string command;
            while(started)
            {
                std::cout << "\nCalculator> ";
                std::cin >> command;
                processCommand(command);
            }
        }
};

int main() 
{
    try 
    {
        Calculator calculator;
        calculator.run();
    }
    catch(const std::exception& e) 
    {
        std::cerr << "Fatal error^ " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}