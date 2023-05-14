#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
using namespace sf;

class Button;

class Panel
{
private:
	enum actions {Copy = 0, Deactivate, Delete};
	float camera_width = 800, camera_height = 600, window_width = 800, window_height = 600, text_size = 1, time_pressed = 0, cx, cy, wx, wy, dposx, dposy, mulx = 1, muly = 1;
	bool mouse_in_panel = false, left = false, right = false, is_moving = false, right_pressed_check = false;
	RectangleShape in, out;
	Vector2f mouse_to_panel;
public:
	Panel(RenderWindow& WINDOW, float X, float Y, float X_SIZE, float Y_SIZE, std::string TEXT_STRING = "", int FONT_SIZE = 20, bool IS_VISIBLE = true, bool IS_FIXED = true, Color OUT_COLOR = Color::Black, Color IN_COLOR = Color::White);
	Panel(RenderWindow& WINDOW, std::string& info);

	RenderWindow* window;
	float x, y, x_size, y_size, font_size, attach_x = 0, attach_y = 0, text_x_size, text_y_size;
	std::string text_string;
	std::string text_position = "center";
	bool visible = true, fixed = true, text_show = true, attached = false, deletable = false, deleted = false, deactivated = false, deactivatable = false, copyable = false, copied = false, activated = false;
	Color out_color, in_color, text_color = Color::Black;
	std::vector<Button> buttons;

	std::string getInfo();
	void attach(float X = 0, float Y = 0);
	bool pressed(std::string key = "left");
	void draw(View camera, Text& text);
	bool logic(float time = 9999999);
};

class TextPanel : public Panel
{
private:
	float show_full = 0, full_x = 0;
	bool base_string_drawn = false, is_pressed = false;
	std::string eng_keys_big[36] = {
		"A" , "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L",
		"M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X",
		"Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	std::string eng_keys_small[36] = {
		"a" , "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l",
		"m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x",
		"y", "z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
public:
	TextPanel(RenderWindow& WINDOW, float X, float Y, float X_SIZE, float Y_SIZE, std::string TEXT_STRING = "", int FONT_SIZE = 20, bool IS_VISIBLE = true, bool IS_FIXED = true, Color OUT_COLOR = Color::Black, Color IN_COLOR = Color::White, Color TEXT_COLOR = Color(0, 100, 255, 255));
	TextPanel(RenderWindow& WINDOW, std::string& info);

	Color color;
	std::string string;
	std::string base_string = "";
	std::string type = "all";
	bool is_changing = false;
	bool changed = false;

	void change(Event event, Text text);
	std::string getInfo();
	void draw(View camera, Text& text, float time = 0);
	bool logic(float time = 9999999);
};

class Button : public Panel
{
public:
	Button(RenderWindow& WINDOW, float X, float Y, float X_SIZE, float Y_SIZE, std::string TEXT_STRING = "", int FONT_SIZE = 20, bool IS_VISIBLE = true, bool IS_FIXED = true, Color OUT_COLOR = Color::Black, Color IN_COLOR = Color::White);

	bool active = true;

	bool pressed();
};

class SelectionPanel : public Panel
{
private:
	std::vector<Button> buttons;
	std::vector<Panel> panels;
public:
	SelectionPanel(RenderWindow& WINDOW, float X, float Y, float X_SIZE, float Y_SIZE, std::string TEXT_STRING = "", int FONT_SIZE = 20, bool IS_VISIBLE = true, bool IS_FIXED = true, Color OUT_COLOR = Color::Black, Color IN_COLOR = Color::White);

	std::string choice = "";
	bool ready = 0;

	void addOption(std::string TEXT_STRING);
	void draw(View camera, Text& text);
	bool logic(float time = 9999999);
};

class MultiPanel : public Panel
{
public:
	MultiPanel(RenderWindow& WINDOW, std::string TYPE, float X, float Y, float X_SIZE, float Y_SIZE, std::string TEXT_STRING = "", int FONT_SIZE = 20, bool IS_VISIBLE = true, bool IS_FIXED = true, Color OUT_COLOR = Color::Black, Color IN_COLOR = Color::White);
	MultiPanel(RenderWindow& WINDOW, std::string& info);

	std::string type;
	std::vector<TextPanel> text_panels;
	std::vector<Button> buttons;
	int lines = 0;
	bool opened = false;

	void addVariable(std::vector<std::string> TEXT_STRINGS, std::vector<std::string> DEFAULTS, int PANELS_AMOUNT = 1);
	std::string getInfo();
	void draw(View camera, Text& text);
	bool logic(float time = 9999999);
};