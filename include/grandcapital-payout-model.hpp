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
#ifndef GRANDCAPITAL_PAYOUT_MODEL_HPP_INCLUDED
#define GRANDCAPITAL_PAYOUT_MODEL_HPP_INCLUDED

#include "payout-model-common.hpp"
#include <vector>
#include "xtime.hpp"

namespace payout_model {

    /** \brief Класс модели процентов выплат брокера Intrade.bar
     */
	class Grandcapital {
    private:
        uint32_t currency_name;         ///< Наименование валюты счета. Как правило, USD или RUB

    public:

        /// Список типов причин отсутствия выплат
        enum PayoutCancelType {
            DAY_OFF = -1,				    ///< Выходной день или праздник
            NIGHT_HOURS = -2, 			    ///< Ночное время, когда брокер не принмает ставки
            BEGIN_EVENING_HOUR = -3, 		///< Первые минуты в начале часа вечером (либо последняя минута в конце часа)
            TOO_LITTLE_TIME = -4, 		    ///< Слишком короткое время экспирации
            TOO_MUCH_TIME = -5,             ///< Слишком длинное время экспирации
            CURRENCY_PAIR_IS_MISSING = -6,  ///< Отсутствует валютная пара с указанным индексом
            TOO_LITTLE_MONEY = -7,		    ///< Слишком низкая ставка
            FXCM_MON = -8,                  ///< Нет котировок от FXCM в понедельник в 0 час UTC
            EXPIRATION_ERROR = -9, 		    ///< Ошибка экспирации
            TOO_LITTLE_WINRATE = -10,		///< Слишком низкий винрейт
        };

        /// Список валют счета
        enum AccountCurrencyName {
            CURRENCY_RUB = 0,       ///< Рублевая валюта счета
            CURRENCY_USD = 1,       ///< Долларовый счет
        };

        /** \brief Проверить имя валютной пары
         * \param currency_pair Имя валютной пары
         * \return Вернет true, если указанная валютная пара поддерживается брокером
         */
        inline static const bool check_currecy_pair_name(const std::string &currency_pair) {
            std::string temp(currency_pair);
            if(temp.length() > 6) temp = temp.substr(0,6);
            auto it = grandcapital_currency_pairs_index.find(temp);
            if(it == grandcapital_currency_pairs_index.end()) {
                return false;
            }
            uint32_t index = it->second;
            if(is_grandcapital_currency_pairs[index]) return true;
            return false;
        }

        /** \brief Проверить минуту дня
         *
         * Данный метод проверяет, разрешает ли брокер торговлю в данное время
         * \param minute_day Минута дня
         * \return Вернет PayoutCancelType::OK если торговать можно, иначе код ошибки
         */
        inline static const int check_minute_day(
                const uint32_t &minute_day,
                const bool is_old_version = false) {
            uint32_t hour = minute_day/xtime::MINUTES_IN_HOUR;
            /* С 22 февраля 2019 торговля доступна с 22:00 до 2:00 по терминальному времени (GMT+2)
             * Это с 20:00 до 0:00 по UTC
             */
            if(hour >= 20) return PayoutCancelType::NIGHT_HOURS;
            return ErrorType::OK;
        }

        /** \brief Проверить метку времени
         * Данный метод проверяет, разрешает ли брокер торговлю в данное время
         * \param timestamp Метка времени
         * \return Вернет PayoutCancelType::OK если торговать можно, иначе код ошибки
         */
        inline static const int check_timestamp(const xtime::timestamp_t &timestamp) {
            xtime::DateTime iDateTime(timestamp);
            const int weekday = iDateTime.get_weekday();

            /* Если операция выполнена в субботу, воскресенье,
			 * 1 января или 25 августа
			 */
            if(	weekday == xtime::SUN || weekday == xtime::SAT ||
				(iDateTime.day == 1 && iDateTime.month == xtime::JAN) ||
				(iDateTime.day == 25 && iDateTime.month == xtime::DEC))
                return PayoutCancelType::DAY_OFF;

            /* С 22 февраля 2019 торговля доступна с 22:00 до 2:00 по терминальному времени (GMT+2)
             * Это с 20:00 до 0:00 по UTC
             */
            if(iDateTime.hour >= 20) return PayoutCancelType::NIGHT_HOURS;
            return ErrorType::OK;
        }

