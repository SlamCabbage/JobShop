//
// Created by ning on 22-10-7.
//

#include "v_receiving_traverse_machine.h"

namespace job_shop {
// 从涂装-PBS进车道取车任务id
    enum receiving_task_id {
        pick_up_from_0_to_1, // 从涂装-PBS进车道取车送至1车道
        pick_up_from_0_to_2, // 从涂装-PBS进车道取车送至2车道
        pick_up_from_0_to_3, // 从涂装-PBS进车道取车送至3车道
        pick_up_from_0_to_4, // 从涂装-PBS进车道取车送至4车道
        pick_up_from_0_to_5, // 从涂装-PBS进车道取车送至5车道
        pick_up_from_0_to_6, // 从涂装-PBS进车道取车送至6车道

        pick_up_from_back_lane_to_1, // 从返回车道取车送至1车道
        pick_up_from_back_lane_to_2, // 从返回车道取车送至2车道
        pick_up_from_back_lane_to_3, // 从返回车道取车送至3车道
        pick_up_from_back_lane_to_4, // 从返回车道取车送至4车道
        pick_up_from_back_lane_to_5, // 从返回车道取车送至5车道
        pick_up_from_back_lane_to_6, // 从返回车道取车送至6车道
        no_task,
    };

// 任务消耗时间
    int receiving_task_id_time[] = {18, 12, 6, 0, 12, 18, 24,
                                    18, 12, 6, 12, 18, -1};

