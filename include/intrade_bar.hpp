#include <vector>
#include <string>
#include <map>
//------------------------------------------------------------------------------------------------------
namespace payout_model
{
	///Список типов причин отсутствия выплат
	enum PayoutCancelType{
			OK=0,							///< Оплата произведена
			DAYOFF=1,						///< Выходной день или праздник
			NIGHTHOURS=2, 					///< Ночное время, когда брокер не принмает ставки
			BEGINEVENINGHOUR=3, 			///< Первые минуты в начале часа вечером (либо последняя минута в конце часа)
			TOOLITTLETIME=4, 				///< Слишком короткое время экспирации
			TOOMUCHTIME=5, 					///< Слишком длинное время экспирации
			CURRENCYPAIRISMISSING=6, 		///< Отсутствует валютная пара с указанным индексом
			TOOLITTLEMONEY=7				///< Слишком низкая ставка
	};
//------------------------------------------------------------------------------------------------------
	class IntradeBar
	{
		private:
			std::string currency_name;								///< Наименование валюты счета. Как правило, USD или RUB
			static std::map<int, std::string> currency_pairs;       ///< Список доступных валютных пар брокера IntradeBar в формате <ключ, название>
		
		public:
		
	
			/** \brief Получить процент выплат

			* Проценты выплат варьируются обычно от 0 до 1.0, где 1.0 соответствует 100% выплате брокера
			* \param[out] payout процент выплат
			* \param[in] timestamp временную метку unix времени (GMT)
			* \param[in] duration длительность опциона в секундах
			* \param[in] currency_pair_indx  номер валютной пары из списка валютных пар брокера
			* \param[in] amount размер ставки бинарного опциона
			* \return состояние выплаты (0 в случае успеха, иначе см. PayoutCancelType)
			*/
			int get_payout(double &payout, 
							const unsigned long long timestamp, 
							const int duration, 
							const int currency_pair_indx, 
							const double amount);
			
			
			/** \brief Получить имя валютной пары по ее номеру

			* \param[in] currency_pair_indx  номер валютной пары из списка валютных пар брокера
			* \return имя валютной пары либо пустую строку, если указанный индекс отсутствует в списке валютных пар
			*/
			std::string get_currecy_pair_name(const int currency_pair_indx);
			
			
			///Получить список всех валютных пар брокера
			std::vector<std::string> get_currecy_pair_list();
			
			/** 
			  * В конструкторе класса IntradeBar определяется валюта счета.
			  * В качестве валюты счета по умолчанию используется RUB
			*/
			IntradeBar()
			{
				currency_name="RUB";
			}
		
			IntradeBar(std::string _currency_name)
			{
				currency_name=_currency_name;
			}
				
			~IntradeBar()
			{
			
			}
		
	};
	
	std::map<int, std::string> IntradeBar::currency_pairs={{0,"EURUSD"},{1,"USDJPY"},{2,"GBPUSD"},{3,"USDCHF"},
																		{4,"USDCAD"},{5,"EURJPY"},{6,"AUDUSD"},{7,"NZDUSD"},
																		{8,"EURGBP"},{9,"EURCHF"},{10,"AUDJPY"},{11,"GBPJPY"},
																		{12,"CHFJPY"},{13,"EURCAD"},{14,"AUDCAD"},{15,"CADJPY"},
																		{16,"NZDJPY"},{17,"AUDNZD"},{18,"GBPAUD"},{19,"EURAUD"},
																		{20,"GBPCHF"},{21,"EURNZD"},{22,"AUDCHF"},{23,"GBPNZD"},
																		{24,"USDRUB"},{25,"GBPCAD"},{26,"GC"}};
	
