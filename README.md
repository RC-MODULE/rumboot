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

- Создать файл rumboot/src/platform/basis/targets/testname.c, за основу взять hello.c из этого же каталога
- Переконфигурировать проект и пересобрать подпроекты

```
cmake path/to/src/
make -B

```

- Соответствующий скрипт запуска будет добавлен в каталог runners/

# Ограничения текущей реализации

- Переменные для чтения/записи можно объявлять только на стеке
- Нет возможности указывать свой lds скрипт
