#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <vector>
#include "Panels.h"
#include "Camera.h"
#include "Raycasting.h"
#include <thread>
#include <fstream>
#include <string>
#include <iostream>
#include "ConsoleScreen.h"
using namespace sf;

std::string openedFile = "";

RenderWindow window(VideoMode(800, 600), "ConsoleGraphics", Style::Default);
View camera;
Clock clock1;

std::vector<SelectionPanel> selection_panels;
std::vector<MultiPanel> multi_panels;
std::vector<MultiPanel> buffer;
std::vector<Button> buttons;
std::vector<int> queue;

Text text;
Font font;

enum panel { selection_panel = 0, multi_panel };
enum button { file = 0, add, run, save, load, ASCII, PNG, paste};

using time_point = std::chrono::system_clock::time_point;

consoleScreen screen;

bool endThread = false, isActive = true, pressed_right = false;
float time_pressed = 0;

std::string OpenFileDialog(CHAR FilePath[MAX_PATH]) {
    OPENFILENAME OpenFileName = { 0 };
    ZeroMemory(&OpenFileName, sizeof(OpenFileName));
    OpenFileName.lStructSize = sizeof(OpenFileName);
    OpenFileName.lpstrFilter = "Binary Files (*.bin)\0*.bin\0";
    OpenFileName.lpstrFile = FilePath;
    OpenFileName.nMaxFile = MAX_PATH;
    OpenFileName.Flags = OFN_EXPLORER | OFN_FORCESHOWHIDDEN;
    if (GetOpenFileName(&OpenFileName)) {
        return FilePath;
    }
    else return "";
}

