#pragma once
#include <SFML/Graphics.hpp>
#include "Interface.h"
using namespace sf;

float speed = 1, zoom_speed = 1;
float zoom = 0;
float standart_width = 800;

void camera_zoom(RenderWindow& window, Event &event, View& camera)
{
	if (event.type == Event::MouseWheelScrolled)
	{
		zoom -= zoom_speed * event.mouseWheelScroll.delta / 10;
		if ((camera.getSize().x > 300 && event.mouseWheelScroll.delta > 0) || (camera.getSize().x < 2000 && event.mouseWheelScroll.delta < 0))
		{
			camera.zoom(1 - zoom_speed * event.mouseWheelScroll.delta / 10);
			speed *= 1 - zoom_speed * event.mouseWheelScroll.delta / 10;
		}
		else
		{
			zoom += zoom_speed * event.mouseWheelScroll.delta / 10;;
		}
	}
}

void camera_motion(RenderWindow &window, View &camera, float time)
{
	time /= 1000;
	speed = standart_width / window.getSize().x;
	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		camera.move(0, -speed * time);
	}
	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		camera.move(-speed * time, 0);
	}
	if (Keyboard::isKeyPressed(Keyboard::S))
	{
		camera.move(0, speed * time);
	}
	if (Keyboard::isKeyPressed(Keyboard::D))
	{
		camera.move(speed * time, 0);
	}
	window.setView(camera);
}