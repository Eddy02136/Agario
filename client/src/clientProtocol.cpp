
#include <components/Shape.hpp>
#include <components/Color.hpp>
#include <components/Position.hpp>
#include <components/View.hpp>
#include <components/Text.hpp>
#include <components/Link.hpp>
#include <System.hpp>
#include <clientProtocol.hpp>

Protocol::Protocol() {}

Protocol& Protocol::get() {
    static Protocol instance;
    return instance;
}

std::map<int, GameEngine::Entity> Protocol::getEntities() const {
    return _entities;
}

void Protocol::createPlayerCallback(SmartBuffer &SmartBuffer) {
    uint16_t id, x, y, size, textSize;
    std::string name = "";
    SmartBuffer >> id >> name >> x >> y >> size >> textSize;
    sf::View view = sf::View(sf::FloatRect(0, 0, 1280, 720));
    std::pair<float, float> pos = {x, y};
    _entities[id] = GameEngine::Entity(id, Shape(Circle, {0, 0}, size), Color({133, 6, 6, 255}), Position({{pos.first, pos.second}}), View(view, {1280, 720}));
    _entities[id + 1] = GameEngine::Entity(id + 1, Text(name, "font/Inter_Bold.ttf", 30), Position({{pos.first, pos.second}}), Link(id));
//    _entities[1000] = GameEngine::Entity(1000, Text("Score: 0", "font/Inter_Bold.ttf", 30), Position({{pos.first, pos.second}}), Link(id));

}

void Protocol::createPlayerBroadcast(SmartBuffer &SmartBuffer) {
    uint16_t id, x, y, size, textSize;
    std::string name = "";
    SmartBuffer >> id >> name >> x >> y >> size >> textSize;
    std::pair<float, float> pos = {x, y};
    _entities[id] = GameEngine::Entity(id, Shape(Circle, {0, 0}, size), Color({133, 6, 6, 255}), Position({{pos.first, pos.second}}));
    _entities[id + 1] = GameEngine::Entity(id + 1, Text(name, "font/Inter_Bold.ttf", 30), Position({{pos.first, pos.second}}), Link(id));
    
}

void Protocol::updatePosition(SmartBuffer &smartBuffer) {
    uint16_t id;
    float x, y;
    smartBuffer >> id >> x >> y;
    std::pair<float, float> pos = {x, y};
    GameEngine::System system;
    system.update(id, _entities, GameEngine::UpdateType::Position, pos); 
    //system.update(id + 1, _entities, GameEngine::UpdateType::Position, pos, 0);
}

void Protocol::createMap(SmartBuffer &smartBuffer) {
    uint16_t id;
    smartBuffer >> id;
    _entities[id] = GameEngine::Entity(id, Shape(Circle, {0, 0}, 5), Color({173, 216, 230, 255}));
}

void Protocol::addFood(SmartBuffer &smartBuffer) {
    uint16_t mapid, foodid, x, y;
    smartBuffer >> mapid >> x >> y;
    if (_entities.find(mapid) != _entities.end()) {
        if (!_entities[mapid].hasComponent<Position>()) {
            _entities[mapid].addComponent(Position({{x, y}}));
        } else {
            _entities[mapid].getComponent<Position>().addPosition(x, y);
        }
    }
}

void Protocol::eatFood(SmartBuffer &smartBuffer) {
    GameEngine::System system;
    uint16_t foodId, mapId, x, y, clientId;
    float size;
    unsigned int textSize;
    uint16_t score;
    smartBuffer >> foodId >> mapId >> x >> y >> clientId >> size >> textSize >> score;
    if (_entities.find(mapId) != _entities.end()) {
        std::pair<float, float> pos = {x, y};
        _entities[mapId].getComponent<Position>().removePosition(pos);
        system.update(clientId, _entities, GameEngine::UpdateType::CircleRadius, size);
        /*if (textSize > 0) {
            system.update(clientId + 1, _entities, GameEngine::UpdateType::TextSize, textSize);
        }*/
        if (_entities[clientId].hasComponent<View>()) {
            auto &viewComp = _entities[clientId].getComponent<View>();
            std::pair<float, float> viewSize = viewComp.getSize();
            float playerSize = size;
            const std::pair<float, float> V0 = {1280.0f, 720.0f};
            const float S0 = 30.0f;
            const float alpha = 0.6f;
            std::pair<float, float> newSize = {
                V0.first * std::pow(playerSize / S0, alpha),
                V0.second * std::pow(playerSize / S0, alpha)
            };
            system.update(clientId, _entities, GameEngine::UpdateType::View, newSize);
            std::pair<float, float> newScorePos = {
              pos.first + 10.0f,
              pos.second + 10.0f
            };
            //system.update(1000, _entities, GameEngine::UpdateType::Position, newScorePos, 0);                                
            //system.update(1000, _entities, GameEngine::UpdateType::Text, "Score: " + std::to_string(score));
        }
    }
}

Protocol::~Protocol() {}