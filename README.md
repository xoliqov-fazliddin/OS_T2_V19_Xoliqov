# OS_T2_V19_Xoliqov

Operatsion tizimlar fani, **2-topshiriq**, **Variant 19** (Ikkilik qidiruv daraxti, BST).
Muallif: Xoliqov Fazliddin, guruh SRM003-2.

## Tarkibi
- `main.c` - BST: qo'shish, qidirish, in-order chiqarish, rekursiv bo'shatish
- `valgrind.txt` - valgrind natijasi

## Kompilyatsiya va ishga tushirish (Linux / WSL2)
```bash
gcc -Wall -Wextra -g -o main main.c
./main
```

## Xotira tekshiruvi
```bash
valgrind --leak-check=full ./main 2> valgrind.txt
```
Natijada `All heap blocks were freed -- no leaks are possible` qatori chiqishi kerak.
