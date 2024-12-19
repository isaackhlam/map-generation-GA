#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>


class RPGTile {
public:
    //static const int WIDTH = 64;
    //static const int HEIGHT = 64;
    static const int TEXTURE_SIZE = 64;
    static float DISPLAY_SIZE;
    std::string imageFile;

    RPGTile(const std::string& filename) : imageFile(filename) {}

    static RPGTile getTile(char type) {
        switch(type) {
            case '0': return MOUNTAIN;
            case '1': return RIVER;
            case '2': return GRASS;
            case '3': return ROCK;
            case '4': return RIVERROCK;
            default: return EMPTY;
        }
    }

    static const RPGTile MOUNTAIN;
    static const RPGTile RIVER;
    static const RPGTile GRASS;
    static const RPGTile ROCK;
    static const RPGTile RIVERROCK;
    static const RPGTile EMPTY;
};


float RPGTile::DISPLAY_SIZE = 64.0f;
const std::string ASSETDIRECTORY = "assets/";
const RPGTile RPGTile::MOUNTAIN(ASSETDIRECTORY + "mountain.png");
const RPGTile RPGTile::RIVER(ASSETDIRECTORY + "river.png");
const RPGTile RPGTile::GRASS(ASSETDIRECTORY + "grass.png");
const RPGTile RPGTile::ROCK(ASSETDIRECTORY + "rock.png");
const RPGTile RPGTile::RIVERROCK(ASSETDIRECTORY + "riverstone.png");
const RPGTile RPGTile::EMPTY(ASSETDIRECTORY + "empty.png");


class RPGMap {
private:
    std::vector<std::string> map;
    std::map<std::string, sf::Texture> textures;
    std::vector<sf::Sprite> sprites;
    size_t mapWidth = 0;
    size_t mapHeight = 0;
public:
    RPGMap(const std::string& filename) {
        loadMap(filename);
    }

    void loadMap(const std::string& filename) {
        std::ifstream file(filename);
        std::string line;
        while (std::getline(file, line)) {
            map.push_back(line);
            mapWidth = std::max(mapWidth, line.length());
        }
        mapHeight = map.size();
    }

    size_t getMapWidth() const { return mapWidth; }
    size_t getMapHeight() const { return mapHeight; }

    void initialize(const sf::Vector2u& windowSize) {
        calculateTileSize(windowSize);
        loadTextures();
        createSprites();
    }

    void calculateTileSize(const sf::Vector2u& windowSize) {
        float scaleX = static_cast<float>(windowSize.x) / (mapWidth * RPGTile::TEXTURE_SIZE);
        float scaleY = static_cast<float>(windowSize.y) / (mapHeight * RPGTile::TEXTURE_SIZE);
        float scale = std::min(scaleX, scaleY);
        RPGTile::DISPLAY_SIZE = RPGTile::TEXTURE_SIZE * scale;
    }

    void loadTextures() {
        std::vector<RPGTile> tiles = {
            RPGTile::MOUNTAIN, RPGTile::RIVER, RPGTile::GRASS,
            RPGTile::ROCK, RPGTile::RIVERROCK, RPGTile::EMPTY
        };

        for (const auto& tile : tiles) {
            sf::Texture texture;
            if (!texture.loadFromFile(tile.imageFile)) {
                std::cerr << "Failed to load texture: " << tile.imageFile << std::endl;
                continue;
            }
            textures[tile.imageFile] = texture;
        }
    }

    void createSprites() {
        float scale = RPGTile::DISPLAY_SIZE / RPGTile::TEXTURE_SIZE;

        for (size_t y = 0; y < map.size(); ++y) {
            for (size_t x = 0; x < map[y].length(); ++x) {
                RPGTile tile = RPGTile::getTile(map[y][x]);
                sf::Sprite sprite;
                sprite.setTexture(textures[tile.imageFile]);
                sprite.setScale(scale, scale);
                sprite.setPosition(x * RPGTile::DISPLAY_SIZE, y * RPGTile::DISPLAY_SIZE);
                sprites.push_back(sprite);
            }
        }
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& sprite : sprites) {
            window.draw(sprite);
        }
    }
};


int main() {
    RPGMap map("assets/default.map");
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

    // Downscale the tile to 16, if keep 64, a 1080p monitor can only display 16 tiles...
    const float TARGET_TILE_SIZE = 16.0f;
    unsigned int desiredWidth = map.getMapWidth() * TARGET_TILE_SIZE;
    unsigned int desiredHeight = map.getMapHeight() * TARGET_TILE_SIZE;
    desiredWidth = std::min(desiredWidth, static_cast<unsigned int>(desktop.width * 0.9f));
    desiredHeight = std::min(desiredHeight, static_cast<unsigned int>(desktop.height * 0.9f));

    sf::RenderWindow window(
        sf::VideoMode(desiredWidth, desiredHeight),
        "RPG Map",
        sf::Style::Default
    );

    map.initialize(window.getSize());

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        map.draw(window);
        window.display();
    }

    return 0;
}