        /** \brief Получить процент выплат
         * Проценты выплат варьируются обычно от 0 до 1.0, где 1.0 соответствует 100% выплате брокера
         * \param[out] payout процент выплат
         * \param[in] timestamp временную метку unix времени (GMT)
         * \param[in] duration длительность опциона в секундах
         * \param[in] currency_pair_index  номер валютной пары из списка валютных пар брокера
         * \param[in] amount размер ставки бинарного опциона
         * \return состояние выплаты (0 в случае успеха, иначе см. PayoutCancelType)
         */
        inline const int get_payout(
                double &payout,
                const xtime::timestamp_t timestamp,
                const uint32_t duration,
                const uint32_t currency_pair_index,
                const double amount) {
            payout = 0.0;
            /* Если продолжительность экспирации меньше 1 минуты (60 секунд) */
            if(duration < 60) return PayoutCancelType::TOO_LITTLE_TIME;
            /* Если продолжительность экспирации больше 2880 минут (172800 секунд) */
            if(duration > 172800) return PayoutCancelType::TOO_MUCH_TIME;
            if(currency_pair_index > grandcapital_currency_pairs.size() ||
                !is_grandcapital_currency_pairs[currency_pair_index])
                return PayoutCancelType::CURRENCY_PAIR_IS_MISSING;

            if((currency_name == CURRENCY_USD && amount < 1)||
                (currency_name == CURRENCY_RUB && amount < 50))
                return PayoutCancelType::TOO_LITTLE_MONEY;

            const uint32_t hour = xtime::get_hour_day(timestamp);
            const uint32_t weekday = xtime::get_weekday(timestamp);
            /* пропускаем выходные дни */
            if(weekday == xtime::SAT || weekday == xtime::SUN)
                return ErrorType::OK;
            if(hour >= 20) return PayoutCancelType::NIGHT_HOURS;
            payout = grandcapital_currency_pairs_payout[currency_pair_index];
            return ErrorType::OK;
        };

        /** \brief Получить процент выплат
         *
         * Проценты выплат варьируются обычно от 0 до 1.0, где 1.0 соответствует 100% выплате брокера
         * \param[out] payout процент выплат
         * \param[in] currency_pair Имя валютной пары
         * \param[in] timestamp временную метку unix времени (GMT)
         * \param[in] duration длительность опциона в секундах
         * \param[in] amount размер ставки бинарного опциона
         * \return состояние выплаты (0 в случае успеха, иначе см. PayoutCancelType)
         */
        inline const int get_payout(
                double &payout,
                const std::string &currency_pair,
                const xtime::timestamp_t timestamp,
                const uint32_t duration,
                const double amount) {
            std::string temp(currency_pair);
            if(temp.length() > 6) temp = temp.substr(0,6);
            auto it = grandcapital_currency_pairs_index.find(temp);
            if(it == grandcapital_currency_pairs_index.end()) {
                return PayoutCancelType::CURRENCY_PAIR_IS_MISSING;
            }
            uint32_t index = it->second;
            if(!is_grandcapital_currency_pairs[index]) return PayoutCancelType::CURRENCY_PAIR_IS_MISSING;
            return get_payout(payout, timestamp, duration, index, amount);
        }

