# PKGi PS3 RUS MOD

[![Downloads][img_downloads]][pkgi_downloads] [![Release][img_latest]][pkgi_latest] [![License][img_license]][pkgi_license]


**PKGi PS3** это порт для PlayStation 3 от PSVita [pkgi](https://github.com/mmozeiko/pkgi).

Хоумбрю приложение `pkgi-ps3` позволяет загружать и устанавливать файлы `.pkg` прямо на вашу PS3.

![image](https://i3.imageban.ru/out/2022/10/29/d2ebf512315935e7826e394672b83df6.jpg)

**Комментарии, идеи, предложения?** Вы можете отписаться [мне](https://github.com/ErikPshat/) всегда в теме [на форуме](https://www.pspx.ru/forum/showthread.php?t=110158).

# Особенности

* **лёгок в использовании** список доступных загрузок, включая поиск, фильтрацию и сортировку.
* **автономный:** ПК не требуется, все происходит прямо на PS3.
* **автоматические загрузки:** просто выберите элемент, и он будет загружен приложением на ваш жесткий диск (`прямая загрузка`) или поставлен в очередь (`фоновая загрузка`) с помощью внутреннего диспетчера загрузки.
* **возобновляет прерванные загрузки:** вы можете остановить загрузку в любое время, переключать приложения и вернуться позже, чтобы возобновить загрузку.
* **активация контента:** приложение может генерировать файлы `.rif` для загруженного контента (система должна быть активирована)

### Примечания:
* **постановка в очередь** загрузка нескольких игр поддерживается только при использовании режима `фоновая загрузка`.
* **фоновые задачи загрузки** появится только после перезагрузки PS3.

# Скачать

Получить [последнюю версию здесь][pkgi_latest].

### Журнал изменений

Смотрите [последние изменения здесь](CHANGELOG.md).

# Инструкция по установке

Вам необходимо создать файл `pkgi.txt` в каталоге `/dev_hdd0/game/NP00PKGI3/USRDIR`, содержащий элементы, доступные для установки.
Формат текстовой базы данных настраивается пользователем. Проверьте [эту секцию](#user-defined-db-format), чтобы узнать, как определить свой собственный формат базы данных.

## Несколько баз данных

Вы можете загружать дополнительные файлы базы данных:

- `pkgi_apps.txt`
- `pkgi_avatars.txt`
- `pkgi_cheats.txt`
- `pkgi_demos.txt`
- `pkgi_dlcs.txt`
- `pkgi_games.txt`
- `pkgi_minis.txt`
- `pkgi_ps1.txt`
- `pkgi_ps2.txt`
- `pkgi_russian.txt`
- `pkgi_themes.txt`
- `pkgi_tunings.txt`

Элементы каждого из этих файлов будут автоматически классифицированы по типу содержимого файла. **Примечание:** Приложение предполагает, что каждый файл базы данных имеет один и тот же формат, как определено в `dbformat.txt`.

## Online DB update

Вы можете обновить и синхронизировать онлайн-базу данных, добавив БД URL-адреса в файл `config.txt` по пути `/dev_hdd0/game/NP00PKGI3/USRDIR`. 

Для примера:

```
url http://www.mysite.com/mylist.csv
url_tunings http://www.myapp.com/tunings.csv
url_emulators http://www.example.com/emulators.csv
```

При настройках в данном примере выше:
* `pkgi.txt` будет обновлён из `mylist.csv`
* `pkgi_tunings.txt` будет обновлён из `tunings.csv`
* `pkgi_emulators.txt` будет обновлён из `emulators.csv`

Когда вы откроете приложение в следующий раз, у вас будет дополнительный пункт меню ![Triangle](https://github.com/bucanero/pkgi-ps3/raw/master/data/TRIANGLE.png) под названием **Обновить**. Когда вы выберете его, локальные базы данных будут синхронизированы с определенными URL-адресами.

# DB formats

The application needs a text database that contains the items available for installation, and it must follow the [default format definition](#default-db-format), or have a [custom format definition](#user-defined-db-format) file.

## Default DB format

The default database file format uses a very simple CSV format where each line means one item in the list:

```
contentid,type,name,description,rap,url,size,checksum
```

where:

| Column | Description |
|--------|-------------|
| `contentid` | is the full content id of the item, for example: `UP0000-NPXX99999_00-0000112223333000`.
| `type` | is a number for the item's content type. See the [table below](#content-types) for details. (set it to 0 if unknown)
| `name` | is a string for the item's name.
| `description` | is a string for the item's description.
| `rap` | the 16 hex bytes for a RAP file, if needed by the item (`.rap` files will be created on `/dev_hdd0/exdata`). Leave empty to skip the `.rap` file.
| `url` | is the HTTP/HTTPS/FTP/FTPS URL where to download the `.pkg` file.
| `size` | is the size in bytes of the `.pkg` file, or 0 if unknown.
| `checksum` | is a SHA256 digest of the `.pkg` file (as 32 hex bytes) to make sure the file is not tampered with. Leave empty to skip the check.

**Note:** `name` and `description` cannot contain newlines or commas.

### Sample DB file

An example `pkgi.txt` file following the `contentid,type,name,description,rap,url,size,checksum` format:
```
EP0000-NP9999999_00-0AB00A00FR000000,0,My PKG Test,A description of my pkg,dac109e963294de6cd6f6faf3f045fe9,http://192.168.1.1/html/mypackage.pkg,2715513,afb545c6e71bd95f77994ab4a659efbb8df32208f601214156ad89b1922e73c3
UP0001-NP00PKGI3_00-0000000000000000,0,PKGi PS3 v0.1.0,,,http://bucanero.heliohost.org/pkgi.pkg,284848,3dc8de2ed94c0f9efeafa81df9b7d58f8c169e2875133d6d2649a7d477c1ae13
```
### Content types

| Type value |	Content type |
|------------|--------------|
| 0	| Все
| 1	| Игры из регионов
| 2	| Игры на русском
| 3	| Игры PS2 для PS3
| 4	| Игры PS1 для PS3
| 5	| miniS для PS3
| 6	| Дополнения DLC
| 7	| Темы оформления
| 8	| Аватары
| 9	| Демо
| 10	| Менеджеры
| 11	| Приложения
| 12	| Читы к играм
| 13	| Обновления

## User-defined DB format

To use a custom database format, you need to create a `dbformat.txt` file, and save it on `/dev_hdd0/game/NP00PKGI3/USRDIR`.

The `dbformat.txt` definition file is a 2-line text file:
* Line 1: the custom delimiter character (e.g.: `;`, `,`, `|`, etc.)
* Line 2: the column names for every column in the custom database, delimited by the proper delimiter defined in line 1

**Note:** For the columns to be properly recognized, use the column tag names defined in the table above.

All the columns are optional. Your database might have more (or less) columns, so any unrecognized column will be skipped.

### Example

Example `dbformat.txt`, for a database using semi-colon (`;`) as separator:

```
;
name;TITLE ID;REGION;description;AUTHOR;TYPE;url;rap;size
```

**Result:** only the `name,description,url,rap,size` fields will be used.

### Example

Example `dbformat.txt`, for a database using character pipe (`|`) as separator:

```
|
REGION|TITLE|name|url|rap|contentid|DATE|PKG FILENAME|size|checksum
```

**Result:** only the `name,url,rap,contentid,size,checksum` fields will be used.

# Usage

Using the application is simple and straight-forward: 

 - Move <kbd>UP</kbd>/<kbd>DOWN</kbd> to select the item you want to download, and press ![X button](https://github.com/bucanero/pkgi-ps3/raw/master/data/CROSS.png).
 - To see the item's details, press ![Square](https://github.com/bucanero/pkgi-ps3/raw/master/data/SQUARE.png).
 - To sort/filter/search press ![Triangle](https://github.com/bucanero/pkgi-ps3/raw/master/data/TRIANGLE.png).
It will open the context menu. Press ![Triangle](https://github.com/bucanero/pkgi-ps3/raw/master/data/TRIANGLE.png) again to confirm the new settings, or press ![O button](https://github.com/bucanero/pkgi-ps3/raw/master/data/CIRCLE.png) to cancel any changes.
- Press left or right trigger buttons <kbd>L1</kbd>/<kbd>R1</kbd> to move pages up or down.
- Press <kbd>L2</kbd>/<kbd>R2</kbd> trigger buttons to switch between categories.

### Notes

- **RAP data:** if the item has `.rap` data, the file will be saved in the `/dev_hdd0/exdata/` folder.


# Q&A

1. Where to get a `rap` string? 
   
   You can use a tool like RIF2RAP to generate a `.rap` from your existing `.rif` files. Then you can use a tool like `hexdump` to get the hex byte string.

2. Where to get `.pkg` links?
   
   You can use [PSDLE][] to find `.pkg` URLs for the games you own. Then either use the original URL, or host the file on your own web server.

3. Where to remove interrupted/failed downloads to free up disk space?
   
   Check the `/dev_hdd0/tmp/pkgi` folder - each download will be in a separate `.pkg` file by its title id. Simply delete the file and start again.

4. Download speed is too slow!
   
   Optimization is still pending. If `direct` download is slow, you can use `background download` mode to download files using the internal PS3 Download Manager.

# Credits

* [Bucanero](http://www.bucanero.com.ar/): Project developer
* [mmozeiko](https://github.com/mmozeiko/): [PS Vita pkgi](https://github.com/mmozeiko/pkgi)

# Building

You need to have installed:

- [PS3 toolchain](https://github.com/bucanero/ps3toolchain)
- [PSL1GHT](https://github.com/ps3dev/PSL1GHT) SDK
- [Tiny3D](https://github.com/wargio/Tiny3D) library
- [YA2D](https://github.com/ErikPshat/pkgi/tree/master/ya2d_ps3) library (an extended version from my repo)
- [PolarSSL](https://github.com/bucanero/ps3libraries/blob/master/scripts/015-polarssl-1.3.9.sh) library (v1.3.9)
- [libcurl](https://github.com/bucanero/ps3libraries/blob/master/scripts/016-libcurl-7.64.1.sh) library (v7.64.1)
- [MikMod](https://github.com/ps3dev/ps3libraries/blob/master/scripts/011-libmikmod-3.1.11.sh) library
- [Mini18n](https://github.com/bucanero/mini18n) library
- [dbglogger](https://github.com/bucanero/psl1ght-libs/tree/master/dbglogger) library (only required for debug logging)

Run `make` to create a release build. After that, run `make pkg` to create a `.pkg` install file. 

You can also set the environment variable `PS3LOAD=tcp:x.x.x.x` to the PS3's IP address;
that will allow you to use `make run` and send `pkgi-ps3.self` directly to the [PS3LoadX listener](https://github.com/bucanero/ps3loadx).

## Debugging

To enable debug logging, build PKGi PS3 with `make DEBUGLOG=1`. The application will send debug messages to
UDP multicast address `239.255.0.100:30000`. To receive them you can use [socat][] on your PC:

    $ socat udp4-recv:30000,ip-add-membership=239.255.0.100:0.0.0.0 -

# License

`pkgi-ps3` is released under the [MIT License](LICENSE).

[PSDLE]: https://repod.github.io/psdle/
[socat]: http://www.dest-unreach.org/socat/
[pkgi_downloads]: https://github.com/ErikPshat/pkgi/releases
[pkgi_latest]: https://github.com/ErikPshat/pkgi/releases/latest
[pkgi_license]: https://github.com/ErikPshat/pkgi/blob/master/LICENSE
[img_downloads]: https://img.shields.io/github/downloads/ErikPshat/pkgi/total.svg?maxAge=3600
[img_latest]: https://img.shields.io/github/release/ErikPshat/pkgi.svg?maxAge=3600
[img_license]: https://img.shields.io/github/license/ErikPshat/pkgi.svg?maxAge=2592000
