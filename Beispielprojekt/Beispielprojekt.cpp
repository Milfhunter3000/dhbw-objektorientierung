#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Planet.h"
#include "Vektor2d.h"

// Simulationsgeschwindigkeit
const double DT = 100.0;

class GameWindow : public Gosu::Window
{
public:
	Gosu::Image bild;
	GameWindow()
		: Window(800, 600)
		, bild("rakete.png")
		// Rakete startet in der Mitte des Bildschirmes
		, pos(graphics().width() / 2.0, graphics().height() / 2.0)
	{
		set_caption("Gosu Tutorial Game mit Git");

		// Erzeuge Planeten
		
	}

	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		bild.draw_rot(pos.get_x(), pos.get_y(), 10.0,
			rot, // Rotationswinkel in Grad
			0.5, 0.5 // Position der "Mitte"
		);

		auto g2 = (gravity * 1000000000000.0).log();

		Vektor2d rose(50.0, 50.0);
		auto g = rose - g2;
		auto s = rose + speed * 1000.0;

		graphics().draw_line(pos.get_x(), pos.get_y(), Gosu::Color::GREEN, input().mouse_x(), input().mouse_y(), Gosu::Color::GREEN, -10.0);
		graphics().draw_line(rose.get_x(), rose.get_y(), Gosu::Color::RED, g.get_x(), g.get_y(), Gosu::Color::RED, 10.0);
		graphics().draw_line(rose.get_x(), rose.get_y(), Gosu::Color::BLUE, s.get_x(), s.get_y(), Gosu::Color::BLUE, 10.0);

		for (auto planet : planets) {
			planet.draw();
		}
	}

	double rot = 0.0;
	Vektor2d pos, speed, gravity;
	double accel = 0.0;
	std::vector<Planet> planets;

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		// Geschwindigkeit f�hrt zu Positions�nderung
		pos += speed * DT;

		// Beschleunigung w�hrend "W" gedr�ckt
		if (input().down(Gosu::KB_W)) {
			accel += 0.00001;
		}
		else {
			accel -= 0.00002;
		}

		// Maximale Beschleunigung
		accel = Gosu::clamp(accel, 0.0, 0.0001);

		// Geschwindigkeit wird in Flugrichtung ge�ndert
		speed += Vektor2d::from_angle(rot, accel);


		gravity = Vektor2d();

		// Planeten ziehen Raumschiff an
		for (auto planet : planets) {

			// Entfernung im Quadrat
			double dist = (pos - planet.pos).length_squared();

			// Winkel des Vektors vom Raumschiff zum Planet
			double angle = pos.angle(planet.pos);

			// Anziehungskraft
			double pull = planet.mass / dist;

			// Kollision
			if (dist < 1000) {
				speed = Vektor2d::from_angle(angle, pull);
				break;
			}
			gravity += Vektor2d::from_angle(angle, pull);
		}

		speed += gravity;

		// Raumschiff in Richtung Mauszeiger drehen
		double angle = pos.angle({ input().mouse_x(), input().mouse_y() });
		rot -= Gosu::angle_diff(angle, rot) / 36.0;
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}