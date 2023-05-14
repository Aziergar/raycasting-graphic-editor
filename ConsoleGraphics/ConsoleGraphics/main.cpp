#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <vector>
#include "Panels.h"
#include "Camera.h"
#include "Interface.h"
#include "Vectors.h"
#include "ConsoleScreen.h"

using namespace sf;

bool is_changing = true;

int main()
{
    HWND ConsoleWnd = GetConsoleWindow();
    HMENU hm = GetSystemMenu(ConsoleWnd, FALSE);
    DeleteMenu(hm, SC_CLOSE, MF_BYCOMMAND);
    DrawMenuBar(ConsoleWnd);
    ShowWindow(GetConsoleWindow(), HIDE_WINDOW);
    camera.reset(FloatRect(0, 0, 800, 600));
    create();
    while (window.isOpen())
    {
        Event event;
        float time = clock1.getElapsedTime().asMicroseconds();
        clock1.restart();
        while (window.pollEvent(event))
        {
            is_changing = false;
            for (int i = 0; i < multi_panels.size(); i++)
            {
                if (multi_panels[i].Panel::buttons.size() == 0)
                {
                    for (int j = 0; j < multi_panels[i].text_panels.size(); j++)
                    {
                        multi_panels[i].text_panels[j].change(event, text);
                        is_changing += multi_panels[i].text_panels[j].is_changing;
                    }
                }
            }
            if (!is_changing)
            {
                camera_zoom(window, event, camera);
            }
            if (event.type == Event::Closed)
            {
                ShowWindow(GetConsoleWindow(), HIDE_WINDOW);
                window.close();
            }
        }
        if (!is_changing)
        {
            camera_motion(window, camera, time);
        }
        update(time);
    }
	return 0;
}