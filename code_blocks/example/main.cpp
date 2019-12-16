#include <iostream>
#include "intrade-bar-payout-model.hpp"

using namespace std;

int main() {
    std::cout << "Hello world!" << std::endl;
    /* класс модели процента выплат брокера */
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

    std::cout << "err " << IntradeBar.get_payout(payout, xtime::get_timestamp(7,5,2019,6), 180, 0, 4000) << std::endl;
    std::cout << "date " << xtime::get_str_date_time(xtime::get_timestamp(7,5,2019,6)) << " payout " << payout << std::endl;

    if(payout_model::IntradeBar::check_minute_day(1300) == payout_model::OK)
        std::cout << "check minute day 1300" << endl;
    if(payout_model::IntradeBar::check_minute_day(33) == payout_model::OK)
        std::cout << "check minute day 33" << endl;
    if(payout_model::IntradeBar::check_timestamp(xtime::get_timestamp(12,12,2019,0,0,0))
        == payout_model::OK)
        std::cout << "check timestamp " << xtime::get_str_date_time(xtime::get_timestamp(12,12,2019,0,0,0)) << endl;

    std::cout << payout_model::IntradeBar::get_currecy_pair_name(0) << endl;
    return 0;
}
