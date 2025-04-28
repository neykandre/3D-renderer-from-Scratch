# 3D-renderer from Scratch
Software 3D-renderer с нуля

## Установка программы
Для установки программы необходимо склонировать репозиторий и собрать проект.
1. Клонирование репозиотрия
```bash
git clone https://github.com/neykandre/3D-renderer-from-Scratch.git
```
3. Переход в директорию с проектом:
```bash
cd 3D-renderer-from-Scratch
```
4. Импортирование сторонних библиотек в виде подмодулей git:
```bash
git submodule init
git submodule update
```
5. Генерация файлов сборки:
```bash
cmake -S . -B build
```
7. Сборка проекта:
```bash
cmake --build build --target 3d-renderer -j 6
```
9. Скомпилированная программа расположена по пути `./build/bin/3d-renderer`
