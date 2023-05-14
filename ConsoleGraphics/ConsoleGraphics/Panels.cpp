#include "Panels.h"
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

Panel::Panel(RenderWindow& WINDOW, float X, float Y, float X_SIZE, float Y_SIZE, std::string TEXT_STRING, int FONT_SIZE, bool IS_VISIBLE, bool IS_FIXED, Color OUT_COLOR, Color IN_COLOR)
{
	x = X;
	y = Y;
	x_size = X_SIZE;
	y_size = Y_SIZE;
	text_string = TEXT_STRING;
	font_size = FONT_SIZE;
	visible = IS_VISIBLE;
	fixed = IS_FIXED;
	window = &WINDOW;
	out_color = OUT_COLOR;
	in_color = IN_COLOR;
	out.setSize(Vector2f(x_size, y_size));
	in.setSize(Vector2f(x_size - 6, y_size - 6));
	out.setFillColor(out_color);
	in.setFillColor(in_color);
	out.setPosition(x, y);
	in.setPosition(x + 3, y + 3);
}

Panel::Panel(RenderWindow& WINDOW, std::string& info) : Panel(WINDOW, 0, 0, 0, 0)
{
	x = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	y = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	x_size = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	y_size = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	attach_x = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	attach_y = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	text_position = info.substr(0, info.find("\n"));
	info.erase(0, info.find("\n") + 1);
	text_string = info.substr(0, info.find("\n"));
	info.erase(0, info.find("\n") + 1);
	font_size = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	attached = stoi(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	deletable = stoi(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	visible = stoi(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	fixed = stoi(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	deactivated = stoi(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	deactivatable = stoi(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	copyable = stoi(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	out_color.r = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	out_color.g = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	out_color.b = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	in_color.r = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	in_color.g = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	in_color.b = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
}

std::string Panel::getInfo()
{
	return std::to_string(x) + "\n" + std::to_string(y) + "\n" + std::to_string(x_size) + "\n"
		+ std::to_string(y_size) + "\n" + std::to_string(attach_x) + "\n" + std::to_string(attach_y) + "\n"
		+ text_position + "\n" + text_string + "\n" + std::to_string(font_size) + "\n" + std::to_string(attached) + "\n"
		+ std::to_string(deletable) + "\n" + std::to_string(visible) + "\n" + std::to_string(fixed) + "\n"
		+ std::to_string(deactivated) + "\n" + std::to_string(deactivatable) + "\n" + std::to_string(copyable) + "\n"
		+ std::to_string(out_color.r) + "\n" + std::to_string(out_color.g) + "\n" + std::to_string(out_color.b)
		+ "\n" + std::to_string(in_color.r) + "\n" + std::to_string(in_color.g) + "\n" + std::to_string(in_color.b) + "\n";
}

void Panel::attach(float X, float Y)
{
	attached = true;
	attach_x = X;
	attach_y = Y;
}

bool Panel::pressed(std::string key)
{
	if (key == "left")
	{
		if (Mouse::isButtonPressed(Mouse::Left) && IntRect(x, y, x_size, y_size).contains(Vector2i(window->mapPixelToCoords(Mouse::getPosition(*window)))))
		{
			left = true;
		}
		if (!Mouse::isButtonPressed(Mouse::Left) && left == true)
		{
			left = false;
			if (IntRect(x, y, x_size, y_size).contains(Vector2i(window->mapPixelToCoords(Mouse::getPosition(*window)))))
			{
				return true;
			}
		}
		return false;
	}
	if (key == "right")
	{
		if (IntRect(x, y, x_size, y_size).contains(Vector2i(window->mapPixelToCoords(Mouse::getPosition(*window)))) && !Mouse::isButtonPressed(Mouse::Right))
		{
			mouse_in_panel = true;
			right = false;
		}
		if (!IntRect(x, y, x_size, y_size).contains(Vector2i(window->mapPixelToCoords(Mouse::getPosition(*window)))) && !Mouse::isButtonPressed(Mouse::Right))
		{
			mouse_in_panel = false;
			right = false;
		}
		if (mouse_in_panel)
		{
			if (IntRect(x, y, x_size, y_size).contains(Vector2i(window->mapPixelToCoords(Mouse::getPosition(*window)))) && Mouse::isButtonPressed(Mouse::Right))
			{
				if (right == false)
				{
					mouse_to_panel = Vector2f(x - window->mapPixelToCoords(Mouse::getPosition(*window)).x, y - window->mapPixelToCoords(Mouse::getPosition(*window)).y);
					right = true;
				}
			}
			if (!Mouse::isButtonPressed(Mouse::Right))
			{
				right = false;
			}
			return right;
		}
		else
		{
			return false;
		}
	}
}

void Panel::draw(View camera, Text& text)
{
	text.setRotation(0);
	text.setString(text_string);
	text.setCharacterSize(font_size);
	text.setFillColor(text_color);
	text.setOutlineColor(text_color);
	cx = camera_width / camera.getSize().x;
	cy = camera_height / camera.getSize().y;
	wx = window_width / window->getSize().x;
	wy = window_height / window->getSize().y;
	dposx = camera.getSize().x / window->getSize().x;
	dposy = camera.getSize().y / window->getSize().y;
	if (attached)
	{
		x_size /= cx;
		y_size /= cy;
		x = camera.getCenter().x - camera.getSize().x / 2 + attach_x * dposx;
		y = camera.getCenter().y - camera.getSize().y / 2 + attach_y * dposy;
		text.setScale(dposx, dposy);
	}
	else
	{
		x *= wx;
		y *= wy;
		font_size *= cy;
	}
	x_size *= wx;
	y_size *= wy;
	text_size *= wy / cy;
	out.setPosition(x, y);
	in.setPosition(x + dposx * 3, y + dposy * 3);
	out.setSize(Vector2f(x_size, y_size));
	in.setSize(Vector2f(x_size - 6 * dposx, y_size - 6 * dposy));
	deactivated ? out.setFillColor(Color::Red) : out.setFillColor(out_color);
	in.setFillColor(in_color);
	if (text_position == "center")
	{
		text.setPosition(x + (x_size - text.getGlobalBounds().width) / 2, y + (y_size - font_size * text_size) / 2 - font_size * text_size * 1 / 7);
	}
	if (text_position == "left")
	{
		text.setPosition(x + 10 * 600 / window->getSize().x * wx / cx, y + (y_size - font_size * text_size) / 2 - font_size * text_size * 1 / 7);
	}
	if (text_position == "right")
	{
		text.setPosition(x + x_size - text.getGlobalBounds().width - 10 * dposx, y + (y_size - font_size * text_size) / 2 - font_size * text_size * 1 / 7);
	}
	text_x_size = text.getGlobalBounds().width;
	text_y_size = text.getGlobalBounds().height;
	if (visible)
	{
		window->draw(out);
		window->draw(in);
		if (text_show)
		{
			window->draw(text);
		}
	}
	window_width = window->getSize().x;
	window_height = window->getSize().y;
	camera_width = camera.getSize().x;
	camera_height = camera.getSize().y;
	if (buttons.size() > 0)
	{
		for (int i = 0; i < buttons.size(); i++)
		{
			buttons[i].draw(camera, text);
		}
		if (!buttons[Copy].logic() && !buttons[Deactivate].logic() && !buttons[Delete].logic() && Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
		{
			buttons.clear();
			time_pressed = 9999999;
		}
	}
}

bool Panel::logic(float time)
{
	activated = false;
	copied = false;
	if (deletable || copyable || deactivatable)
	{
		if (pressed("right"))
		{
			time_pressed += time;
			right_pressed_check = true;
		}
		else if (right_pressed_check && time_pressed < 200000)
		{
			int i = 0;
			right_pressed_check = false;
			time_pressed = 0;
			if (buttons.size() == 0)
			{
				mulx /= wx;
				muly /= wy;
				float posx = window->mapPixelToCoords(Mouse::getPosition(*window)).x * mulx / 800 * window->getSize().x;
				float posy = window->mapPixelToCoords(Mouse::getPosition(*window)).y * muly / 600 * window->getSize().y;
				if (copyable)
				{
					buttons.push_back(Button(*window, posx, posy, 100, 30, "Copy"));
					i++;
				}
				else
				{
					buttons.push_back(Button(*window, posx, posy, 100, 30, "Copy", 20, false));
					buttons[buttons.size() - 1].active = false;
				}
				if (deactivatable)
				{
					if(deactivated) buttons.push_back(Button(*window, posx, posy + (buttons[buttons.size() - 1].y_size - 3) * i, 100, 30, "Activate"));
					else buttons.push_back(Button(*window, posx, posy + (buttons[buttons.size() - 1].y_size - 3) * i, 100, 30, "Deactivate"));
					i++;
				}
				else
				{
					buttons.push_back(Button(*window, posx, posy, 100, 30, "Deactivate", 20, false));
					buttons[buttons.size() - 1].active = false;
				}
				if (deletable)
				{
					buttons.push_back(Button(*window, posx, posy + (buttons[buttons.size() - 1].y_size - 3) * i, 100, 30, "Delete"));
					i++;
				}
				else
				{
					buttons.push_back(Button(*window, posx, posy, 100, 30, "Delete", 20, false));
					buttons[buttons.size() - 1].active = false;
				}
			}
		}
		else
		{
			right_pressed_check = false;
			time_pressed = 0;
		}
	}
	if (buttons.size() > 0 && buttons[Delete].pressed())
	{
		deleted = true;
	}
	else if (buttons.size() > 0 && buttons[Deactivate].pressed())
	{
		deactivated = !deactivated;
		deactivated ? buttons[Deactivate].text_string = "Activate" : buttons[Deactivate].text_string = "Deactivate";
		if (!deactivated) activated = true;
	}
	else if (buttons.size() > 0 && buttons[Copy].pressed())
	{
		copied = true;
		buttons.clear();
	}
	if (!fixed)
	{
		if (pressed("right"))
		{
			out.setPosition(window->mapPixelToCoords(Mouse::getPosition(*window)).x, window->mapPixelToCoords(Mouse::getPosition(*window)).y);
			in.setPosition(window->mapPixelToCoords(Mouse::getPosition(*window)).x + 3, window->mapPixelToCoords(Mouse::getPosition(*window)).y + 3);
			out.move(mouse_to_panel);
			in.move(mouse_to_panel);
			x = out.getPosition().x;
			y = out.getPosition().y;
			return true;
		}
	}
	if (IntRect(x, y, x_size, y_size).contains(Vector2i(window->mapPixelToCoords(Mouse::getPosition(*window)))) || 
		(buttons.size() > 0 && (buttons[Copy].logic() || buttons[Deactivate].logic() || buttons[Delete].logic())))
	{
		return true;
	}
	else return false;
}


TextPanel::TextPanel(RenderWindow& WINDOW, float X, float Y, float X_SIZE, float Y_SIZE, std::string TEXT_STRING, int FONT_SIZE, bool IS_VISIBLE, bool IS_FIXED, Color OUT_COLOR, Color IN_COLOR, Color TEXT_COLOR)
	:Panel(WINDOW, X, Y, X_SIZE, Y_SIZE, TEXT_STRING, FONT_SIZE, IS_VISIBLE, IS_FIXED, OUT_COLOR, IN_COLOR)
{
	color = TEXT_COLOR;
	string = text_string;
}

TextPanel::TextPanel(RenderWindow& WINDOW, std::string& info) : Panel(WINDOW, info)
{
	string = info.substr(0, info.find("\n"));
	info.erase(0, info.find("\n") + 1);
	base_string = info.substr(0, info.find("\n"));
	info.erase(0, info.find("\n") + 1);
	type = info.substr(0, info.find("\n"));
	info.erase(0, info.find("\n") + 1);
	color.r = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	color.g = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	color.b = stof(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
}

void TextPanel::change(Event event, Text text)
{
	if (is_changing)
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			is_pressed = true;
		}
		if (!Mouse::isButtonPressed(Mouse::Left) && !Panel::logic() && is_pressed == true)
		{
			text_color = Color::Black;
			is_changing = false;
		}
		if (Keyboard::isKeyPressed(Keyboard::Enter))
		{
			text_color = Color::Black;
			is_changing = false;
		}
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::BackSpace)
		{
			if (string.size() > 0)
			{
				string.erase(string.end() - 1);
				changed = true;
			}
		}
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Dash && type != "letters")
		{
			string += "-";
			changed = true;
		}
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space && type != "digits")
		{
			string += " ";
			changed = true;
		}
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Period)
		{
			string += ".";
			changed = true;
		}
		if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::LShift) && event.key.code == Keyboard::Slash && type != "digits")
		{
			string += "?";
			changed = true;
		}
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Comma && type != "digits")
		{
			string += ",";
			changed = true;
		}
		else if (event.type == Event::KeyPressed && event.key.code >= Keyboard::A && event.key.code <= Keyboard::Num9 && (type == "all" || (type == "digits" && event.key.code >= Keyboard::Num0) || (type == "letters" && event.key.code <= Keyboard::Z)))
		{
			if (Keyboard::isKeyPressed(Keyboard::LShift) && (event.key.code <= Keyboard::Z || event.key.code >= Keyboard::Num0))
			{
				if (event.key.code == Keyboard::Num1 && type != "digits")
				{
					string += "!";
				}
				else
				{
					string += eng_keys_big[event.key.code];
				}
			}
			else
			{
				string += eng_keys_small[event.key.code];
			}
			changed = true;
		}
		if (!Mouse::isButtonPressed(Mouse::Left))
		{
			is_pressed = false;
		}
	}
}

