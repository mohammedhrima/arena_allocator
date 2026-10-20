/* ************************************************************************** */
/*                                                                            */
/*    _   _ ____      _        main.c                                         */
/*   | | | |  _ \    / \                                                      */
/*   | | | | |_) |  / _ \      Mohammed Hrima                                 */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com                       */
/*    \___/|_| \_\/_/   \_\    created: 2023/11/28 21:02:32                   */
/*                                                                            */
/*   updated: 2026/10/20 05:13:09                                             */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

typedef struct Word Word;
struct Word {
    char *text;
    int count;
    Word *next;
};

char *arena_strndup(const char *s, size_t len) {
    char *copy = arena_alloc(len + 1, 1);
    memcpy(copy, s, len);
    return copy;
}

Word *count_word(Word *list, const char *s, size_t len) {
    for (Word *w = list; w != NULL; w = w->next) {
        if (strlen(w->text) == len && strncmp(w->text, s, len) == 0) {
            w->count++;
            return list;
        }
    }
    Word *w = arena_alloc(1, sizeof(Word));
    w->text = arena_strndup(s, len);
    w->count = 1;
    w->next = list;
    return w;
}

int main(void) {
    const char *text = "the cat sat on the mat and the dog sat on the cat";

    arena_reserve(1024);
    Word *words = NULL;
    for (const char *s = text; *s != '\0';) {
        while (*s == ' ')
            s++;
        const char *e = s;
        while (*e != '\0' && *e != ' ')
            e++;
        if (e > s)
            words = count_word(words, s, e - s);
        s = e;
    }

    for (Word *w = words; w != NULL; w = w->next)
        printf("%-4s %d\n", w->text, w->count);

    printf("memory used: %zu bytes\n", arena_free());
    return 0;
}
