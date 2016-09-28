Указатели в языке СИ: тест
##########################

:date: 2016-09-28 21:00
:status: draft

.. default-role:: code
.. contents:: Содержание

Тест
====

Во всех задачах требуется ответить, что будет выведено на экран в результате выполнения программы.

Задача №1
---------

.. code-block:: cpp

   int x = 1;
   int *y = &x;
   int z = x;
   *y = 2;

   std::cout << x << " " << y << " " << z << std::endl;

Задача №2
---------

.. code-block:: cpp

   int x = 1;
   int *y = &x;
   int &z = x;
   *y = 2;
   z = 3;
   
   std::cout << x << " " << y << " " << z << std::endl;

Задача №3
---------

.. code-block:: cpp

   int *x = 0;
   *x = 7;

   std::cout << x << " " << *x << std::endl;

Задача №4
---------

.. code-block:: cpp

   int x[2] = {1, 2};
   int *y = x;
   int **z = &y;

   *((*z)+x[0]) = -1;

   std::cout << x[0] << " " << x[1] << " " << y << " " << z << std::endl;