std::string TextPanel::getInfo()
{
	return Panel::getInfo() + string + "\n" + base_string + "\n" + type + "\n" + std::to_string(color.r) + "\n"
		+ std::to_string(color.g) + "\n" + std::to_string(color.b) + "\n";
}

void TextPanel::draw(View camera, Text& text, float time)
{
	changed = false;
	Panel::draw(camera, text);
	if (show_full >= 700000)
	{
		x_size -= full_x;
		x += full_x / 2;
	}
	if (Panel::logic())
	{
		if (show_full < 700000)
		{
			show_full += time;
		}
	}
	else
	{
		show_full = 0;
	}
	if (base_string_drawn)
	{
		if (text_string.length() >= base_string.length())
		{
			text_string.erase(text_string.begin(), text_string.begin() + base_string.length());
		}
		base_string_drawn = false;
	}
	if (text_color == color)
	{
		text_string = string;
	}
	if (text_color == Color::Black)
	{
		text_string = base_string + string;
		base_string_drawn = true;
		text.setString(text_string);
		if (text.getGlobalBounds().width + text.getPosition().x - x > x_size)
		{
			full_x = text.getGlobalBounds().width - x_size + 10;
			while (text.getGlobalBounds().width + text.getPosition().x - x > x_size)
			{
				text_string.erase(text_string.end() - 1);
				text.setString(text_string);
			}
			text_string.erase(text_string.end() - 3, text_string.end());
			text_string += "...";
			text.setString(text_string);
		}
	}
	if (show_full >= 700000)
	{
		x_size += full_x;
		x -= full_x / 2;
		text_string = base_string + string;
		text.setString(text_string);
	}
	if (Panel::buttons.size() > 0)
	{
		for (int i = 0; i < Panel::buttons.size(); i++) Panel::buttons[i].draw(camera, text);
	}
}

