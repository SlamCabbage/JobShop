//
// Created by 廖礼洲 on 2022/10/7.
//

#pragma once

#include "Base.h"
#include "Eigen/Core"
#include "Eigen/Eigen"
#include <queue>

namespace job_shop {

    class Lane : public Base{
    public:
        // 车道保存的车辆信息
        Eigen::Matrix<int, 7, 10> lane_car = -1 * Eigen::Matrix<int, 7, 10>::Ones();
        // 车道保存的占用车辆时间信息
        Eigen::Matrix<int, 7, 10> lane_time = -1 * Eigen::Matrix<int, 7, 10>::Ones();
        // 车道占用信息，秒级别更新
        Eigen::Matrix<int, 7, 1> lane_occupancy = Eigen::Matrix<int, 7, 1>::Zero();
        // 用一个列表表示到右端的信息
        std::queue<std::pair<int, std::pair<int, int>>> rightmost_car;
        // 用一个列表表示到左端的信息
        std::queue<std::pair<int, std::pair<int, int>>> leftmost_car;
        // 结果矩阵
        Eigen::Vector<int, 318> result_cur;

    public:
        // 更新一秒后的状态
        void UpdateLaneTime(bool revsere=false);
        // 添加新的车身
        void AddCar(int row, int car_id);
        // 翻转车身
        void ReverseCar(int car_type);
        // 重新添加翻转车身
        void AddReverseCar();
        // 删除车身
        void DeleteCar(int i, int j);
        // 记录车身所在位置
        void recorder(std::ofstream& outFile, const int& reciver, const int& deliver, const int& sented, const int& index_next);
    };

}// namespace job_shop
