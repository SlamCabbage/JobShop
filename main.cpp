#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#include "include/Lane.h"
#include "include/v_delivery_traverse_machine.h"
#include "include/v_receiving_traverse_machine.h"
#include "include/car_class.h"
#include "include/calculate_fitness.h"

using namespace std;

int main() {
    srand(time(0));


    ofstream ff;
    ofstream fff;
    ff.open("/home/ning/JobShop-1/JobShop/data/1.txt", std::ios::out);
    fff.open("/home/ning/JobShop-1/JobShop/data/result111.csv", std::ios::out);

    double score_max = -100;
    job_shop::calculate_fitness ca;

    for (int i = 0; i <50; ++i) {

        // 读取data
        std::vector<int> power;
        std::vector<int> drive;
        std::vector<int> car_type;
        std::queue<int> remain_car_index;
        const std::string data_path =
                "/home/ning/JobShop-1/JobShop/data/data.txt";
        std::fstream file;
        file.open(data_path, ios::in);
        if (!file.is_open()) {
            std::cerr << "Error opening file";
            exit(1);
        }
        std::string lineStr;
        int k = 0;
        while (getline(file, lineStr)) {
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
                    remain_car_index.push(k++);
                }

                i++;
            }
        }
        int car_num = static_cast<int>(power.size());

        int deliver_task_index = 10000;
        int recevier_task_index = 0;

        Eigen::Matrix<int, 20000, 1> Task = 12 * Eigen::Matrix<int, 20000, 1>::Ones();

        // 实例化对象
        job_shop::v_receiving_traverse_machine v_receiver;
        job_shop::v_delivery_traverse_machine v_deliver;
        job_shop::Lane lane;

        // 当前时刻t
        int t = 0;
        // 判断是否完成任务

        v_receiver.assign_task(remain_car_index, car_type, lane);

        bool is_reverse_map_updated = false;
        bool r_has_mission = v_receiver.judge_receiver_task_phase(lane, car_type, remain_car_index, 0);

        while (v_deliver.delivered_car_index.size() != car_num) {

            is_reverse_map_updated = false;
            bool d_has_mission = v_deliver.judge_delivery_task_phase(lane, car_type, is_reverse_map_updated, t);
            bool is_map_updated = false;

            if (!d_has_mission) {
                if (!lane.rightmost_car.empty()) {
                    int task_index = v_deliver.assign_task(lane.rightmost_car, car_type, lane);
                    d_has_mission = true;
                    v_deliver.delivery_t_now++;
                    Task(deliver_task_index++) = task_index;

                    if (task_index == 3) {
                        auto temp = lane.rightmost_car.front();
                        v_deliver.delivery_carry_car_id = temp.first;
//                    cout << "22222 " << v_deliver.delivery_carry_car_id << " " << temp.first << endl;
                        lane.DeleteCar(temp.second.first, temp.second.second);
                        v_deliver.delivered_car_index.push_back(v_deliver.delivery_carry_car_id);

                        if (!lane.rightmost_car.empty()) {
                            v_deliver.assign_task(lane.rightmost_car, car_type, lane);
                        } else {
                            v_deliver.delivery_carry_car_id = -1;
                            v_deliver.delivery_t_now = -1;
                            v_deliver.delivery_task_id_now = 12;
                        }
                    }
                    if (task_index == 9) {
                        auto temp = lane.rightmost_car.front();
                        v_deliver.delivery_carry_car_id = temp.first;
//                        cout << "22222 " << v_deliver.delivery_carry_car_id << " " << temp.first << endl;
                        lane.DeleteCar(temp.second.first, temp.second.second);
                    }
                } else {
                    if (t != 0)
                        lane.UpdateLaneTime();
                    is_map_updated = true;
                    if (!lane.rightmost_car.empty()) {
                        // 分配任务并存储到Task向量中
                        int task_index = v_deliver.assign_task(lane.rightmost_car, car_type, lane);
                        d_has_mission = true;
                        v_deliver.delivery_t_now++;
                        Task(deliver_task_index++) = task_index;
                        if (task_index == 3) {
                            auto temp = lane.rightmost_car.front();
                            v_deliver.delivery_carry_car_id = temp.first;
                            lane.DeleteCar(temp.second.first, temp.second.second);
                            v_deliver.delivered_car_index.push_back(v_deliver.delivery_carry_car_id);

                            if (!lane.rightmost_car.empty()) {
                                v_deliver.assign_task(lane.rightmost_car, car_type, lane);
                            } else {
                                v_deliver.delivery_carry_car_id = -1;
                                v_deliver.delivery_t_now = -1;
                                v_deliver.delivery_task_id_now = 12;
                            }
                        }
                        if (task_index == 9) {
                            auto temp = lane.rightmost_car.front();
                            v_deliver.delivery_carry_car_id = temp.first;
                            lane.DeleteCar(temp.second.first, temp.second.second);
                        }
                    }
                }
            }

            if (!is_map_updated && t != 0) {
                lane.UpdateLaneTime();
            }


            bool r_has_mission = v_receiver.judge_receiver_task_phase(lane, car_type, remain_car_index, t);

            if (!r_has_mission) {
                if (!lane.leftmost_car.empty()) {
                    // 首先判断是否还有返回车道的任务，有的话则优先完成返回车道任务
                    int task_index = v_receiver.assign_task(lane, car_type);
//                    cout << task_index << endl;
                    v_receiver.receiving_t_now++;
                    Task[recevier_task_index++] = task_index;
                } else {
                    // 没有返回车道任务，则更新返回车道的状态
                    lane.UpdateLaneTime(true);
                    is_reverse_map_updated = true;
                    // 判断更新后的返回车道是否有任务即是否为空
                    if (!lane.leftmost_car.empty()) {
                        // 如果不为空，则优先接返回车道的任务
                        int task_index = v_receiver.assign_task(lane, car_type);
                        v_receiver.receiving_t_now++;
                        Task[recevier_task_index++] = task_index;
                    } else if(!remain_car_index.empty()) {
                        // 如果还是为空，则去接顺序的任务;
                        // 判断顺序任务是否存在，如果有则安排，没有则不安排
                        int task_index;

                        task_index = v_receiver.assign_task(remain_car_index, car_type, lane);
                        if (task_index == 12){
                            v_receiver.receiving_t_now = -1;
                            Task[recevier_task_index++] = task_index;
                        }else{
                            v_receiver.receiving_t_now++;
                            Task[recevier_task_index++] = task_index;
                        }

                        if (task_index == 3 && v_receiver.time_add_wait == -1) {
                            // 直接添加
                            lane.AddCar(3, v_receiver.carry_car_id, t);
                            // 添加完成後空閒
                            v_receiver.receiving_task_id_now = 12;
                            v_receiver.carry_car_id = -1;
                            v_receiver.receiving_t_now = -1;
                            v_receiver.time_add_wait = -1;

                            task_index = v_receiver.assign_task(remain_car_index, car_type, lane);
                            v_receiver.receiving_t_now++;
                            Task[recevier_task_index++] = task_index;
                        }
                    }
                }
//      cout << "5. receiver分配任务完成" << endl;
            }

            /// 5. 更新返回车道的状态
            if (!is_reverse_map_updated) {
                lane.UpdateLaneTime(true);
            }

            t++;
            //cout << v_deliver.delivered_car_index.size() << " " << car_num << endl;
        }

        double score =    ca.fitness(v_deliver.delivered_car_index,car_type,t,v_deliver.back_road_used_time);

        if (score > score_max ){
            score_max = score;
            ff << "t = " << t << endl;
            ff << "lane_time : " << endl;
            ff << lane.lane_time << endl;
            ff << "lane_car : " << endl;
            ff << lane.lane_car << endl;
            ff << "v_deliver.delivery_t_now: " << endl;
            ff << v_deliver.delivery_t_now << endl;
            ff << "deliver_task_id_now: " << endl;
            ff << v_deliver.delivery_task_id_now << endl;
            ff << "receiving_task_id_now: " << endl;
            ff << v_receiver.receiving_task_id_now << endl;
            ff << "receiving_time" << endl;
            ff << v_receiver.receiving_t_now << endl;
            ff << "time_add_wait: " << endl;
            ff << v_receiver.time_add_wait << endl;
            ff << "receiver : car_id" << endl;
            ff << v_receiver.carry_car_id << endl;
            ff << "deliver : car_id" << endl;
            ff << v_deliver.delivery_carry_car_id << endl;
            ff << "v_deliver.delivered_car_index : output car_id: " << endl;
            ff << v_deliver.back_road_used_time<<endl;
            for (int j : v_deliver.delivered_car_index) {
                ff << j << " ";
            }
            ff << endl;
            ff << "--------------------------------------" << endl;
            lane.recorder(fff, v_receiver.carry_car_id, v_deliver.delivery_carry_car_id,
                          v_deliver.delivered_car_index.size() > 0 ? v_deliver.delivered_car_index.back() : -1,
                          remain_car_index.empty() ? -1 : remain_car_index.front());
        }

   }



    cout<<"score_max = "<<score_max<<endl;
    return 0;

}




