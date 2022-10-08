#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#include "include/Lane.h"
#include "include/v_delivery_traverse_machine.h"
#include "include/v_receiving_traverse_machine.h"

using namespace std;

int main() {
  srand((int)time(0));

  // 读取data
  std::vector<int> power;
  std::vector<int> drive;
  std::vector<int> car_type;
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
  getline(file, lineStr);
  cout << lineStr << endl;
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
      }
      i++;
    }
  }
  cout << power.size() << " " << drive.size() << " " << car_type.size() << endl;

  // 实例化对象
  job_shop::v_receiving_traverse_machine v_receiver;
  job_shop::v_delivery_traverse_machine v_deliver;
  job_shop::Lane lane;

  // 当前时刻t

  int t = 0;
  // 判断是否完成任务
  while (1) {

    // 更新并判断送车横移机状态，1表示当前时刻有任务，不需要分配任务，0表示当前时刻空闲，需要后续分配任务
    /// 1. 更新deliver状态 and 2.deliver前向任务情况
    bool has_mission = v_deliver.judge_delivery_task_phase(lane);
    bool is_map_updated = false;

    if (!has_mission) {                  //
      if (!lane.rightmost_car.empty()) { // 若队列q不为空
        // TODO 分配任务，取车放到返回道或者放到出车口
        v_deliver.assign_task(lane.rightmost_car, car_type);
      } else { // 队列q为空,更新地图
        lane.UpdateLaneTime();
        is_map_updated = true;
        if (!lane.rightmost_car.empty()) {
          v_deliver.assign_task(lane.rightmost_car, car_type);
        }
      }
    }

    /// 3. 判断是否已经更新地图
    if (!is_map_updated) {
      lane.UpdateLaneTime();
    }

    /// 4. 更新receiver状态，5.receiver判断情况
    // bool receiver_phase = v_receiver.receiving_judge_task_phase();

    t++;
  }

  return 0;
}