bool TextPanel::logic(float time)
{
	if (pressed())
	{
		is_changing = !is_changing;
	}
	if (is_changing)
	{
		text_color = color;
	}
	else
	{
		text_color = Color::Black;
	}
	if (is_changing)
	{
		return true;
	}
	return Panel::logic(time);
}


Button::Button(RenderWindow& WINDOW, float X, float Y, float X_SIZE, float Y_SIZE, std::string TEXT_STRING, int FONT_SIZE, bool IS_VISIBLE, bool IS_FIXED, Color OUT_COLOR, Color IN_COLOR)
	:Panel(WINDOW, X, Y, X_SIZE, Y_SIZE, TEXT_STRING, FONT_SIZE, IS_VISIBLE, IS_FIXED, OUT_COLOR, IN_COLOR) {}

bool Button::pressed()
{
	if (active)
	{
		return(Panel::pressed());
	}
	else return false;
}


SelectionPanel::SelectionPanel(RenderWindow& WINDOW, float X, float Y, float X_SIZE, float Y_SIZE, std::string TEXT_STRING, int FONT_SIZE, bool IS_VISIBLE, bool IS_FIXED, Color OUT_COLOR, Color IN_COLOR)
	:Panel(WINDOW, X, Y, X_SIZE, Y_SIZE, TEXT_STRING, FONT_SIZE, IS_VISIBLE, IS_FIXED, OUT_COLOR, IN_COLOR)
{
	buttons.push_back(Button(*window, x, y, x_size, y_size, text_string, std::min(x_size, y_size) * 0.7));
	buttons[0].active = false;
	text_string = "";
}

