#ifndef PAYOUT_MODEL_COMMON_HPP_INCLUDED
#define PAYOUT_MODEL_COMMON_HPP_INCLUDED

#include <string>
#include <array>

namespace payout_model {
    /// Список типов причин отсутствия выплат
    enum ErrorType {
        OK = 0,						    ///< Ошибки нет
    };

    static const int INTRADE_BAR_CURRENCY_PAIRS = 26;  /**< Количество торговых символов */
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
}

#endif // PAYOUT_MODEL_COMMON_HPP_INCLUDED
