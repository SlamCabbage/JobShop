#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <random>

#include "include/Lane.h"
#include "include/v_delivery_traverse_machine.h"
#include "include/v_receiving_traverse_machine.h"

using namespace std;

int main() {
    std::vector<int> power;
    std::vector<int> drive;
    std::vector<int> car_type;
    const std::string data_path = "/Users/liaolizhou/Desktop/研究生/研究生学生工作/华为杯数学建模2022/code/jobshop/JobShop/data/data.txt";
    std::fstream file;
    file.open(data_path, ios::in);
    if (!file.is_open()) {
        std::cerr << "Error opening file";
        exit(1);
    }
    std::string lineStr;
    getline(file, lineStr);
    cout << lineStr << endl;
    while (getline(file, lineStr)) {
//        cout << lineStr << endl;
        std::stringstream ss(lineStr);
        std::string str;
        int i = 0;
        while (getline(ss, str, ',')) {
            int cur = std::stoi(str);
            if (i == 0) {
                power.emplace_back(cur);
            } else if (i == 1) {
                drive.emplace_back(cur);
            } else {
                car_type.emplace_back(cur);
            }
            i++;
        }
    }
    cout << power.size() << " " << drive.size() << " " << car_type.size() << endl;

    default_random_engine random_generator;
    uniform_int_distribution<unsigned> scope(0, 9);

    return 0;
}
