
// TEAM BEE
// BUZZBOMBER game made in C++ using sfml.
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;
using namespace sf;

// constants defined earlier for different speeds and other values in game
const int WIN_WIDTH = 800, WIN_HEIGHT = 600;
const float PLAYER_SPEED = 2.0f, BULLET_SPEED = 3.0f;
const float WORKER_BEE_SPEED = 0.8f, KILLER_BEE_SPEED = 1.3f;
const int WORKER_BEES[] = { 3, 4, 5 }, KILLER_BEES[] = { 1, 2, 3 };
const int TIER_DROP = 30, MAX_BULLETS = 50, BULLET_REWARD = 25;

// different structures defined for different game components
struct Bullet {
    CircleShape shape;
    Bullet(float x, float y) {
        shape.setRadius(3);
        shape.setFillColor(Color::Red);      //defining graphics of bullet
        shape.setPosition(x, y);
    }
    void move() { shape.move(0, -BULLET_SPEED); }   //move bullets up
};

struct Bee {
    Sprite sprite;
    float speed, direction;
    bool alive = true;
    Bee(float x, float y, float spd, const Texture& tex, float w, float h, float dir)
        : speed(spd), direction(dir) {
        sprite.setTexture(tex);
        sprite.setScale(w / tex.getSize().x, h / tex.getSize().y);
        sprite.setPosition(x, y);
    }
    void move() { sprite.move(speed * direction, 0); }
    void drop() { sprite.move(0, TIER_DROP); }
    bool at_edge() const { return sprite.getPosition().x <= 0 || sprite.getPosition().x >= WIN_WIDTH - sprite.getGlobalBounds().width; }
    bool escaped() const { return sprite.getPosition().y >= WIN_HEIGHT; }
};

// honeycomb struct and function
struct Honeycomb {
    Sprite sprite;
    Clock timer;
    static constexpr float IMMUNE_TIME = 0.2f;

    Honeycomb(float x, float y, const Texture& tex) {
        sprite.setTexture(tex);
        sprite.setScale(0.05f, 0.05f);
        sprite.setPosition(x, y);
    }

    bool immune() const {
        return timer.getElapsedTime().asSeconds() < IMMUNE_TIME;
    }
};

// function to show messages on screen
void show_message(RenderWindow& window, const string& message, const Font& font) {
    Text text(message, font, 50);
    text.setFillColor(Color::White);
    text.setPosition((WIN_WIDTH - text.getGlobalBounds().width) / 2, (WIN_HEIGHT - text.getGlobalBounds().height) / 2);
    window.clear();
    window.draw(text);
    window.display();
    Clock delay;
    while (delay.getElapsedTime().asSeconds() < 2.0f) {}
}