void showImage(bool& endThread, int imageType)
{
    std::vector<object> objects;
    std::vector<light> lights;
    std::vector<rayCastingCamera> cams;
    try
    {
        int i;
        try
        {
            if (multi_panels.size() == 0) throw exception();
            for (i = 0; i < multi_panels.size(); i++)
            {
                if (!multi_panels[i].deactivated)
                {
                    if (multi_panels[i].type == "Camera")
                    {
                        cams.push_back(rayCastingCamera(stof(multi_panels[i].text_panels[1].string), stof(multi_panels[i].text_panels[2].string),
                            stof(multi_panels[i].text_panels[3].string), stof(multi_panels[i].text_panels[7].string), stof(multi_panels[i].text_panels[8].string),
                            vector3(stof(multi_panels[i].text_panels[4].string), stof(multi_panels[i].text_panels[5].string), stof(multi_panels[i].text_panels[6].string))));
                        cams[cams.size() - 1].pixel_size = stof(multi_panels[i].text_panels[9].string);
                        cams[cams.size() - 1].raysInPixel = stof(multi_panels[i].text_panels[10].string);
                    }
                    else if (multi_panels[i].type == "Sphere")
                    {
                        objects.push_back(object().createSphere(stof(multi_panels[i].text_panels[1].string), stof(multi_panels[i].text_panels[2].string),
                            stof(multi_panels[i].text_panels[3].string), stof(multi_panels[i].text_panels[4].string),
                            Color(stof(multi_panels[i].text_panels[5].string), stof(multi_panels[i].text_panels[6].string),
                                stof(multi_panels[i].text_panels[7].string)), stof(multi_panels[i].text_panels[8].string), stof(multi_panels[i].text_panels[9].string)));
                    }
                    else if (multi_panels[i].type == "Plane")
                    {
                        objects.push_back(object().createPlane(vector3(stof(multi_panels[i].text_panels[4].string), stof(multi_panels[i].text_panels[5].string),
                            stof(multi_panels[i].text_panels[6].string), stof(multi_panels[i].text_panels[1].string), stof(multi_panels[i].text_panels[2].string),
                            stof(multi_panels[i].text_panels[3].string)), Color(stof(multi_panels[i].text_panels[7].string), stof(multi_panels[i].text_panels[8].string),
                                stof(multi_panels[i].text_panels[9].string)), stof(multi_panels[i].text_panels[10].string), stof(multi_panels[i].text_panels[11].string)));
                    }
                    else if (multi_panels[i].type == "Hexahedron")
                    {
                        vector3 boxPoints[8];
                        for (int j = 0; j < 24; j += 3)
                        {
                            boxPoints[j / 3] = vector3(stof(multi_panels[i].text_panels[j + 1].string), stof(multi_panels[i].text_panels[j + 2].string), stof(multi_panels[i].text_panels[j + 3].string));
                        }
                        objects.push_back(object().createBox(boxPoints, Color(stof(multi_panels[i].text_panels[25].string), stof(multi_panels[i].text_panels[26].string),
                            stof(multi_panels[i].text_panels[27].string)), stof(multi_panels[i].text_panels[28].string), stof(multi_panels[i].text_panels[29].string)));
                    }
                    else if (multi_panels[i].type == "Light")
                    {
                        lights.push_back(light(stof(multi_panels[i].text_panels[1].string), stof(multi_panels[i].text_panels[2].string), stof(multi_panels[i].text_panels[3].string),
                            stof(multi_panels[i].text_panels[7].string), Color(stof(multi_panels[i].text_panels[4].string), stof(multi_panels[i].text_panels[5].string),
                                stof(multi_panels[i].text_panels[6].string)), stof(multi_panels[i].text_panels[8].string)));
                    }
                }
            }
        }
        catch (...)
        {
            if(multi_panels.size() > 0) throw std::runtime_error(multi_panels[i].text_panels[0].string + ": " + "wrong input");
            else throw std::runtime_error("No objects found");
        }

        try
        {
            SYSTEM_INFO sysinfo;
            GetSystemInfo(&sysinfo);
            int threads = std::max(1, (int)sysinfo.dwNumberOfProcessors - 2);
            cams.at(0).threads = threads;
            Image image;
            image.create(cams[0].width, cams[0].height, Color::Black);
            std::vector<std::vector<Color>> pixels = cams[0].draw(objects, lights);
            if (pixels.size() == 0)
            {
                isActive = true;
                ShowWindow(GetConsoleWindow(), HIDE_WINDOW);
                buttons[run].text_string = "Run";
                window.setVisible(true);
                return;
            }
            if (imageType == ASCII)
            {
                isActive = true;
                window.setVisible(true);
                screen.SetWindow(cams[0].width, cams[0].height);
                screen.toScreen(screen.getScreenString(pixels), endThread);
            }
            else if (imageType == PNG)
            {
                for (int i = 0; i < pixels.size(); i++)
                {
                    for (int j = 0; j < pixels[i].size(); j++)
                    {
                        image.setPixel(i, j, pixels[i][j]);
                    }
                }
                if (openedFile != "")
                {
                    CreateDirectory(openedFile.substr(0, openedFile.length() - 4).c_str(), NULL);
                    std::string dateTime = std::to_string(time(0));
                    image.saveToFile(openedFile.substr(0, openedFile.length() - 4) + "/" + dateTime + ".png");
                }
                Texture texture;
                texture.loadFromImage(image);
                Sprite sprite(texture);
                RenderWindow imageWindow(VideoMode(cams[0].width, cams[0].height), "RayCasting", Style::Close);
                Event event;
                imageWindow.draw(sprite);
                imageWindow.display();
                while (imageWindow.isOpen())
                {
                    while (imageWindow.pollEvent(event))
                    {
                        if (event.type == Event::Closed)
                        {
                            ShowWindow(GetConsoleWindow(), HIDE_WINDOW);
                            buttons[run].text_string = "Run";
                            isActive = true;
                            imageWindow.close();
                            window.setVisible(true);
                        }
                    }
                }
            }
        }
        catch (...)
        {
            if(cams.size() == 0) throw std::runtime_error("No cameras found");
            else if (objects.size() == 0) throw std::runtime_error("No drawable objects found");
            else throw std::runtime_error("Scene rendering error");
        }
    }
    catch (std::exception& e)
    {
        isActive = true;
        ShowWindow(GetConsoleWindow(), HIDE_WINDOW);
        buttons[run].text_string = "Run";
        window.setVisible(true);
        MessageBox(0, e.what(), "Error", MB_OK);
        return;
    }
}

