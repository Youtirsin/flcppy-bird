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

class Bird {
 public:
  static std::unique_ptr<Bird> make() noexcept {
    return std::unique_ptr<Bird>(new Bird);
  }

  sf::Sprite& sprite() const noexcept { return *m_sprite; }

  void update() noexcept {
    m_texture = (m_texture + 1) % 3;
    m_sprite->setTexture(*m_textures[m_texture]);
    m_falldown += Config::gravity;
    auto bound = m_sprite->getGlobalBounds();
    m_sprite->setPosition(bound.left < 50 ? bound.left + 3 : bound.left, bound.top + m_falldown < 0 ? 0 : bound.top + m_falldown);
  }

  void handle_event(const sf::Event& event) noexcept {
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
      m_falldown = -4;
      Assets::instance()->play_audios("wing");
    }
  }

 private:
  Bird() noexcept:
    m_sprite(std::make_unique<sf::Sprite>()) {
    m_textures[0] = Assets::instance()->images("redbird-upflap");
    m_textures[1] = Assets::instance()->images("redbird-midflap");
    m_textures[2] = Assets::instance()->images("redbird-downflap");
    m_texture = 0;
    m_sprite->setTexture(*m_textures[m_texture]);
    m_falldown = 0;
    auto bound = m_sprite->getGlobalBounds();
    m_sprite->setPosition(0, (Config::height - bound.height) / 2);
  }

 private:
  std::array<sf::Texture*, 3> m_textures;
  std::unique_ptr<sf::Sprite> m_sprite;
  int m_texture;
  float m_falldown;
};