#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <chrono>

class SmokePartikles {
    sf::Vector2f pos;
    float intens, size, color;

    sf::RectangleShape s;


    public:
        // Intense 1-100 (Prozent)
        SmokePartikles(float pos_x, float pos_y, float intens) {
            pos.x = pos_x;
            pos.y = pos_y;
            this->intens = intens;

            size = 20;

            s.setPosition(pos);
            s.setSize({size, size});
        }

        void get_color(){
            color = intens * 25.5;
        }

        void render(sf::RenderWindow &wind) {
            s.setPosition(pos);
            s.setFillColor(sf::Color(color, color, color));
            wind.draw(s);
        }

        float get_intens() {return intens;};

        void update_physics(){
            pos.x = pos.x + size;
            pos.y = pos.y + size;
        }
};


int main(int argc, char *argv[]) {
  sf::RenderWindow window(sf::VideoMode(1280, 720), "My Program");
  window.setFramerateLimit(60);

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

    // window.draw, etc.

    window.display();
  }

  return 0;
}
