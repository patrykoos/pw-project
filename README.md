# pw-project
(concurrent programming project)
programy cmake i testy nie zostały tu dodane.
Celem zadania jest zaimplementowanie w C++ algorytmów w sposób sekwencyjny oraz 
współbieżnych trzech algorytmów, rozwiązujących następujące problemy:

- Dyskretny problem plecakowy (problem 1.)
- Sortowanie przez scalanie (Merge sort) lub sortowanie szybkie (Quicksort) (problem 2.)
- Znajdowanie elementu maksymalnego (problem 3.)

Implementacje algorytmów muszą znajdować prawidłowe rozwiązanie (a nie jego
przybliżenie). Implementacje sekwencyjne powinny być zaimplementowanie wydajnie:

- Problem 1. przy użyciu programowania dynamicznego
- Problem 2. w czasie O(NlogN)
- Problem 3. w czasie O(N)

Implementacje współbieżne powinny osiągać praktyczne przyspieszenie
(to znaczy działać szybciej niż ich wersje sekwencyjne dla  odpowiednio dużych danych
wejściowych), przy czym osiągnięte przyspieszenia będą różne dla poszczególnych algorytmów 
i różnych danych wejściowych. Celem zadania nie jest implementacja najszybszych współbieżnych 
algorytmów dla powyższych problemów opublikowanych w pracach naukowych, tylko wymyślenie 
własnych praktycznych modyfikacji oraz analiza ich wydajności.

Do rozwiązania powinien być dołączony raport w PDF, przedstawiający analizę
czasu działania zaimplementowanych algorytmów w sposób sekwencyjny oraz
współbieżny dla różnej liczby wątków i rdzeni. Raport powinien zawierać stosowne
wykresy i komentarz, ale nie być dłuższy niż dwie strony. Każdy eksperyment powinien być
przeprowadzony kilkukrotnie, w celu poznania wartości minimalnych, maksymalnych
i średnich. Najlepiej, gdyby eksperymenty zostały przeprowadzone w dwóch różnych
środowiskach np. laptop lub komputer w laboratorium i maszyna students (to zrozumiałe,
że laptop może mieć niewystarczającą liczbę rdzeni do przeprowadzenia niektórych
eksperymentów). Analiza powinna też uwzględnić dwa różne typy kompilacji:
"-DCMAKE_BUILD_TYPE=Release" oraz  "-DCMAKE_BUILD_TYPE=Debug". W raporcie można uwzględnić również 
wyniki własnych testów.