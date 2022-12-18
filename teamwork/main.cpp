#include <SFML/Window.hpp>
#include "kostya.h"

struct Hit
{
    vec3 position;
    vec3 normal;
    vec3 albedo;
};

int main()
{
    andrey();
    kirill();
    kostya();
    
    using u32 = unsigned int;

    u32 const width  = 1920u;
    u32 const height = 1080u;

    float const fwidth  =  width;
    float const fheight = height;

    Camera const camera =
    {
        .position = {-4.f, 3.6f, 12.f},
        .at = {0.f, 0.f, 0.f},
        .up = {0.f, 1.f, 0.f},
        .aspectRatio = fwidth / fheight,
        .verticalFOV = 0.55f,
    };
    
    // Объявление и создать нового окна рендеринга
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML window");
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
        // Завершение текущего кадра и отображение его содержимого на экране
        window.display();
    }
    
    std::cout << "P3\n" << width << " " << height << "\n255\n";
    for(u32 y = 0u; y < height; ++y)
    for(u32 x = 0u; x <  width; ++x)
    {
        float const u = -1.f + 2.f * float(x) / fwidth;
        float const v = -1.f + 2.f * float(y) / fheight;
        float const du = 1.f / fwidth;
        float const dv = 1.f / fheight;

        // 4 rays for antialiasing
        vec3 const color = tonemap
        (
            trace(camera.castRay(u - du, -v - dv))
          + trace(camera.castRay(u - du, -v + dv))
          + trace(camera.castRay(u + du, -v - dv))
          + trace(camera.castRay(u + du, -v + dv))
        );

        std::cout << u32(color.x) << " ";
        std::cout << u32(color.y) << " ";
        std::cout << u32(color.z) << " ";
    }

    destroyTree(tree);
    deleteImage(image);
    return 0;
}