	/** \brief Получить процент выплат

			* Проценты выплат варьируются обычно от 0 до 1.0, где 1.0 соответствует 100% выплате брокера
			* \param[out] payout процент выплат
			* \param[in] timestamp временную метку unix времени (GMT)
			* \param[in] duration длительность опциона в секундах
			* \param[in] currency_pair_indx  номер валютной пары из списка валютных пар брокера
			* \param[in] amount размер ставки бинарного опциона
			* \return состояние выплаты (0 в случае успеха, иначе см. PayoutCancelType)
	*/
	inline int IntradeBar::get_payout(double &payout, 
										const unsigned long long timestamp, 
										const int duration, 
										const int currency_pair_indx, 
										const double amount)
	{
		struct tm *operation_time_in_tm;						///< Время начала операции по Гринвичу, представленное в формате временной структуры tm

		time_t operation_time_in_time_t=timestamp;				///< Время начала операции в формате time_t, представляющщее собой временную метку unix времени (GMT)
		
		/*
		 *Функция gmtime позволяет получить указатель на структуру tm, содержащую календарное время в разделенной на компоненты форме. 
		 * В качестве параметра в функцию передается указатель на временную метку времени GMT, для которой необходимо осуществить преобразование
		 */
		operation_time_in_tm=gmtime(&operation_time_in_time_t);
		
		/*
		 *year - год проведения операции
		 *tm_year - соедержит количество лет от 1900 года до даты, хранящейся в струкуре
		 */
		int year=operation_time_in_tm->tm_year+1900;	
		
		/*
		 * month - месяц проведения операции
		 * tm_month - соедержит количество месяцев с 1 января года tm_year до даты, хранящейся в струкуре
		 */
		int month=operation_time_in_tm->tm_mon+1;
		
		// day - число месяца
		int day=operation_time_in_tm->tm_mday;
		
		// hour - количество часов от 0 до 23
		int hour=operation_time_in_tm->tm_hour;
		int minute=operation_time_in_tm->tm_min;
		int second=operation_time_in_tm->tm_sec;
		
		// weekday - день недели от 0 до 6, начиная с воскресенья 
		int weekday=operation_time_in_tm->tm_wday;
		
		std::string currency_pair_name; //Наименование валютной парі
				
		// Если операция выполнена в субботу, воскресенье, 1 января или 25 августа
		// Выплата отсутствует payout=0
		// Возврат кода причины отсутствия выплаты PayoutCancelType::DAYOFF 
		if(weekday==6||weekday==0||(day==1&&month==1)||(day==25&&month==12))
		{
			payout=0;
			return PayoutCancelType::DAYOFF;
		}//if
		
		// Если операция выполнена после 21:00 по Гринвичу
		// Выплата отсутствует payout=0
		// Возврат кода причины отсутствия выплаты PayoutCancelType::NIGHTHOURS 
		if(hour>=21)
		{
			payout=0;
			return PayoutCancelType::NIGHTHOURS;
		}//if
		
		// Если операция выполнена после с 14:00 до 20:00 по Гринвичу в две первые или последнюю минуту часа
		// Выплата отсутствует payout=0
		// Возврат кода причины отсутствия выплаты PayoutCancelType::BEGINEVENINGHOUR 
		if(((hour>=14&&hour<=19)&&(minute==59||minute==0||minute==1))||
			(hour==13&&minute==59)||(hour==21&&(minute==0||minute==1)))
		{
			payout=0;
			return PayoutCancelType::BEGINEVENINGHOUR;
		}//if
		
		// Если продолжительность экспирации меньше 3 минут (180 секунд)
		// Выплата отсутствует payout=0
		// Возврат кода причины отсутствия выплаты PayoutCancelType::BEGINEVENINGHOUR 
		if(duration<180)
		{
			payout=0;
			return PayoutCancelType::TOOLITTLETIME;
		}//if
		
		// Если продолжительность экспирации больше 500 минут (30000 секунд)
		// Выплата отсутствует payout=0
		// Возврат кода причины отсутствия выплаты PayoutCancelType::TOOMUCHTIME 
		if(duration>30000)
		{
			payout=0;
			return PayoutCancelType::TOOMUCHTIME;
		}//if
		
		// Получение имени валютной пары, участвующей в операции по ее индексу
		// Соотвествие индексов валютных пар и их имен определено множеством currency_pairs
		
		currency_pair_name=get_currecy_pair_name(currency_pair_indx);
		
		// Если метод get_currecy_pair_name вернул пустую строку, т.е. в списке отсутствует валютная пара с индексом currency_pair_indx
		// Выплата отсутствует payout=0
		// Возврат кода причины отсутствия выплаты PayoutCancelType::CURRENCYPAIRISMISSING 
		if(currency_pair_name.empty())
		{
			payout=0;
			return PayoutCancelType::CURRENCYPAIRISMISSING;
		}//if

		// Если указана сумма меньшая либо равная 0
		// Выплата отсутствует payout=0
		// Возврат кода причины отсутствия выплаты PayoutCancelType::TOOLITTLEMONEY 
		if(amount<=0)
		{
			payout=0;
			return PayoutCancelType::TOOLITTLEMONEY;
		}
		
		// Если операция выполнена после 9 января 2019 года
		if(year>=2019&&month>=1&&day>=9)
		{
			// Если счет в долларах и ставка больше 80 долларов или счет в рублях и ставка больше 5000 рублей
			if((currency_name=="USD"&&amount>=80)||(currency_name=="RUB"&&amount>=5000))
			{
				payout=0.85;// Процент выплат составит 85 (0,85)
			}
			// Если счет в долларах и ставка меньше 80 долларов или счет в рублях и ставка меньше 5000 рублей
			else
			{
				// Если продолжительность экспирации от 3 до 4 минут
				if(duration>=180&&duration<240)
				{
					payout=0.82; // Процент выплат составит 82 (0,82)
				}
				else
				{
					// Если продолжительность экспирации от 4 до 500 минут
					if(duration>=240&&duration<=30000)
					{
						payout=0.79; // Процент выплат составит 79 (0,79)
					}
				}
			}
		}// if
		// Если операция выполнена после 9 января 2019 года
		else
		{
			// Если счет в долларах и ставка больше 80 долларов или счет в рублях и ставка больше 5000 рублей
			if((currency_name=="USD"&&amount>=80)||(currency_name=="RUB"&&amount>=5000))
			{
				payout=0.84; // Процент выплат составит 84 (0,84)
			}
			// Если счет в долларах и ставка меньше 80 долларов или счет в рублях и ставка меньше 5000 рублей
			else
			{
				// Если продолжительность экспирации от 3 до 4 минут
				if(duration>=180&&duration<240)
				{
					payout=0.8; // Процент выплат составит 80 (0,8)
				}
				else
				{
				    // Если продолжительность экспирации от 4 до 500 минут
					if(duration>=240&&duration<=30000)
					{
						payout=0.77; // Процент выплат составит 77 (0,77)
					}
				}
			}
		}
		
		return 0;
	};
	
	
	/** \brief Получить имя валютной пары по ее номеру

	 * \param[in] currency_pair_indx  номер валютной пары из списка валютных пар брокера
	 * \return имя валютной пары либо пустую строку, если указанный индекс отсутствует в списке валютных пар
	*/
	inline std::string IntradeBar::get_currecy_pair_name(const int currency_pair_indx)
	{
		// Перебор в цикле всех элементов множества currency_pairs при помощи итератора
		// currency_pairs.begin() - установка итератора на начало множества
		// currency_pair_index!=currency_pairs.end() - проверка нахолится ли итератор на последнем элементе множества
		// ++currency_pair_index - перевод итератора на следующий элемент множества
		for(auto currency_pair_index=currency_pairs.begin(); currency_pair_index!=currency_pairs.end(); ++currency_pair_index)
		{
			if((*currency_pair_index).first==currency_pair_indx) //Если индекс элемента, на котором находится итератор равен искомому
				// Возврат имени валютной пары, соответствующей этому индексу
				return (*currency_pair_index).second;
		}
		// В это место кода попадаем, если в множестве currency_pairs отсутствует валютная пара с индексом currency_pair_index
		return std::string();//Возврат пустой строки
	};
	
	///Получить список всех валютных пар брокера
	inline std::vector<std::string> IntradeBar::get_currecy_pair_list()
	{
		std::vector<std::string> currency_pair_list; // Вектор, в котором содержатся имена всех валютных пар
		currency_pair_list.clear(); //Очистка вектора от мусора
		
		// Перебор в цикле всех элементов множества currency_pairs при помощи итератора
		// currency_pairs.begin() - установка итератора на начало множества
		// currency_pair_index!=currency_pairs.end() - проверка нахолится ли итератор на последнем элементе множества
		// ++currency_pair_index - перевод итератора на следующий элемент множества
		for(auto currency_pair_index=currency_pairs.begin(); currency_pair_index!=currency_pairs.end(); ++currency_pair_index)
		{
			// В конец вектора currency_pair_list добавляется имя валютной пары, на которой находится итератор
			currency_pair_list.push_back((*currency_pair_index).second);
		}
		return currency_pair_list; //Возврат вектора, содержащего имена всех валютных пар
	};
	
	
	
}