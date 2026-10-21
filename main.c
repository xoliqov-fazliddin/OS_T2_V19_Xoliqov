/*
 * Operatsion tizimlar fani | 2-topshiriq | Variant 19
 * Mavzu : Ikkilik qidiruv daraxti (BST) - dinamik xotira bilan
 * Muallif: Xoliqov Fazliddin, guruh SRM003-2
 *
 * Amallar: qo'shish, qidirish, in-order chiqarish, rekursiv bo'shatish.
 * Kompilyatsiya: gcc -Wall -Wextra -g -o main main.c
 * Tekshiruv    : valgrind --leak-check=full ./main
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_N 1000   /* boshlang'ich kiritiladigan sonlarning eng ko'p soni */

typedef struct Tugun {
    int qiymat;
    struct Tugun *chap;   /* qiymati kichik bolalar */
    struct Tugun *ong;    /* qiymati katta bolalar  */
} Tugun;

/* ---------- Daraxt amallari ---------- */

/* Yangi tugun uchun xotira ajratadi; xotira yetmasa NULL qaytaradi. */
static Tugun *yangi_tugun(int qiymat)
{
    Tugun *t = malloc(sizeof *t);
    if (t == NULL)
        return NULL;
    t->qiymat = qiymat;
    t->chap = NULL;
    t->ong = NULL;
    return t;
}

/*
 * Qo'shish. Ildizga ko'rsatkich MANZILI (Tugun **) uzatiladi, shuning uchun
 * funksiya chaqiruvchining 'ildiz' o'zgaruvchisini o'zgartira oladi.
 * Qaytish: 1 - qo'shildi, 0 - bunday qiymat bor (takror), -1 - xotira yetmadi.
 */
int qosh(Tugun **ildiz, int qiymat)
{
    Tugun **joy = ildiz;

    while (*joy != NULL) {
        if (qiymat == (*joy)->qiymat)
            return 0;
        joy = (qiymat < (*joy)->qiymat) ? &(*joy)->chap : &(*joy)->ong;
    }
    *joy = yangi_tugun(qiymat);
    return (*joy == NULL) ? -1 : 1;
}

/* Qidirish: topilsa tugunga pointer, aks holda NULL. qadam - tekshirilgan tugunlar soni. */
Tugun *qidir(Tugun *ildiz, int qiymat, int *qadam)
{
    *qadam = 0;
    while (ildiz != NULL) {
        (*qadam)++;
        if (qiymat == ildiz->qiymat)
            return ildiz;
        ildiz = (qiymat < ildiz->qiymat) ? ildiz->chap : ildiz->ong;
    }
    return NULL;
}

/* In-order: chap -> o'zi -> o'ng (natija o'sish tartibida). */
void inorder(const Tugun *t)
{
    if (t == NULL)
        return;
    inorder(t->chap);
    printf("%d ", t->qiymat);
    inorder(t->ong);
}

/* Post-order bo'shatish: avval bolalar, keyin tugunning o'zi. */
void bosat(Tugun *t)
{
    if (t == NULL)
        return;
    bosat(t->chap);
    bosat(t->ong);
    free(t);
}

int soni(const Tugun *t)
{
    return t == NULL ? 0 : 1 + soni(t->chap) + soni(t->ong);
}

int balandlik(const Tugun *t)
{
    int a, b;
    if (t == NULL)
        return 0;
    a = balandlik(t->chap);
    b = balandlik(t->ong);
    return 1 + (a > b ? a : b);
}

/* ---------- Kiritish ---------- */

