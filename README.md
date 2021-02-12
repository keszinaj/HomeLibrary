# HomeLibrary

# Dokumentacja uruchamieniowa
System operacyjny: Linux

Kompilator: gcc

Pobranie dodatkowych bibliotek potrzrebynych do uruchomienia programu:

```bash
 sudo apt-get install libncurses5-dev libncursesw5-dev
```
Instrukcja jak poprawnie zainstalować biblioteke ncurses:
 https://www.cyberciti.biz/faq/linux-install-ncurses-library-headers-on-debian-ubuntu-centos-fedora/

Kompilacja:

 ```bash
    make
```

 # Instrukcja obsługi programu
 Każde okienko ma  miejsce pod spodem gdzie opisane są wszystkie możliwe komendy i ruchy

 # Opis funkcjonalności projektu

display.c - odpowiada za obsługę menu oraz jego wyświetlanie
logic.c - tutaj znajdują sie wszystkie funkcje związane z obsługą bazy danych 
oraz logika dzialania całego programu i przetwarzania danych
win_book.c - odpowiada oknu które pojawi się po wejściu w książki
win_edit_book.c - odpowiada oknu które pojawi się po edytowaniu książki
win_search.c -odpowiada oknu które pojawi się podczas szukania książki
win_lent_books.c -odpowiada oknu które pojawi się podczas przeglądania pożyczonych książek
win_add_book.c-odpowiada oknu które pojawi się podczas dodawania książki
ncurses_my_fun.c- tutaj znajdują się funkcje które pomogły mi w pracy z biblioteką ncurses
info_win.c-tutaj znajduje się zbiór prostych okien które mają za zadanie tylko informować użytkownika o tym co zrobił 
