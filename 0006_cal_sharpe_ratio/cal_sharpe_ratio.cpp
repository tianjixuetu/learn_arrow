#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <chrono>

// 计算均值
double Mean(const std::vector<double>& data) {
    double sum = 0.0;
    for (const double& value : data) {
        sum += value;
    }
    return sum / data.size();
}

// 计算标准差
double StandardDeviation(const std::vector<double>& data) {
    double mean = Mean(data);
    double variance = 0.0;
    for (const double& value : data) {
        variance += std::pow(value - mean, 2);
    }
    return std::sqrt(variance / (data.size()-1));
}

int main() {

    auto start_time = std::chrono::high_resolution_clock::now();
    std::ifstream file("./fund_nav.csv");
    if (!file.is_open()) {
        std::cerr << "Failed to open the CSV file." << std::endl;
        return 1;
    }

    std::vector<double> returns;
    double previous_nav = 0.0;
    std::string line;
    getline(file, line); // Skip the header line

    while (getline(file, line)) {
        std::istringstream ss(line);
        std::string date, nav_str,cum_nav_str;
        
        getline(ss, date, ',');
        getline(ss, nav_str, ',');
        getline(ss, cum_nav_str, ',');

        //std::cout << line << std::endl;

        try {
            double nav = std::stod(cum_nav_str);
            if (previous_nav > 0.0) {
                double daily_return = (nav - previous_nav) / previous_nav;
                returns.push_back(daily_return);
            }
            previous_nav = nav;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid data found: " << nav_str << std::endl;
        }
    }

    file.close();

    if (returns.empty()) {
        std::cerr << "No returns data found." << std::endl;
        return 1;
    }

    // 计算均值和标准差
    double mean_return = Mean(returns);
    double std_deviation = StandardDeviation(returns);

    // 计算夏普比率
    double trading_days_per_year = 252.0;
    double sqrt_trading_days_per_year = std::sqrt(trading_days_per_year);
    double sharpe_ratio = (mean_return * sqrt_trading_days_per_year) / std_deviation;
    // std::cout << "mean_return " << mean_return << std::endl;
    // std::cout << "std_deviation " << std_deviation << std::endl;
    // std::cout << "sqrt_trading_days_per_year " << sqrt_trading_days_per_year << std::endl;

    std::cout << "the result of sharpe ratio : " << sharpe_ratio << std::endl;
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    std::cout << "the consume time of pure c++ read data and caculate the sharpe_ratio: " << duration.count()/1000.0 << " ms" << std::endl;

    return 0;
}
