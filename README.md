// программа в процессе разработки, данный вариант не окончательный 

# Описание 
Данная программа является попыткой создать собственный контейнер, подобный std::vector, с учебной и познавательной целью.
 
# Особенности 
1. Основной особенностью класса Vector является безопасность относительно исключений. Использовалась концепция "Линия Калба", разработанная Джоном Калбом: http://exceptionsafecode.com/.
На примере конструктора рассмотрим главную идею: выше линии Калба (показана красным) код не меняет состояние класса, ниже линии код не бросает исключений.

	![1](https://raw.githubusercontent.com/Genomorf/Vector/master/screenshots/Kalb_line.png)
 
	Именно поэтому данный код изобилует созданием (и дальнейшим swap'ом) временных объектов, над которыми проводятся операции, чтобы при бросании исключений не нарушить инварианты и состояние класса.

2. Также класс Vector разделен на 2 самостоятельных интерфейса. Первый - VectorBuffer - явяляется RAII оберткой над сырым динамическим массивом, который инкапсулирует работу с памятью и инварианты size и capacity.
Основной класс Vector предоставляет интерфейс контейнера. Таким образом, код разделяет ответственности за владение ресурсом и логикой контейнера. Такое проектирование является попыткой имплементировать Single Responsibility Principle и Interface Segregation Principle.

3. Наконец, в коде присутствует наивный random access итератор для вектора. Имплементированы iterator traits и основные операции.

# Особенности использования языка С++
* Безопасный код относительно исключений 
* Операции над сырой памятью: ::operator new, placement new
* Семантика перемещения: std::move, std::forward, std::swap
* Наивное SFINAE: enable_if
* Использование шаблонов, пачки параметров
* Итераторы, iterator traits 
* ООП: правило пяти (VectorBuffer), инкапсуляция, SRP, ISP, RAII 

# Проблемы и недоработки
1. Очень медленный код. 100'000 push_back'ов занимают 3.0 секунды против ~0.0 у std::vector. 
2. Большое количество создания временных объектов, которые служат безопасности исключений.
3. Отсутствие поддержки const Vector.
4. Копипаста, неоптимизированный код и многое другое...

# TODO:
* const, constexpr методы
* Unit Tests
* реализовать остальные методы как у std::vector
* оптимизация медленного кода
* достигнуть производительности как у std::vector