// main function called
int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT), "Buzz Bombers");

    // loading all graphics and audio saved in assets folder
    Texture bgTex, workerTex, killerTex, playerTex, combTex;
    if (!(bgTex.loadFromFile("Assets/VVG.png") &&
        workerTex.loadFromFile("Assets/bee.png") &&
        killerTex.loadFromFile("Assets/Kbee.png") &&
        playerTex.loadFromFile("Assets/spr.png") &&
        combTex.loadFromFile("Assets/comb.png"))) {
        cerr << "Error: Could not load assets." << endl;
        return -1;
    }

    SoundBuffer laserBuffer;
    if (!laserBuffer.loadFromFile("Assets/laser2.ogg")) {
        cerr << "Error: Could not load laser sound." << endl;
        return -1;
    }

    //audio modules for sounds
    Music bgMusic;
    if (!bgMusic.openFromFile("Assets/start.ogg")) {
        cerr << "Error: Could not load background music." << endl;
        return -1;
    }
    bgMusic.setVolume(20);
    bgMusic.setLoop(true);
    bgMusic.play();

    Sound laserSound;
    laserSound.setBuffer(laserBuffer);
    laserSound.setVolume(100.0f);

    //fonts loaded for different text in game
    Font font;
    if (!font.loadFromFile("Assets/arial.ttf")) {
        cerr << "Error: Could not load font." << endl;
        return -1;
    }

    Sprite bgSprite(bgTex);
    bgSprite.setScale(static_cast<float>(WIN_WIDTH) / bgTex.getSize().x, static_cast<float>(WIN_HEIGHT) / bgTex.getSize().y);


    // menu screen set up
    Text title("Buzz Bombers", font, 50), startText("Press ENTER to Start", font, 30), exitText("Press ESC to Exit", font, 30);
    title.setFillColor(Color::White);
    startText.setFillColor(Color::Blue);
    exitText.setFillColor(Color::Red);
    title.setPosition((WIN_WIDTH - title.getGlobalBounds().width) / 2, 150);
    startText.setPosition((WIN_WIDTH - startText.getGlobalBounds().width) / 2, 300);
    exitText.setPosition((WIN_WIDTH - exitText.getGlobalBounds().width) / 2, 360);

    while (window.isOpen()) {
        bool inMenu = true;
        while (inMenu && window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) window.close();
                if (Keyboard::isKeyPressed(Keyboard::Enter)) inMenu = false;
                if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();
            }
            window.clear();
            window.draw(bgSprite);
            window.draw(title);
            window.draw(startText);
            window.draw(exitText);
            window.display();
        }

        // Now, all code below initializes game logics and keeps it updated as game goes on
        Sprite player(playerTex);
        player.setScale(0.1f, 0.1f);
        player.setPosition((WIN_WIDTH - player.getGlobalBounds().width) / 2, WIN_HEIGHT - 80);

        vector<Bullet> bullets;
        vector<Bee> bees;
        vector<Honeycomb> combs;

        Text scoreText("Score: 0", font, 20);
        scoreText.setPosition(5, 5);

        int score = 0, level = 0, bulletsLeft = MAX_BULLETS;
        bool levelStart = true;
        Clock levelTimer;

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) window.close();
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space && bulletsLeft > 0) {
                    bullets.emplace_back(player.getPosition().x + player.getGlobalBounds().width / 2, player.getPosition().y);
                    laserSound.play();
                    bulletsLeft--;
                }
            }

            if (Keyboard::isKeyPressed(Keyboard::Left) && player.getPosition().x > 0) player.move(-PLAYER_SPEED, 0);
            if (Keyboard::isKeyPressed(Keyboard::Right) && player.getPosition().x < WIN_WIDTH - player.getGlobalBounds().width) player.move(PLAYER_SPEED, 0);

            bullets.erase(remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return b.shape.getPosition().y < 0; }), bullets.end());
            for (auto& b : bullets) b.move();

            for (auto& bee : bees) {
                bee.move();
                if (bee.at_edge()) {
                    bee.drop();
                    bee.direction = -bee.direction;
                }
                if (bee.escaped()) {
                    show_message(window, "Game Over! Total Points: " + to_string(score), font);
                    return 0;
                }
            }

            for (auto& bee : bees) {
                for (auto& bullet : bullets) {
                    if (bee.alive && bee.sprite.getGlobalBounds().intersects(bullet.shape.getGlobalBounds())) {
                        bee.alive = false;
                        combs.emplace_back(bee.sprite.getPosition().x + bee.sprite.getGlobalBounds().width / 2 - 7.5f, bee.sprite.getPosition().y + bee.sprite.getGlobalBounds().height / 2 - 7.5f, combTex);
                        score += (bee.speed > WORKER_BEE_SPEED ? 20 : 10);
                    }
                }
            }

            bees.erase(remove_if(bees.begin(), bees.end(), [](const Bee& bee) { return !bee.alive; }), bees.end());

            for (auto& comb : combs) {
                for (auto& bullet : bullets) {
                    if (!comb.immune() && comb.sprite.getGlobalBounds().intersects(bullet.shape.getGlobalBounds())) {
                        comb.sprite.setColor(Color::Transparent);
                    }
                }
            }

            combs.erase(remove_if(combs.begin(), combs.end(), [](const Honeycomb& comb) { return comb.sprite.getColor() == Color::Transparent; }), combs.end());

            if (score / 50 > (score - 1) / 50) bulletsLeft = min(bulletsLeft + BULLET_REWARD, MAX_BULLETS);

            if (bees.empty()) {
                level++;
                if (level > 3) {
                    // add points from any remaining honeycombs
                    for (const auto& comb : combs) {
                        if (comb.sprite.getColor() != Color::Transparent) {
                            score += 50;
                        }
                    }

                    show_message(window, "Game Finished! Total Points: " + to_string(score), font);
                    return 0;
                }

                float dir = 1.0f;
                for (int i = 0; i < WORKER_BEES[level - 1]; ++i) {
                    bees.emplace_back(rand() % (WIN_WIDTH - 30), (i / 5) * TIER_DROP, WORKER_BEE_SPEED, workerTex, 30.0f, 22.0f, dir);
                }
                dir = -1.0f;
                for (int i = 0; i < KILLER_BEES[level - 1]; ++i) {
                    bees.emplace_back(rand() % (WIN_WIDTH - 30), (i / 5) * TIER_DROP, KILLER_BEE_SPEED, killerTex, 32.0f, 22.0f, dir);
                }

                for (const auto& comb : combs) {
                    if (comb.sprite.getColor() != Color::Transparent) score += 50;
                }

                levelStart = true;
                levelTimer.restart();
            }

            if (bulletsLeft <= 0 && !levelStart) {
                show_message(window, "Game Over! Total Points: " + to_string(score), font);
                return 0;
            }

            if (levelStart && levelTimer.getElapsedTime().asSeconds() > 2) levelStart = false;

            window.clear();
            window.draw(bgSprite);
            window.draw(player);

            for (const auto& bullet : bullets) window.draw(bullet.shape);
            for (const auto& bee : bees) if (bee.alive) window.draw(bee.sprite);
            for (const auto& comb : combs) if (comb.sprite.getColor() != Color::Transparent) window.draw(comb.sprite);

            scoreText.setString("Score: " + to_string(score));
            window.draw(scoreText);

            Text bulletText("Bullets: " + to_string(bulletsLeft), font, 20);
            bulletText.setPosition(WIN_WIDTH - 150, 10);
            window.draw(bulletText);

            if (levelStart) {
                Text levelText("Level: " + to_string(level), font, 30);
                levelText.setPosition((WIN_WIDTH - levelText.getGlobalBounds().width) / 2, (WIN_HEIGHT - levelText.getGlobalBounds().height) / 2);
                window.draw(levelText);
            }

            window.display();
        }
    }

    return 0;
}














