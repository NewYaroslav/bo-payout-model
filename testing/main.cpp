#include <iostream>
#include <sstream>
#include <iomanip>
#include <locale>

#include "IntradeBar.cpp"

using namespace payout_model;

int main(int argc, char const *argv[])
{
	int currency_code; 								//Код валюты счета: 1 - доллар, 2 - рубльstruct 
	tm test_tm={0};									//Струтура, компоненты которой содержат дату и время операции
	
	char test_time_string[20]; 						//Строка, в которую считывается дата и время операции
	int year, month, day, hour,minute,second; 		//Значения компонент даты и времени операции
	
	int durationtime; 								//Продолжительность операции        
	int currency_pair_index;						//Индекс валютной пары
	double amount;									//Ставка в выбранной валюте
	
	IntradeBar *intradeBar;
	
	// Установка русской локали для корректного отображения кириллицы
	setlocale(LC_ALL, "rus");
	
	std::cout<<"Введите дату и время операции в формате dd.mm.yyyy hh:MM:ss..."; 
	
	//Введенные дату и время операции считываем в строку вместе с пробелами 
	std::cin.getline(test_time_string,sizeof(test_time_string));
	//На основе полученной строки создаем поток
	std::istringstream test_string_stream(test_time_string);    
	
	//Заполняем компоненты структуры test_tm значениями компонент даты(число, месяц, год) и времени (час, минута, секунда) операции
	//Предполагается, что дата записана в формате
	test_string_stream>>std::get_time(&test_tm,"%d.%m.%Y %T");  
	//Установка флага перехода на летнее время
	test_tm.tm_isdst=1; 
	//Если введенная строка не может быть преобразована  в структуру tm, как дата в указанном формате
	if(test_string_stream.fail()) 
	{
		std::cout<<"Дата указана некорректно"<<std::endl;
		return 0;
	}

	std::cout<<"Укажите, в какой валюте счет: 1 - доллар, 2 - рубль"<<std::endl;
	std::cin>>currency_code;
	
	//Если произошла ошибка в потоке ввода, например, вместо целого числа ввели символ или действительное число
	if(!std::cin.good())
	{
		std::cout<<"Код валюты указан некорректно. Допустимо только 1 или 2";
		return 0;
	}
	if(!((currency_code==1)||(currency_code==2)))
	{
		std::cout<<"Код валюты указан некорректно. Допустимо только 1 или 2";
		return 0;
	}
	
	
	std::cout<<std::endl<<"Введите продолжительность экспирации в секундах (от 180 до 30000)...";
	std::cin>>durationtime;
	if(!std::cin.good())
	{
		std::cout<<"Продолжительность экспирации задана некорректно";
		return 0;
	}
	
	std::cout<<std::endl<<"Введите индекс валютной пары (от 0 до 26)...";
	std::cin>>currency_pair_index;
	if(!std::cin.good())
	{
		std::cout<<"Индекс валютной пары задан некорректно";
		return 0;
	}
	
	std::cout<<std::endl<<"Укажите сумму операции...";
	std::cin>>amount;
	if(!std::cin.good())
	{
		std::cout<<"Сумма операции указана неверно...";
		return 0;
	}
	std::cout<<std::endl<<std::endl;
	//После проверок, сделаных выше currency_code=1 или currency_code=2
	if(currency_code==1)
	{
		intradeBar=new IntradeBar("USD");
	}
	else
	{
		intradeBar=new IntradeBar("RUB");
	}
	//IntradeBar intradeBar("RUB");
	double payout=0;
	time_t test_time_t;
	//Преобразование времени, записанного в структуру test_tm в число типа time_t с учетом часового пояса
	test_time_t=mktime(&test_tm);
	unsigned long long timestamp=test_time_t;
	//Код ошибки, возвращенный методом get_payout
	int return_code;
	return_code=intradeBar->get_payout(payout,timestamp,durationtime,currency_pair_index,amount);
	
	//Анализ кода ошибки и информирование пользователя об ошибке
	switch(return_code)
	{
		case PayoutCancelType::DAYOFF:
			std::cout<<"Выходной. Процент выплат: 0"<<std::endl;
			break;
		case PayoutCancelType::NIGHTHOURS:
			std::cout<<"Ночь"<<std::endl;
			break;
		case PayoutCancelType::BEGINEVENINGHOUR:
			std::cout<<"Начало вечернего часа"<<std::endl;
			break;
		case PayoutCancelType::TOOLITTLETIME:
			std::cout<<"Слишком короткая экспирация"<<std::endl;
			break;
		case PayoutCancelType::TOOMUCHTIME:
			std::cout<<"Слишком длинная экспирация"<<std::endl;
			break;
		case PayoutCancelType::CURRENCYPAIRISMISSING:
			std::cout<<"Отсутствует указанная валютная пара"<<std::endl;
			break;
		case PayoutCancelType::TOOLITTLEMONEY:
			std::cout<<"Некорректно указана сумма денег"<<std::endl;
			break;
		default:std::cout<<"Процент выплат: "<<payout<<std::endl;
	}
	std::string test_string; //Строка, содержащая имя валютной пары.
	test_string=intradeBar->get_currecy_pair_name(currency_pair_index);
	if(test_string.empty())//Если строка с именем валютной пары пустая
	{
		std::cout<<"Индекс валютной пары указан не корректно"<<std::endl;
	}
	else
	{
		std::cout<<"Валютная пара: "<<test_string<<std::endl;
	}
	std::cout<<"Список валютных пар:"<<std::endl;
	//Создание вектора, в котором будут храниться имена всех валютных пар
	std::vector<std::string> test_vector=intradeBar->get_currecy_pair_list();
	//Перебор все элементов
	for(auto it=test_vector.begin(); it!=test_vector.end();++it)
	{
		std::cout<<*it<<std::endl;
	}
	return 0;
}