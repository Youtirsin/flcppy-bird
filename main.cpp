#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <deque>
#include <iostream>
#include <memory>
#include <thread>

#include "assets.hpp"
#include "config.hpp"
#include "objects/background.hpp"
#include "objects/bird.hpp"
#include "objects/floor.hpp"
#include "objects/game_over_message.hpp"
#include "objects/game_start_message.hpp"
#include "objects/pipe.hpp"
#include "objects/score.hpp"

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "assets directory path must be specified." << std::endl;
    return 1;
  }
  const std::string assets_path = argv[1];

  try {
    Assets::instance()->load_images(assets_path + "/sprites");
    Assets::instance()->load_audios(assets_path + "/audios");
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }

  sf::RenderWindow window(sf::VideoMode(Config::width, Config::height), "flcppy bird");
  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);

  sf::Clock pipe_gen_clock;

  srand(static_cast<unsigned int>(time(0))); // for pipe generation

  // sprites
  std::array<std::unique_ptr<Background>, 2> backgrounds = {Background::make(0), Background::make(1)};
  std::array<std::unique_ptr<Floor>, 2> floors = {Floor::make(0), Floor::make(1)};
  std::deque<std::unique_ptr<Pipe>> pipes;
  pipes.push_back(Pipe::make(*floors[0]));
  std::unique_ptr<Bird> bird;
  std::unique_ptr<GameStartMessage> game_start_message;
  std::unique_ptr<GameOverMessage> game_over_message;
  std::unique_ptr<Score> score;

  bool game_started = false;
  bool game_over = false;

  auto init = [&]() {
    game_over = false;
    game_started = false;
    bird.reset();
    game_over_message.reset();
    score.reset();
    game_start_message = GameStartMessage::make();
    pipes.clear();
  };

  auto start = [&]() {
    game_started = true;
    bird = Bird::make();
    game_start_message.reset();
    score = Score::make();
  };

  auto over = [&]() {
    game_started = false;
    game_over = true;
    game_over_message = GameOverMessage::make();
  };

  auto handle_event = [&](sf::Event& event) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::Resized) { // keep scale when resize window
        window.setSize(sf::Vector2u(event.size.height * Config::width / Config::height, event.size.height));
      } else if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::Space) {
          if (!game_started && !game_over) start();
        } else if (event.key.code == sf::Keyboard::Escape) {
          if (game_over) init();
        }
      }

      if (game_started && bird) bird->handle_event(event);
  };

  auto update = [&]() {
    if (!game_over) {
      if (game_started) {
        // bird
        if (bird) {
          bird->update();
          for (auto&& f : floors) {
            if (f->check_collision(*bird)) {
              Assets::instance()->play_audios("hit");
              over();
            }
          }
          for (auto&& p : pipes) {
            if (p->check_collision(*bird)) {
              Assets::instance()->play_audios("hit");
              over();
            }
            if (p->passed(*bird)) {
              score->plus_plus();
              Assets::instance()->play_audios("point");
            }
          }
        }
        // pipe
        if (pipe_gen_clock.getElapsedTime() > sf::seconds(1.5f)) {
          pipes.push_back(Pipe::make(*floors[0]));

          pipe_gen_clock.restart();
        }
        for (auto&& p : pipes) p->update();
        // remove dead pipes
        while (!pipes.empty() && !pipes.front()->alive()) pipes.pop_front();
      }
      // background
      for (auto&& b : backgrounds) b->update();
      // floor
      for (auto&& f : floors) f->update();
    }
  };


  auto draw = [&]() {
    for (auto&& b : backgrounds) window.draw(b->sprite());
    for (auto&& p : pipes) {
      if (p) p->draw(window);
    }
    for (auto&& f : floors) window.draw(f->sprite());
    if (bird) window.draw(bird->sprite());
    if (game_start_message) window.draw(game_start_message->sprite());
    if (game_over_message) window.draw(game_over_message->sprite());
    if (score) score->draw(window);
  };

  init();

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) handle_event(event);

    update();

    window.clear();
    draw();
    window.display();
  }

  return 0;
}