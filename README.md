![logo](doc/logo-640-160.png)
# bo-payout-model
С++ header-only библиотека с моделями процентов выплат брокеров бинарных опицонов

### Описание

На данный момент библиотека содержит только модель процентов выплат брокера [intrade.bar](www.intrade.bar/67204)

## Особенности intrade.bar

- Торговля открывается в понедельник, начиная с *1 часа по UTC* (*4 часа по МСК*).
- Начиная с *13:59:00* в начале часа и в течении нескольких минут после часа брокер запрещает торговать. 
Т.е. сделки нельзя открывать в период *XX:59:00 - XX:03:59*, где XX - час дня, начиная с 13 часов по UTC.
- Брокер имеет два типа опционов. Данная библиотека поддерживает *спринт-опционы*.
- По спринт-опционам процент выплат **82% для экспирации 3 минуты**. Для остальных экспираций выплата брокера **79%**.
- Начиная со ставки *5000 RUB* или *80 USD* процент выплат **85%** для любых экспираций.
- Минимальная ставка *50 RUB* или *1 USD* для валютных пар. Для золота (XAUUSD) минимальная ставка *500 RUB*.
- На данный момент поддерживает следующие валютные пары (22 шт): 
	EURUSD,USDJPY,~GBPUSD~,USDCHF,
	USDCAD,EURJPY,AUDUSD,NZDUSD,
	EURGBP,EURCHF,AUDJPY,GBPJPY,
	~CHFJPY~,EURCAD,AUDCAD,CADJPY,
	NZDJPY,AUDNZD,GBPAUD,EURAUD,
	GBPCHF,~EURNZD~,AUDCHF,GBPNZD,
	~GBPCAD~,XAUUSD

**Зачеркнутые валютные пары не поддерживаются брокером.**

### Как использовать

Подключите файл *intrade-bar-payout-model.hpp* в свой проект, чтобы использовать модель процентов выплат брокера [intrade.bar](www.intrade.bar/67204).

**Получение процентов выплат**

```C++
#include <iostream>
#include "intrade-bar-payout-model.hpp"

using namespace std;

int main() {
	// класс модели процентов выплат
    payout_model::IntradeBar IntradeBar;
	/* как вариант, можно задать валюту аккаунта и опцию "использовать последние условия брокера"
	 * payout_model::IntradeBar IntradeBar(payout_model::CURRENCY_USD, true);
	 */

	// выплата брокера, от 0.0 до 1.0, где 1.0 - 100% выплата
    double payout = 0.0;
	
	// это метка времени с датой по *UTC 07.05.2019 06:53:00*
	xtime::timestamp_t timestamp = xtime::get_timestamp(7,5,2019,6,53,00); 
	
	/* это длительность опциона в секундах 
	 * (здесь 3 минуты - минимальная длительность у брокера intrade.bar
	 */
	int duration = 180;
	
	/* это символ, на котором "торгуем". См. массив payout_model::intrade_bar_currency_pairs
	 * Символ под индексом 0 соответствует валютной паре EURUSD
	 */
	int symbol_ind = 0;
	
	// размер ставки в валюте счета, в данном случае в RUB
	double amount = 4000;
	
	// получаем процент выплат
    int err = IntradeBar.get_payout(payout, timestamp, duration, symbol_ind, amount);
	
	// на экране будет напечатано: *date 07.05.2019 06:53:00 payout 0.82*
    std::cout << "date " << xtime::get_str_date_time(timestamp) << " payout " << payout << std::endl;
	
	/* переменная err будет хранить код ошибки. 
	 * Если ошибок нет, переменная будет равна 0. 
	 * Иначе см. перечисление PayoutCancelType
	 */

    return 0;
}
```

**Проверка возможности торговать в указанное время**

```C++
/* проверим, можно ли торговать в 1300 минуту дня? (это 21:40), Ответ - нет */
if(payout_model::IntradeBar::check_minute_day(1300) == payout_model::OK)
	std::cout << "yes" << endl;
else 
	std::cout << "no" << endl;

/* проверим, можно ли торговать в 33 минуту дня? (это 00:33), Ответ - да */
if(payout_model::IntradeBar::check_minute_day(33) == payout_model::OK)
	std::cout << "yes" << endl;
else
	std::cout << "no" << endl;
	
/* проверим, можно ли торговать в 00:00 12 декабря 2019? Ответ - да */
if(payout_model::IntradeBar::check_timestamp(xtime::get_timestamp(12,12,2019,0,0,0))
	== payout_model::OK)
	std::cout << "yes" << endl;
else
	std::cout << "no" << endl;

```


### Полезные ссылки

* Статистика процентов выплат брокера *OlympTrade*: [https://github.com/NewYaroslav/olymptrade_historical_data](https://github.com/NewYaroslav/olymptrade_historical_data)
* Статистика процентов выплат брокера *Binary*: [https://github.com/NewYaroslav/binary_historical_data](https://github.com/NewYaroslav/binary_historical_data)
* Котировки брокера *Binary*: [https://github.com/NewYaroslav/binary.com_history_quotes](https://github.com/NewYaroslav/binary.com_history_quotes)
* Котировки брокера *IntradeBar*: [https://github.com/NewYaroslav/intrade-bar-historical-data](https://github.com/NewYaroslav/intrade-bar-historical-data)


