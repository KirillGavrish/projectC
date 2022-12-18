#include <SFML/Window.hpp>
#include "kostya.h"

int main()
{
    andrey();
    kirill();
    kostya();
    
    // Объявление и создать нового окна рендеринга
sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
// Ограничение частоты кадров до 60 кадров в секунду
window.setFramerateLimit(60);
// Основной цикл - заканчивается, как только окно закрывается
while (window.isOpen())
{
   // Обработка событий
   sf::Event event;
   while (window.pollEvent(event))
   {
       // Запрос на закрытие окна
       if (event.type == sf::Event::Closed)
           window.close();
   }
   // Очищение всего окна перед отображением нового фрейма
   window.clear();
   // Рисование нескольких графических объектов
   window.draw(sprite);
   window.draw(circle);
   window.draw(text);
   // Завершение текущего кадра и отображение его содержимого на экране
   window.display();
}
    
    return 0;
}
