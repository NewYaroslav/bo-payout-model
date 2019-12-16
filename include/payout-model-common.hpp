#ifndef PAYOUT_MODEL_COMMON_HPP_INCLUDED
#define PAYOUT_MODEL_COMMON_HPP_INCLUDED

#include <string>
#include <array>
#include <map>

namespace payout_model {
    /// Список типов причин отсутствия выплат
    enum ErrorType {
        OK = 0, ///< Ошибки нет
    };

    static const uint32_t INTRADE_BAR_CURRENCY_PAIRS = 26;  /**< Количество торговых символов */
    const std::array<std::string, INTRADE_BAR_CURRENCY_PAIRS>
            intrade_bar_currency_pairs = {
        "EURUSD","USDJPY","GBPUSD","USDCHF",
        "USDCAD","EURJPY","AUDUSD","NZDUSD",
        "EURGBP","EURCHF","AUDJPY","GBPJPY",
        "CHFJPY","EURCAD","AUDCAD","CADJPY",
        "NZDJPY","AUDNZD","GBPAUD","EURAUD",
        "GBPCHF","EURNZD","AUDCHF","GBPNZD",
        "GBPCAD","XAUUSD",
    }; ///< Список доступных валютных пар брокера IntradeBar

    static const std::map<std::string, uint32_t> intrade_bar_currency_pairs_index = {
        {"EURUSD",0},{"USDJPY",1},{"GBPUSD",2},{"USDCHF",3},
        {"USDCAD",4},{"EURJPY",5},{"AUDUSD",6},{"NZDUSD",7},
        {"EURGBP",8},{"EURCHF",9},{"AUDJPY",10},{"GBPJPY",11},
        {"CHFJPY",12},{"EURCAD",13},{"AUDCAD",14},{"CADJPY",15},
        {"NZDJPY",16},{"AUDNZD",17},{"GBPAUD",18},{"EURAUD",19},
        {"GBPCHF",20},{"EURNZD",21},{"AUDCHF",22},{"GBPNZD",23},
        {"GBPCAD",24},{"XAUUSD",25}
    };  /**< Пары ключ-значение для имен символов и их порядкового номера */

    static const uint32_t INTRADE_BAR_CURRENCY_PAIRS_REAL = 22; /**< Количество реально используемых торговых символов */

    const std::array<bool, INTRADE_BAR_CURRENCY_PAIRS>
            is_intrade_bar_currency_pairs = {
        true,true,false,true,
        true,true,true,true,
        true,true,true,true,
        false,true,true,true,
        true,true,true,true,
        true,false,true,true,
        false,true,
    }; ///< Список доступных валютных пар брокера IntradeBar
}

#endif // PAYOUT_MODEL_COMMON_HPP_INCLUDED
