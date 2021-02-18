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
#ifndef INTRADE_BAR_PAYOUT_MODEL_H_INCLUDED
#define INTRADE_BAR_PAYOUT_MODEL_H_INCLUDED

#include "payout-model-common.hpp"
#include <vector>
#include "xtime.hpp"

namespace payout_model {

    /** \brief Класс модели процентов выплат брокера Intrade.bar
     */
	class IntradeBar {
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
            EXIT_OVER_END_DAY = -11,        ///< Выход за конец дня
        };

        /// Список валют счета
        enum AccountCurrencyName {
            CURRENCY_RUB = 0,       ///< Рублевая валюта счета
            CURRENCY_USD = 1,       ///< Долларовый счет
        };

        const double THRESHOLD_AMOUNT_RUB = 5000;
        const double THRESHOLD_AMOUNT_USD = 80;
        const double MIN_AMOUNT_RUB = 50;
        const double MIN_AMOUNT_USD = 1;
        const double MAX_AMOUNT_RUB = 25000;
        const double MAX_AMOUNT_USD = 500;


        /** \brief Проверить имя валютной пары
         * \param currency_pair Имя валютной пары
         * \return Вернет true, если указанная валютная пара поддерживается брокером
         */
        inline static const bool check_currecy_pair_name(const std::string &currency_pair) {
            auto it = intrade_bar_currency_pairs_index.find(currency_pair);
            if(it == intrade_bar_currency_pairs_index.end()) {
                return false;
            }
            uint32_t index = it->second;
            if(is_intrade_bar_currency_pairs[index]) return true;
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
            /* нельзя открывать сделки с 0 часов по МСК до 4
             * или 1 по UTC
             */
            if(hour >= 21 || hour < 1) return PayoutCancelType::NIGHT_HOURS;
            return ErrorType::OK;
        }

