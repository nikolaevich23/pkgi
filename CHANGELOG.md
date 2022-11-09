# Changelog

All notable changes to the `pkgi-ps3` project will be documented in this file. This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [v1.2.4.221109](https://github.com/ErikPshat/pkgi/releases/tag/v1.2.4.221109) - 2022-11-09
* Добавлен 21 чит
* Добавлены волны и изменения XMB (замена иконок HEN)

## [v1.2.4.221101](https://github.com/ErikPshat/pkgi/releases/tag/v1.2.4.221028) - 2022-11-01
* Добавлена игра на русском Tom Clancy’s Splinter Cell Blacklist [NPEB01379][FullRus]

## [v1.2.4.221028](https://github.com/ErikPshat/pkgi/releases/tag/v1.2.4.221028) - 2022-10-28
* Небольшие правки в коде и в описании тюнинга
* Перевод на другие языки поправлен

## [v1.2.4.221027](https://github.com/ErikPshat/pkgi/releases/tag/v1.2.4.221027) - 2022-10-27
* Во вкладке Приложения теперь приложения и менеджеры
* Во вкладке Тюнинг лежат волны Wave, колдбуты Coldboot и дополнение в меню XMB в раздел пользователя (функции перезагрузки). Для установки следует Включить запись в /dev_blind
* Добавил описание для кнопок L1 R1 L2 R2

:exclamation: **Примечание: функции перегрузки работают только когда HEN активен, при выключенном хен система выдаст ошибку о неправильно запущенном системном обеспечении (нужно будет подключить ds3 и нажать  и потом пропустить проверку )**

## [v1.2.3.220826](https://github.com/ErikPshat/pkgi/releases/tag/v1.2.3.220826) - 2022-08-26
* Изменен QR-код в сообщение о необходимости активации для быстрого перехода в тему
* Музыка изменена.
* Небольшое изменение в коде.

## [v1.2.3.220802](https://github.com/ErikPshat/pkgi/releases/tag/v1.2.3.220802) - 2022-08-02
* Добавлен QR-код в сообщение о необходимости активации для быстрого перехода в тему
* Музыка по умолчанию выключена.

## [v1.2.3.220726](https://github.com/ErikPshat/pkgi/releases/tag/v1.2.3.220726) - 2022-07-26

* Добавлена проверка на активацию, если система не активирована выдаст предупреждение и внизу будет писать, что система не активирована. Если активирована - внизу будет информация  на каком пользователе активация.

## [v1.2.2.220701](https://github.com/ErikPshat/pkgi/releases/tag/v1.2.2.220701) - 2022-07-01

*  Убрана проверка на установленную игру по title id (ибо при установке показывало, что установлены и другие компоненты, хотя вы их не качали)

## [v1.2.2.1](https://github.com/ErikPshat/pkgi/releases/tag/v1.2.2.1) - 2021-11-30

### Added

* Русский перевод

## [v1.2.2](https://github.com/bucanero/pkgi-ps3/releases/tag/v1.2.2) - 2021-10-22

### Added

* Перенос всего сетевого кода в libcurl
* Поддержка ссылок HTTP, HTTPS, FTP, FTPS с TLS v1.2
* Финский перевод
* Французский перевод
* Немецкий перевод
* Итальянский перевод
* Польский перевод
* Португальский перевод
* Турецкий перевод

### Fixed

* Crash when item list is empty

## [v1.2.0](https://github.com/bucanero/pkgi-ps3/releases/tag/v1.2.0) - 2021-10-09

### Added

* Мультиязыковая поддержка
* Определение языка на основе настроек PS3
* Испанский перевод
* Онлайн-сканирование доступных обновлений контента
* Поддержка TLS v1.2 с использованием более новой версии libcurl + polarSSL
* UI: использование настраиваемых диалогов

## [v1.1.9.0](https://github.com/nikolaevich23/pkgi/releases/tag/v1.1.9.0) - 2021-11-17

### Changed

* Добавлена иконка бэкграунда PIC1.PNG.
* Пополнена и поправлена база игр.

* Особая благодарность 'PluSan' за облачное хранилище и списков для Themes и miniS.
* Особая благодарность 'Serp87' за поддержку и пополнение базы читов.
* Особая благодарность 'Z0rdan' за старт составления баз PS1 и PS2 игр.
* Особая благодарность 'PluSan' и 'max_maysky' за составление базы русских игр.

## [v1.1.8.713](https://github.com/nikolaevich23/pkgi/releases/tag/v1.1.8.713) - 2021-10-05

### Changed

* Составлен список Themes, работающих даже на OFW при включении консоли.

* Особая благодарность 'PluSan' за облачное хранилище и списков для Themes и miniS.
* Особая благодарность 'Serp87' за поддержку и пополнение базы читов.
* Особая благодарность 'Z0rdan' за старт составления баз PS1 и PS2 игр.
* Особая благодарность 'PluSan' и 'max_maysky' за составление базы русских игр.

## [v1.1.8.712](https://github.com/nikolaevich23/pkgi/releases/tag/v1.1.8.712) - 2021-10-04

### Changed

* Составлен список miniS и добавлена категория "miniS для PS3".
* В главном меню столбик регионов заменён на тип контента.
* В окне "Детали", по кнопке [], добавлена информация о регионе и типе контента.
* Прочие мелкие правки в коде и пополнение баз игр.

* Особая благодарность 'Serp87' за поддержку и пополнение базы читов.
* Особая благодарность 'Z0rdan' за старт составления баз PS1 и PS2 игр.
* Особая благодарность 'PluSan' и 'max_maysky' за составление базы русских игр.

## [v1.1.8.6](https://github.com/nikolaevich23/pkgi/releases/tag/v1.1.8.6) - 2021-08-21

### Changed

* Добавлена новая категория "Игры PS2 для PS3".
* Добавлена новая категория "Игры PS1 для PS3".
* Пополнена база читов до 500 шт.
* Прочие мелкие правки в коде и пополнения баз игр.

* Особая благодарность 'Serp87' за поддержку и пополнение базы читов.
* Особая благодарность 'Z0rdan' за старт составления баз PS1 и PS2 игр.
* Особая благодарность 'PluSan' и 'max_maysky' за составление базы русских игр.

## [v1.1.8.5](https://github.com/nikolaevich23/pkgi/releases/tag/v1.1.8.5) - 2021-03-05

### Changed

* Добавлена мелодия SND0.AT3 чтобы не было скучно.
* Пополнена база читов до 476 штук.
* Пополнена база "Игры на русском" полностью, практически на 99,9% из имеющихся в PSN.

* Особая благодарность 'Serp87' за поддержку и наполнение базы читов.
* Особая благодарность 'PluSan' и 'max_maysky' за составление базы русских игр!

## [v1.1.8.4](https://github.com/nikolaevich23/pkgi/releases/tag/v1.1.8.4) - 2021-01-29

### Changed

* Изменена иконка программы ICON0.PNG.
* Изменён бэкграунд в программе jpg на background.png.
* Пополнена база читов до 460 штук.

## [v1.1.8.3](https://github.com/nikolaevich23/pkgi/releases/tag/v1.1.8.3) - 2021-01-11

### Changed

* Программа перемещена в меню "Network".
* Добавлена база читов.

## [v1.1.8.2](https://github.com/nikolaevich23/pkgi/releases/tag/v1.1.8.2) - 2021-01-07

### Changed

### Fixed

* Исправлено отображение регионов вместо знаков "???".

## [v1.1.8.1](https://github.com/nikolaevich23/pkgi/releases/tag/v1.1.8.1) - 2020-12-26

### Changed

* Стартовая русская версия

## [v1.1.8](https://github.com/bucanero/pkgi-ps3/releases/tag/v1.1.8) - 2020-12-08

### Added

* Content icon download from the TMDB (based on Title ID)
* Use content icons for package bubbles (XMB)
* Set <kbd>L2</kbd>/<kbd>R2</kbd> buttons as shortcuts to switch between content categories

### Fixed

* XMB callback handling

## [v1.1.6](https://github.com/bucanero/pkgi-ps3/releases/tag/v1.1.6) - 2020-06-23

### Added

* Generate `.rif` files when downloading items (system must be activated)
* Improved speed when creating empty files in background download mode
* Enter button detection (`cross`/`circle`)

## [v1.1.3](https://github.com/bucanero/pkgi-ps3/releases/tag/v1.1.3) - 2020-05-20

### Added

* Improved auto-update code to download `.pkg` updates directly from GitHub
* Increased database memory limit to 32768 items
* Changed App location to the XMB `Network` tab

### Fixed

* The app now allows to download items that already exist
* Start/Stop music works without requiring to restart the application

## [v1.1.2](https://github.com/bucanero/pkgi-ps3/releases/tag/v1.1.2) - 2019-12-31

### Added

* Added content categorization and filtering
* Added support for loading multiple database files
* Added support for online db update/sync

### Fixed

* Filter unsupported or missing URLs when loading a database

## [v1.1.0](https://github.com/bucanero/pkgi-ps3/releases/tag/v1.1.0) - 2019-12-23

### Added

* Added TTF fonts to support Japanese characters
* Added SSL support (the app can download `https` links)
* Added package install bubble for Direct downloads
* Added background music
* Added settings options for music and auto-update

### Fixed

* Fixed UI issue where texts could go beyond the screen limits

## [v1.0.8](https://github.com/bucanero/pkgi-ps3/releases/tag/v1.0.8) - 2019-12-19

### Added

* Added analog pad support
* Added CPU/RSX temperature status
* Added "Details" screen
* Added automatic download after version update check 

### Fixed

* Improved empty `.pkg` file generation using async IO.
* Improved UI

## [v1.0.5](https://github.com/bucanero/pkgi-ps3/releases/tag/v1.0.5) - 2019-12-14

### Added

* Generic text database format support
* Credits `(SELECT)` and Exit `(START)` confirmation dialogs
* Changelog file

### Fixed

* The app now creates `/dev_hdd0/exdata`, if folder doesn't exists
* Fixed unresponsive `background download` dialog while creating a PKG file
* Fixed a bug when the URL was missing

## [v1.0.0](https://github.com/bucanero/pkgi-ps3/releases/tag/v1.0.0) - 2019-12-11

### Added

* Text search filtering using on-screen keyboard
* Background download task mode (uses internal Download Manager)
* New version check

### Fixed

* Fixed incorrect progress bar information during direct download

## [v0.0.1-beta](https://github.com/bucanero/pkgi-ps3/releases/tag/v0.0.1-beta) - 2019-12-04

### Added

* First public beta release
