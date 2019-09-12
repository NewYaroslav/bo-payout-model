#include <iostream>
#include "intrade_bar.hpp"

using namespace std;

int main()
{
    cout << "Hello world!" << endl;
    payout_model::IntradeBar IntradeBar;

    double payout = 0.0;
    IntradeBar.get_payout(payout, xtime::get_timestamp(1,1,2018), 180, 0, 4000);
    std::cout << "date " << xtime::get_str_date_time(xtime::get_timestamp(1,1,2018)) << " payout " << payout << std::endl;

    IntradeBar.get_payout(payout, xtime::get_timestamp(1,2,2018,12), 180, 0, 4000);
    std::cout << "date " << xtime::get_str_date_time(xtime::get_timestamp(1,2,2018,12)) << " payout " << payout << std::endl;


    IntradeBar.get_payout(payout, xtime::get_timestamp(3,3,2018,6), 180, 0, 4000);
    std::cout << "date " << xtime::get_str_date_time(xtime::get_timestamp(3,3,2018,6)) << " payout " << payout << std::endl;

    IntradeBar.get_payout(payout, xtime::get_timestamp(5,3,2018,6), 180, 0, 5000);
    std::cout << "date " << xtime::get_str_date_time(xtime::get_timestamp(5,3,2018,6)) << " payout " << payout << std::endl;

    IntradeBar.get_payout(payout, xtime::get_timestamp(7,5,2019,6), 180, 0, 4000);
    std::cout << "date " << xtime::get_str_date_time(xtime::get_timestamp(7,5,2019,6)) << " payout " << payout << std::endl;
    return 0;
}