        /** \brief Проверить метку времени
         * Данный метод проверяет, разрешает ли брокер торговлю в данное время
         * \param timestamp Метка времени
         * \param is_old_version Использовать старую версию процентов выплат
         * \return Вернет PayoutCancelType::OK если торговать можно, иначе код ошибки
         */
        inline static const int check_timestamp(
                const xtime::timestamp_t &timestamp,
                const bool is_old_version = false) {
            xtime::DateTime iDateTime(timestamp);
            const int weekday = iDateTime.get_weekday();

            /* Если операция выполнена в субботу, воскресенье,
			 * 1 января или 25 августа
			 */
            if(	weekday == xtime::SUN || weekday == xtime::SAT ||
				(iDateTime.day == 1 && iDateTime.month == xtime::JAN) ||
				(iDateTime.day == 25 && iDateTime.month == xtime::DEC))
                return PayoutCancelType::DAY_OFF;

            /* пропускаем 0 час по UTC в понедельник */
            if(weekday == xtime::MON && iDateTime.hour == 0)
                return PayoutCancelType::FXCM_MON;

            /* Если операция выполнена после 21:00 по Гринвичу до 00:00 */
            if(iDateTime.hour >= 21 || iDateTime.hour < 1) return PayoutCancelType::NIGHT_HOURS;
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

            /* обрабатываем выход экспирации за конец дня */
            const xtime::timestamp_t last_time =
                21 * xtime::SECONDS_IN_HOUR +
                xtime::get_first_timestamp_day(timestamp);
            if ((timestamp + duration) > last_time)
                return PayoutCancelType::EXIT_OVER_END_DAY;

            /* Если продолжительность экспирации меньше 3 минут (180 секунд) */
            if (duration < 180) return PayoutCancelType::TOO_LITTLE_TIME;

            /* Если продолжительность экспирации больше 500 минут (30000 секунд) */
            if (duration > 30000) return PayoutCancelType::TOO_MUCH_TIME;

            if (currency_pair_index > intrade_bar_currency_pairs.size() ||
                !is_intrade_bar_currency_pairs[currency_pair_index])
                return PayoutCancelType::CURRENCY_PAIR_IS_MISSING;

            if ((currency_name == CURRENCY_USD && amount < MIN_AMOUNT_USD)||
                (currency_name == CURRENCY_RUB && amount < MIN_AMOUNT_RUB))
                return PayoutCancelType::TOO_LITTLE_MONEY;

            const uint32_t hour = xtime::get_hour_day(timestamp);
            const uint32_t minute = xtime::get_minute_hour(timestamp);
            const uint32_t weekday = xtime::get_weekday(timestamp);
            /* пропускаем выходные дни */
            if(weekday == xtime::SAT || weekday == xtime::SUN)
                return ErrorType::OK;
            /* пропускаем 0 час по UTC в понедельник */
            if(weekday == xtime::MON && hour == 0)
                return PayoutCancelType::FXCM_MON;
            if(hour >= 21 || hour < 1) return PayoutCancelType::NIGHT_HOURS;
            /* с 4 часа по МСК до 9 утра по МСК процент выполат 60%
             * с 17 часов по МСК  процент выплат в течении 3 минут в начале часа и конце часа также составляет 60%
             */
            if(hour <= 6 || hour >= 14) {
                /* с 4 часа по МСК до 9 утра по МСК процент выполат 60% или 63%
                 * с 17 часов по МСК  процент выплат в течении 3 минут в начале часа и конце часа также составляет 60% или 63%
                 */
                if(minute >= 57 || minute <= 2) {
                    if ((currency_name == CURRENCY_USD && amount >= THRESHOLD_AMOUNT_USD)||
                        (currency_name == CURRENCY_RUB && amount >= THRESHOLD_AMOUNT_RUB)) {
                        payout = 0.63;
                    } else {
                        payout = 0.6;
                    }
                    return ErrorType::OK;
                }
            }
            if(hour == 13 && minute >= 57) {
                if ((currency_name == CURRENCY_USD && amount >= THRESHOLD_AMOUNT_USD)||
                    (currency_name == CURRENCY_RUB && amount >= THRESHOLD_AMOUNT_RUB)) {
                    payout = 0.63;
                } else {
                    payout = 0.6;
                }
                return ErrorType::OK;
            }
            /* Если счет в долларах и ставка больше 80 долларов или счет в рублях и ставка больше THRESHOLD_AMOUNT_RUB рублей */
            if((currency_name == CURRENCY_USD && amount >= THRESHOLD_AMOUNT_USD)||
                (currency_name == CURRENCY_RUB && amount >= THRESHOLD_AMOUNT_RUB)) {
                payout = 0.85; // Процент выплат составит 85 (0,85)
            } else {
                /* Если счет в долларах и ставка меньше 80 долларов или счет в рублях и ставка меньше THRESHOLD_AMOUNT_RUB рублей */
                if(duration == 180) {
                    /* Если продолжительность экспирации 3 минуты
                     * Процент выплат составит 82 (0,82)
                     */
                    payout = 0.82;
                } else {
                    /* Если продолжительность экспирации от 4 до 500 минут */
                    if(duration >= 240 && duration <= 30000) {
                        payout = 0.79; // Процент выплат составит 79 (0,79)
                    } else return PayoutCancelType::EXPIRATION_ERROR;
                }
            }
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
            auto it = intrade_bar_currency_pairs_index.find(currency_pair);
            if(it == intrade_bar_currency_pairs_index.end()) {
                return PayoutCancelType::CURRENCY_PAIR_IS_MISSING;
            }
            uint32_t index = it->second;
            if(!is_intrade_bar_currency_pairs[index]) return PayoutCancelType::CURRENCY_PAIR_IS_MISSING;
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

            /* обрабатываем выход экспирации за конец дня */
            const xtime::timestamp_t last_time =
                21 * xtime::SECONDS_IN_HOUR +
                xtime::get_first_timestamp_day(timestamp);
            if ((timestamp + duration) > last_time)
                return PayoutCancelType::EXIT_OVER_END_DAY;

            /* Если продолжительность экспирации меньше 3 минут (180 секунд) */
            if(duration < 180) return PayoutCancelType::TOO_LITTLE_TIME;
            /* Если продолжительность экспирации больше 500 минут (30000 секунд) */
            if(duration > 30000) return PayoutCancelType::TOO_MUCH_TIME;

            /* получение индекса валютной пары и проверка символа на выплату */
            auto it = intrade_bar_currency_pairs_index.find(currency_pair);
            if(it == intrade_bar_currency_pairs_index.end()) {
                return PayoutCancelType::CURRENCY_PAIR_IS_MISSING;
            }
            uint32_t index = it->second;
            if(!is_intrade_bar_currency_pairs[index]) return PayoutCancelType::CURRENCY_PAIR_IS_MISSING;

            const uint32_t hour = xtime::get_hour_day(timestamp);
            const uint32_t minute = xtime::get_minute_hour(timestamp);
            const uint32_t weekday = xtime::get_weekday(timestamp);
            /* пропускаем выходные дни */
            if(weekday == xtime::SAT || weekday == xtime::SUN)  return ErrorType::OK;
            /* пропускаем 0 час по UTC в понедельник */
            if(weekday == xtime::MON && hour == 0) return PayoutCancelType::FXCM_MON;
            if(hour >= 21 || hour < 1) return PayoutCancelType::NIGHT_HOURS;
            if(hour <= 6 || hour >= 14 || (hour == 13 && minute >= 57)) {
                /* с 1 часа по МСК до 8 утра по МСК процент выполат 60%
                 * с 17 часов по МСК  процент выплат в течении 3 минут в начале часа и конце часа также составляет 60%
                 */
                if(minute >= 57 || minute <= 2) {
                    payout = 0.6;
                    if(winrate <= (1.0 / 1.6)) return PayoutCancelType::TOO_LITTLE_WINRATE;
					const double calc_payout = std::min(payout_limiter, 0.6);
					const double calc_winrate = std::min(winrate_limiter, winrate);
					if(calc_winrate <= (1.0 / 1.6)) return PayoutCancelType::TOO_LITTLE_WINRATE;
                    const double rate = (((1.0 + calc_payout) * calc_winrate - 1.0) / calc_payout) * attenuator;
                    amount = balance * rate;

                    if((currency_name == CURRENCY_USD && amount < MIN_AMOUNT_USD)||
                    (currency_name == CURRENCY_RUB && amount < MIN_AMOUNT_RUB)) {
                        amount = 0;
                        return PayoutCancelType::TOO_LITTLE_MONEY;
                    }

                    if ((currency_name == CURRENCY_USD && amount >= THRESHOLD_AMOUNT_USD)||
                        (currency_name == CURRENCY_RUB && amount >= THRESHOLD_AMOUNT_RUB)) {
                        payout = 0.63;
                        if(winrate <= (1.0 / 1.63)) return PayoutCancelType::TOO_LITTLE_WINRATE;
                        const double calc_payout = std::min(payout_limiter, 0.63);
                        const double calc_winrate = std::min(winrate_limiter, winrate);
                        if(calc_winrate <= (1.0 / 1.63)) return PayoutCancelType::TOO_LITTLE_WINRATE;
                        const double rate = (((1.0 + calc_payout) * calc_winrate - 1.0) / calc_payout) * attenuator;
                        amount = balance * rate;
                    }
                    return ErrorType::OK;
                }
            }
            if(duration == 180) {
                if(winrate <= (1.0 / 1.85)) return PayoutCancelType::TOO_LITTLE_WINRATE;

                const double calc_high_payout = std::min(payout_limiter, 0.85);
                const double calc_winrate = std::min(winrate_limiter, winrate);

                if(calc_winrate <= (1.0 / 1.85)) return PayoutCancelType::TOO_LITTLE_WINRATE;

                const double high_rate = (((1.0 + calc_high_payout) * calc_winrate - 1.0) / calc_high_payout) * attenuator;
                const double high_amount = balance * high_rate;
                if((currency_name == CURRENCY_USD && high_amount >= THRESHOLD_AMOUNT_USD)||
                    (currency_name == CURRENCY_RUB && high_amount >= THRESHOLD_AMOUNT_RUB)) {
                    payout = 0.85;
                    amount = high_amount;
                    if((currency_name == CURRENCY_USD && amount < MIN_AMOUNT_USD)||
                    (currency_name == CURRENCY_RUB && amount < MIN_AMOUNT_RUB)) {
                        amount = 0;
                        return PayoutCancelType::TOO_LITTLE_MONEY;
                    }
                    return ErrorType::OK;
                }
                if(winrate <= (1.0 / 1.82)) return PayoutCancelType::TOO_LITTLE_WINRATE;
                if(calc_winrate <= (1.0 / 1.82)) return PayoutCancelType::TOO_LITTLE_WINRATE;
                payout = 0.82;

                const double calc_low_payout = std::min(payout_limiter, 0.82);
                const double low_rate = (((1.0 + calc_low_payout) * calc_winrate - 1.0) / calc_low_payout) * attenuator;
                amount = balance * low_rate;
                if((currency_name == CURRENCY_USD && amount < MIN_AMOUNT_USD)||
                (currency_name == CURRENCY_RUB && amount < MIN_AMOUNT_RUB)) {
                    amount = 0;
                    return PayoutCancelType::TOO_LITTLE_MONEY;
                }
                return ErrorType::OK;
            } else
            if(duration >= 240 && duration <= 30000) {
                if(winrate <= (1.0 / 1.85)) return PayoutCancelType::TOO_LITTLE_WINRATE;
                const double calc_winrate = std::min(winrate_limiter, winrate);
                if(calc_winrate <= (1.0 / 1.82)) return PayoutCancelType::TOO_LITTLE_WINRATE;
                const double calc_high_payout = std::min(payout_limiter, 0.85);
                const double high_rate = (((1.0 + calc_high_payout) * calc_winrate - 1.0) / calc_high_payout) * attenuator;
                const double high_amount = balance * high_rate;
                if((currency_name == CURRENCY_USD && high_amount >= THRESHOLD_AMOUNT_USD)||
                    (currency_name == CURRENCY_RUB && high_amount >= THRESHOLD_AMOUNT_RUB)) {
                    payout = 0.85;
                    amount = high_amount;
                    if((currency_name == CURRENCY_USD && amount < MIN_AMOUNT_USD)||
                    (currency_name == CURRENCY_RUB && amount < MIN_AMOUNT_RUB)) {
                        amount = 0;
                        return PayoutCancelType::TOO_LITTLE_MONEY;
                    }
                    return ErrorType::OK;
                }
                if(winrate <= (1.0 / 1.79)) return PayoutCancelType::TOO_LITTLE_WINRATE;
                payout = 0.79;
                //const double calc_winrate = std::min(winrate_limiter, winrate);
                if(calc_winrate <= (1.0 / 1.82)) return PayoutCancelType::TOO_LITTLE_WINRATE;
                const double calc_low_payout = std::min(payout_limiter, 0.79);
                const double low_rate = (((1.0 + calc_low_payout)* calc_winrate - 1.0) / calc_low_payout) * attenuator;
                amount = balance * low_rate;
                if((currency_name == CURRENCY_USD && amount < MIN_AMOUNT_USD)||
                (currency_name == CURRENCY_RUB && amount < MIN_AMOUNT_RUB)) {
                    amount = 0;
                    return PayoutCancelType::TOO_LITTLE_MONEY;
                }
                return ErrorType::OK;
            } else return PayoutCancelType::EXPIRATION_ERROR;
            return ErrorType::OK;
        }

        /** \brief Получить имя валютной пары по ее номеру
         * \param[in] currency_pair_index  номер валютной пары из списка валютных пар брокера
         * \return имя валютной пары либо пустую строку, если указанный индекс отсутствует в списке валютных пар
         */
        inline const static std::string get_currecy_pair_name(const uint32_t currency_pair_index) {
            if(currency_pair_index < intrade_bar_currency_pairs.size())
                return intrade_bar_currency_pairs[currency_pair_index];
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
        IntradeBar(const uint32_t user_currency_name = CURRENCY_RUB) :
            currency_name(user_currency_name) {
        }

        ~IntradeBar() {}
	};
}

#endif // INTRADE_BAR_PAYOUT_MODEL_H_INCLUDED
