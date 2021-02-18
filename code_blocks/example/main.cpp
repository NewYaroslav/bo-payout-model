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

    IntradeBar.get_payout(payout, xtime::get_timestamp(3,3,2018,4,59), 180, 0, 5000);
    std::cout << "date " << xtime::get_str_date_time(xtime::get_timestamp(3,3,2018,4,59)) << " payout " << payout << std::endl;

    IntradeBar.get_payout(payout, xtime::get_timestamp(3,3,2018,5), 180, 0, 4000);
    std::cout << "date " << xtime::get_str_date_time(xtime::get_timestamp(3,3,2018,5)) << " payout " << payout << std::endl;

    IntradeBar.get_payout(payout, xtime::get_timestamp(5,3,2018,6), 180, 0, 5000);
    std::cout << "date " << xtime::get_str_date_time(xtime::get_timestamp(5,3,2018,6)) << " payout " << payout << std::endl;

    IntradeBar.get_payout(payout, xtime::get_timestamp(5,3,2018,8), 180, 0, 5000);
    std::cout << "date " << xtime::get_str_date_time(xtime::get_timestamp(5,3,2018,8)) << " payout " << payout << std::endl;

    IntradeBar.get_payout(payout, xtime::get_timestamp(5,3,2018,6), 180, 0, 4000);
    std::cout << "date " << xtime::get_str_date_time(xtime::get_timestamp(5,3,2018,6)) << " payout " << payout << std::endl;

    IntradeBar.get_payout(payout, "EURUSD", xtime::get_timestamp(5,3,2019,9), 180, 5000);
    std::cout << "date " << xtime::get_str_date_time(xtime::get_timestamp(5,3,2019,9)) << " payout " << payout << std::endl;

    IntradeBar.get_payout(payout, "EURUSD", xtime::get_timestamp(5,3,2019,20,58), 180, 5000);
    std::cout << "date " << xtime::get_str_date_time(xtime::get_timestamp(5,3,2019,20,58)) << " payout " << payout << std::endl;

    std::cout << "err " << IntradeBar.get_payout(payout, xtime::get_timestamp(7,5,2019,6), 180, 0, 4000) << std::endl;

    std::cout << "date " << xtime::get_str_date_time(xtime::get_timestamp(7,5,2019,6)) << " payout " << payout << std::endl;

    if(payout_model::IntradeBar::check_minute_day(1300) == payout_model::OK)
        std::cout << "check minute day 1300" << endl;
    if(payout_model::IntradeBar::check_minute_day(33) == payout_model::OK)
        std::cout << "check minute day 33" << endl;
    if(payout_model::IntradeBar::check_minute_day(1260) == payout_model::OK)
        std::cout << "check minute day 1260" << endl;
    if(payout_model::IntradeBar::check_timestamp(xtime::get_timestamp(12,12,2019,0,0,0))
        == payout_model::OK)
        std::cout << "check timestamp " << xtime::get_str_date_time(xtime::get_timestamp(12,12,2019,0,0,0)) << endl;

    std::cout << payout_model::IntradeBar::get_currecy_pair_name(0) << endl;
    std::cout << "EURUSD: " << payout_model::IntradeBar::check_currecy_pair_name("EURUSD") << endl;
    std::cout << "GBPCAD: " << payout_model::IntradeBar::check_currecy_pair_name("GBPCAD") << endl;

    double amount = 0.0;
    int err = IntradeBar.get_amount(amount, payout, "EURUSD", xtime::get_timestamp(4,1,2018,1,4), 180, 96000, 0.6, 0.4);
    std::cout << "amount: " << amount << " / balance 96000, payout: " << payout << " code: " << err << std::endl;
    err =  IntradeBar.get_amount(amount, payout, "EURUSD", xtime::get_timestamp(4,1,2018,1,4), 180, 97000, 0.6, 0.4);
    std::cout << "amount: " << amount << " / balance 97000, payout: " << payout << " code: " << err << std::endl;

    err =  IntradeBar.get_amount(amount, payout, "EURUSD", xtime::get_timestamp(31,1,2020,20,54), 180, 48390, 0.56, 0.2);
    std::cout << "amount: " << amount << " / balance 48390, payout: " << payout << " code: " << err << std::endl;

    err =  IntradeBar.get_amount(amount, payout, "EURUSD", xtime::get_timestamp(31,1,2020,20,54), 180, 48390, 0.6, 0.2);
    std::cout << "amount: " << amount << " / balance 48390, payout: " << payout << " code: " << err << std::endl;

    err =  IntradeBar.get_amount(amount, payout, "EURUSD", xtime::get_timestamp(31,1,2020,20,54), 180, 48390, 0.6, 0.2, 0.8);
    std::cout << "amount: " << amount << " / balance 48390, payout: " << payout << " code: " << err << std::endl;

    err =  IntradeBar.get_amount(amount, payout, "EURUSD", xtime::get_timestamp(31,1,2020,20,54), 180, 483900, 0.6, 0.2, 0.8);
    std::cout << "amount: " << amount << " / balance 483900, payout: " << payout << " code: " << err << std::endl;
    return 0;
}
