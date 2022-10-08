//
// Created by ning on 22-10-8.
//

#include "calculate_fitness.h"

double job_shop::calculate_fitness::fitness(std::vector<int> &result_out_ ,std::vector<int> car_type_,int time_cost,int back_road_used_time_) {

    std::vector<int> power_;
    std::vector<int> drive_;

    for (int i = 0; i < 318; ++i) {
        int car_id = result_out_[i];
        int car_type = car_type_[car_id];

        bool ishybrid = (car_type - 1)/2?0:1;//混动--1，非混--0
        power_.push_back(ishybrid);

        bool isFourwheel = car_type%2?0:1;//4驱--1，2驱--0
        drive_.push_back(isFourwheel);
    }

    double fitness = 100;
    //返回道扣分
    double back_road_dp = 0.2*back_road_used_time_;
    //时间扣分
    double overtime_dp = 0.1*(100 - 0.01*(time_cost - 9 * 318 - 72));

    //四驱转换扣分
    int turntime;
    int head = drive_[0];
    int pre = 0;
    double score_dirve = 30;
    for (int i = 1; i < drive_.size(); ++i) {
        if(drive_[i-1] != head && drive_[i] == head ){
            if( (i-pre)%2 != 0){
                score_dirve -= 0.3;
                pre = i;
                continue;
            }
            int count = 0;
            for(int j = pre; j < i-1; ++j){
                if(drive_[j] == head) {
                    count++;
                }
            }
            if(count != (i-pre)/2){
                score_dirve -= 0.3;
            }
            pre = i;
        }
        if(i == drive_.size()-1){
            if( (i+1-pre)%2 != 0){
                score_dirve -= 0.3;
                pre = i;
                continue;
            }
            int count = 0;
            for(int j = pre; j < i; ++j){
                if(drive_[j] == head) {
                    count++;
                }
            }
            if(count != (i+1-pre)/2){
                score_dirve -= 0.3;
            }
        }
    }

    //动力转换扣分
    bool first_one = true;
    int first_one_ = 0;
    double score_power = 40;
    for(int i = 0; i < power_.size(); ++i){
        if(power_[i] && first_one){
            first_one = false;
            first_one_ = i;
        }else if(power_[i] && !first_one){
            if(i - first_one_ - 1 != 2){
                score_power -= 0.4;
            }
            first_one_ = i;
        }
    }




    return fitness;



}