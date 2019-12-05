# Документация (jenkins)

[Документация по API, собирается автоматически](http://shadowblade.module.ru/job/Basis-B1/job/rumboot/CMAKE_BUILD_TYPE=Debug,RUMBOOT_PLATFORM=native,label=taruca/Doxygen_HTML_Docs/)


# Как собрать rumboot?

Здесь приведена последовательность сборки rumboot для компиляции программ для готовой микросхемы или запуска native тестов. 

## Требуемое ПО

- cmake
- Инструментальная цепочка для целевой платформы с newlib 
- ccache (опционально. Кеширует результаты компиляции и ускоряет повторные сборки)
- doxygen (Опционально. Необходим для сборки документации)
- python3 (Необходим для корректной работы программы подготовки образов вторичного загрузчика)

## Общая последовательность сборки

- Скачайте исходные коды rumboot из git

```
    git clone git@git.module.ru:verification-components/RumBoot.git
```

- Подготовте сборочную директорию

```
    cd rumboot
    mkdir build
    cd build
```

- Убедитесь пути к компилятору есть в $PATH
```
arm-rcm-eabihf-gcc --version
cmake --version
```

- Сконфигурируйте проект, указав желаемую платформу и тип сборки

```
    cmake .. -DRUMBOOT_PLATFORM=basis -DCMAKE_BUILD_TYPE=PostProduction 
```

- Скомпилируйте проект
```
    make -j20
```

В директории build будет созданы .bin файлы тестов, которые можно будет записать на карту памяти, SPI Flash или загрузчить используя xmodem.

## Запуск и внутрисхемная прошивка .bin файлов

### Запуск при помощи rumboot-xrun

Для запусука из накристальной памяти необходимо воспользоваться программной rumboot-xrun из комплекта rumboot-tools и подключить UART0 платы к хост-компьютеру. 

Инструкция по установки на windows, см тут: 

http://git.module.ru/verification-components/rumboot-packimage.py/wikis/HOWTO/%D0%9F%D0%BE%D0%B4%D0%B3%D0%BE%D1%82%D0%BE%D0%B2%D0%BA%D0%B0-%D0%BE%D0%BA%D1%80%D1%83%D0%B6%D0%B5%D0%BD%D0%B8%D1%8F-Windows


rumboot-xrun содержит встроенную справку доступную по ключу --help

```
0 ✓ necromant @ silverblade ~/Work/Basis/rumboot/rumboot-packimage.py $ rumboot-xrun --help
usage: rumboot-xrun [-h] -f FILE [-l LOG] [-p value] [-b value] [-r value]
                    [-S value] [-P value]

rumboot-xrun 1.4 - RumBoot X-Modem execution tool
(C) 2018 Andrew Andrianov, RC Module
https://github.com/RC-MODULE

optional arguments:
  -h, --help            show this help message and exit
  -f FILE, --file FILE  image file
  -l LOG, --log LOG     Log terminal output to file
  -p value, --port value
                        Serial port to use
  -b value, --baud value
                        Serial port to use
  -r value, --reset value
                        Reset sequence to use (none, pl2303, mt125.05)
  -S value, --ft232-serial value
                        FT232 serial number for MT125.05
  -P value, --pl2303-port value
                        PL2303 physical port (for -P of pl2303gpio)
```

Минимальный пример (linux)

```
rumboot-xrun -f myfile.bin -p /dev/ttyUSB0
```

Минимальный пример (windows)

```
rumboot-xrun -f myfile.bin -p com7:
```


По умолчанию программа пытается использовать последовательный порт /dev/ttyUSB0 (если не указана опция -p). Если не указан флаг -b, то скорость порта будет использована в зависимости от скорости по умолчанию для выбранной микросхемы (тип микросхемы определяется из заголовка в .bin файле).  

rumboot-xrun работает как программа-терминал и выводит на экран все, что было принято по последовательному порту, в т.ч. отладочный вывод запущенной программы. Журнал работы можно записать в файл используя опцию -l file.log. 

Если запущенный тест завершается возвратом в bootrom, то rumboot-xrun завершится с тем же кодом возврата (полезно для скриптов).

### Использование автоматического сброса платы

rumboot-xrun и rumboot-xflash могут использовать разные способы обеспечения автоматического сброса платы


## Специальные опции для подготовки цепочек из тестов для функционального контроля

Для тестов-цепочек, объединяющих в себе последовательное испольнение нескольких тестов, будет сгенерирован .bin файл, объединяющий в себе несколько тестов. Достаточно просто записать его на SPI флешку. 
Для подготовки такого же образа для работы с SD карточки, необходимо передать на стадии конфигурации опцию -DRUMBOOT_COMBOIMAGE_PAD=512

Например: 

```
    cmake .. -DRUMBOOT_PLATFORM=basis -DCMAKE_BUILD_TYPE=PostProduction -DRUMBOOT_COMBOIMAGE_PAD=512
```

## Опции конфигурации cmake

- _RUMBOOT_PLATFORM=native_ тип платформы
- _RUMBOOT_DISABLE_CCACHE=No_ - принудительное отключение ccache (по умолчанию включается, если в системе есть ccache)
- _RUMBOOT_TRACE=y_ - Автоматическая трассировка исполняемых функций
- _RUMBOOT_PROFILE=y_ - Автоматическое профилирование исполняемых функций
- _CMAKE_BUILD_TYPE=Production_ - Тип сборки (Debug, Production, PostProduction)

## Типы сборок

- Debug - используется для моделирования. Ввод/вывод через систему событий.
- Production - используется для bootrom загрузчика. Система событий, в виде канала для отладочного вывода используется уарт.
- PostProduction - Для запуска тестов на реальном железе. Для вывода отладки используется UART, система событий реализована поверх UART.

## Макросы препроцессора, передаваемые системой сборки

- _RUMBOOT_VERSION_ - Версия rumboot (коммит из git'а)
- _CMAKE_BUILD_TYPE_ - Тип сборки (Debug, Production)
- _CMAKE_BUILD_TYPE_DEBUG_; _CMAKE_BUILD_TYPE_PRODUCTION_ - Тип сборки в виде одного дефайна (Debug, Production) для проверки через #ifdef
- _RUMBOOT_PLATFORM_ - Выбранная платформа rumboot (basis, oi10, mpw-proto, ...)
- _RUMBOOT_ARCH_ - Целевая архитектура (arm, ppc, native)

## Сборка документации

```
cd rumboot
mkdir build-doc
cd build-doc
cmake ..
make docs
```

HTML Документация будет сгенерирована в каталоге doc/

# Добавление нового теста (при сборке вместе с проектом СБИС, напр basis)

## Структура каталогов и заголовочных файлов

Примеры ниже для платформы basis, для других платформ просто измените basis на имя целевой платформы.

- include/ - все заголовочные файлы располагаются здесь. Подключаются через #include <file.h>. Внутри заголовочные файлы необходимо структурировать согласно указаниям ниже.

- include/platform/basis/platform - заголововочные файлы специфичные для проекта basis. Подключаются через #include <platform/имя_файла.h>.

- include/arch/arm/arch - заголовочные файлы специфичные для архитектуры. Подключаются через #include <arch/имя_файла.h>

- include/platform/basis/platform/devices.h - заголовочный файл с регистровой картой basis. Там должны быть ТОЛЬКО базовые адреса блоков В ПОРЯДКЕ ВОЗРАСТАНИЯ АДРЕСА (чтобы было проще сверять с документацией). Подключается через #include <platform/devices.h>

- include/platform/basis/platform/interrupts.h - заголовочный файл с картой прерываний. Там должны быть ТОЛЬКО номера прерываний В ПОРЯДКЕ ВОЗРАСТАНИЯ (чтобы было проще сверять с документацией). Подключается через #include <platform/interrupts.h>

- include/arch/arm/ - общие заголовочные файлы для конкретной архитектуры (arm, ppc). Здесь в первую очередь макросы для атомарных блоков кода.

- include/regs/\*.h - файлы с офсетами регистров внутри блоков должны располагаться в отдельных заголовочных файлах в этом каталоге. Подключаются через #include <regs/имя_файла.h>.

_N.B.:_ Если блок специфичен для проекта (например, SCTL, BISR) то его регистры лучше размещать в include/platform/basis/platform/regs и подключать через #include <platform/regs/\*.h>

- include/devices/\*.h - файлы с прототипами библиотечных функций для работы с блоками. Подключаются через #include <devices/имя_файла.h>

_N.B.:_ Если блок специфичен для проекта (например, SCTL, BISR) то API для его работы лучше размещать в src/platform/basis/, а заголовочный файл с API регистры лучше размещать в include/platform/basis/platform/devices и подключать через #include <platform/devices/\*.h>

- include/rumboot/ - место для API rumboot. НЕ НАДО СЮДА СКЛАДЫВАТЬ библиотечный код/заголовочные файлы для блоков.
Подключается через #include <rumboot/имя_файла.h>

- src/ - все исходные коды располагаются здесь

- src/platform/basis - весь код, специфичный для проекта БАЗИС должен располагаться здесь. Иными словами - код, который НЕ МОЖЕТ быть использован при тестировании других проектов

- src/lib - библиотечный код, который может быть переиспользован в других проектах.

- src/lib/algo - библиотечный код, полезные алгоритмы

- src/lib/drivers - библиотеки с функциями для работы с устройствами

- src/lib/bootsrc - компоненты источников загрузки

## Простые тесты

Простые тесты представляют собой тесты состоящие из одного файла. Они собираются вместо BootROM и исполняются оттуда. Для их создания не требуется редактировать CMakeLists.txt Достаточно выполнить следующие шаги:

- Создать файл rumboot/src/platform/basis/targets/simple-rom/testname.c, за основу взять hello.c из этого же каталога
- Функция main() должна возвращать 0 для успешного завершения теста. Любое другое значение, возвращаемое из main() будет интерпретировано как ошибка.
- Переконфигурировать проект СБИС и пересобрать все подпроекты. т.е. запустить в командной строке в директории сборки нижеследующее:

```
cmake path/to/basis/source/
make
```
- Запустить соответствующий сценарий в директории runners/

_ОЧЕНЬ ВАЖНО:_
-  Автоматически добавляемые тесты НЕ попадают в список "короткой" регрессии, которая запускается системой непрерывной интеграции на каждый коммит. Добавление тестов в этот список производится только вручную в файле cmake/platforms/basis.cmake (см. макрофункцию RUMBOOT_SHORT_REGESSION_LIST)

- Не надо два раза добавлять тесты в полную регрессию, полная включает в себя в т.ч. и короткую

## Добавление сложных тестов

Сложные тесты состоят из нескольких файлов и/или используют пользовательский ld сценарий, произвольный слепок симуляции и т.п. Для их добавления необходимо отредактировать файл cmake/platforms/basis.cmake (см. макрофункции       RUMBOOT_FULL_REGESSION_LIST/RUMBOOT_SHORT_REGESSION_LIST)
Туда необходимо добавить вызов функции add_rumboot_target()

```
add_rumboot_target(
    CONFIGURATION name
    TESTGROUP short v0 v1
    NAME mytest
    SNAPSHOT default
    PREFIX mytestprefix
    LDS basis/rom.lds
    FILES mytest/main.c mytest/lib.c
    CFLAGS -DMACRO=value
    IRUN_FLAGS +VAR=value
    CHECKCMD shellcmd arg
    PREPCMD shecmd arg arg
    TIMEOUT 2 ms
    TIMEOUT_CTEST 3600
    DEPENDS prefix-name
    BOOTROM prefix-name
    PACKIMAGE_FLAGS flags
    OBJCOPY_FLAGS  flag1 flag2
    VARIABLE PATHVAR
    APPEND filename
    FEATURES [STUB] [LUA] [COVERAGE] [LPROBE] [PACKIMAGE] [PACKIMAGE] [ROMGEN] [NOCODE] [COVERAGE]
    LOAD
        PLUSARG TARGET
        PLUSARG SELF
        PLUSARG TARGET1,TARGET2
)
```

### Аргументы документированы ниже:

#### Обязательные:

- FILES - Один или более .c/.asm файлов с исходным кодом теста.

#### Опциональные:

- CONFIGURATION - использование типовой конфигурации для сборки теста (чтобы не указывать вручную все остальные аргументы). Если не указан - используется конфигурация по умолчанию. Для BasisB1 - ROM.

- TESTGROUP - Задает дополнительные группы тестов, в которые входит данный тест. В полную регрессию включаются по умолчанию ВСЕ тесты. Короткая регрессия (которая гоняется jenkins'ом на каждый коммит) называется short.

- PREFIX - пользовательский префикс, будет фигурировать в имени теста.

- NAME - имя теста. При отсутствии будет сгенерировано на основе имени первого файла в списке

- TIMEOUT - максимальное допустимое время моделирования для данного теста (время модели).

- TIMEOUT_CTEST - максимальное допустимое время моделирования для данного теста
(Для ctest, реальное время, в секундах)

- LDS - путь к используемому ld сценарию, относительно каталога lds/ в дереве исходников rumboot. При отсутствии используется ld скрипт по умолчанию для платформы. Для базиса - basis/rom.lds

- SNAPSHOT - имя снэпшота, используемого для запуска данного теста. При отсутствии используется значение по умолчанию для платформы. Для базиса -  default

- CFLAGS - Дополнительные флаги компилятору.

- IRUN_FLAGS - Дополнительные флаги irun'у при запуске данного теста

- PACKIMAGE_FLAGS - Опции для rumboot-packimage.py. При отсутствии просто добавляет правильную контрольную сумму

- OBJCOPY_FLAGS - Дополнительные опции для objcopy при создании .bin файла

- CHECKCMD - Выполнить эту команду в директории запуска теста для проверки результата прохождения теста. Если команда вернет 0, то тест будет считаться пройденным. Код возврата от моделирования при этом не будет учитываться.

- PREPCMD - Выполнить эту команду в директории запуска теста перед запуском моделироваия. Полезно для генерации эталонных данных

- BOOTROM - имя цели, которая генерирует BOOTROM, необходимый для запуска данного теста. Зависимость указывается в формате prefix-name.

- DEPENDS - Указывает на зависимые цели, которые надо собрать перед сборкой/запуском данной цели (С указанием префикса. Цель из директивы BOOTROM дублировать здесь не требуется, если BOOTROM требуется для данной конфигурации, то он будет собран всегда)

- VARIABLE - Устанавливает переменную, имя которой передано аргументом в абсолютный путь к компилируемому ELF файлу в директории сборке (без .bin/.dmp).

- DUMPFLAGS - Устанавливает флаги для дизассемблирования

- OPTIMIZE - Устанавливает флаги оптимизации для целей. При установке опции при объявлении конфигурации, все библиотечные функции в этой конфигурации будут собраны с данной опцией оптимизации. При указании этой опции при объявлении таргета - все файлы таргета. Если у конфигурации не указан уровень оптимизации, то будет использована оптимизация -Os
 
- APPEND - Содержимое указанного файла будет записано в конец .bin файла. (Для тестирования ini-конфигуратора и тому подобных вещей) 

- LOAD - Перечисляет загружаемые .bin файлы. Аргумент принимает пары значний PLUSARG и TARGET.
    * PLUSARG - имя PLUSARG переменной, куда будет помещен путь к BIN файлу.
    * TARGET - имя цели (с префиксом, аналонично DEPENDS). Вместо TARGET можно так же использовать просто абсолютный путь к файлу. Ключевое слово SELF указывает, что нужно загрузить текущий компилируемый файл.

    * Если окружение ожидает получить список из двух и более файлов через запятую (например обертка из двух SPI flash памятей), то можно перечислить цели через запятую. Например:
    ```
        LOAD SPI0BIN target1,target2
    ```  



Пример:
```
    add_rumboot_target(
        ...
        LOAD IM0 SELF
             IM1 spl-test
             IM2 /opt/lib/testdata.bin
        )
```

- CHECKPOINTS - Список (через запятую!) контрольных точек моделирования, которые должны быть автоматически сохранены в ходе моделирования. Контрольные точки указываются в форме имя_библиотеки.имя_снепшота.

Пример:
```
add_rumboot_target(
        CONFIGURATION IRAM
        FILES common/tools/print-heaps.c
        CHECKPOINTS testbench.simulator_state,testbench.other
)
```

- RESTORE имя контрольной точки с которой производить восстановление состояние при запуске с флагом --restore. Контрольные точки указываются в форме имя_библиотеки.имя_снепшота

add_rumboot_target(
        CONFIGURATION IRAM
        FILES common/tools/print-heaps.c
        RESTORE testbench.simulator_state
)

- FEATURES - Указывает одно или несколько дополнительных свойств данной цели. Возможнные значения приведены ниже:

    * STUB - указывает, что данная цель является "заглушкой", вспомогательным бинарным файлом (т.е. для нее не будет сгенерировано скрипта запуска, при сборке системы, она будет исключена из списка для регрессионного тестирования)

    * LUA - В данной конфигурации возможна работа LUA интерпретатора. Он будет скомпилирован и скомпанован с данной конфигурацией

    * LPROBE - Данная цель является lprobe-сценарием

    * COVERAGE - для данной цели возможна сборка покрытия. Для включения инструментирования данной цели используйте -DRUMBOOT_COVERAGE=Y при конфигурации сборки

    * PACKIMAGE - Говорит, что к бинарным файлам надо добавить контрольную сумму при помощи rumboot-packimage.py. Дополнительные опции для rumboot-packimage.py можно передавать через директиву PACKIMAGE_FLAGS.

    * NOCODE - Эта цель не требует ничего компилировать, а только распихать по памятям другие цели и создать скрипт запуска (Для тестов загрузчика) или сделать комбинированный образ из нескольких таргетов

    * BANNER - Автоматически выводить полное имя теста перед запуском пользовательского main()

## Общие рекомендации к написанию тестов

- Используйте библиотеку testsuite
- Код теста IP блоков лучше выносить в отдельную библиотеку, размещая ее в lib/. Аргументом функция, выполняющая тестирование должна получать базовый адрес IP блока
- Документируйте библиотечные функции используя систему документирования исходного кода doxygen
- Старайтесь, чтобы тест шел не более 5-10 минут реального времени, для тестов короткой регрессии и не более часа - для тестов полной регрессии


## Список тестов (библиотека testsuite)
Для экономии времени и простоты можно комбинировать простые однотипные тесты периферии в наборы.

Пример использования библиотеки testsuite - см. rumboot/src/platform/basis/targets/simple-rom/testname.c


- Соответствующий скрипт запуска будет добавлен в каталог runners/

## Многопоточный запуск тестов

```
    cmake path/to/basis/source
    make
    ctest -j4
```

Будет запускать до четырех процессов ncsim. Логи сохраняются в директории Testing. Перед установкой большого числа потоков убедитесь в наличии большого количества оперативной памяти на выбранном сервере

## Как обеспечить безболезненную переносимость теста на PCIe-хост режим.

- ВСЕГДА используйте базовые адреса и номера прерываний из interrupts.h и devices.h. Не надо хардкодить адреса, это очень вредно для переносимости.

- Не надо выделять память для DMA операций на стеке или в виде статически заданного массива. Используйте вызовы rumboot_malloc_from_heap()
