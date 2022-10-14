//
// Created by ning on 22-10-9.
//

#ifndef JOBSHOP_CAR_CLASS_H
#define JOBSHOP_CAR_CLASS_H
#include "iostream"
#include "vector"
#include "Eigen/Dense"

namespace job_shop{
    class car_class {
    public:
        std::vector<int> position;

        void get_poision(int car_id,
                         std::vector<int> car_in,
                         int receiver_carry_car_id_,
                         Eigen::Matrix<int, 7, 10> &lane_car_ ,
                         int deliver_carry_car_id_,
                         std::vector<int> &ouput);
    };
}



#endif //JOBSHOP_CAR_CLASS_H