void SelectionPanel::addOption(std::string TEXT_STRING)
{
	y_size += y_size / buttons.size();

	buttons.push_back(Button(*window, 0, 0, x_size / 10, x_size / 10));
	buttons[buttons.size() - 1].attach_x = x_size / 10;
	buttons[buttons.size() - 1].attach_y = y_size - (buttons[0].y_size + buttons[buttons.size() - 1].y_size) / 2;

	panels.push_back(Panel(*window, 0, 0, x_size / 10, x_size / 10, TEXT_STRING, x_size / 10));
	panels[panels.size() - 1].attach_x = x_size / 10 * 5;
	panels[panels.size() - 1].attach_y = y_size - (buttons[0].y_size + panels[panels.size() - 1].y_size) / 2;
	panels[panels.size() - 1].out_color = Color::White;
}

void SelectionPanel::draw(View camera, Text& text)
{
	Panel::draw(camera, text);
	for (int i = 0; i < panels.size(); i++)
	{
		panels[i].x = x + panels[i].attach_x * 800 / window->getSize().x;
		panels[i].y = y + panels[i].attach_y * 600 / window->getSize().y;
		panels[i].draw(camera, text);
	}
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i].x = x + buttons[i].attach_x * 800 / window->getSize().x;
		buttons[i].y = y + buttons[i].attach_y * 600 / window->getSize().y;
		buttons[i].draw(camera, text);
	}
	if (Panel::buttons.size() > 0)
	{
		for (int i = 0; i < Panel::buttons.size(); i++) Panel::buttons[i].draw(camera, text);
	}
}

