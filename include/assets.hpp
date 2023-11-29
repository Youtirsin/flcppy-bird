#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/SoundSource.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <filesystem>
#include <forward_list>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>

class Assets {
 public:
  static std::shared_ptr<Assets> instance() noexcept {
    static std::shared_ptr<Assets> assets(new Assets);
    return assets;
  }

  sf::Texture* images(const std::string& name) {
    if (m_images.find(name) == m_images.end()) throw std::runtime_error("image doesnt exist.");
    return m_images[name].get();
  }

  void play_audios(const std::string& name) {
    if (m_audios.find(name) == m_audios.end()) throw std::runtime_error("audio doesnt exist.");
    auto sound = std::make_unique<sf::Sound>(*m_audios[name]);
    sound->play();
    m_sounds.push_front(std::move(sound));
  }

  void load_images(const std::string& directory) {
    namespace fs = std::filesystem;
    if (!fs::exists(directory)) throw std::runtime_error("filepath doesnt exist");

    for (auto&& entry : fs::directory_iterator(directory)) {
      std::string filename = entry.path().filename().string();
      std::string name = filename.substr(0, filename.find('.'));
      m_images[name] = std::make_unique<sf::Texture>();
      if (!m_images[name]->loadFromFile(entry.path().string()))
        throw std::runtime_error("failed to load image from " + entry.path().string());
    }
  }

  void load_audios(const std::string& directory) {
    namespace fs = std::filesystem;
    if (!fs::exists(directory)) throw std::runtime_error("filepath doesnt exist");

    for (auto&& entry : fs::directory_iterator(directory)) {
      std::string filename = entry.path().filename().string();
      std::string name = filename.substr(0, filename.find('.'));
      m_audios[name] = std::make_unique<sf::SoundBuffer>();
      if (!m_audios[name]->loadFromFile(entry.path().string()))
        throw std::runtime_error("failed to load audio from " + entry.path().string());
    }

    // recollect unsed sounds
    std::thread([&]() {
      while (true) {
        m_sounds.remove_if([](const std::unique_ptr<sf::Sound>& sound) {
          return sound->getStatus() == sf::Sound::Stopped;
        });
        std::this_thread::sleep_for(std::chrono::seconds(5));
      }
    }).detach();
  }

 private:
  Assets() = default;

 private:
  std::unordered_map<std::string, std::unique_ptr<sf::Texture>> m_images;
  std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>> m_audios;
  std::forward_list<std::unique_ptr<sf::Sound>> m_sounds;
};