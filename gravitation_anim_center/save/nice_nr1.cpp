#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Event.hpp>
#include <bits/stdc++.h>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

class GravitySource {
  sf::Vector2f pos;
  float strength;

  sf::CircleShape s;

public:
  GravitySource(float pos_x, float pos_y, float strength) {
    pos.x = pos_x;
    pos.y = pos_y;
    this->strength = strength;

    s.setPosition(pos);
    s.setFillColor(sf::Color::White);
    s.setRadius(0);
  }

  void render(sf::RenderWindow &wind) {
    s.setPosition(pos);
    wind.draw(s);
  }

  sf::Vector2f get_pos() { return pos; };

  float get_strength() { return strength; };
};

class Particle {
  sf::Vector2f pos;
  sf::Vector2f vel;

  sf::CircleShape s;

public:
  Particle(float pos_x, float pos_y, float vel_x, float vel_y) {
    pos.x = pos_x;
    pos.y = pos_y;

    vel.x = vel_x;
    vel.y = vel_y;

    s.setPosition(pos);
    // int randomr, randomg, randomb;
    // randomr = 1 + rand()%(255);
    // randomg = 1 + rand()%(255);
    // randomb = 1 + rand()%(255);
    // s.setFillColor(c);
    s.setRadius(1);
  }

  void render(sf::RenderWindow &wind) {
    s.setPosition(pos);
    wind.draw(s);
  }

  void update_physics(GravitySource &s) {
    float distance_x = s.get_pos().x - pos.x;
    float distance_y = s.get_pos().y - pos.y;

    float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

    float inverse_distance = 1.f / distance;

    float normalized_x = inverse_distance * distance_x;
    float normalized_y = inverse_distance * distance_y;

    float inverse_square_dropoff = inverse_distance * inverse_distance;

    float acceleration_x =
        normalized_x * s.get_strength() * inverse_square_dropoff;
    float acceleration_y =
        normalized_y * s.get_strength() * inverse_square_dropoff;

    vel.x += acceleration_x;
    vel.y += acceleration_y;

    pos.x += vel.x;
    pos.y += vel.y;
  }
};

int main() {
  sf::RenderWindow window(sf::VideoMode(1280, 720), "My Program");
  window.setFramerateLimit(60);

  sf::Event event;

  if (event.type == sf::Event::Resized) {
    sf::FloatRect view(0, 0, event.size.width, event.size.height);
    window.setView(sf::View(view));
  }

  GravitySource source(window.getSize().x * 0.5, window.getSize().y * 0.5,
                       100);

  std::vector<Particle> myParticles;

  // int myPartNum = 40;
  //
  // myParticles.reserve(myPartNum);
  // for (int i = 0; i < myPartNum; ++i) {
  //   myParticles.push_back(Particle(window.getSize().x * 0.5 + i * 10,
  //                                  window.getSize().y * 0.5 + 300, 2, 0));
  // }

  std::chrono::high_resolution_clock::time_point start;
  std::chrono::high_resolution_clock::time_point end;
  float fps;
  sf::Color color;
  // sf::Uint32 c = (color.r << 16) | (color.g << 8) | color.b;

  sf::Font font;
  // Load and check the availability of the font file
  if (!font.loadFromFile("arial.ttf")) {
    cout << "can't load font" << endl;
  }
  sf::Text printNum;

  for (int i = 0; i < 200; ++i) {
    for (int a = 0; a < 200; ++a) {

      myParticles.push_back(Particle(window.getSize().x * 0.5 - 100 + i,
                                     window.getSize().y * 0.5 - 100 + a, 4, 0));
    }
  }

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();

      if (event.type == sf::Event::Resized) {
        sf::FloatRect view(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(view));
      }
    }

    window.clear();

    // Performed. Now perform GPU stuff...
    start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < std::size(myParticles); ++i) {
      myParticles[i].update_physics(source);
      myParticles[i].render(window);
    }

    // window.draw, etc.
    end = std::chrono::high_resolution_clock::now();

    fps =
        (float)1e9 /
        (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
            .count();

    // if (fps >= 60 and size(myParticles) <= 15000) {
    //   for (int i = 0; i < 10; ++i) {
    //
    //     c += 25;

    // decompose into a new color
    // color.r = static_cast<sf::Uint8>((c & 0xff0000) >> 16);
    // color.g = static_cast<sf::Uint8>((c & 0xff00) >> 8);
    // color.b = static_cast<sf::Uint8>(c & 0xff);
    //
    // // loop when max (white) is reached
    // if (c > 0x00ffffff) {
    //   c = 0;
    // }
    // myParticles.push_back(Particle(
    //     window.getSize().x * 0.5 + 10 + rand() % (50),
    //     window.getSize().y * 0.5 + 300 + rand() % (200), 2 + rand() % (2), 0,
    //     1 + rand() % (255), 1 + rand() % (255), 1 + rand() % (255)));
    // Particle(window.getSize().x * 0.5 + 10 + rand() % (50),
    //          window.getSize().y * 0.5 + 300 + rand() % (200),
    //          // 2 + rand() % (2), -4 + rand() % (2), color));

    // No Random

    // Particle(window.getSize().x * 0.5 + 10 + i,
    //          window.getSize().y * 0.5 + 300 + 6 * i,
    //          2 + i,-2, color));

    // std::cout << fps;
    // std::cout << "\n Par: ";
    // std::cout << size(myParticles);
    // std::cout << "\n FPS: ";

    source.render(window);

    printNum.setString("Num: " + to_string(size(myParticles)));
    printNum.setCharacterSize(40);
    printNum.setFillColor(sf::Color::White);
    printNum.setFont(font);

    window.draw(printNum);

    window.display();
  }

  return 0;
}
