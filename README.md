# Документация (jenkins)

[Документация по API, собирается автоматически](http://shadowblade.module.ru/job/Basis-B1/job/rumboot/CMAKE_BUILD_TYPE=Debug,RUMBOOT_PLATFORM=native,label=taruca/Doxygen_HTML_Docs/)

# Сборка документации

```
cd rumboot
mkdir build-doc
cd build-doc
cmake ..
make docs
```

HTML Документация будет сгенерирована в каталоге doc/

# Добавление нового теста (при сборке вместе с проектом СБИС basis)

## Структура каталогов и заголовочных файлов

- include/ - все заголовочные файлы располагаются здесь. Подключаются через #include <file.h>

- include/platform/basis/platform - заголововочные файлы специфичные для проекта basis. Подключаются через #include <platform/имя_файла.h>

- include/platform/basis/platform/devices.h - заголовочный файл с регистровой картой basis. Там должны быть ТОЛЬКО базовые адреса блоков В ПОРЯДКЕ ВОЗРАСТАНИЯ АДРЕСА (чтобы было проще сверять с документацией). Подключается через #include <platform/devices.h>

- include/devices/\*.h - файлы с офсетами регистров внутри блоков должны располагаться в отдельных заголовочных файлах в этом каталоге. Подключаются через #include <devices/имя_файла.h>

- src/ - все исходные коды располагаются здесь

- src/platforms/basis - весь код, специфичный для проекта БАЗИС должен располагаться здесь. Иными словами - код, который НЕ МОЖЕТ быть использован при тестировании других проектов

- src/lib - библиотечный код, который может быть использован в других проектах.

## Простые тесты

Простые тесты представляю собой тесты состоящие из одного файла. Они собираются вместо BootROM и исполняются оттуда. Для их создания не требуется редактировать CMakeLists.txt Достаточно выполнить следующие шаги:

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
    TIMEOUT 2 ms
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

- LDS - путь к используемому ld сценарию, относительно каталога lds/ в дереве исходников rumboot. При отсутствии используется ld скрипт по умолчанию для платформы. Для базиса - basis/rom.lds

- SNAPSHOT - имя снэпшота, используемого для запуска данного теста. При отсутствии используется значение по умолчанию для платформы. Для базиса -  default

- CFLAGS - Дополнительные флаги компилятору.

- IRUN_FLAGS - Дополнительные флаги irun'у при запуске данного теста

- CHECKCMD - Выполнить эту команду в директории запуска теста для проверки результата прохождения теста. Если команда вернет 0, то тест будет считаться пройденным. Код возврата от моделирования при этом не будет учитываться.

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
