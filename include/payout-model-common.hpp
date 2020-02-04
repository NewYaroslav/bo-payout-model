/*
* bo-payout-model - C ++ header-only library with binary payout brokers percent payout models
*
* Copyright (c) 2020 Elektro Yar. Email: git.electroyar@gmail.com
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
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

    static const uint32_t INTRADE_BAR_CURRENCY_PAIRS = 26;  /**< Количество торговых символов у брокера Intrade.bar */
    static const uint32_t GRANDCAPITAL_CURRENCY_PAIRS = 27;  /**< Количество торговых символов у брокера Grandcapital */

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

    /*

    86% платят на следующих валютных парах: EURUSD
    85% платят на следующих валютных парах: AUDCAD, AUDCHF, AUDJPY, AUDNZD, AUDUSD, CADCHF, CADJPY, EURGBP, EURJPY, GBPUSD, XAUUSD, NZDJPY, NZDUSD, USDCAD
    80% платят на следующих валютных парах: CHFJPY, EURAUD, EURCAD, EURNZD, GBPAUD, GBPCAD, GBPCHF, GBPJPY, USDCHF, USDJPY
    60% платят на следующих валютных парах: EURCHF, XAGUSD

    */
    const std::array<std::string, GRANDCAPITAL_CURRENCY_PAIRS>
            grandcapital_currency_pairs = {
        "EURUSD","USDJPY","GBPUSD","USDCHF",
        "USDCAD","EURJPY","AUDUSD","NZDUSD",
        "EURGBP","EURCHF","AUDJPY","GBPJPY",
        "CHFJPY","EURCAD","AUDCAD","CADJPY",
        "NZDJPY","AUDNZD","GBPAUD","EURAUD",
        "GBPCHF","EURNZD","AUDCHF","CADCHF",
        "GBPCAD","XAUUSD","XAGUSD"
    }; ///< Список доступных валютных пар брокера Grandcapital

    static const std::map<std::string, uint32_t> intrade_bar_currency_pairs_index = {
        {"EURUSD",0},{"USDJPY",1},{"GBPUSD",2},{"USDCHF",3},
        {"USDCAD",4},{"EURJPY",5},{"AUDUSD",6},{"NZDUSD",7},
        {"EURGBP",8},{"EURCHF",9},{"AUDJPY",10},{"GBPJPY",11},
        {"CHFJPY",12},{"EURCAD",13},{"AUDCAD",14},{"CADJPY",15},
        {"NZDJPY",16},{"AUDNZD",17},{"GBPAUD",18},{"EURAUD",19},
        {"GBPCHF",20},{"EURNZD",21},{"AUDCHF",22},{"GBPNZD",23},
        {"GBPCAD",24},{"XAUUSD",25}
    };  /**< Пары ключ-значение для имен символов и их порядкового номера */

    static const std::map<std::string, uint32_t> grandcapital_currency_pairs_index = {
        {"EURUSD",0},{"USDJPY",1},{"GBPUSD",2},{"USDCHF",3},
        {"USDCAD",4},{"EURJPY",5},{"AUDUSD",6},{"NZDUSD",7},
        {"EURGBP",8},{"EURCHF",9},{"AUDJPY",10},{"GBPJPY",11},
        {"CHFJPY",12},{"EURCAD",13},{"AUDCAD",14},{"CADJPY",15},
        {"NZDJPY",16},{"AUDNZD",17},{"GBPAUD",18},{"EURAUD",19},
        {"GBPCHF",20},{"EURNZD",21},{"AUDCHF",22},{"CADCHF",23},
        {"GBPCAD",24},{"XAUUSD",25},{"XAGUSD",26}
    };  /**< Пары ключ-значение для имен символов и их порядкового номера */

    static const uint32_t INTRADE_BAR_CURRENCY_PAIRS_REAL = 22; /**< Количество реально используемых торговых символов */
    static const uint32_t GRANDCAPITAL_CURRENCY_PAIRS_REAL = 27; /**< Количество реально используемых торговых символов */

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

    const std::array<bool, GRANDCAPITAL_CURRENCY_PAIRS>
            is_grandcapital_currency_pairs = {
        true,true,true,true,
        true,true,true,true,
        true,true,true,true,
        true,true,true,true,
        true,true,true,true,
        true,true,true,true,
        true,true,true,
    }; ///< Список доступных валютных пар брокера Grandcapital

    /*

    86% платят на следующих валютных парах: EURUSD
    85% платят на следующих валютных парах: AUDCAD, AUDCHF, AUDJPY, AUDNZD, AUDUSD, CADCHF, CADJPY, EURGBP, EURJPY, GBPUSD, XAUUSD, NZDJPY, NZDUSD, USDCAD
    80% платят на следующих валютных парах: CHFJPY, EURAUD, EURCAD, EURNZD, GBPAUD, GBPCAD, GBPCHF, GBPJPY, USDCHF, USDJPY
    60% платят на следующих валютных парах: EURCHF, XAGUSD
    const std::array<std::string, GRANDCAPITAL_CURRENCY_PAIRS>
            grandcapital_currency_pairs = {
        "EURUSD","USDJPY","GBPUSD","USDCHF",
        "USDCAD","EURJPY","AUDUSD","NZDUSD",
        "EURGBP","EURCHF","AUDJPY","GBPJPY",
        "CHFJPY","EURCAD","AUDCAD","CADJPY",
        "NZDJPY","AUDNZD","GBPAUD","EURAUD",
        "GBPCHF","EURNZD","AUDCHF","CADCHF",
        "GBPCAD","XAUUSD","XAGUSD"
    }; ///< Список доступных валютных пар брокера Grandcapital
    */
    const std::array<double, GRANDCAPITAL_CURRENCY_PAIRS>
            grandcapital_currency_pairs_payout = {
        0.86,0.80,0.85,0.80,
        0.85,0.85,0.85,0.85,
        0.85,0.60,0.85,0.80,
        0.80,0.80,0.85,0.85,
        0.85,0.85,0.80,0.80,
        0.80,0.80,0.85,0.85,
        0.80,0.85,0.60,
    }; ///< Список процентов выплат по валютным парам брокера Grandcapital
}

#endif // PAYOUT_MODEL_COMMON_HPP_INCLUDED
