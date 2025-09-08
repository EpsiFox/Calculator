#include <iostream>
#include <fstream>
#include <concepts>
#include <sstream>
#include <iomanip>
#include <string>
#include <cctype>
#include <string_view>


/////////////////// CONSTANTS ///////////////////

constexpr std::string_view STR_FIRST = "first";
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
        std::cout << "N must be natural" << std::endl;
        return 0;
    }
    switch (n) 
    {
        case 0: return 1;
        case 1: return x;
        default: return x * pow(x, n - 1);
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

void writeResultToFile(double result, double first_number, double second_number, int operation_number) 
{
    std::cout << "Starting..." << std::endl;

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    oss << "result of calculating: " << result << "\n";
    oss << "used: first number: " << first_number << ", second number: " << second_number << ", number of operation: " << operation_number;
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

double getNumberFromLine(const std::string& line) 
{
    std::istringstream iss(line);
    std::string tmp;
    double value = 0;

    iss >> tmp >> tmp >> value;

    return value;
}

double doCalculateByOperation(double first_number, double second_number, Operation operation) 
{
    switch(operation) 
    {
        case Operation::Add: return first_number + second_number;
        case Operation::Sub: return first_number - second_number;
        case Operation::Div: if(second_number == 0) return 0; 
                             return first_number / second_number;
        case Operation::Mul: return first_number * second_number;
        case Operation::Pow: return pow(first_number, second_number); 
        case Operation::Integral: return integral(first_number, second_number);
        case Operation::Min: return min(first_number, second_number); 
        case Operation::Max: return max(first_number, second_number);
        default: throw std::invalid_argument("Unknown operation");
    }
}

double calculate(double a, double b, int operation)
{
    switch(operation)
    {
        case 1: 
            return a + b;
        case 2:
            return a - b;
        case 3:
            if(b == 0) return 0;
            return a / b;
        case 4:
            return a * b;
        case 5:
            return pow(a, b);
        case 6:
            return integral(a, b);
        case 7:
            return min(a, b);
        case 8: 
            return max(a, b);
        default: 
            return 0;
    }
}

/////////////////// CLI INTERFACE ///////////////////

bool isCloseCommand(const std::string& u_input) 
{
    std::string lower;
    lower.reserve(u_input.size());
    for(char c : u_input) 
    {
        lower.push_back(std::tolower(c));
    }
    return (lower == "close");
}

double readNumberOrString(bool& started) 
{
    std::string input;
    std::cin >> input;

    if(isCloseCommand(input)) 
    {
        std::cout << std::endl << "Closing calculator." << std::endl;
        started = false;
        return 0;
    }

    try 
    {
        return std::stod(input);
    } 
    catch (...) 
    {
        std::cerr << "Invalid input. Please enter a number or 'Close'" << std::endl;
        return readNumberOrString(started);
    }
}

void printAllOperations() 
{
    std::cout << "there a 8 operations which the calculator provides: " << std::endl
                  << "1: a basic addition operation"                        << std::endl
                  << "2: a basic substraction operation"                    << std::endl
                  << "3: a basic dividing operation"                        << std::endl
                  << "4: a basic multiplication operation"                  << std::endl
                  << "5: a pow operation"                                   << std::endl
                  << "6: a integral operation"                              << std::endl
                  << "7: a min of two numbers"                              << std::endl
                  << "8: a max of two numbers"                              << std::endl;
}

void calculateFromFile(std::string& startCalculator, bool started) 
{
    std::cout << "enter a file name" << std::endl;
    std::string file_name;
    std::cin >> file_name;

    std::string first_variable = readStringFromFile(file_name, (std::string)STR_FIRST);
    std::string second_variable = readStringFromFile(file_name, (std::string)STR_SECOND);
    std::string operation_str = readStringFromFile(file_name, (std::string)STR_NUMBER);

    double first_var = getNumberFromLine(first_variable);
    double second_var = getNumberFromLine(second_variable);
    Operation op = getOperationFromLine(operation_str);

    double d_result = doCalculateByOperation(first_var, second_var, op);

    writeResultToFile(d_result, file_name);
    std::cout << "Print 'Close' to close calculator" << std::endl;
    std::cin >> startCalculator;
    if(startCalculator == "Close" || startCalculator == "close") 
    {
        std::cout << std::endl << "Closing calculator." << std::endl;
        started = false;
    }

    std::cout << "The result is: " << d_result << std::endl;
}

bool askUser(const std::string& question) 
{
    std::string answer;
    std::cout << question << " (y/n, yes/no, true/false): " << std::endl;
    std::cin >> answer;
    return (answer == "y" || answer == "yes" || answer == "true");
}

void closeCalculator(std::string& startCalculator, bool started) 
{
    std::cout << "Print 'Close' to close calculator" << std::endl;
    std::cin >> startCalculator;
    if(startCalculator == "Close" || startCalculator == "close") 
    {
        std::cout << std::endl << "Closing calculator." << std::endl;
        started = false;
    }
}

int main() 
{
    std::cout << "Print 'Start' to start calculator" << std::endl;
    std::string startCalculator;
    std::cin >> startCalculator;
    bool started = false;

    if(startCalculator == "Start" || startCalculator == "start") 
    {
        started = true;
    }
    while(started) 
    {
        std::cout << "Calculator!" << std::endl;

        while(true) 
        {
            std::cout << "What do you want?" << std::endl
                  << "1: do calculate in cmd" << std::endl
                  << "2: read expression from file" << std::endl;
        
            int a = 0;
            std::cin >> a;

            if(a == 2) 
            {
                calculateFromFile(startCalculator, started);
                break;
            }
            else if(a == 1) 
            {
                break;
            }
            else if(a < 1 || a > 2) 
            {
                std::cout << "please, enter only 1 or 2" << std::endl;
            }

        }

        printAllOperations();
        
        std::cout << "Write your first number" << std::endl;
        double first_number = readNumberOrString(started);
        if (!started) break;

        std::cout << "Write your second number" << std::endl;
        double second_number = readNumberOrString(started);
        if (!started) break;

        std::cout << "Write a operation number" << std::endl;
        int operation_number = readNumberOrString(started);;
        if (!started) break;

        std::cout << "Calculating..." << std::endl;
        double result = calculate(first_number, second_number, operation_number);

        std::cout << "Result: " << result << std::endl; 

        if(askUser("Do you want to write result to file?")) 
        {
            writeResultToFile(result, first_number, second_number, operation_number);
            closeCalculator(startCalculator, started); 
        }
        else if(askUser("Do you want to read expression from file?")) 
        {
            calculateFromFile(startCalculator, started);
        }
        else 
        {
            closeCalculator(startCalculator, started); 
        }

        std::string u_answer;
        std::cout << "Do you want to continue?" << std::endl;
        std::cin >> u_answer;
        if(u_answer == "n" || u_answer == "no")
        {
            started = false;
        }
    }
}