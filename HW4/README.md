# Homework #4

**Задание 1**
Сгенерируйте слачайные графы и протестируйте пересчет числа 
маршрутов в графе через вычисление степени матрицы связности $A^m$.
 Реализуйте через быстрое возвежение в степень за $log_2(m)$ 
матричных умножений.

> См. файл `matrix_power.c`. Генерирование матрицы связности графа задается случайно при каждом запуске. Переменные $N$ (размер графа) и $power$ (показатель степени) задаются во внешнем окружении.
>
> Пример работы программы приведен ниже.

![alt text](https://github.com/Dmitry-Galkin/HPC_homeworks/blob/main/HW4/matrix_power.png)

**Задание 2**
Сгенерируйте (или предложите) небольшой случайный граф и реализуйте
 процедуру вычисления вектора Pagerank. Сопоставьте с наивным 
ранжированием по числу входящих ссылок ranking.

> См. файл `page_rank.c`. 
> 
> В качестве графа взят граф Московского метро. Исходные файлы: `data/stations_data.txt` и `data/routes_data.txt`. 
> В файле `moscow_subway.ipynb` эти данные уже преобразовал в граф в виде матрицы смежности (файл `data/SubwayAdj.txt`) и в список названий станций (файл `data/StationName.txt`). 
> 
> Расчет был проведен для коэффициент перехода $\alpha=0.99$. Оставил вероятность 1% для телепортации пассажиров:)
> 
> Полученный вектор `PageRank` был отсортирован по убыванию. Начало и конец списка станций метро
> с соответсвующими значениями `PageRank `приведены ниже.
> 
> Видно, что наиболее "авторитетные" станции - центр и кольцо.
> 
> С наивным ранжированием не сопоставлял. Т.к. большинство станций имеет по 2, реже по 3 и более входящих ребра. И в приницпе результат понятен.

![alt text](https://github.com/Dmitry-Galkin/HPC_homeworks/blob/main/HW4/pagerank_head.png)

![alt text](https://github.com/Dmitry-Galkin/HPC_homeworks/blob/main/HW4/pagerank_tail.png)

**Задание 3**
Воспользуйтесь всеми трюками по оптимизации производительности: 
optimization, blas, openmp, loops placing.

> См. файл `page_rank_boost.c`.
> 
> Попробовал только `optimization` и `openmp`. Результат времени работы алгоритма `PageRank` приведен в таблице ниже.

|Оптимизация|Время выполнения, мс|
|---|---|
|Без оптимизации|137|
|**-O3**|45|
|**OpenMP** + **-O3**|8|

