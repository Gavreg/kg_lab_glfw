## Описание
LAB1 - проект для лабораторной работы без освещения

LAB2 - проект для лабораторной работы с поддержкой освещения и текстурирования

LAB2_training - проект для тренировки второй ЛР

LAB4+ - проект для курсовой

Внутри каждой папки лежит CMakeLists.txt


## Сборка

### Windows + Visual Studio

Для конфигурации проекта необходимо установить CMake (если нет winget, то CMake можно скачать с официального сайта).

```
winget install cmake
```

Для работы в Windows необходимо иметь Visual Studio с установленными компонентами для С++:

![image](images/windows_vs.jpg)


Для создания проекта, необимо перейти в папку нужной лабораторной работы и сконфигурировать проект командой (Shift+ПКМ в нужной папке -> Открыть в терминале):

<details>

```
PS C:\...\LAB2_training> cmake -B build

-- Building for: Visual Studio 18 2026
-- Selecting Windows SDK version 10.0.26100.0 to target Windows 10.0.26200.
-- The C compiler identification is MSVC 19.50.35727.0
-- The CXX compiler identification is MSVC 19.50.35727.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - not found
-- Found Threads: TRUE
-- Including Win32 support
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE)
-- Documentation generation requires Doxygen 1.9.8 or later
-- Found OpenGL: opengl32
-- Configuring done (20.0s)
-- Generating done (0.3s)
-- Build files have been written to: C:/.../LAB2_training/build

PS C:\...\LAB2_training> 

```

</details>

Cmake обнаружит необходимую версию VS, а так же компиляторы С и С++. После чего в папке build будет создано решение Visual Studio.

В решении будет находится несколько проектов: проекты, необходимые для сборки библиотеки GLWF, и проект для лабораторной работы kg_lab. Его необходимо сделать основным:

<img src="images/windows_vs2.jpg" height=400/>

#### Примечание
Может быть такое, что создасться makefile, тогда можно попытаться указать дополнительный параметр **-G "MinGW Makefiles"** к команде **cmake**. 
Если все же продолжает создаваться makefile, можно ознакомиться с инструкцией по ручной компляции из следующего раздела.

### MacOS
Сначала необходимо установить пакеты OpenGL, GLFW и CMake и какой-нибудь компилятор (в примере gcc).

```
brew install glfw freeglut cmake make gcc
```

Далее в папке репозитория:

```
cmake -B build
```

Создасться makefile в папке buid.
Для компиляции необходимо перейти в папку buid и вызвать make

```
cd ./buid
```
```
make
```

Запуск программы из папки buid:
```
./kg_lab
```

#### Дополнительно

Вызов cmake для Debug сборки (добавляется флаг для отладки, см. [CMakeLists](./LAB1/CMakeLists.txt)):

```
cmake -DCMAKE_BUILD_TYPE=Debug -B build
```

Все вышеперечисленные команды можно встроить в свой IDE. 
Например в VS Code, настроив файлы tasks.json и launch.json с дополнением, поддерживающим Debug сессии.

### Linux

Анологично как для MacOS необходимо установить пакеты OpenGL, GLFW и CMake и какой-нибудь компилятор.

Debian-подобные дистрибутивы
```
sudo apt update
sudo apt install build-essential cmake libgl1-mesa-dev libglfw3-dev
```

Arch-подобные дистрибутивы
```
sudo pacman -Sy cmake make mesa glfw gcc
```

Далее те же действия как для MacOS.
