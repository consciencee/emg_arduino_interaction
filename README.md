# Примеры считывания данных ЭМГ-датчиков

### На стороне Arduino

- [grove/grove.ino](grove/grove.ino) - содержит адаптированный [код, предоставленный seeed в качестве образца](https://github.com/Seeed-Studio/Grove_EMG_detector_demo_code/). Содержит определенного рода начальную калибровку данных и сглаживание сигнала в процессе его чтения.

- [minimal/minimal.ino](minimal/minimal.ino) - содержит минимальную версию кода, необходимого для считывания/передачи ЭМГ-данных. С данным файлом необходимо ознакомиться чтобы понять базовую структуру кода и понять, от чего отталкиваться.

### На стороне Python

(зависимости для данной части находятся в файле [requirements.txt](requirements.txt), установка: `pip install -r requirements.txt`)

- [main.py](main.py) - подключается к последовательному порту, читает оттуда данные и выводит их на график.

Подразумевается, что python-часть должна запускаться уже после того, как будет включен и подсоединен к последовательному порту Arduino с залитым на него кодом.