/* 1 - to'g'ri son, -1 - kiritish tugadi (EOF). Noto'g'ri qiymat qayta so'raladi. */
static int son_oqish(const char *taklif, long min, long max, int *natija)
{
    char buf[64];
    char *oxiri;
    long v;

    for (;;) {
        printf("%s", taklif);
        if (fgets(buf, sizeof buf, stdin) == NULL)
            return -1;
        if (strchr(buf, '\n') == NULL && !feof(stdin)) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printf("Xato: juda uzun kiritish.\n");
            continue;
        }
        errno = 0;
        v = strtol(buf, &oxiri, 10);
        while (*oxiri == ' ' || *oxiri == '\t' || *oxiri == '\r' || *oxiri == '\n')
            oxiri++;
        if (oxiri == buf || *oxiri != '\0') {
            printf("Xato: butun son kiriting (harf yoki bo'sh qiymat mumkin emas).\n");
            continue;
        }
        if (errno == ERANGE || v < min || v > max) {
            printf("Xato: son %ld dan %ld gacha bo'lishi kerak.\n", min, max);
            continue;
        }
        *natija = (int)v;
        return 1;
    }
}

static void qosh_va_xabar(Tugun **ildiz, int q, int *xotira_xato)
{
    int r = qosh(ildiz, q);
    if (r == 1)
        printf("  %d qo'shildi.\n", q);
    else if (r == 0)
        printf("  %d allaqachon daraxtda bor, qo'shilmadi.\n", q);
    else {
        printf("Xato: xotira yetarli emas!\n");
        *xotira_xato = 1;
    }
}

/* ---------- Asosiy dastur ---------- */

int main(void)
{
    Tugun *ildiz = NULL;
    int *massiv = NULL;
    int n, i, tanlov, q;
    int xotira_xato = 0;

    /* Boshlang'ich ma'lumot hajmi dastur ishlayotganda kiritiladi */
    if (son_oqish("Boshlang'ich sonlar soni N (1-1000): ", 1, MAX_N, &n) != 1)
        return 0;

    massiv = malloc((size_t)n * sizeof *massiv);   /* dinamik massiv */
    if (massiv == NULL) {
        printf("Xato: xotira yetarli emas!\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        char taklif[40];
        snprintf(taklif, sizeof taklif, "%d-son: ", i + 1);
        if (son_oqish(taklif, -2147483647L, 2147483647L, massiv + i) != 1) {
            free(massiv);
            bosat(ildiz);
            return 0;
        }
    }

    for (i = 0; i < n && !xotira_xato; i++)
        qosh_va_xabar(&ildiz, *(massiv + i), &xotira_xato);
    free(massiv);          /* massiv endi kerak emas */
    massiv = NULL;         /* osilib qolgan pointerdan saqlanish */

    while (!xotira_xato) {
        printf("\n=== BST MENYUSI ===\n");
        printf("1. Qo'shish\n2. Qidirish\n3. In-order chiqarish\n");
        printf("4. Daraxt haqida (tugunlar soni, balandlik)\n5. Chiqish\n");
        if (son_oqish("Tanlang (1-5): ", 1, 5, &tanlov) != 1)
            break;
        if (tanlov == 5)
            break;

        if (tanlov == 1) {
            if (son_oqish("Qo'shiladigan son: ", -2147483647L, 2147483647L, &q) != 1)
                break;
            qosh_va_xabar(&ildiz, q, &xotira_xato);
        } else if (tanlov == 2) {
            int qadam;
            Tugun *t;
            if (son_oqish("Qidiriladigan son: ", -2147483647L, 2147483647L, &q) != 1)
                break;
            t = qidir(ildiz, q, &qadam);
            if (t != NULL)
                printf("  %d topildi (%d qadamda), manzili: %p\n", q, qadam, (void *)t);
            else
                printf("  %d topilmadi (%d qadam tekshirildi).\n", q, qadam);
        } else if (tanlov == 3) {
            printf("  In-order: ");
            inorder(ildiz);
            printf("\n");
        } else {
            printf("  Tugunlar soni: %d, balandlik: %d\n", soni(ildiz), balandlik(ildiz));
        }
    }

    bosat(ildiz);          /* butun daraxtni rekursiv bo'shatamiz */
    ildiz = NULL;
    printf("Xotira bo'shatildi. Dastur tugadi.\n");
    return xotira_xato ? 1 : 0;
}
