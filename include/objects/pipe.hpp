#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <memory>

#include "assets.hpp"
#include "config.hpp"
#include "objects/floor.hpp"

class Pipe {
 public:
  static std::unique_ptr<Pipe> make(const Floor& floor) noexcept {
    return std::unique_ptr<Pipe>(new Pipe(floor));
  }

  void draw(sf::RenderWindow& window) const noexcept {
    window.draw(*m_sprite_top);
    window.draw(*m_sprite_bottom);
  }

  bool alive() const noexcept { return m_alive; }

  bool passed(const Bird& bird) noexcept {
    auto bound = m_sprite_top->getGlobalBounds();
    if (bound.left < bird.sprite().getGlobalBounds().left - bound.width && !m_passed) {
      m_passed = true;
      return true;
    }
    return false;
  }

  void update() noexcept {
    if (m_alive) {
      auto bound = m_sprite_top->getGlobalBounds();
      m_sprite_top->setPosition(bound.left - 2, bound.top);
      bound = m_sprite_bottom->getGlobalBounds();
      m_sprite_bottom->setPosition(bound.left - 2, bound.top);
      if (bound.left + bound.width < 0) m_alive = false;
    }
  }

  bool check_collision(const Bird& bird) const noexcept {
    return m_sprite_top->getGlobalBounds().intersects(bird.sprite().getGlobalBounds()) ||
           m_sprite_bottom->getGlobalBounds().intersects(bird.sprite().getGlobalBounds());
  }

 private:
  Pipe(const Floor& floor) noexcept:
    m_sprite_top(std::make_unique<sf::Sprite>()),
    m_sprite_bottom(std::make_unique<sf::Sprite>()) {
    m_sprite_top->setTexture(*Assets::instance()->images("pipe-green-top"));
    m_sprite_bottom->setTexture(*Assets::instance()->images("pipe-green-bottom"));

    auto bound = m_sprite_top->getGlobalBounds();
    int max_y = std::min(static_cast<int>(bound.height), static_cast<int>(floor.sprite().getGlobalBounds().top));
    int pos_y = rand() % max_y;

    m_sprite_top->setPosition(Config::width, bound.top - bound.height + pos_y);

    bound = m_sprite_bottom->getGlobalBounds();
    m_sprite_bottom->setPosition(Config::width, bound.top + pos_y + 100);

    m_alive = true;
    m_passed = false;
  }

 private:
  std::unique_ptr<sf::Sprite> m_sprite_top, m_sprite_bottom;
  bool m_alive, m_passed;
};