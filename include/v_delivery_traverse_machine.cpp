//
// Created by ning on 22-10-7.
//

#include "v_delivery_traverse_machine.h"
#include "Eigen/Dense"
#include "Lane.h"

//从涂装-PBS进车道取车任务id
enum delivery_task_id{
    pick_up_from_1_to_0,//从1车道取车送至PBS-总装送车道
    pick_up_from_2_to_0,//从2车道取车送至PBS-总装送车道
    pick_up_from_3_to_0,//从3车道取车送至PBS-总装送车道
    pick_up_from_4_to_0,//从4车道取车送至PBS-总装送车道
    pick_up_from_5_to_0,//从5车道取车送至PBS-总装送车道
    pick_up_from_6_to_0,//从6车道取车送至PBS-总装送车道

    pick_up_from_1_to_back_lane,//从返回车道取车送至1车道
    pick_up_from_2_to_back_lane,//从返回车道取车送至2车道
    pick_up_from_3_to_back_lane,//从返回车道取车送至3车道
    pick_up_from_4_to_back_lane,//从返回车道取车送至4车道
    pick_up_from_5_to_back_lane,//从返回车道取车送至5车道
    pick_up_from_6_to_back_lane,//从返回车道取车送至6车道
    no_task_,
};

//任务消耗时间
int delivery_task_id_time[]= {18,12,6,0,12,18,24,18,12,6,12,18,0};

void job_shop::v_delivery_traverse_machine::judge_delivery_task_phase() {
    this->delivery_t_now++;

    if (this->delivery_task_id_now <= 5) {
        //当前任务为前6个，取某车道车身放入PBS-总装出车口
        if (this->delivery_t_now == delivery_task_id_time[this->delivery_task_id_now] / 2) {
            //到达取车身的车道
            //TODO 执行取车动作，更新车道信息
            std::cout<<"到达"<<this->delivery_task_id_now+ 1 <<"车道"<<std::endl;
        }

        //送车横移机回到4车道（起点）
        if (this->delivery_t_now == delivery_task_id_time[this->delivery_task_id_now]) {
            std::cout<<"返回"<< 4 <<"车道"<<std::endl;

            this->delivery_t_now = 0;
            this->delivery_t_task = 0;
            this->delivery_task_id_now = no_task_;

            //TODO 根据车道信息，决定后续任务


        }
    }

        //当前任务为7-10个，取某车道的车的车放入返回道
    else if (this->delivery_task_id_now > 5 && this->delivery_task_id_now <= 9){
        //送车横移机到达该取车车道
        if (this->delivery_t_now == (delivery_task_id_time[delivery_task_id_now]-6)/2) {

            std::cout<<"到达"<< delivery_task_id_now - 6<<"车道"<<std::endl;

            //TODO 执行取返回道车身动作，更新车道信息
        }

        //送车横移机到达返回道
        if (this->delivery_t_now == delivery_task_id_time[this->delivery_task_id_now] - 3) {
            std::cout<<"到达返回车道"<<std::endl;
            //TODO 放置车身，更新车道信息

        }

        //送车横移机返回四车道（起点）
        if (this->delivery_t_now == delivery_task_id_time[this->delivery_task_id_now]){

            std::cout<<"到达"<< 4 <<"车道"<<std::endl;

            this->delivery_t_now = 0;
            this->delivery_t_task = 0;
            this->delivery_task_id_now = no_task_;
            //TODO 根据车道信息，决定后续操作

        }




    }
    else if (this->delivery_task_id_now == 10||this->delivery_task_id_now == 11){
        //送车横移机到达5或者6车道,取车
        if (this->delivery_t_now==delivery_task_id_time[delivery_task_id_now]/2){
            //TODO 执行取车动作，更新车道信息


        }
        //送车横移机到达返回车道，放车
        if (this->delivery_t_now == delivery_task_id_time[delivery_task_id_now] - 3){
            //TODO 执行放车动作，更新车道信息

        }


    }







}