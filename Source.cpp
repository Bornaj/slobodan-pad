#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;
#define pi 3.141592
#define pum 50
#define gustoca_zraka 1.2922 //1295

float input(string t) {
    float in;
    cout << t;
    cin >> in;
    return in;
}

int main() {
    float a, A, r, gustoca, m = 1, visina = 0, t = 0, vrijeme_skoka, eta;
    bool stoji = 0;
    Vector2f velocity;
    const int ygraf = 720;

    r = input("upisi radius u m: ");
    A = r * r * pi;
    a = input("upisi akceleraciju: ");
    gustoca = input("Upisi gustocu fluida (% atmosfere): ")/100.f * gustoca_zraka;
    if (gustoca != 0) {
        m = input("upisi masu tijela: ");
        while (m == 0) m = input("upisi masu tijela (m ne moze biti 0): ");
    }
    velocity.y = input("upisi pocetnu brzinu u y smjeru u m/s: ");
    velocity.x = input("upisi pocetnu brzinu u x smjeru u m/s: ");
    visina = input("upisi pocetnu visinu: ");
    eta = sqrt(input("upisi efikasnost skoka u %: ")/100);

    
    Vertex point;
    VertexArray points, line, graf_points_vy, graf_points_vx, graf_points_v, graf_visina;
    points.setPrimitiveType(LineStrip);
    line.setPrimitiveType(Lines);
    graf_points_vy.setPrimitiveType(LineStrip);
    graf_points_vx.setPrimitiveType(LineStrip);
    graf_points_v.setPrimitiveType(LineStrip);
    graf_visina.setPrimitiveType(LineStrip);
    
    Clock clock;

    View camera;

    RenderWindow window(VideoMode(800, 800), "haha padam!");
    
    Vector2f pozicija = Vector2f(window.getSize().x / 2.f - r * pum, -(visina * pum - window.getSize().y + 2 * r * pum));
    CircleShape tijelo;
    tijelo.setRadius(pum * r);
    tijelo.setPosition(pozicija);

    line.append(Vector2f(0, window.getSize().y));
    line.append(Vector2f(window.getSize().x, window.getSize().y));

    const float delta_t = 0.01;
    const float drag_coefficient = 0.47;

    window.setFramerateLimit(1/delta_t + 3);
    camera.setCenter(tijelo.getPosition().x + r, window.getSize().y / 2);

    int n = 0;

    while (window.isOpen())
    {
        clock.restart();
        n++;
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {
                switch (event.key.code) {
                case Keyboard::Left:
                    camera.move(-10 * pum, 0);
                    break;
                case (Keyboard::Right):
                    camera.move(10 * pum, 0);
                    break;

                case (Keyboard::Up):
                    camera.move(0, -10 * pum);
                    break;
                case (Keyboard::Down):
                    camera.move(0, 10 * pum);
                    break;
                case Keyboard::Escape:
                    window.close();
                    break;
                case Keyboard::R:
                    camera.setCenter(tijelo.getPosition().x + r, window.getSize().y / 2);
                    break;
                }
            }
            if (event.type == Event::MouseWheelMoved) {
                if (event.mouseWheel.delta == -1) camera.zoom(1.5);
                else camera.zoom(1 / 1.5);
            }
        }

        window.setView(camera);

        window.clear();
        window.draw(points);
        window.draw(tijelo);
        window.draw(line);
        window.display();

        // info za graf
        graf_points_vy.append(Vector2f(n, (velocity.y + ygraf / 2)));
        graf_points_vx.append(Vector2f(n, (velocity.x + ygraf / 2)));
        graf_points_v.append(Vector2f(n, (sqrt(velocity.y * velocity.y + velocity.x * velocity.x) + ygraf / 2)));
        graf_visina.append(Vector2f(n, tijelo.getPosition().y + ygraf / 2));

        // brine se da ne propada
        if (tijelo.getPosition().y + 2 * r * pum > window.getSize().y) tijelo.setPosition(tijelo.getPosition().x, window.getSize().y - 2 * r * pum);

        if (tijelo.getPosition().y + 2 * r * pum >= window.getSize().y && !stoji && velocity.y > 0) {
            cout << "tijelo je palo" << endl;
            cout << "zadnja brzina: " << velocity.y << endl;
            cout << "vrijeme: " << n * delta_t << endl;
            vrijeme_skoka = n * delta_t - t;
            cout << "vrijeme skoka: " << vrijeme_skoka << endl;
            t = n * delta_t;
            velocity.y *= -eta;
            if (vrijeme_skoka <= delta_t) {
                cout << "stoji" << endl;
                stoji = 1;
                velocity.y = 0;
                a = 0;
            }
        }
        point.position = Vector2f(tijelo.getPosition().x + r * pum, tijelo.getPosition().y + r * pum);
        points.append(point);
        if (velocity.x > 0) velocity.x = velocity.x - (0.5 * gustoca * A * drag_coefficient * velocity.x * velocity.x * delta_t / m);
        else if (velocity.x < 0) velocity.x = velocity.x + (0.5 * gustoca * A * drag_coefficient * velocity.x * velocity.x * delta_t / m);
        if (velocity.y >= 0) velocity.y = velocity.y + (a - (0.5 * gustoca * A * drag_coefficient * velocity.y * velocity.y * delta_t / m)) * delta_t;
        else velocity.y = velocity.y + (a + (0.5 * gustoca * A * drag_coefficient * velocity.y * velocity.y * delta_t / m)) * delta_t;
        tijelo.move(velocity.x * pum * delta_t, velocity.y * pum * delta_t);
        sleep(seconds(delta_t - clock.getElapsedTime().asSeconds()));
    }
    // graf dio programa
    int xgraf = n + 1;

    if (n + 1 > 1280) xgraf = 1280;
    RenderWindow graf(VideoMode(xgraf, ygraf), "graf pada");
    
    camera = graf.getDefaultView();

    while (graf.isOpen())
    {
        Event event;
        while (graf.pollEvent(event))
        {
            if (event.type == Event::Closed)
                graf.close();
            if (event.type == Event::KeyPressed) {
                switch (event.key.code) {
                case Keyboard::Left:
                    camera.move(-10, 0);
                    break;
                case (Keyboard::Right):
                    camera.move(10, 0);
                    break;

                case (Keyboard::Up):
                    camera.move(0, -10);
                    break;
                case (Keyboard::Down):
                    camera.move(0, 10);
                    break;
                case Keyboard::Escape:
                    graf.close();
                    break;
                }
            }
            if (event.type == Event::MouseWheelMoved) {
                if (event.mouseWheel.delta == -1) camera.zoom(1.5);
                else camera.zoom(1 / 1.5);
            }
        }
        
        VertexArray line;
        line.setPrimitiveType(Lines);
        line.append(Vector2f(0, graf.getSize().y / 2));
        line.append(Vector2f(n + 1, graf.getSize().y / 2));

        graf.setView(camera);
        graf.clear();
        graf.draw(line);
        for (int i = 0; i < graf_points_vy.getVertexCount(); i++) {
            graf_points_vy[i].color = Color::Red;
            graf_points_vx[i].color = Color::Green;
            graf_points_v[i].color = Color::Yellow;
        }
        graf.draw(graf_points_vy);
        graf.draw(graf_points_vx);
        graf.draw(graf_points_v);
        graf.draw(graf_visina);
        graf.display();
    }
    return 0;
}