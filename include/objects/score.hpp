#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <array>
#include <memory>

#include "assets.hpp"
#include "config.hpp"

class Score {
 public:
  static std::unique_ptr<Score> make() noexcept {
    return std::unique_ptr<Score>(new Score);
  }

  void draw(sf::RenderWindow& window) const noexcept {
    window.draw(*m_sprite1);
    if (m_val >= 10) window.draw(*m_sprite10);
  }

  void plus_plus() noexcept {
    m_val = (m_val + 1) % 100;
    m_sprite1->setTexture(*m_textures[m_val % 10]);
    auto bound = m_sprite1->getGlobalBounds();
    if (m_val >= 10) {
      m_sprite10->setTexture(*m_textures[m_val / 10 % 10]);
      m_sprite1->setPosition((Config::width - bound.width * 2) / 2 + bound.width, 30);
    } else {
      m_sprite1->setPosition((Config::width - bound.width) / 2, 30);
    }
  }

 private:
  Score() noexcept:
    m_sprite10(std::make_unique<sf::Sprite>()),
    m_sprite1(std::make_unique<sf::Sprite>()) {
    for (int i = 0; i < 10; ++i)
      m_textures[i] = Assets::instance()->images(std::to_string(i));

    m_val = 0;
    m_sprite10->setTexture(*m_textures[0]);
    m_sprite1->setTexture(*m_textures[0]);
    auto bound = m_sprite10->getGlobalBounds();
    m_sprite10->setPosition((Config::width - bound.width * 2) / 2, 30);
    m_sprite1->setPosition((Config::width - bound.width) / 2, 30);
  }

 private:
  std::array<sf::Texture*, 10> m_textures;
  std::unique_ptr<sf::Sprite> m_sprite10, m_sprite1;
  int m_val;
};