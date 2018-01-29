# Полезные трюки для отладки и профилирования rumboot

Эти средства можно использовать для упрощения отладки rumboot и/или тестов на модели. Прошу всех добавлять в этот документ полезные трюки и не стесняться ;)

## Ускорение моделирования

- Используйте вспомогательную опцию -DTURBO=y при конфигурации проекта для включения набора проекто-специфичных опций для ускорения моделирования. (Для базиса - блэкбокс sgmii phy, отключение ovl ассертов и tarmac)

Пример:
```
cmake .. -DTURBO=y
make
...
```

- Используйте две директории сборки, в одной из которых проект сконфигурирован с опцией TURBO (для быстрой проверки кода), а во второй без данной опции (для отладки возникающих проблем)

## Трассировка вызываемых функций

Для включения режима трассировки вызываемых функций, при конфигурации проекта необходимо сказать cmake, что бы тот установил переменную RUMBOOT_TRACE в Yes.

Пример:
```
    cmake .. -DRUMBOOT_TRACE=yes
    make
```

В этом режиме в стандартный вывод будут выведены имена исполняемых функций и их адреса в памяти. Отступы слева показывают вложенность вызова функций.

- Для исключения функции из трассировки необходимо использовать модификатор функции `__attribute__((no_instrument_function))`

Пример:
```
    void __attribute__((no_instrument_function)) do_not_trace_me() {
        /* code here */
    }
```

Пример вывода с включенной трассировкой:

```
61906.25 ns ARM: TRACE:   000005d8 <rumboot_platform_setup>:
63593.75 ns ARM: TRACE:    00000d40 <sp804_config>:
65281.25 ns ARM: TRACE:     00000c40 <iowrite8>:
68218.75 ns ARM: TRACE:    00000c84 <sp804_enable>:
69781.25 ns ARM: TRACE:     00000bfc <ioread8>:
72093.75 ns ARM: TRACE:     00000c40 <iowrite8>:
74843.75 ns ARM: TRACE:    00000598 <arm_vbar_set>:
77718.75 ns ARM: TRACE:   00001abc <rumboot_irq_cli>:
79343.75 ns ARM: TRACE:    00000914 <rumboot_arch_irq_disable>:
82312.50 ns ARM: TRACE:   00000870 <rumboot_platform_irq_init>:
83875.00 ns ARM: TRACE:    00000788 <gic_cpuif_write>:
85375.00 ns ARM: TRACE:     00000700 <iowrite32>:
88156.25 ns ARM: TRACE:    00000788 <gic_cpuif_write>:
89656.25 ns ARM: TRACE:     00000700 <iowrite32>:
92343.75 ns ARM: TRACE:    00000820 <gic_dist_write>:
93843.75 ns ARM: TRACE:     00000700 <iowrite32>:
98093.75 ns ARM: TRACE:   000004d8 <main>:
99718.75 ns ARM: TRACE:    000018d4 <test_suite_run>:
101468.75 ns ARM: TRACE:     000017dc <test_suite_run_single>:
102437.50 ns ARM: --- Executing test: SP804 ID Regs - SP804_0 ---
104000.00 ns ARM: TRACE:      00000534 <rumboot_platform_get_uptime>:
105656.25 ns ARM: TRACE:       00000cec <sp804_get_value>:
107187.50 ns ARM: TRACE:        00000434 <ioread32>:
111000.00 ns ARM: TRACE:      00000474 <test_dit_periph_id_regs>:
112468.75 ns ARM: TRACE:       000003b4 <ioread32>:
114906.25 ns ARM: --- PASSED ---
117437.50 ns ARM: TRACE:     000017dc <test_suite_run_single>:
118406.25 ns ARM: --- Executing test: SP804 ID Regs - SP804_1 ---
119968.75 ns ARM: TRACE:      00000534 <rumboot_platform_get_uptime>:
121593.75 ns ARM: TRACE:       00000cec <sp804_get_value>:
123156.25 ns ARM: TRACE:        00000434 <ioread32>:
126937.50 ns ARM: TRACE:      00000474 <test_dit_periph_id_regs>:
128406.25 ns ARM: TRACE:       000003b4 <ioread32>:
130843.75 ns ARM: --- PASSED ---
133375.00 ns ARM: TRACE:     000017dc <test_suite_run_single>:
134343.75 ns ARM: --- Executing test: SP804 ID Regs - SP804_2 ---
135937.50 ns ARM: TRACE:      00000534 <rumboot_platform_get_uptime>:
137562.50 ns ARM: TRACE:       00000cec <sp804_get_value>:
139125.00 ns ARM: TRACE:        00000434 <ioread32>:
142906.25 ns ARM: TRACE:      00000474 <test_dit_periph_id_regs>:
144375.00 ns ARM: TRACE:       000003b4 <ioread32>:
146812.50 ns ARM: --- PASSED ---
149343.75 ns ARM: TRACE:     000017dc <test_suite_run_single>:
150312.50 ns ARM: --- Executing test: SP804 ID Regs - SP804_3 ---
151875.00 ns ARM: TRACE:      00000534 <rumboot_platform_get_uptime>:
153500.00 ns ARM: TRACE:       00000cec <sp804_get_value>:
155062.50 ns ARM: TRACE:        00000434 <ioread32>:
158875.00 ns ARM: TRACE:      00000474 <test_dit_periph_id_regs>:
160343.75 ns ARM: TRACE:       000003b4 <ioread32>:
162781.25 ns ARM: --- PASSED ---
165281.25 ns ARM: 0 tests from suite failed
167375.00 ns ARM: TRACE:   00001c00 <exit>:
168843.75 ns ARM: TRACE:    00001bc4 <_exit>:
170500.00 ns ARM: TRACE:     000008cc <rumboot_arch_irq_disable>:
```

