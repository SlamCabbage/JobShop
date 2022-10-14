//
// Created by 廖礼洲 on 2022/10/7.
//


#ifndef JOB_SHOP_BASE_H
#define JOB_SHOP_BASE_H

namespace job_shop {
    class Base {
    public:
        enum CarType{
            HybridTwo = 1,
            HybridFour = 2,
            FuelTwo = 3,
            FuelFour = 4
        };
    };
}// namespace job_shop

#endif//JOB_SHOP_BASE_H
