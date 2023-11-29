#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <memory>

#include "assets.hpp"

class Background {
 public:
  static std::unique_ptr<Background> make(int idx) noexcept {
    return std::unique_ptr<Background>(new Background(idx));
  }

  sf::Sprite& sprite() const noexcept { return *m_sprite; }

  void update() noexcept {
    auto bound = m_sprite->getGlobalBounds();
    m_sprite->setPosition(bound.left - 1, bound.top);
    if (bound.left + bound.width < 0)
      m_sprite->setPosition(bound.width - 2, bound.top);
  }

 private:
  Background(int idx) noexcept:
    m_sprite(std::make_unique<sf::Sprite>()) {
    m_sprite->setTexture(*Assets::instance()->images("background"));
    auto bound = m_sprite->getGlobalBounds();
    m_sprite->setPosition(bound.left + idx * bound.width, bound.top);
  }

 private:
  std::unique_ptr<sf::Sprite> m_sprite;
};