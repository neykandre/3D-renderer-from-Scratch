# 3D-renderer from Scratch

Software 3D-renderer с нуля

## Установка программы

Для установки программы необходимо склонировать репозиторий и собрать проект.

1. Клонирование репозитория

```bash
git clone https://github.com/neykandre/3D-renderer-from-Scratch.git
```

2. Переход в директорию с проектом:

```bash
cd 3D-renderer-from-Scratch
```

3. Импортирование сторонних библиотек в виде подмодулей git:

```bash
git submodule init
git submodule update
```

4. Генерация файлов сборки:

```bash
cmake -S . -B build
```

5. Сборка проекта:

```bash
cmake --build build --target 3d-renderer -j 6
```

6. Скомпилированная программа расположена по пути `./build/bin/3d-renderer`

## Демонстрация

![demo](./demo/Screencast%20from%202025-04-27%2021-46-13%20(online-video-cutter.com).gif)