#include <iostream>
#include <fstream>
#include <concepts>
#include <sstream>
#include <iomanip>
#include <string>

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
    double area = 0;
    return area = ((3*h)/8)*(f(a) + 3*f((2*a+b)/3) + 3*f((a+2*b)/3) + f(b));
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


double calculate(double a, double b, int operation)
{
    switch(operation)
    {
        case 1: 
            return a + b;
        case 2:
            return a - b;
        case 3:
            if (b == 0) return 0;
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

int main() 
{
    std::cout << "Calculator!" << std::endl;
    std::cout << "there a 8 operations which the calculator provides: " << std::endl
              << "1: a basic addition operation"                        << std::endl
              << "2: a basic substraction operation"                    << std::endl
              << "3: a basic dividing operation"                        << std::endl
              << "4: a basic multiplication operation"                  << std::endl
              << "5: a pow operation"                                   << std::endl
              << "6: a integral operation"                              << std::endl
              << "7: a min of two numbers"                              << std::endl
              << "8: a max of two numbers"                              << std::endl;

    std::cout << "Write ur first number" << std::endl;
    float first_number;
    std::cin >> first_number;

    std::cout << "Write ur second number" << std::endl;
    float second_number;
    std::cin >> second_number;

    std::cout << "Write a operation number" << std::endl;
    int operation_number;
    std::cin >> operation_number;

    std::cout << "Calculating..." << std::endl;
    auto result = calculate(first_number, second_number, operation_number);

    std::cout << "Result: " << result << std::endl; 

    std::cout << "U want to write result to file?" << std::endl;
    std::string answer;
    std::cin >> answer;

    if (answer == "true" || answer == "yes" || answer == "y") 
    {
        std::cout << "Starting..." << std::endl;

        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2);

        oss << "result of calculating: " << result << "\n";
        oss << "used: first number: " << first_number << ", second number: " << second_number << ", number of operation: " << operation_number;

        std::cout << oss.str();
        writeToFile("result.txt", oss.str());

        std::cout << std::endl << "Closing calculator." << std::endl;
        return 0;
    }
    else if (answer == "false" || answer == "no" || answer == "n") 
    {
        std::cout << "Closing calculator." << std::endl;
        return 0;
    }
}