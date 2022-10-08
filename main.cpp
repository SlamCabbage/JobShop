#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#include "include/Lane.h"
#include "include/v_delivery_traverse_machine.h"
#include "include/v_receiving_traverse_machine.h"

using namespace std;

Eigen::Matrix<int, 8000, 1> Task = 12 * Eigen::Matrix<int, 8000, 1>::Ones();

int main() {
  // 读取data
  std::vector<int> power;
  std::vector<int> drive;
  std::vector<int> car_type;
  std::queue<int> remain_car_index;
  const std::string data_path =
      "/Users/liaolizhou/Desktop/研究生/研究生学生工作/华为杯数学建模2022/code/"
      "JobShop/data/data.txt";
  std::fstream file;
  file.open(data_path, ios::in);
  if (!file.is_open()) {
    std::cerr << "Error opening file";
    exit(1);
  }
  std::string lineStr;
  while (getline(file, lineStr)) {
    std::stringstream ss(lineStr);
    std::string str;
    int i = 0;
    int k = 0;
    while (getline(ss, str, ',')) {
      int cur = std::stoi(str);
      if (i == 0) {
        power.emplace_back(cur);
      } else if (i == 1) {
        drive.emplace_back(cur);
      } else {
        car_type.emplace_back(cur);
        remain_car_index.emplace(k++);
      }
      i++;
    }
  }
  cout << power.size() << " " << drive.size() << " " << car_type.size() << endl;

  // 实例化对象
  job_shop::v_receiving_traverse_machine v_receiver;
  job_shop::v_delivery_traverse_machine v_deliver;
  job_shop::Lane lane;

  int car_num = static_cast<int>(power.size());
  int deliver_task_index = 4000;
  int recevier_task_index = 0;
  // 当前时刻t
  int t = 0;
  // 判断是否完成任务
  while (v_deliver.delivered_car_index.empty() != car_num) {

    // 更新并判断送车横移机状态，1表示当前时刻有任务，不需要分配任务，0表示当前时刻空闲，需要后续分配任务
    /// 1. 更新deliver状态 and 2.deliver前向任务情况
    bool has_mission = v_deliver.judge_delivery_task_phase(lane, car_type);
    bool is_map_updated = false;
//    cout << "1. 更新deliver状态完毕" << endl;

    if (!has_mission) {
      if (!lane.rightmost_car.empty()) {
        // 分配任务并存储到Task向量中
        cout << "不为空"<<endl;
        int task_index = v_deliver.assign_task(lane.rightmost_car, car_type);
        Task(deliver_task_index++) = task_index;
      } else { // 队列q为空,更新地图
        lane.UpdateLaneTime();
//        cout << "77更新地图" << endl;
        is_map_updated = true;
        if (!lane.rightmost_car.empty()) {
          // 分配任务并存储到Task向量中
          int task_index = v_deliver.assign_task(lane.rightmost_car, car_type);
          Task(deliver_task_index++) = task_index;
        }
      }
//      cout << "2. 分配任务完毕" << endl;
    }

    /// 3. 判断是否已经更新地图
    if (!is_map_updated) {
//      cout << "3. 开始执行地图更新" << endl;
      lane.UpdateLaneTime();
//      cout << "92更新地图" << endl;
    }

    bool is_reverse_map_updated = false;
    /// 4. 更新receiver状态，5.receiver判断情况
    bool r_has_mission = v_receiver.judge_receiver_task_phase(lane);
//    cout << "4. 更新receiver状态完成" << endl;
    if (!r_has_mission) {
      if (!lane.leftmost_car.empty()) {
        // 首先判断是否还有返回车道的任务，有的话则优先完成返回车道任务
        int task_index = v_receiver.assign_task(lane, car_type);
        if(task_index != -1) Task[recevier_task_index++] = task_index;
      } else {
        // 没有返回车道任务，则更新返回车道的状态
        lane.UpdateLaneTime(true);
        is_reverse_map_updated = true;
        // 判断更新后的返回车道是否有任务即是否为空
        if (!lane.leftmost_car.empty()) {
          // 如果不为空，则优先接返回车道的任务
          int task_index = v_receiver.assign_task(lane, car_type);
          if(task_index != -1) Task[recevier_task_index++] = task_index;
        } else {
          // 如果还是为空，则去接顺序的任务;
          // 判断顺序任务是否存在，如果有则安排，没有则不安排
          if(!remain_car_index.empty()) Task[recevier_task_index++] = v_receiver.assign_task(remain_car_index, car_type, lane);
        }
      }
//      cout << "5. receiver分配任务完成" << endl;
    }
    /// 5. 更新返回车道的状态
    if(!is_reverse_map_updated) {
      lane.UpdateLaneTime(true);
    }
//    cout << "当前时间: " << t << "\n" << "执行进度：" << v_deliver.delivered_car_index.empty() << " / " <<  car_num << endl;
    cout << lane.lane_time << endl;
    t++;
  }

  return 0;
}