        /** \brief Получить абсолютный размер ставки и процент выплат
         *
         * Проценты выплат варьируются обычно от 0 до 1.0, где 1.0 соответствует 100% выплате брокера
         * \param[out] amount размер ставки бинарного опциона
		 * \param[out] payout процент выплат
         * \param[in] currency_pair Имя валютной пары
         * \param[in] timestamp временную метку unix времени (GMT)
         * \param[in] duration длительность опциона в секундах
         * \param[in] balance Размер депозита
		 * \param[in] winrate Винрейт
		 * \param[in] attenuator Коэффициент ослабления Келли
		 * \param[in] payout_limiter Ограничитель процента выплат (по умолчанию не используется)
		 * \param[in] winrate_limiter Ограничитель винрейта (по умолчанию не используется)
         * \return состояние выплаты (0 в случае успеха, иначе см. PayoutCancelType)
         */
        inline const int get_amount(
                double &amount,
                double &payout,
                const std::string &currency_pair,
                const xtime::timestamp_t timestamp,
                const uint32_t duration,
                const double balance,
                const double winrate,
                const double attenuator,
                const double payout_limiter = 1.0,
                const double winrate_limiter = 1.0) {
            amount = 0;
            payout = 0;
            /* Если продолжительность экспирации меньше 1 минуты (60 секунд) */
            if(duration < 60) return PayoutCancelType::TOO_LITTLE_TIME;
            /* Если продолжительность экспирации больше 2880 минут (172800 секунд) */
            if(duration > 172800) return PayoutCancelType::TOO_MUCH_TIME;

            std::string temp(currency_pair);
            if(temp.length() > 6) temp = temp.substr(0,6);

            /* получение индекса валютной пары и проверка символа на выплату */
            auto it = grandcapital_currency_pairs_index.find(temp);
            if(it == grandcapital_currency_pairs_index.end()) {
                return PayoutCancelType::CURRENCY_PAIR_IS_MISSING;
            }
            uint32_t index = it->second;
            if(!is_grandcapital_currency_pairs[index]) return PayoutCancelType::CURRENCY_PAIR_IS_MISSING;

            const uint32_t hour = xtime::get_hour_day(timestamp);
            const uint32_t weekday = xtime::get_weekday(timestamp);
            /* пропускаем выходные дни */
            if(weekday == xtime::SAT || weekday == xtime::SUN)  return ErrorType::OK;
            if(hour >= 20) return PayoutCancelType::NIGHT_HOURS;
            payout = grandcapital_currency_pairs_payout[index];
            if(winrate <= (1.0 / (1.0 + payout))) return PayoutCancelType::TOO_LITTLE_WINRATE;
            const double calc_payout = std::min(payout_limiter, payout);
            const double calc_winrate = std::min(winrate_limiter, winrate);
            if(calc_winrate <= (1.0 / (1.0 + payout))) return PayoutCancelType::TOO_LITTLE_WINRATE;
            const double rate = (((1.0 + calc_payout) * calc_winrate - 1.0) / calc_payout) * attenuator;
            amount = balance * rate;
            if((currency_name == CURRENCY_USD && amount < 1)||
            (currency_name == CURRENCY_RUB && amount < 50)) {
                amount = 0;
                return PayoutCancelType::TOO_LITTLE_MONEY;
            }
            return ErrorType::OK;
        }

        /** \brief Получить имя валютной пары по ее номеру
         * \param[in] currency_pair_index  номер валютной пары из списка валютных пар брокера
         * \return имя валютной пары либо пустую строку, если указанный индекс отсутствует в списке валютных пар
         */
        inline const static std::string get_currecy_pair_name(const uint32_t currency_pair_index) {
            if(currency_pair_index < grandcapital_currency_pairs.size())
                return grandcapital_currency_pairs[currency_pair_index];
            return std::string();//Возврат пустой строки
        };

        /** \brief Установить рублевый счет или долларовый
         * \param is_rub Рубли, если true. Иначе USD
         */
        void set_rub_account_currency(const bool is_rub) {
            if(is_rub) currency_name = CURRENCY_RUB;
            else currency_name = CURRENCY_USD;
        }

        /** \brief Конструктор класса модели процентов выплат брокера intrade.bar
         * \param user_currency_name Валюта счета, по умолчанию RUB
         */
        Grandcapital(const uint32_t user_currency_name = CURRENCY_RUB) :
            currency_name(user_currency_name) {
        }

        ~Grandcapital() {}
	};
}


#endif // GRANDCAPITAL-PAYOUT-MODEL_HPP_INCLUDED
