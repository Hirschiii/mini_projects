#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <cstdio>
#include <cstdlib>
#include <time.h>
// #include <SFML/Graphics/Color.hpp>
// #include <SFML/Graphics/RectangleShape.hpp>
// #include <SFML/Graphics/RenderWindow.hpp>
// #include <SFML/System/Vector2.hpp>
#include <chrono>
#include <iostream>
#include <list>
#include <string>
#include <vector>

bool player = true;
const int grid_num = 400;
int dist = 5;
bool grid[grid_num][grid_num];
bool next_grid[grid_num][grid_num];
std::list<int> list_neigh_x = {-1, 0, 1, -1, 1, -1, 0, 1};
std::list<int> list_neigh_y = {-1, -1, -1, 0, 0, 1, 1, 1};

std::array<int, 16> neighbours = {-1, 0,  1,  -1, 1, -1, 0, 1,
                                  -1, -1, -1, 0,  0, 1,  1, 1};

bool randomBool() { return rand() > (RAND_MAX / 2); }

class Grid {
  sf::RectangleShape r;

public:
  Grid() { grid[0][0] = true; };

  void drawPlayer(sf::RenderWindow &window) {
    r.setSize(sf::Vector2f(dist, dist));
    r.setFillColor(sf::Color::White);
    sf::Vector2f start_pos;
    start_pos.x = window.getSize().x / 2.f - ((grid_num * dist) / 2.f);
    start_pos.y = window.getSize().y / 2.f - ((grid_num * dist) / 2.f);

    for (int i = 0; i < grid_num; i++) {
      for (int a = 0; a < grid_num; a++) {
        if (grid[i][a] == true) {
          r.setPosition(start_pos.x + i * dist, start_pos.y + a * dist);
          window.draw(r);
        }
      }
    }
  }

  void drawGridLine(sf::RenderWindow &window, bool justborder) {
    sf::Vector2f start_pos;
    start_pos.x = window.getSize().x / 2.f - ((grid_num * dist) / 2.f);
    start_pos.y = window.getSize().y / 2.f - ((grid_num * dist) / 2.f);

    sf::RectangleShape line(sf::Vector2f((dist * grid_num), 1));

    if (justborder) {
      line.setRotation(90);
      line.setPosition(start_pos.x, start_pos.y);
      window.draw(line);
      line.setPosition(start_pos.x + grid_num * dist, start_pos.y);
      window.draw(line);
      line.setRotation(0);
      line.setPosition(start_pos.x, start_pos.y);
      window.draw(line);
      line.setPosition(start_pos.x, start_pos.y + grid_num * dist);
      window.draw(line);
    } else {

      for (int i = 0; i < grid_num + 1; i++) {
        line.setPosition(start_pos.x + i * dist, start_pos.y);
        line.setRotation(90);
        window.draw(line);
        for (int a = 0; a < grid_num + 1; a++) {
          line.setPosition(start_pos.x, start_pos.y + i * dist);
          line.setRotation(0);
          window.draw(line);
        }
      }
    }
  }

  bool checkRules(int x, int y) {
    int neighbour_x, neighbour_y;
    int neigh_alive = 0;
    bool cell = grid[x][y];

    //   if (x == 0) {
    //     x = 9;
    //   } else {
    //     x = x - 1;
    //   }
    //   if (grid[x][y] == true) {
    //       return true;
    //   } else {
    //       return false;
    //   }

    // Count neighbours alive

    for (int i = 0; i < 8; i++) {
      neighbour_x = x + neighbours[i];
      neighbour_y = y + neighbours[i + 8];
      if (neighbour_y < 0) {
        neighbour_y = grid_num - 1;
      } else if (neighbour_y > grid_num - 1) {
        neighbour_y = 0;
      }
      if (neighbour_x < 0) {
        neighbour_x = grid_num - 1;
      } else if (neighbour_x > grid_num - 1) {
        neighbour_x = 0;
      }
      if (grid[neighbour_x][neighbour_y] == true) {
        neigh_alive++;
      }
    }
    if (neigh_alive >= 2) {
      if (neigh_alive >= 4) {
        return false;
      } else if (neigh_alive == 3 and cell == false) {
        return true;
      } else if (cell == true) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  }

  void nextGen() {
    int new_i, new_x;
    for (int i = 0; i < grid_num; i++) {
      for (int a = 0; a < grid_num; a++) {
        next_grid[i][a] = checkRules(i, a);
      }
    }
    syncGrid();
  }

  void syncGrid() {
    for (int i = 0; i < grid_num; i++) {
      for (int a = 0; a < grid_num; a++) {
        grid[i][a] = next_grid[i][a];
      }
    }
  }
};

int main(int argc, char *argv[]) {

  Grid golGrid;

  for (int i = 0; i < grid_num; i++) {
    for (int a = 0; a < grid_num; a++) {
      grid[i][a] = false;
    }
  }
  grid[1][1] = true;
  grid[2][2] = true;
  grid[3][2] = true;
  grid[3][1] = true;
  grid[3][0] = true;
  //
  // Glider Gun
  for (int i = 0; i < grid_num; i++) {
    for (int a = 0; a < grid_num; a++) {
      grid[i][a] = randomBool();
    }
  }

  sf::RenderWindow window(sf::VideoMode(1280, 720), "GameOfLife");
  window.setFramerateLimit(60);

  float timer = 0, delay = 0.001;

  sf::Clock clock;

  if (window.getSize().x > window.getSize().y) {
    dist = (window.getSize().y - 10) / grid_num;
  } else {
    dist = (window.getSize().x - 10) / grid_num;
  }

  while (window.isOpen()) {

    float time = clock.getElapsedTime().asSeconds();
    clock.restart();
    timer += time;
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        window.close();

      if (event.type == sf::Event::Resized) {
        sf::FloatRect view(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(view));
        if (window.getSize().x > window.getSize().y) {
          dist = (window.getSize().y - 10) / grid_num;
        } else {
          dist = (window.getSize().x - 10) / grid_num;
        }
      }
    }

    ///////Tick//////
    // if (timer > delay and player) {
      std::chrono::high_resolution_clock::time_point start;
      golGrid.nextGen();
      std::chrono::high_resolution_clock::time_point end;
      // timer = 0;
    // }

    window.clear();

    golGrid.drawGridLine(window, true);
    golGrid.drawPlayer(window);
    // window.draw, etc.
    window.display();
  }

  return 0;
}