bool SelectionPanel::logic(float time)
{
	for (int i = 0; i < buttons.size(); i++)
	{
		if (buttons[i].pressed())
		{
			if (i == 0)
			{
				ready = true;
			}
			else
			{
				choice = panels[i - 1].text_string;
				buttons[0].active = true;
				buttons[i].in_color = Color::Black;
				buttons[i].out_color = Color::White;
				for (int j = 0; j < buttons.size(); j++)
				{
					if (j != i)
					{
						buttons[j].in_color = Color::White;
						buttons[j].out_color = Color::Black;
					}
				}
			}
		}
	}
	return Panel::logic(time);
}


MultiPanel::MultiPanel(RenderWindow& WINDOW, std::string TYPE, float X, float Y, float X_SIZE, float Y_SIZE, std::string TEXT_STRING, int FONT_SIZE, bool IS_VISIBLE, bool IS_FIXED, Color OUT_COLOR, Color IN_COLOR)
	:Panel(WINDOW, X, Y, X_SIZE, Y_SIZE, TEXT_STRING, FONT_SIZE, IS_VISIBLE, IS_FIXED, OUT_COLOR, IN_COLOR)
{
	type = TYPE;
	text_panels.push_back(TextPanel(*window, 0, 0, x_size, y_size * 4 / 5, text_string, std::min(x_size, y_size) * 0.7));
	text_panels[0].out_color = Color::White;
	text_string = "";
	buttons.push_back(Button(*window, 0, 0, x_size, y_size / 5));
	buttons[0].y_size = text_panels[0].y_size / 4;
	buttons[0].attach_y = text_panels[0].y_size;
}