void create()
{
    font.loadFromFile("Fonts/Roboto-Light.ttf");
    text.setFont(font);
    buttons.push_back(Button(window, 0, 0, 70, 38, "File", 25));
    buttons.push_back(Button(window, 0, 0, 145, 38, "Add object", 25));
    buttons.push_back(Button(window, 0, 0, 70, 38, "Run", 25));
    buttons.push_back(Button(window, 0, 0, 70, 38, "Save", 25, false));
    buttons.push_back(Button(window, 0, 0, 70, 38, "Load", 25, false));
    buttons.push_back(Button(window, 0, 0, 70, 38, "ASCII", 25, false));
    buttons.push_back(Button(window, 0, 0, 70, 38, "PNG", 25, false));
    buttons.push_back(Button(window, 0, 0, 100, 30, "Paste", 20, false));
    buttons[file].attached = true;
    buttons[add].attached = true;
    buttons[run].attached = true;
    buttons[save].attached = true;
    buttons[load].attached = true;
    buttons[ASCII].attached = true;
    buttons[PNG].attached = true;
    buttons[add].attach_x = 67;
    buttons[run].attach_x = 209;
    buttons[save].attach_y = 35;
    buttons[load].attach_y = 70;
    buttons[ASCII].attach_x = 209;
    buttons[PNG].attach_x = 209;
    buttons[ASCII].attach_y = 35;
    buttons[PNG].attach_y = 70;
    buttons[save].active = false;
    buttons[load].active = false;
    buttons[ASCII].active = false;
    buttons[PNG].active = false;
    buttons[paste].active = false;
}