## Организация кода для работы с блоком

- Разделяйте код теста и библиотечные функции для работы с блоком.

- Библиотеки для работы с блоками размещайте в src/lib/drivers/

- Заголовочные файлы, которые содержат программный интерфейс для работы с блоком размещайте в include/devices/

- Заголовочные файлы, содержащие офсеты регистров блока размещайте в include/regs/regs_имя_устройства.h

- Не включайте заголовочные файлы с регистровой картой блока в тестовые сценарии. Эти данные нужны только для библиотечного кода.

## Сбор журнала чтения/записи регистров

При отладки библиотечного кода для работы с конкретным блоком бывает полезно собрать журнал чтения/записи регистров. Для этого необходимо:

- Сделать функции обертки для вызовов ioread/iowrite в файле, реализующем библиотеку для работы с данным устройством. Пример ниже:

```

//#define DEV_DEBUG 1

static inline void __attribute__((no_instrument_function)) dev_write(uint32_t reg, uint32_t value)
{
    #ifdef DEV_DEBUG
    rumboot_printf("write [0x%x]<=0x%x\n", reg, value);
    #endif
    iowrite32(value, reg);

}

static inline uint32_t __attribute__((no_instrument_function)) dev_read(uint32_t reg, uint32_t value)
{
    uint32_t ret = ioread32(reg);
    #ifdef DEV_DEBUG
    rumboot_printf("read [0x%x]==0x%x\n", reg, ret);
    #endif
}

```

- Использовать эти обертки для всех операций чтения/записи регистров

- Раскомментировать макрос DEV_DEBUG на время отладки, закомментировать когда библиотечный код отлажен, и готов к пременению.

- Обертки должны быть определены в файле, содержащим библиотечный код для работы с блоком, и не должны быть видны где-то еще (ключевое слово static)

- Для минимизации накладных расходов обертки должны быть помечены как inline

- Для исключения функции-обертки из вывода трассировки можно использовать заклинание '__attribute__((no_instrument_function))'

## Программный разбор стека вызовов при ошибках

Функция `rumboot_platform_panic(char *why, ...)` может быть использована для аварийного завершения моделирования с ошибкой и для программных ассертов. Перед завершением моделирования данная функция запускает процесс разбора содержимого стека и на экран выводится трасса вызовов функций, которые повлекли за собой данную ошибку/нежелательное поведение.

Функция принимает аргументы аналогично rumboot_printf() и может вывести пользовательское сообщение почему произошла данная ошибка перед выводом трассировки стека.

_Пример_

Вызов
```
    rumboot_platform_panic("A sample panic: %x\n", tmp);
```

Был добавлен в тест jenkins/sp804-pehiph-id.c

Результат вывода при моделировании:

```
64437.50 ns ARM: --- Executing test: SP804 ID Regs - SP804_0 ---
66906.25 ns ARM: PANIC: A sample panic: 4
94437.50 ns ARM: TRACE:  3d8: | 6c4 <rumboot_platform_panic>
94437.50 ns ARM: TRACE:  1084: | r3
94437.50 ns ARM: TRACE:  1138: | 1024 <test_suite_run_single>
94437.50 ns ARM: TRACE:  40c: | 10f8 <test_suite_run>
94437.50 ns ARM: TRACE:  12ec: | 3f8 <main>
94437.50 ns ARM: TRACE:  1c4: | 1238 <rumboot_main>
```

Данную трассу вызовов следует читать так:

в функции rumboot_main была вызвана функция main, в которой была вызвана функция
test_suite_run, в которой была вызвана функция test_suite_run_single в которой производился вызов функции по указателю, в котором была вызвана функция rumboot_platform_panic().

_N.B.:_

- В ряде случаев, при вызове функции по указателю, вместо имени функции будет имя регистра, в котором был адрес функции.

- inline функции (и функции, которые компилятор решил сделать инлайн) будут отсутствовать в данном списке.


## Ручное профилирование кода

- Для измерение времени выполнения участков кода Используйте функцию `rumboot_platform_perf(char *tag)`

- Вызов `rumboot_platform_perf("my_time_critical_code")` отмечает начальный момент времени

- Вызов `rumboot_platform_perf(NULL)` отмечает конечный момент времени

- Вызовы могут быть вложенными

- По завершении моделирования в директории с логами генерируется интерактивный html-отчет (performance_report.html), который можно просмотреть браузером

## Автоматическое профилирование кода

TODO: ...

## Сбор покрытия библиотечного кода

Для сбора покрытия библиотечного кода rumboot необходимо сконфигурировать проект СБИС с параметром -DRUMBOOT_COVERAGE=YES

Пример:
```
cmake /path/to/source -DRUMBOOT_COVERAGE=YES
```

Далее можно запустить один или несколько тестов сценариями запуска из runners

```
./runners/run-default-mytest
```

или всю регрессию используя ctest (не забываем сначала собрать весь проект командой make)
```
make
ctest -j4
```

Для генерации html отчета о покрытии кода воспользуйтесь сценарием view_rumboot_coverage, который будет сгенерирован в директории сборки.

```
./view_rumboot_coverage
```

Интерактивный HTML отчет будет сгенерирован в директории lib-coverage-html/
Для просмотра откройте index.html из этой директории любым браузером.
