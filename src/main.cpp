#include <iostream>
#include <SFML/Graphics.hpp>
#include "shaders/rain.hpp"
#include "constants/player.hpp"

int main() {
    sf::VideoMode video_mode({ 1600, 900 });
    constexpr auto window_style = sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close;

    sf::RenderWindow window(
        video_mode,
        "meow game",
        window_style
    );

    window.setFramerateLimit(240);

    sf::View overlay;

    sf::Vector2f windowFSize({
        static_cast<float>(window.getSize().x),
        static_cast<float>(window.getSize().y)
    });

    sf::RectangleShape rect(windowFSize);

    float dt = 1;
    PlayerMove playerMoving;
    Player player(window, 50.f, dt, playerMoving);


    if (!sf::Shader::isAvailable()) {
        std::cerr << "Shaders are not available on this system." << std::endl;
        return 1;
    }

    sf::Shader shader;
    
    if (!shader.loadFromMemory(fragmentShader, sf::Shader::Type::Fragment)) {
        std::cerr << "Failed to load shader. frag" << std::endl;
        return 1;
    }

    shader.setUniform("u_resolution", windowFSize);
    shader.setUniform("u_slow", 4.f);
    shader.setUniform("u_rarity", .7f);
    
    sf::Clock shaderClock;

    sf::Clock clock;

    while (window.isOpen()) {
        clock.start();
        while (std::optional<sf::Event> event = window.pollEvent()) {
            if (event -> is <sf::Event::Closed>()) window.close();
            if (event -> is <sf::Event::Resized>()) {
                sf::Vector2u windowResize {
                    event->getIf<sf::Event::Resized>()->size.x,
                    event->getIf<sf::Event::Resized>()->size.y
                };
                window.setSize(windowResize);
                overlay.setSize(sf::Vector2f(windowResize));
                overlay.setCenter(sf::Vector2f(windowResize) / 2.0f);
                window.setView(overlay);
                rect.setSize(sf::Vector2f(windowResize));
                shader.setUniform("u_resolution", sf::Vector2f(windowResize));
            }
            if (event -> is <sf::Event::KeyPressed>()) {
                sf::Keyboard::Key key = event->getIf<sf::Event::KeyPressed>()->code;
                switch (key) {
                    case sf::Keyboard::Key::W:
                        playerMoving.Up = true;
                        break;
                    case sf::Keyboard::Key::S:
                        playerMoving.Down = true;
                        break;
                    case sf::Keyboard::Key::A:
                        playerMoving.Left = true;
                        break;
                    case sf::Keyboard::Key::D:
                        playerMoving.Right = true;
                        break;
                }
            }
            if (event -> is <sf::Event::KeyReleased>()) {
                sf::Keyboard::Key key = event->getIf<sf::Event::KeyReleased>()->code;
                switch (key) {
                    case sf::Keyboard::Key::W:
                        playerMoving.Up = false;
                        break;
                    case sf::Keyboard::Key::S:
                        playerMoving.Down = false;
                        break;
                    case sf::Keyboard::Key::A:
                        playerMoving.Left = false;
                        break;
                    case sf::Keyboard::Key::D:
                        playerMoving.Right = false;
                        break;
                }
            }
        }
        float currentTime = shaderClock.getElapsedTime().asSeconds();
        shader.setUniform("u_time", currentTime);

        window.clear(sf::Color::Black);

        player.update();
        player.draw();

        window.draw(rect, &shader);


        window.display();
        dt = static_cast<float>(clock.getElapsedTime().asMicroseconds())/1000;
        clock.reset();
    }
    window.close();
    return 0;
}