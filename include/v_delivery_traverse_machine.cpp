//
// Created by ning on 22-10-7.
//

#include "v_delivery_traverse_machine.h"


//从涂装-PBS进车道取车任务id
enum delivery_task_id{
    pick_up_from_1_to_0,//从1车道取车送至PBS-总装送车道0
    pick_up_from_2_to_0,//从2车道取车送至PBS-总装送车道1
    pick_up_from_3_to_0,//从3车道取车送至PBS-总装送车道2
    pick_up_from_4_to_0,//从4车道取车送至PBS-总装送车道3
    pick_up_from_5_to_0,//从5车道取车送至PBS-总装送车道4
    pick_up_from_6_to_0,//从6车道取车送至PBS-总装送车道5

    pick_up_from_1_to_back_lane,//从返回车道取车送至1车道6
    pick_up_from_2_to_back_lane,//从返回车道取车送至2车道7
    pick_up_from_3_to_back_lane,//从返回车道取车送至3车道8
    pick_up_from_4_to_back_lane,//从返回车道取车送至4车道9
    pick_up_from_5_to_back_lane,//从返回车道取车送至5车道10
    pick_up_from_6_to_back_lane,//从返回车道取车送至6车道11
    no_task_,//12
};

//任务消耗时间
int delivery_task_id_time[]= {18,12,6,0,12,18,24,18,12,6,12,18,0};

//判断任务状态,有任务返回1，没有任务返回0
bool job_shop::v_delivery_traverse_machine::judge_delivery_task_phase(Lane &lane_in) {//
    this->delivery_t_now++;

    //当前任务为前6个，取某车道车身放入PBS-总装出车口
    if (this->delivery_task_id_now <= 5) {

        //到达取车身的车道
        if (this->delivery_t_now == delivery_task_id_time[this->delivery_task_id_now] / 2) {
            // 执行取车动作，更新车道信息，更新delivery_carry_car_id
            auto temp = lane_in.rightmost_car.front();
            this->delivery_task_id_now = temp.first;

            lane_in.DeleteCar(temp.second.first, temp.second.second);

            std::cout<<"到达"<<this->delivery_task_id_now+ 1 <<"车道"<<std::endl;

            return 1;
        }
        else if (this->delivery_t_now == delivery_task_id_time[this->delivery_task_id_now]) {//送车横移机回到4车道（起点）----空闲
            std::cout<<"返回"<< 4 <<"车道"<<std::endl;

            this->delivery_t_now = 0;
            this->delivery_t_task = 0;
            this->delivery_task_id_now = no_task_;
            this->delivery_carry_car_id = -1;

            return 0;
        }else{
            std::cout<<"正前往车道放车或者正返回起点途中。"<<std::endl;
            return 1;
        }
    }

    else if (this->delivery_task_id_now > 5 && this->delivery_task_id_now <= 9){//当前任务为7-10个，取某车道的车的车放入返回道

        //送车横移机到达该取车车道
        if (this->delivery_t_now == (delivery_task_id_time[delivery_task_id_now]-6)/2) {

            std::cout<<"到达"<< delivery_task_id_now - 6 <<"车道"<<std::endl;

            //TODO 执行取返回道车身动作，更新车道信息，更新delivery_carry_car_id
            auto temp = lane_in.rightmost_car.front();
            this->delivery_task_id_now = temp.first;

            lane_in.DeleteCar(temp.second.first, temp.second.second);

            return 1;

        }else if (this->delivery_t_now == delivery_task_id_time[this->delivery_task_id_now] - 3) {//送车横移机到达返回道
            std::cout<<"到达返回车道"<<std::endl;
            // 放置车身，更新车道信息
            //lane_in.ReverseCar(this->delivery_carry_car_id);

            this->delivery_carry_car_id = -1;

            return 1;

        }else if (this->delivery_t_now == delivery_task_id_time[this->delivery_task_id_now]){//送车横移机返回四车道（起点）

            std::cout<<"到达"<< 4 <<"车道"<<std::endl;

            this->delivery_t_now = 0;
            this->delivery_t_task = 0;
            this->delivery_task_id_now = no_task_;
            this->delivered_car_index.push_back(this->delivery_carry_car_id);
            this->last_delivered = this->delivery_carry_car_id;
            this->delivery_carry_car_id = -1;

            return 0;

        }else{
            std::cout<<"正前往车道放车或者正返回起点途中。"<<std::endl;
            return 1;
        }




    }
    else if (this->delivery_task_id_now == 10||this->delivery_task_id_now == 11){

        if (this->delivery_t_now==delivery_task_id_time[delivery_task_id_now]/2){//送车横移机到达5或者6车道,取车
            // 执行取车动作，更新车道信息,更新delivery_carry_car_id
            auto temp = lane_in.rightmost_car.front();
            this->delivery_task_id_now = temp.first;

            lane_in.DeleteCar(temp.second.first, temp.second.second);

        }else if (this->delivery_t_now == delivery_task_id_time[delivery_task_id_now] - 3){//送车横移机到达返回车道，放车

            //TODO 执行放车动作，更新车道信息
            this->delivery_carry_car_id = -1;

        }else if (this->delivery_t_now == delivery_task_id_time[delivery_task_id_now]){//送车横移机回到4车道（起点）----空闲

            this->delivery_t_now = 0;
            this->delivery_t_task = 0;
            this->delivered_car_index.push_back(this->delivery_carry_car_id);
            this->delivery_carry_car_id = -1;
            this->delivery_task_id_now = no_task_;

            return 0;

        }else{
            std::cout<<"正前往车道放车或者正返回起点途中。/"<<std::endl;
            return 1;
        }


    }
    else{//送车横移机空闲

        return 0;
    }
    return 0;
}

void job_shop::v_delivery_traverse_machine::assign_task(std::queue<std::pair<int, std::pair<int, int>>> &q_in ,
                                                        std::vector<int> &car_type_ ) {

    auto temp = q_in.front();
    //车身索引
    int car_index = temp.first;
    auto location = temp.second;
    //最先到达1车位车身所在车道
    int line_id = location.first;

    int car_type_in = car_type_[car_index];//混动二驱--1，混动四驱--2，非混动二驱--3，非混动四驱--4

    //TODO 读取已经送走的车的队列,根据来车类型以及队列分配任务，决定是否送到返回道





}