![logo](doc/logo-640-160.png)
# bo-payout-model
С++ header-only библиотека с моделями процентов выплат брокеров бинарных опицонов

### Описание

На данный момент библиотека содержит следующие модели: 

* модель процентов выплат брокера [intrade.bar](https://intrade.bar)
* модель процентов выплат брокера [grandcapital](https://gcoption.com/)

**Если вы хотите поддержать развитие данной библиотеки и собираетесь торговать у брокера intrade.bar, зарегистрируйтесь, пожалуйста, [по этой партнерской ссылке](https://intrade.bar/67204). Один процент от оборота (т.е. не важно, в + или - вы торгуете) ваших средств будет идти на поддержку проекта. Спасибо!**

## Особенности торговли у брокера intrade.bar

**Устаревшая информация, оставлена для памяти**

- Торговля открывается в понедельник, начиная с *22 часа по UTC* (*1 час по МСК*). **Возможны изменения!!!**
- Начиная с *13:57:00* 3 минуты в начале часа и в течении 3 минут после часа брокер *занижает процент выплат до 60%*.
- Аналогично правилу выше начиная с *22 часа по UTC* до *5 часа по UTC* на 3 минуты в начале часа и в течении 3 минут после часа брокер занижает процент выплат до 60%.
Т.е. сделки нельзя открывать в период *XX:57:00 - XX:02:59*, где XX - час дня, начиная с 13 часов по UTC или с 22 по 5 час UTC (для 5 часа ЕСТЬ занижение процента выплат после часа, т.е. до *05:02:59*).

**Актуальная информация, начиная с 27.01.2020**

- Торговля открывается каждый день с *1 часа по UTC* (*4 час по МСК*). **Возможны изменения!!!**
- Начиная с *13:57:00* 3 минуты в начале часа и в течении 3 минут после часа брокер *занижает процент выплат до 60%*.
- Аналогично правилу выше начиная с *1 часа по UTC* до *6 часа по UTC* на 3 минуты в начале часа и в течении 3 минут после часа брокер занижает процент выплат до 60%.
Т.е. сделки нельзя открывать в период *XX:57:00 - XX:02:59*, где XX - час дня, начиная с 13 часов по UTC или с 1 по 6 час UTC (для 6 часа ЕСТЬ занижение процента выплат после часа, т.е. до *05:02:59*).

- Брокер имеет два типа опционов. Данная библиотека поддерживает только *спринт-опционы*.
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

## Особенности торговли у брокера grandcapital

* С 22 февраля 2019 торговля доступна с 22:00 до 2:00 по терминальному времени (GMT+2). Ссылка на новость:
https://grandcapital.ru/about/company_news/izmenilos-vremya-torgovli-instrumentami-gruppy-opt/

* Процент выплат фиксированны и зависит от инструмента
86% платят на следующих валютных парах: EURUSD
85% платят на следующих валютных парах: AUDCAD, AUDCHF, AUDJPY, AUDNZD, AUDUSD, CADCHF, CADJPY, EURGBP, EURJPY, GBPUSD, XAUUSD, NZDJPY, NZDUSD, USDCAD
80% платят на следующих валютных парах: CHFJPY, EURAUD, EURCAD, EURNZD, GBPAUD, GBPCAD, GBPCHF, GBPJPY, USDCHF, USDJPY
60% платят на следующих валютных парах: EURCHF, XAGUSD 

* Размер ставки: от 1 USD с шагом 0.01 или от 50 RUR

### Как использовать

К примеру, подключите файл *intrade-bar-payout-model.hpp* в свой проект, чтобы использовать модель процентов выплат брокера [intrade.bar](www.intrade.bar/67204).

**Получение процентов выплат**

```C++
#include <iostream>
#include "intrade-bar-payout-model.hpp"

using namespace std;

int main() {
	/* класс модели процентов выплат */
    payout_model::IntradeBar IntradeBar;
	
	/* можно задать валюту аккаунта
	 * payout_model::IntradeBar IntradeBar(payout_model::CURRENCY_USD);
	 */
	 
	/* проверить наличие валютной пары у брокера */
	std::cout << "EURUSD: " << payout_model::IntradeBar::check_currecy_pair_name("EURUSD") << endl;
    std::cout << "GBPCAD: " << payout_model::IntradeBar::check_currecy_pair_name("GBPCAD") << endl;

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
	
	/* размер ставки в валюте счета, в данном случае в RUB */
	double amount = 4000;
	
	/* получаем процент выплат */
    int err = IntradeBar.get_payout(payout, timestamp, duration, symbol_ind, amount);
	/* еще вариант получить процент выплат, указав имя валютной пары, а не индекс в библиотеке
	 * переменная err для обоих методов get_payout будет хранить код ошибки.
	 * Если ошибок нет, переменная будет равна 0.
	 * Иначе см. перечисление PayoutCancelType
	 */
	err = IntradeBar.get_payout(payout, "EURUSD", xtime::get_timestamp(5,3,2019,9), 180, 5000);
	
	/* на экране будет напечатано: date 07.05.2019 06:53:00 payout 0.82 */
    std::cout << "date " << xtime::get_str_date_time(timestamp) << " payout " << payout << std::endl;
	
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


