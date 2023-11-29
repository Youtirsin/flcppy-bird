#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <memory>

#include "assets.hpp"
#include "bird.hpp"
#include "config.hpp"

class Floor {
 public:
  static std::unique_ptr<Floor> make(int idx) noexcept {
    return std::unique_ptr<Floor>(new Floor(idx));
  }

  sf::Sprite& sprite() const noexcept { return *m_sprite; }

  void update() noexcept {
    auto bound = m_sprite->getGlobalBounds();
    m_sprite->setPosition(bound.left - 1, bound.top);
    if (bound.left + bound.width < 0)
      m_sprite->setPosition(bound.width - 2, bound.top);
  }

  bool check_collision(const Bird& bird) const noexcept {
    return m_sprite->getGlobalBounds().intersects(bird.sprite().getGlobalBounds());
  }

 private:
  Floor(int idx) noexcept:
    m_sprite(std::make_unique<sf::Sprite>()) {
    m_sprite->setTexture(*Assets::instance()->images("floor"));
    auto bound = m_sprite->getGlobalBounds();
    m_sprite->setPosition(bound.left + idx * bound.width, Config::height - bound.height);
  }

 private:
  std::unique_ptr<sf::Sprite> m_sprite;
};