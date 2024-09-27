# Простой таймер/секундомер для даркрума

## Основные принципы

1. Энергоэффективность: если таймер не трогать, то он переходит в режим сна. В режиме сна таймер совсем не потребляет энергии (30 мкA). И может пролежать так несколько лет. При этом в обычном режиме таймер потребляет мало энергии (в среднем около 2.5 мА)
2. Подсветка дисплея: используется LED дисплей в минимальной режиме яркости. Этого должно быть более чем достаточно для удобного просмотра времени при красном свете
3. Простота: есть 2 кнопки и энкодер (одна кнопка на энкодере). Они интуитивно понятно -- обычная кнопка старта/паузы, кнопка на энкодере -- переход. Удержание кнопки на энкодере -- сброс. Удержание кнопки старта -- выключение дисплея
4. Быстрота установки времени: минуты и секунды выстанавливаются отдельно и в любом направлении с помощью энкодера. Это позволяет выставить любое время за несколько секунд

## Режимы

Eсть 4 режима: режим установки времени, режим запущенного таймера, режим запущенного секундомера, режим остановленного секундомера

* Во всех режимах удержание кнопки старта приводит к отключение дисплея. В режиме остановки секундомера и установки времени устройство уходит в сон. Включить дисплей и выйти из сна можно по нажатию любой кнопки или вращению энкодера

### Режим установки времени

Можно распознать по тому, что в нем мигает время

* Крутилка на энкодере меняет время
* Нажатие на энкодер переключает смену между минутами и секундами
* Удержание энкодера сбрасывает установку в 0
* Нажатие на кнопку старт при 0 запускает секундомер, не на 0 -- таймер

### Режим запущенного таймера

Можно распознать по тому, что в нем время идет назад

* Удержание энкодера сбрасывает таймер в значение с которого он был запущен
* Остальные нажатия игнорируются


### Режим запущенного секундомера

Можно распознать по тому, что в нем время идет вперед
    
* Нажатие на старт стопает секундомер
* Удержание энкодера сбрасывает таймер в 0

### Режим остановленного секундомера

Можно распознать по тому, что в нем мигает двоеточие

* Крутилка на энкодере и нажатие на энкодере переключает нас в режим установки времени (с того времени на котором мы остановились)
* Нажатие на старт продолжает работу секундомера
* Удержание энкодера сбрасывает таймер в 0
