# NOKIA-PK
Repozytorium grupy **8 KS**

## Lista osób

- Adrian Kulawik (Adrian104)
- Paweł Mączałowski (Pawloland)
- Jakub Mosur (mo-jakub)
- Patryk Magiera (PatMag9)

## Pozostałe informacje
**Tablica**: GitHub Projects (https://github.com/users/Adrian104/projects/1)

## Instrukcja kompilacji
Aby przeprowadzić proces kompilacji, należy wpisać polecenia:

1. Należy sklonować repozytorium za pomocą `git clone https://github.com/Adrian104/Nokia-PK-2025.git`
2. Następnie wchodzimy do katalogu wpisując `cd Nokia-PK-2025`
3. Tworzymy katalog build poleceniem `mkdir build`
4. Tworzymy pliki do budowania za pomocą `cmake -S src -B build`
5. Wchodzimy do katalogu z plikami budowania `cd build`
6. Budujemy za pomocą `make -j $(nproc)`
7. Projekt zbudowany, można uruchomić programy.