MultiPanel::MultiPanel(RenderWindow& WINDOW, std::string& info) : Panel(WINDOW, info)
{
	type = info.substr(0, info.find("\n"));
	info.erase(0, info.find("\n") + 1);
	lines = stoi(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	int n = stoi(info.substr(0, info.find("\n")));
	info.erase(0, info.find("\n") + 1);
	for (int i = 0; i < n; i++)
	{
		text_panels.push_back(TextPanel(*window, info));
	}
	buttons.push_back(Button(*window, 0, 0, x_size, y_size / 5));
	buttons[0].y_size = text_panels[0].y_size / 4;
	buttons[0].attach_y = text_panels[0].y_size;
}


void MultiPanel::addVariable(std::vector<std::string> TEXT_STRINGS, std::vector<std::string> DEFAULTS, int PANELS_AMOUNT)
{
	lines ++;
	y_size += text_panels[0].y_size;
	for (int i = 0; i < PANELS_AMOUNT; i++)
	{
		text_panels.push_back(TextPanel(*window, 0, 0, x_size * (0.9 - 0.05 * (PANELS_AMOUNT - 1)) / PANELS_AMOUNT, text_panels[0].y_size / 2, DEFAULTS[std::min(i, (int)(DEFAULTS.size() - 1))], (int)(text_panels[0].y_size * 0.9 / 2)));
		text_panels[text_panels.size() - 1].base_string = TEXT_STRINGS[i];
		text_panels[text_panels.size() - 1].text_position = "left";
		text_panels[text_panels.size() - 1].type = "digits";
		text_panels[text_panels.size() - 1].attach_x = x_size * 0.05 + (text_panels[text_panels.size() - 1].x_size + x_size * 0.05) * i;
		text_panels[text_panels.size() - 1].attach_y = text_panels[0].y_size + buttons[0].y_size + text_panels[0].y_size * 0.25 + text_panels[0].y_size * 0.75 * (lines - 1);
	}
}

std::string MultiPanel::getInfo()
{
	std::string info = Panel::getInfo() + type + "\n" + std::to_string(lines) + "\n" + std::to_string(text_panels.size()) + "\n";
	for (int i = 0; i < text_panels.size(); i++)
	{
		info += text_panels[i].getInfo();
	}
	return info;
}

void MultiPanel::draw(View camera, Text& text)
{
	Panel::draw(camera, text);
	buttons[0].x = x + buttons[0].attach_x * 800 / window->getSize().x;
	buttons[0].y = y + buttons[0].attach_y * 600 / window->getSize().y;
	buttons[0].draw(camera, text);
	if (!opened)
	{
		deactivated ? text_panels[0].out_color = Color::Red : text_panels[0].out_color = Color::White;
		y_size = text_panels[0].y_size * 5 / 4;
		deactivated ? buttons[0].in_color = Color::Red : buttons[0].in_color = Color::Black;
		buttons[0].out_color = Color::White;
		text_panels[0].x = x + text_panels[0].attach_x * 800 / window->getSize().x;
		text_panels[0].y = y + text_panels[0].attach_y * 600 / window->getSize().y;
		text_panels[0].draw(camera, text);
		out_color = Color::White;
	}
	else
	{
		text_panels[0].out_color = Color::White;
		y_size = text_panels[0].y_size * 5 / 4 + text_panels[0].y_size * lines - text_panels[0].y_size * 1 / 4 * (lines - 1);
		buttons[0].in_color = Color::White;
		deactivated ? buttons[0].out_color = Color::Red : buttons[0].out_color = Color::Black;
		out_color = Color::Black;
		for (int i = 0; i < text_panels.size(); i++)
		{
			if (i > 0) deactivated ? text_panels[i].out_color = Color::Red : text_panels[i].out_color = Color::Black;
			text_panels[i].x = x + text_panels[i].attach_x * 800 / window->getSize().x;
			text_panels[i].y = y + text_panels[i].attach_y * 600 / window->getSize().y;
			text_panels[i].draw(camera, text);
		}
	}
	if (Panel::buttons.size() > 0)
	{
		for (int i = 0; i < Panel::buttons.size(); i++) Panel::buttons[i].draw(camera, text);
	}
}

bool MultiPanel::logic(float time)
{
	if (buttons[0].pressed() && Panel::buttons.size() == 0)
	{
		opened = !opened;
	}
	return Panel::logic(time);
}