void update(float time)
{
    if (!isActive) return;
    window.clear(Color::White);
    if (buttons[add].pressed())
    {
        queue.push_back(0);
        selection_panels.push_back(SelectionPanel(window, camera.getCenter().x - 800/3, camera.getCenter().y - 600 / 3, 300, 60, "Create", 25, true, false));
        selection_panels[selection_panels.size() - 1].deletable = true;
        selection_panels[selection_panels.size() - 1].addOption("Hexahedron");
        selection_panels[selection_panels.size() - 1].addOption("Sphere");
        selection_panels[selection_panels.size() - 1].addOption("Plane");
        selection_panels[selection_panels.size() - 1].addOption("Light");
        selection_panels[selection_panels.size() - 1].addOption("Camera");
    }
    else if (buttons[file].pressed())
    {
        buttons[save].active = !buttons[save].active;
        buttons[load].active = !buttons[load].active;
        buttons[save].visible = !buttons[save].visible;
        buttons[load].visible = !buttons[load].visible;
    }
    else if (buttons[save].pressed())
    {
        if(openedFile == "") openedFile = "ConsoleGraphics.bin";
        CHAR FilePath[MAX_PATH] = "";
        strcpy_s(FilePath, openedFile.c_str());
        std::string savedFile = OpenFileDialog(FilePath);
        if (savedFile != "")
        {
            try
            {
                int width = window.getSize().x;
                int height = window.getSize().y;
                std::vector<int> opened_panels;
                window.setSize(Vector2u(800, 600));
                camera.reset(FloatRect(0, 0, 800, 600));
                for (int i = 0; i < multi_panels.size(); i++)
                {
                    if (!multi_panels[i].opened)
                    {
                        multi_panels[i].opened = true;
                        opened_panels.push_back(i);
                    }
                }
                update(time);
                std::ofstream out(savedFile);
                out << multi_panels.size() << "\n";
                for (int i = 0; i < multi_panels.size(); i++)
                {
                    out << multi_panels[i].getInfo();
                }
                out.close();
                openedFile = savedFile;
                window.setSize(Vector2u(width, height));
                update(time);
                for (int i = 0; i < opened_panels.size(); i++)
                {
                    multi_panels[opened_panels[i]].opened = false;
                }
            }
            catch (std::exception e) {}
        }
        buttons[save].active = !buttons[save].active;
        buttons[load].active = !buttons[load].active;
        buttons[save].visible = !buttons[save].visible;
        buttons[load].visible = !buttons[load].visible;
    }
    else if (buttons[load].pressed())
    {
        CHAR FilePath[MAX_PATH] = "";
        openedFile = OpenFileDialog(FilePath);
        if (openedFile != "")
        {
            multi_panels.clear();
            queue.clear();
            try
            {
                int width = window.getSize().x;
                int height = window.getSize().y;
                window.setSize(Vector2u(800, 600));
                camera.reset(FloatRect(0, 0, 800, 600));
                std::ifstream in(openedFile);
                std::string input;
                std::string info;
                getline(in, input);
                int n = stoi(input);
                while (getline(in, input))
                {
                    info += input + "\n";
                }
                for (int i = 0; i < n; i++)
                {
                    multi_panels.push_back(MultiPanel(window, info));
                }
                in.close();
                for (int i = 0; i < multi_panels.size(); i++)
                {
                    queue.push_back(multi_panel);
                }
                window.setSize(Vector2u(width, height));
            }
            catch (std::exception e) {}
        }
        buttons[save].active = !buttons[save].active;
        buttons[load].active = !buttons[load].active;
        buttons[save].visible = !buttons[save].visible;
        buttons[load].visible = !buttons[load].visible;
    }
    else if (buttons[run].pressed())
    {
        if (buttons[run].text_string == "Run")
        {
            buttons[ASCII].active = !buttons[ASCII].active;
            buttons[PNG].active = !buttons[PNG].active;
            buttons[ASCII].visible = !buttons[ASCII].visible;
            buttons[PNG].visible = !buttons[PNG].visible;
        }
        else
        {
            endThread = true;
            ShowWindow(GetConsoleWindow(), HIDE_WINDOW);
            buttons[run].text_string = "Run";
        }
    }
    else if (buttons[ASCII].pressed())
    {
        ShowWindow(GetConsoleWindow(), SHOW_OPENWINDOW);
        endThread = false;
        window.setVisible(false);
        isActive = false;
        buttons[run].text_string = "Stop";
        std::thread rayCastingThread(showImage, std::ref(endThread), ASCII);
        rayCastingThread.detach();
        buttons[ASCII].active = !buttons[ASCII].active;
        buttons[PNG].active = !buttons[PNG].active;
        buttons[ASCII].visible = !buttons[ASCII].visible;
        buttons[PNG].visible = !buttons[PNG].visible;
        return;
    }
    else if (buttons[PNG].pressed())
    {
        endThread = false;
        window.setVisible(false);
        isActive = false;
        std::thread rayCastingThread(showImage, std::ref(endThread), PNG);
        rayCastingThread.detach();
        buttons[ASCII].active = !buttons[ASCII].active;
        buttons[PNG].active = !buttons[PNG].active;
        buttons[ASCII].visible = !buttons[ASCII].visible;
        buttons[PNG].visible = !buttons[PNG].visible;
        return;
    }
    else if (buttons[paste].pressed())
    {
        if (buffer.size() > 0)
        {
            queue.push_back(multi_panel);
            buffer[0].x = window.mapPixelToCoords(Mouse::getPosition(window)).x;
            buffer[0].y = window.mapPixelToCoords(Mouse::getPosition(window)).y;
            buffer[0].activated = true;
            buffer[0].copied = false;
            multi_panels.push_back(buffer[0]);
            multi_panels[multi_panels.size() - 1].draw(camera, text);
        }
        buttons[paste].visible = false;
        buttons[paste].active = false;
    }
    if ((Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right)) && !buttons[file].logic() && !buttons[save].logic() && !buttons[load].logic())
    {
        buttons[save].active = false;
        buttons[save].visible = false;
        buttons[load].active = false;
        buttons[load].visible = false;
    }
    if ((Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right)) && !buttons[run].logic() && !buttons[ASCII].logic() && !buttons[PNG].logic())
    {
        buttons[ASCII].active = false;
        buttons[ASCII].visible = false;
        buttons[PNG].active = false;
        buttons[PNG].visible = false;
    }
    for (int i = 0; i < selection_panels.size(); i++)
    {
        if (selection_panels[i].ready)
        {
            queue.push_back(multi_panel);
            multi_panels.push_back(MultiPanel(window, selection_panels[i].choice, selection_panels[i].x, selection_panels[i].y, 300, 60, "Name", 25, true, false));
            multi_panels[multi_panels.size() - 1].deletable = true;
            multi_panels[multi_panels.size() - 1].deactivatable = true;
            multi_panels[multi_panels.size() - 1].copyable = true;
            if (selection_panels[i].choice == "Sphere")
            {
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "X: ", "Y: ", "Z: " }, std::vector<std::string>() = { "0" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "Radius: " }, std::vector<std::string>() = { "20" });
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "R: ", "G: ", "B: " }, std::vector<std::string>() = { "255" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "Specularity: " }, std::vector<std::string>() = { "0" });
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "Reflectivity: " }, std::vector<std::string>() = { "0" });
            }
            else if (selection_panels[i].choice == "Plane")
            {
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "X: ", "Y: ", "Z: " }, std::vector<std::string>() = { "0" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "N X: ", "N Y: ", "N Z: " }, std::vector<std::string>() = { "0" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "R: ", "G: ", "B: " }, std::vector<std::string>() = { "255" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "Specularity: " }, std::vector<std::string>() = { "0" });
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "Reflectivity: " }, std::vector<std::string>() = { "0" });
            }
            else if (selection_panels[i].choice == "Hexahedron")
            {
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "X1: ", "Y1: ", "Z1: " }, std::vector<std::string>() = { "0" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "X2: ", "Y2: ", "Z2: " }, std::vector<std::string>() = { "0" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "X3: ", "Y3: ", "Z3: " }, std::vector<std::string>() = { "0" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "X4: ", "Y4: ", "Z4: " }, std::vector<std::string>() = { "0" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "X5: ", "Y5: ", "Z5: " }, std::vector<std::string>() = { "0" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "X6: ", "Y6: ", "Z6: " }, std::vector<std::string>() = { "0" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "X7: ", "Y7: ", "Z7: " }, std::vector<std::string>() = { "0" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "X8: ", "Y8: ", "Z8: " }, std::vector<std::string>() = { "0" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "R: ", "G: ", "B: " }, std::vector<std::string>() = { "255" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "Specularity: " }, std::vector<std::string>() = { "0" });
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "Reflectivity: " }, std::vector<std::string>() = { "0" });
            }
            else if (selection_panels[i].choice == "Light")
            {
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "X: ", "Y: ", "Z: " }, std::vector<std::string>() = { "0" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "R: ", "G: ", "B: " }, std::vector<std::string>() = { "255" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "Intensity: " }, std::vector<std::string>() = { "0" });
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "Atmosphere: " }, std::vector<std::string>() = { "0" });
            }
            else if (selection_panels[i].choice == "Camera")
            {
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "X: ", "Y: ", "Z: " }, std::vector<std::string>() = { "0" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "Dir X: ", "Dir Y: ", "Dir Z: " }, std::vector<std::string>() = { "0" }, 3);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "Width: ", "Height: " }, std::vector<std::string>() = { "1080", "1080" }, 2);
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "Pixel size: " }, std::vector<std::string>() = { "1" });
                multi_panels[multi_panels.size() - 1].addVariable(std::vector<std::string>() = { "Rays for pixel: " }, std::vector<std::string>() = { "1" });
                multi_panels[multi_panels.size() - 1].activated = true;
            }
            selection_panels.erase(selection_panels.begin() + i);
            queue.erase(queue.end() - 2);
        }
    }

    int i0 = 0, i1 = 0;
    bool logic = true;
    for (int i = 0; i < buttons.size(); i++)
    {
        if (buttons[i].logic() && (i < save || buttons[save].active) && (i < ASCII || buttons[ASCII].active) && (i < paste || buttons[paste].active))
        {
            logic = false;
        }
    }
    for (int i = queue.size() - 1; i >= 0; i--)
    {
        if (queue[i] == selection_panel)
        {
            if (logic)
            {
                if (selection_panels[selection_panels.size() - 1 - i0].logic(time))
                {
                    if (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
                    {
                        queue.push_back(selection_panel);
                        queue.erase(queue.begin() + i);
                        selection_panels.push_back(selection_panels[selection_panels.size() - 1 - i0]);
                        selection_panels.erase(selection_panels.begin() + selection_panels.size() - 2 - i0);
                        i0--;
                        i--;
                    }
                    logic = false;
                }
            }
            i0++;
            continue;
        }
        if (queue[i] == multi_panel)
        {
            if (logic)
            {
                if (multi_panels[multi_panels.size() - 1 - i1].activated && multi_panels[multi_panels.size() - 1 - i1].type == "Camera")
                {
                    for (int j = 0; j < multi_panels.size(); j++)
                    {
                        if(multi_panels[j].type == "Camera") multi_panels[j].deactivated = true;
                    }
                    multi_panels[multi_panels.size() - 1 - i1].deactivated = false;
                }
                if (multi_panels[multi_panels.size() - 1 - i1].copied)
                {
                    buffer.clear();
                    buffer.push_back(multi_panels[multi_panels.size() - 1 - i1]);
                }
                if (multi_panels[multi_panels.size() - 1 - i1].logic(time))
                {
                    if (multi_panels[multi_panels.size() - 1 - i1].Panel::buttons.size() == 0)
                    {
                        for (int j = 0; j < multi_panels[multi_panels.size() - 1 - i1].text_panels.size(); j++)
                        {
                            multi_panels[multi_panels.size() - 1 - i1].text_panels[j].logic();
                        }
                    }
                    if (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
                    {
                        queue.push_back(multi_panel);
                        queue.erase(queue.begin() + i);
                        multi_panels.push_back(multi_panels[multi_panels.size() - 1 - i1]);
                        multi_panels.erase(multi_panels.begin() + multi_panels.size() - 2 - i1);
                        i1--;
                        i--;
                    }
                    logic = false;
                }
            }
            i1++;
        }
    }

    i0 = 0, i1 = 0;
    for (int i = 0; i < queue.size(); i++)
    {
        if (queue[i] == selection_panel)
        {
            if (selection_panels[i0].deleted)
            {
                selection_panels.erase(selection_panels.begin() + i0);
                queue.erase(queue.begin() + i);
                i--;
            }
            else
            {
                selection_panels[i0].draw(camera, text);
                i0++;
            }
        }
        else if (queue[i] == multi_panel)
        {
            if (multi_panels[i1].deleted)
            {
                multi_panels.erase(multi_panels.begin() + i1);
                queue.erase(queue.begin() + i);
                i--;
            }
            else
            {
                multi_panels[i1].draw(camera, text);
                i1++;
            }
        }
    }
    if ((Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right)) && !buttons[paste].logic())
    {
        buttons[paste].visible = false;
        buttons[paste].active = false;
    }
    if (Mouse::isButtonPressed(Mouse::Right) && logic)
    {
        pressed_right = true;
        time_pressed += time;
    }
    else time_pressed = 0;
    if (pressed_right && !Mouse::isButtonPressed(Mouse::Right) && logic && time_pressed < 200000)
    {
        time_pressed = 0;
        pressed_right = false;
        buttons[paste].x = window.mapPixelToCoords(Mouse::getPosition(window)).x;
        buttons[paste].y = window.mapPixelToCoords(Mouse::getPosition(window)).y;
        buttons[paste].visible = true;
        buttons[paste].active = true;
    }
    if(!Mouse::isButtonPressed(Mouse::Right)) pressed_right = false;

    for (int i = 0; i < buttons.size(); i++)
    {
        buttons[i].draw(camera, text);
    }
    window.display();
    if(buffer.size() > 0) buffer[0].draw(camera, text);
}