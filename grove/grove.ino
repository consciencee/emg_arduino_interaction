// Grove - EMG Sensor demo code
// This demo will need a Grove - Led Bar to show the motion 
// Grove - EMG Sensor connect to A0
// Grove - LED Bar connect to D8, D9
// note: it'll take about serval seconds to detect static analog value
// when you should hold your muscle static. You will see led bar from level 10 turn to 
// level 0, it means static analog value get ok

// Я внесла минимальные изменения в данный код, чтобы он мог запускаться у нас.
// Во-первых, у нас нет светодиодов - так что я выпилила все обращения к этому блоку 
// (оригинальный скрипт подразумевал изменение яркости светодиодов в зависимости от уровня сигнала).
// Во-вторых, убрала лишние Serial.print выражения, т.к. Serial у нас служит каналом связи 
// с пока что примитивным пайтоновским слушателем данных. А эта прога ничего кроме значений сигнала
// на вход не ждет :) Значит не нужно нам пока писать лишнюю информацию в Serial, даже в целях дебага


int max_analog_dta      = 300;              // max analog data
int min_analog_dta      = 100;              // min analog data
int static_analog_dta   = 0;                // static analog data

// У них какая-то усложненная процедура считывания сигнала с датчика. 
// Возможно, таким образом они реализуют некое сглаживание сигнала
// get analog value
int getAnalog(int pin) // это не системная функция. Ее писали разработчики данного кода
{
    long sum = 0;
    
    for(int i=0; i<32; i++)
    {
        // А вот это - analogRead - базовая функция считывания сигнала. 
        // Она просто берет данные с датчика без какой-либо обработки
        sum += analogRead(pin);
    }
    
    int dta = sum>>5;
    
    max_analog_dta = dta>max_analog_dta ? dta : max_analog_dta;         // if max data
    min_analog_dta = min_analog_dta>dta ? dta : min_analog_dta;         // if min data
    
    return sum>>5;
}

// это стандартная ардуиновская функция. Отвечает за все действия, которые должны быть 
// сделаны до запуска основной части программы. Т.е. содержит всякие настройки, калибровки и т.д.
// Обязательно должна присутствовать в коде. Но при этом ее содержимое составляется разработчиком.
void setup() 
{
    // Бодрейт 9600 - используем строго такое значение. 
    // Ограничения вызваны режимом работы bluetooth-модуля
    Serial.begin(9600);
    
    long sum = 0;

    for(int i=0; i<=10; i++)
    {
        for(int j=0; j<100; j++)
        {
            sum += getAnalog(A0);
            delay(1); // встроенная функция
        }
        
    }
    
    sum /= 1100;
    
    static_analog_dta = sum;
}

int level       = 5;
int level_buf   = 5;

// это стандартная ардуиновская функция. Отвечает за все действия, которые должны итеративно выполняться 
// в процессе функционирования программы. Т.е. - основная логика. В нашем случае на каждой итерации производится 
// сбор данных с ЭМГ-датчика и отправка этих данных в последовательный порт.
// Обязательно должна присутствовать в коде. Но при этом ее содержимое составляется разработчиком.
void loop()
{

    int val = getAnalog(A0);                    // get Analog value
    
    // В оригинальном коде level нужен был для того, чтобы менять яркость светодиодов.
    // Возможно, нам это значение тоже может пригодиться - типа как "уровень сигнала", 
    // который мы можем использовать вместо сырых данных. Тут нужно рассмотреть разные 
    // варианты и решить, что лучше подойдет, и что будет удобнее обрабатывать
    int level2;
    
    if(val>static_analog_dta)                   // larger than static_analog_dta
    {
        level2 = 5 + map(val, static_analog_dta, max_analog_dta, 0, 5); // встроенная функция
    }
    else 
    {
        level2 = 5 - map(val, min_analog_dta, static_analog_dta, 0, 5);
    }
    
    // to smooth the change of led bar
    if(level2 > level)
    {
        level++;
    }
    else if(level2 < level)
    {
        level--;
    }

    if(level != level_buf)
    {
        level_buf = level;
    }
    
    // send value to serial
    Serial.println(val);
    // or you can send level:
    // Serial.println(level);
    
    delay(10);
}
