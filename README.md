# binary_options_payout_model
С++ header-only библиотека с моделями процентов выплат брокеров бинарных опицонов

### Описание

На данный момент библиотека содержит только модель процентов выплат брокера [intrade.bar](www.intrade.bar/67204)

### Как использовать

Пример использования:

```C++
#include <iostream>
#include "intrade_bar.hpp"

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
	 * (здесь 3 минуты - минимальная длительность у брокера [intrade.bar](www.intrade.bar/67204)
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

### Полезные ссылки

* Статистика процентов выплат брокера *OlympTrade*: [https://github.com/NewYaroslav/olymptrade_historical_data](https://github.com/NewYaroslav/olymptrade_historical_data)
