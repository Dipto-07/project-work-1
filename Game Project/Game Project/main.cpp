#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
using namespace sf;
using namespace std;

// Player er lives & points store korar jonno structure
struct Player {
    int lives = 3;
    int points = 0;
};

// Life komle ball & paddle ke reset korar function
void resetGame(float& x, float& y, float& dx, float& dy, Sprite& sPaddle, Player& player) {
    x = 300;
    y = 300;

    dx = 3.0f;   // HALF SPEED
    dy = 3.0f;   // HALF SPEED

    sPaddle.setPosition(520 / 2 - sPaddle.getGlobalBounds().width / 2, 440);
    player.lives--;
}

int main()
{
    srand(time(0));

    // Image folder path
    string BASE = "D:/Game Folder/Game Project/Game Project/images/";

    RenderWindow app(VideoMode(520, 450), "Arkanoid!");
    app.setFramerateLimit(60);

    // Texture load
    Texture t1, t2, t3, t4;
    t1.loadFromFile(BASE + "block01.png");
    t2.loadFromFile(BASE + "background.jpg");
    t3.loadFromFile(BASE + "ball.png");
    t4.loadFromFile(BASE + "paddle.png");

    // Sprite assign
    Sprite sBackground(t2), sBall(t3), sPaddle(t4);

    sPaddle.setPosition(520 / 2 - sPaddle.getGlobalBounds().width / 2, 440);

    // Blocks create
    Sprite block[1000];
    int n = 0;
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            block[n].setTexture(t1);
            block[n].setPosition(i * 43, j * 20);
            n++;
        }

    Player player;

    // Ball Half Speed
    float dx = 1.5f, dy = 1.5f;
    float x = 300, y = 300;

    RectangleShape pointsBar;
    pointsBar.setSize(sf::Vector2f(player.points, 10));
    pointsBar.setFillColor(Color::Green);
    pointsBar.setPosition(10, 10);

    while (app.isOpen())
    {
        Event e;
        while (app.pollEvent(e))
        {
            if (e.type == Event::Closed)
                app.close();
        }

        // Paddle Movement
        float px = sPaddle.getPosition().x;
        float pWidth = sPaddle.getGlobalBounds().width;

        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            if (px + pWidth < 520)
                sPaddle.move(6, 0);
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            if (px > 0)
                sPaddle.move(-6, 0);
        }

        // Ball movement
        x += dx;
        for (int i = 0; i < n; i++)
            if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
            {
                block[i].setPosition(-100, 0);
                dx = -dx;
                player.points += 5;
            }

        y += dy;
        for (int i = 0; i < n; i++)
            if (FloatRect(x + 3, y + 3, 6, 6).intersects(block[i].getGlobalBounds()))
            {
                block[i].setPosition(-100, 0);
                dy = -dy;
                player.points += 5;
            }

        // Wall Collision
        if (x < 0 || x > 520) dx = -dx;
        if (y < 0) dy = -dy;

        // Life Lost
        if (y > 450) {
            if (player.lives > 1) {
                resetGame(x, y, dx, dy, sPaddle, player);
            }
            else {
                app.close();
            }
        }

        // Paddle Collision
        if (FloatRect(x, y, 12, 12).intersects(sPaddle.getGlobalBounds())) {
            dy = -(rand() % 3 + 1.5f);  // half-speed bounce
        }

        sBall.setPosition(x, y);

        pointsBar.setSize(sf::Vector2f(player.points, 10));

        app.clear();
        app.draw(sBackground);
        app.draw(sBall);
        app.draw(sPaddle);

        for (int i = 0; i < n; i++)
            app.draw(block[i]);

        app.draw(pointsBar);

        app.display();
    }

    return 0;
}