    bool v_receiving_traverse_machine::judge_receiver_task_phase(
            Lane &lane_in, std::vector<int> &car_type_in_, std::queue<int> &q_in, int t) {
        this->receiving_t_now++;
        // 当前任务为前6个，取涂装-PBS进车口的车放入某车道
        if (time_add_wait == -1) {
            if (this->receiving_task_id_now <= 5) {
                // 到达该放置车身的车道
                if (this->receiving_t_now ==
                    receiving_task_id_time[this->receiving_task_id_now] / 2) {
                    lane_in.AddCar(receiving_task_id_now, carry_car_id, t);
                    this->carry_car_id = -1;
                    if (this->receiving_task_id_now == 3) {
                        this->receiving_t_now = -1;
                        this->receiving_task_id_now = no_task;
                        this->assign_task(q_in, car_type_in_, lane_in);
                        return true;
                    }
                } else if (
                        this->receiving_t_now ==
                        receiving_task_id_time
                        [this->receiving_task_id_now]) { // 接车横移机回到4车道（起点）----空闲

                    this->receiving_t_now = -1;
                    this->receiving_task_id_now = no_task;
                    return false;
                } else {
                    //        std::cout << "正前往车道放车或者正返回起点途中" << std::endl;
                }
            } else if (this->receiving_task_id_now >= 10 && this->receiving_task_id_now < 12) {
                if (this->receiving_t_now ==
                    receiving_task_id_time[this->receiving_task_id_now] /
                    2) { // 接车横移机到达要放置车身车道
                    // todo 执行放车动作，更新车道信息
                    lane_in.AddCar(receiving_task_id_now - 6, carry_car_id, t);
                    this->carry_car_id = -1;
                } else if (this->receiving_t_now == 3) {

                    // TODO 执行取返回道车身动作，更新车道信息
                    auto temp = lane_in.leftmost_car.front();
                    this->carry_car_id = temp.first;
                    lane_in.AddReverseCar();
                } else if (this->receiving_t_now ==
                           receiving_task_id_time[this->receiving_task_id_now]) {
                    this->receiving_t_now = -1;
                    this->receiving_task_id_now = no_task;
                    return false;
                }
            } else if (this->receiving_task_id_now >= 6 &&
                       this->receiving_task_id_now < 10) {
                if (this->receiving_t_now ==
                    receiving_task_id_time[this->receiving_task_id_now]) {
                    if (receiving_task_id_now == 9){
                        lane_in.AddCar(receiving_task_id_now - 6, carry_car_id, t);
                        this->carry_car_id = -1;
                    }
                    this->receiving_t_now = -1;
                    this->receiving_task_id_now = no_task;
                    return false;
                }else if (this->receiving_t_now == 3) {
                    auto temp = lane_in.leftmost_car.front();
                    this->carry_car_id = temp.first;
                    lane_in.AddReverseCar();
                } else if (this->receiving_t_now ==
                          receiving_task_id_time[this->receiving_task_id_now] / 2 +
                          3) { // 接车横移机到达要放置车身车道
                    lane_in.AddCar(receiving_task_id_now - 6, carry_car_id, t);
                    this->carry_car_id = -1;
                }
            } else { // 空闲
                this->receiving_t_now = -1;
                return false;
            }
        } else {
            if (this->receiving_task_id_now <= 5) {
                // 到达该放置车身的车道
                if (this->receiving_t_now == time_add_wait) {
                    lane_in.AddCar(receiving_task_id_now, carry_car_id, t);
                    this->carry_car_id = -1;
                    if (this->receiving_task_id_now == 3) {

                        this->time_add_wait = -1;
                        this->receiving_task_id_now = no_task;
                        this->assign_task(q_in, car_type_in_, lane_in);
                        this->receiving_t_now = 0;
                    }
                } else if (this->receiving_t_now ==
                           receiving_task_id_time[this->receiving_task_id_now] / 2 +
                           time_add_wait) {
                    this->receiving_t_now = -1;
                    this->time_add_wait = -1;
                    this->receiving_task_id_now = no_task;
                    return false;
                }
            } else if (this->receiving_task_id_now > 5 &&
                       this->receiving_task_id_now <= 11) {
                if (this->receiving_task_id_now >= 10) {
                    if (this->receiving_t_now ==
                        time_add_wait) { // 接车横移机到达要放置车身车道
                        lane_in.AddCar(receiving_task_id_now - 6, carry_car_id, t);
                        this->carry_car_id = -1;
                    } else if (this->receiving_t_now == 3) {
                        auto temp = lane_in.leftmost_car.front();
                        this->carry_car_id = temp.first;
                        lane_in.AddReverseCar();
                    } else if (this->receiving_t_now ==
                               receiving_task_id_time[this->receiving_task_id_now] / 2 +
                               time_add_wait) { // 接车横移机回到4车道（起点）----空闲
                        this->receiving_t_now = -1;
                        this->time_add_wait = -1;
                        this->receiving_task_id_now = no_task;
                        return false;
                    }
                } else if (this->receiving_task_id_now >= 6 &&
                           this->receiving_task_id_now < 10) {
                    if (this->receiving_t_now ==
                        time_add_wait) { // 接车横移机到达要放置车身车道
                        //std::cout << "car_id: " << carry_car_id << std::endl;
                        lane_in.AddCar(receiving_task_id_now - 6, carry_car_id, t);
                        this->carry_car_id = -1;
                    } else if (this->receiving_t_now == 3) {
                        auto temp = lane_in.leftmost_car.front();
                        this->carry_car_id = temp.first;
                        lane_in.AddReverseCar();
                    } else if (this->receiving_t_now ==
                               receiving_task_id_time[this->receiving_task_id_now] / 2 +
                               time_add_wait - 3) {
                        this->receiving_t_now = -1;
                        this->time_add_wait = -1;
                        this->receiving_task_id_now = no_task;
                        return false;
                    }
                }
            } else {
                this->receiving_t_now = -1;
                this->time_add_wait = -1;
                this->receiving_task_id_now = no_task;
                // TODO 根据车道信息分配任务
                return false;
            }
        }
        return true;
    }

// 根据翻转车道的队列分配任务
    int v_receiving_traverse_machine::assign_task(Lane &lane_in,
                                                  std::vector<int> &car_type_) {
        // 给receiver分配一个任务（6-12）：根据时间优先级进行分配车道
        int min_task = 100;
        int min_time = 100;
        int time;
        for (int i = 0; i < 6; ++i) {
            if (lane_in.lane_time(i, 9) != -1) continue;
            time = receiving_task_id_time[i + 6];
            if (time < min_time) {
                min_time = time;
                min_task = i + 6;
            }
        }
        if (min_task != 100) {
            receiving_task_id_now = min_task;
            time_add_wait = -1;
        } else {
            receiving_task_id_now = 12;
            time_add_wait = -1;
            return 12;
        }
        return min_task;
    }

// 根据输入顺序分配任务
    int v_receiving_traverse_machine::assign_task(std::queue<int> &q_in,
                                                  std::vector<int> &car_type_,
                                                  Lane &lane_in) {
        int temp_index = q_in.front();

        // 按顺序进行获取涂装PBS上的车身index

        int min_task = 100;
        int min_time = 100;
        int time;
        std::vector<int> time_used;
        std::vector<int> task_id;
        for (int i = 0; i < 6; ++i) {
            if (lane_in.lane_time(i, 9) != -1) continue;
            time = receiving_task_id_time[i];
            time_used.push_back(time);
            task_id.push_back(i);
        }

        int num = task_id.size() * 10 + 1;
        double xx = (double) rand() / RAND_MAX;
        double result = xx * (num - 1) + 1;
        int r = floor(abs(result));

        if (task_id.size() != 0){
            min_task = task_id[r % task_id.size()];
        }


        if (min_task != 100) {
            q_in.pop();
            carry_car_id = temp_index;
            receiving_task_id_now = min_task;
            time_add_wait = -1;
        } else {
            receiving_task_id_now = 12;
            time_add_wait = -1;
            return receiving_task_id_now;
        }
        return min_task;
    }

// 生成一个范围在【a,b）内的随机整数，注意：：需要在main函数中写随机数种子：：srand((int)time(0));
    int v_receiving_traverse_machine::get_rand_number(int a, int b) {

        double x = (double) rand() / RAND_MAX;
        double result = x * (b - a) + a;
        double r = floor(abs(result));

        return r;
    }

} // namespace job_shop