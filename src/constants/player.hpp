#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

struct PlayerMove {
    bool Up = false;
    bool Down = false;
    bool Left = false;
    bool Right = false;
};

class Player {
private:
    sf::RenderWindow& window;
    float radius;
    float x = 0.f;
    float y = 0.f;
    sf::CircleShape shape;
    float& delta;
    PlayerMove& direction;
public: 
    Player(sf::RenderWindow& window, float radius, float& delta, PlayerMove& direction) : window(window), radius(radius), delta(delta), direction(direction) {
        shape.setRadius(radius);
    }

    void draw () {
        window.draw(shape);

    }

    void update() {
        if (direction.Up) moveUp();
        if (direction.Down) moveDown();
        if (direction.Left) moveLeft();
        if (direction.Right) moveRight();
    }
private:
    void _move(float dx, float dy) { x += dx; y += dy; shape.setPosition({x, y}); }
    void tp(float newX, float newY) { x = newX; y = newY; shape.setPosition({x, y}); }

    void moveUp() { _move(0.f, -1.f * delta); }
    void moveDown() { _move(0.f, 1.f * delta); }
    void moveLeft() { _move(-1.f * delta, 0.f); }
    void moveRight() { _move(1.f * delta, 0.f); }

};