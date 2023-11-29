#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include "assets.hpp"
#include "config.hpp"

class GameOverMessage {
 public:
  static std::unique_ptr<GameOverMessage> make() noexcept {
    return std::unique_ptr<GameOverMessage>(new GameOverMessage);
  }

  sf::Sprite& sprite() const noexcept { return *m_sprite; }

 private:
  GameOverMessage() noexcept:
    m_sprite(std::make_unique<sf::Sprite>()) {
    m_sprite->setTexture(*Assets::instance()->images("gameover"));
    auto bound = m_sprite->getGlobalBounds();
    m_sprite->setPosition((Config::width - bound.width) / 2, (Config::height - bound.height) / 2);
  }

 private:
  std::unique_ptr<sf::Sprite> m_sprite;
};