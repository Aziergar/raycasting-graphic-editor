#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

struct consoleScreen
{
    HANDLE hConsole = GetStdHandle(-12);
    DWORD dwBytesWritten = 0;

    CONSOLE_SCREEN_BUFFER_INFO csbi;

    float columns, rows;
    const float aspect = 11.f / 24.f;
    int width = 30 / aspect;
    int height = 30;

    string gradient = " .:!/r(l1Z4H9W8$@";

    string resize(vector<vector<string>> screen)
    {
        string resizedScreen = "";
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        float sizeMultiplier = min(columns / (screen[0].size() - 1), rows / screen.size());
        for (int l = 0; l < int((rows - screen.size() * sizeMultiplier) / 2); l++)
        {
            resizedScreen += "\n";
        }
        for (int i = 0; i < screen.size() * sizeMultiplier; i++)
        {
            for (int l = 0; l < int((columns - (screen[0].size() - 1) * sizeMultiplier) / 2); l++)
            {
                resizedScreen += " ";
            }
            for (int j = 0; j < (screen[0].size() - 1) * sizeMultiplier; j++)
            {
                resizedScreen += screen[int(i / sizeMultiplier)][int(j / sizeMultiplier)];
            }
            resizedScreen += '\n';
        }
        for (int l = 0; l < int((rows - screen.size() * sizeMultiplier) / 2) - 1; l++)
        {
            resizedScreen += "\n";
        }
        return resizedScreen;
    }

    void SetWindow(int WIDTH, int HEIGHT)
    {
        width = WIDTH / aspect;
        height = HEIGHT;
    }

    vector<vector<string>> getScreenString(vector<vector<sf::Color>> pixels)
    {
        vector<vector<string>> result;
        for (int i = 0; i < height; i++)
        {
            result.push_back(vector<string>());
            for (int j = 0; j < width; j++)
            {
                float r = pixels[j * aspect][i].r;
                float g = pixels[j * aspect][i].g;
                float b = pixels[j * aspect][i].b;
                result[i].push_back(string(1, gradient[(int)(r + g + b) * (gradient.length() - 1) / 765]));
            }
            result[i].push_back("\n");
        }
        return result;
    }

    void toScreen(vector<vector<string>> screen, bool& endThread)
    {
        for (;;)
        {
            if (endThread) return;
            try
            {
                GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
                if (columns != csbi.srWindow.Right - csbi.srWindow.Left + 1 || rows != csbi.srWindow.Bottom - csbi.srWindow.Top + 1)
                {
                    system("cls");
                    cout << resize(screen);
                }
            }
            catch (...) {}
        }
    }
};