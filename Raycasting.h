#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <chrono>
#include "Vectors.h"
#include <string>
#include <future>
#include <mutex>
#include <Windows.h>
using namespace sf;

#define PI 3.14159265
std::mutex m;

struct object
{
	double x, y, z, radius, specularity, reflectivity;
	vector3 planeNormal;
	vector3 points[8], normals[6];
	int pointsForNormals[6][4] = { {0, 1, 2, 3} , {1, 2, 6, 5}, {3, 2, 6, 7} , {0, 3, 7, 4} , {0, 1, 5, 4} , {4, 5, 6, 7} };
	int boxPlane;
	std::string type;
	Color color;

	object createSphere(float X, float Y, float Z, float RADIUS, Color COLOR = Color::White, float SPECULARITY = 0, float REFLECTIVITY = 0)
	{
		object obj;
		obj.type = "sphere";
		obj.x = X;
		obj.y = Y;
		obj.z = Z;
		obj.radius = RADIUS;
		obj.color = COLOR;
		obj.specularity = SPECULARITY;
		obj.reflectivity = std::max(std::min(REFLECTIVITY, 1.f), 0.f);
		return obj;
	}

	object createPlane(vector3 PLANENORMAL, Color COLOR = Color::White, float SPECULARITY = 0, float REFLECTIVITY = 0)
	{
		object obj;
		obj.type = "plane";
		obj.planeNormal = PLANENORMAL;
		obj.planeNormal.normalize();
		obj.color = COLOR;
		obj.specularity = SPECULARITY;
		obj.reflectivity = std::max(std::min(REFLECTIVITY, 1.f), 0.f);
		return obj;
	}

	object createBox(vector3 POINTS[8], Color COLOR = Color::White, float SPECULARITY = 0, float REFLECTIVITY = 0)
	{
		object obj;
		obj.type = "box";
		for (int i = 0; i < 8; i++) obj.points[i] = POINTS[i];
		for (int i = 0; i < 6; i++) obj.normals[i] = getPlaneNormal(obj.points[pointsForNormals[i][0]], obj.points[pointsForNormals[i][1]], obj.points[pointsForNormals[i][2]]);
		obj.color = COLOR;
		obj.specularity = SPECULARITY;
		obj.reflectivity = std::max(std::min(REFLECTIVITY, 1.f), 0.f);
		return obj;
	}

	vector3 getNormal(vector3 ray, double t)
	{
		if (type == "sphere")
		{
			vector3 normal(ray.x * t - (x - ray.x0), ray.y * t - (y - ray.y0), ray.z * t - (z - ray.z0));
			normal.normalize();
			return normal;
		}
		if (type == "plane")
		{
			if (dot(planeNormal, ray) > 0) planeNormal = -planeNormal;
			return planeNormal;
		}
		if (type == "box")
		{
			intersects(ray, t);
			if (dot(normals[boxPlane], ray) > 0) normals[boxPlane] = -normals[boxPlane];
			return normals[boxPlane];
		}
	}

	vector3 reflect(vector3 ray, float t)
	{
		vector3 reflectedRay;
		vector3 normal = getNormal(ray, t);
		reflectedRay = ray + normal * cos_angle(ray, -normal) * 2;
		reflectedRay.normalize();
		reflectedRay.x0 = ray.x0 + ray.x * t + reflectedRay.x * 0.01;
		reflectedRay.y0 = ray.y0 + ray.y * t + reflectedRay.y * 0.01;
		reflectedRay.z0 = ray.z0 + ray.z * t + reflectedRay.z * 0.01;
		return reflectedRay;
	}

	bool planeIntersects(vector3 direction, vector3 planeNormal, double& result)
	{
		double x0 = direction.x0, y0 = direction.y0, z0 = direction.z0;
		vector3 dotToPlane = vector3(planeNormal.x0 - x0, planeNormal.y0 - y0, planeNormal.z0 - z0);
		if (dot(planeNormal, dotToPlane) < 0) planeNormal = -planeNormal;
		if (dot(direction, planeNormal) > 0)
		{
			result = (dot(planeNormal, planeNormal.start()) - dot(planeNormal, direction.start())) / dot(planeNormal, direction);
			return true;
		}
		return false;
	}

	bool intersects(vector3 direction, double& result)
	{
		double x0 = direction.x0, y0 = direction.y0, z0 = direction.z0;
		if (type == "sphere")
		{
			double b = direction.x * (x - x0) + direction.y * (y - y0) + direction.z * (z - z0);
			double c = (x - x0) * (x - x0) + (y - y0) * (y - y0) + (z - z0) * (z - z0) - radius * radius;
			double discriminant = b * b - c;
			if (discriminant >= 0)
			{
				double point1 = b + sqrtf(discriminant);
				double point2 = b - sqrtf(discriminant);
				double minpoint = std::min(point1, point2);
				double maxpoint = std::max(point1, point2);
				if (minpoint >= 0)
				{
					result = minpoint;
					return true;
				}
				else if (maxpoint >= 0)
				{
					result = maxpoint;
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		if (type == "plane")
		{
			return planeIntersects(direction, planeNormal, result);
		}
		if (type == "box")
		{
			double d = -1;
			for (int i = 0; i < 6; i++)
			{
				if (planeIntersects(direction, normals[i], result))
				{
					vector3 point = direction * result;
					vector3 v[4];
					for (int j = 0; j < 4; j++)
					{
						v[j] = points[pointsForNormals[i][j]];
						v[j].x -= point.x + point.x0;
						v[j].y -= point.y + point.y0;
						v[j].z -= point.z + point.z0;
					}
					double angle = 0;
					for (int j = 0; j < 4; j++)
					{
						int next = j + 1;
						if (next == 4) next = 0;
						angle += acos(cos_angle(v[j], v[next]));
					}
					if (abs(angle - 2 * PI) > 0.0001) continue;
					if (d == -1 || result < d)
					{
						d = result;
						boxPlane = i;
					}
				}
			}
			if (d != -1)
			{
				result = d;
				return true;
			}
			return false;
		}
	}
};

struct light
{
	float x, y, z, intensity, atmosphere;
	Color color;

	light(float X, float Y, float Z, float INTENSITY, Color COLOR = Color::White, float ATMOSPHERE = 0)
		: x(X), y(Y), z(Z), intensity(INTENSITY), color(COLOR), atmosphere(ATMOSPHERE) {}

	Color dotColor(vector3 ray, float t, vector3 normal, Color objectColor, float reflectivity, float specularity)
	{
		ray = vector3(x - ray.x0, y - ray.y0, z - ray.z0) - ray * t;
		Color diffuseColor(0, 0, 0);
		Color specularColor(0, 0, 0);

		diffuseColor.r = float(objectColor.r * color.r) / 255;
		diffuseColor.g = float(objectColor.g * color.g) / 255;
		diffuseColor.b = float(objectColor.b * color.b) / 255;
		float brightness = intensity * cos_angle(ray, normal) / pow(ray.size(), atmosphere);
		brightness = std::max(brightness * (1 - specularity), 0.f);
		diffuseColor.r = std::min(diffuseColor.r * brightness * (1 - reflectivity), 255.f);
		diffuseColor.g = std::min(diffuseColor.g * brightness * (1 - reflectivity), 255.f);
		diffuseColor.b = std::min(diffuseColor.b * brightness * (1 - reflectivity), 255.f);

		if (specularity > 0)
		{
			specularColor.r = float(objectColor.r * color.r) / 255;
			specularColor.g = float(objectColor.g * color.g) / 255;
			specularColor.b = float(objectColor.b * color.b) / 255;
			float brightness = intensity * pow(cos_angle(ray, normal), 32) / pow(ray.size(), atmosphere);
			brightness = std::max(brightness * specularity, 0.f);
			specularColor.r = std::min(specularColor.r * brightness, 255.f);
			specularColor.g = std::min(specularColor.g * brightness, 255.f);
			specularColor.b = std::min(specularColor.b * brightness, 255.f);
		}

		return diffuseColor + specularColor;
	}
};

struct rayCastingCamera
{
	int threads = 1;
	float x, y, z, width, height;
	int pixel_size = 1, raysInPixel = 1;
	bool end = false;
	vector3 direction, cameraRight, cameraUp;
	vector3 ray;
	std::vector<float> objects_t;
	rayCastingCamera(float X, float Y, float Z, float WIDTH, float HEIGHT, vector3 DIRECTION) : x(X), y(Y), z(Z), width(WIDTH), height(HEIGHT), direction(DIRECTION) {}

	void applyDirection()
	{
		direction.normalize();
		cameraUp = vector3(0, -1, 0);
		cameraRight = direction.cross(cameraUp);
		cameraUp = cameraRight.cross(direction);
		if (cameraRight.isNull())
		{
			cameraRight = vector3(-1, 0, 0);
			cameraUp = direction.cross(cameraRight);
			cameraRight = cameraUp.cross(direction);
		}
	}

	void setRay(float i, float j)
	{
		float size = std::max(height, width);
		i = i / size - 0.5 * height / size;
		j = j / size - 0.5 * width / size;
		ray.x = j * cameraRight.x + i * cameraUp.x + direction.x;
		ray.y = j * cameraRight.y + i * cameraUp.y + direction.y;
		ray.z = j * cameraRight.z + i * cameraUp.z + direction.z;
		ray.x0 = x;
		ray.y0 = y;
		ray.z0 = z;
		ray.normalize();
	}

	void fill(std::vector<float>& v, float value, int size)
	{
		if (v.size() < size)
		{
			v.clear();
			for (int i = 0; i < size; i++)
			{
				v.push_back(value);
			}
		}
	}

	int find_l_min(std::vector<float>& t_v, std::vector<object> obj_v, vector3 ray)
	{
		int l_min = 0;
		double t = -1;
		for (int i = 0; i < obj_v.size(); i++)
		{
			t_v.at(i) = -1;
			if (obj_v.at(i).intersects(ray, t))
			{
				if (t_v[l_min] == -1 || t < t_v[l_min])
				{
					l_min = i;
				}
				t_v[i] = t;
			}
		}
		return l_min;
	}

	void drawPixel(vector3 ray, Color& dotColor, std::vector<object> objects, std::vector<light> lights, std::vector<float> objects_t, int reflections = 0)
	{
		int l_min = find_l_min(objects_t, objects, ray);
		if (objects_t.at(l_min) != -1)
		{
			if (objects.at(l_min).reflectivity > 0 && reflections < 10)
			{
				reflections++;
				Color mirrorColor = objects[l_min].color;
				drawPixel(objects[l_min].reflect(ray, objects_t[l_min]), dotColor, objects, lights, objects_t, reflections);
				dotColor.r = dotColor.r * objects[l_min].reflectivity + float(mirrorColor.r) / 255;
				dotColor.g = dotColor.g * objects[l_min].reflectivity + float(mirrorColor.g) / 255;
				dotColor.b = dotColor.b * objects[l_min].reflectivity + float(mirrorColor.b) / 255;
			}
			for (int l = 0; l < lights.size(); l++)
			{
				std::vector<float> objects_t2 = objects_t;
				float dotX = ray.x * objects_t[l_min] + ray.x0;
				float dotY = ray.y * objects_t[l_min] + ray.y0;
				float dotZ = ray.z * objects_t[l_min] + ray.z0;
				vector3 dotToLight(dotX - lights[l].x, dotY - lights[l].y, dotZ - lights[l].z);
				dotToLight.normalize();
				dotToLight.x0 = lights[l].x;
				dotToLight.y0 = lights[l].y;
				dotToLight.z0 = lights[l].z;
				int l_min2 = find_l_min(objects_t2, objects, dotToLight);
				if (objects_t2[l_min2] != -1 && l_min2 != l_min)
				{
					continue;
				}
				vector3 normal = objects[l_min].getNormal(ray, objects_t[l_min]);
				Color lightedDotColor = lights[l].dotColor(ray, objects_t[l_min], normal, objects[l_min].color, objects[l_min].reflectivity, objects[l_min].specularity);
				dotColor.r = std::min(dotColor.r + lightedDotColor.r, 255);
				dotColor.g = std::min(dotColor.g + lightedDotColor.g, 255);
				dotColor.b = std::min(dotColor.b + lightedDotColor.b, 255);
			}
		}
	}

	void drawAsync(std::vector<object> objects, std::vector<light> lights, std::vector<std::vector<int>>& minMax, int index, std::vector<std::vector<Color>>& result, double& percentage, bool& end)
	{
		m.lock();
		int iMin = minMax[index][0];
		int iMax = minMax[index][1];
		minMax[index][0] = -1;
		m.unlock();
		fill(objects_t, -1, objects.size());
		applyDirection();
		while (true)
		{
			for (float i = iMin; i < iMax; i += pixel_size)
			{
				for (float j = 0; j < width; j += pixel_size)
				{
					if (end) return;
					Color dotColor;
					float avgR = 0, avgG = 0, avgB = 0;

					for (float i1 = 0; i1 < raysInPixel; i1++)
					{
						for (float j1 = 0; j1 < raysInPixel; j1++)
						{
							if (end) return;
							dotColor = Color(0, 0, 0);
							setRay(i + i1 * float(pixel_size) / raysInPixel, j + j1 * float(pixel_size) / raysInPixel);
							drawPixel(ray, dotColor, objects, lights, objects_t);
							avgR += float(dotColor.r) / pow(raysInPixel, 2);
							avgG += float(dotColor.g) / pow(raysInPixel, 2);
							avgB += float(dotColor.b) / pow(raysInPixel, 2);
						}
					}
					m.lock();
					for (int l = 0; l < pixel_size; l++)
					{
						for (int k = 0; k < pixel_size; k++)
						{
							if (end)
							{
								m.unlock();
								return;
							}
							result[j + l][i + k] = Color(avgR, avgG, avgB);
						}
					}
					percentage += (double)(pixel_size * pixel_size) * 100.f / (height * width);
					m.unlock();
				}
			}
			m.lock();
			for (int i = index; i < minMax.size(); i++)
			{
				if (minMax[i][0] != -1)
				{
					index = i;
					break;
				}
			}
			if (minMax[index][0] == -1)
			{
				m.unlock();
				return;
			}
			iMin = minMax[index][0];
			iMax = minMax[index][1];
			minMax[index][0] = -1;
			m.unlock();
		}
	}

	std::vector<std::vector<Color>> draw(std::vector<object> objects, std::vector<light> lights)
	{
		system("cls");
		RenderWindow infoWindow(VideoMode(300, 78), "RayCasting", Style::Close);
		Event event;
		std::vector<std::vector<Color>> result(width + 100, std::vector<Color>(height + 100));
		fill(objects_t, -1, objects.size());
		applyDirection();
		std::vector < std::future<void>> futures;
		double percentage = 0, oldPercentage = 0;
		auto start = std::chrono::high_resolution_clock::now();
		std::vector<std::vector<int>> minMax;
		int jMax = pow(raysInPixel, 2) * 10;
		while (jMax * threads > std::min(height, width))
		{
			jMax /= 2;
			if (jMax == 0) throw std::runtime_error("Wrong camera size");
		}
		for (int i = 0; i < threads; i++)
		{
			for (int j = 0; j < jMax; j++)
			{
				int iMin = (int)(height * i / threads);
				int iMax = (int)(height * (i + 1) / threads);
				std::vector<int> array;
				array.push_back((int)(std::max(iMin, iMax * j / jMax)));
				array.push_back((int)(iMax * (j + 1) / jMax));
				minMax.push_back(array);
			}
		}
		for (int i = 0; i < threads; i++)
		{
			rayCastingCamera cam(x, y, z, width, height, direction);
			cam.pixel_size = pixel_size;
			cam.raysInPixel = raysInPixel;
			futures.push_back(std::async(std::launch::async, &rayCastingCamera::drawAsync, cam, objects, lights, std::ref(minMax), i, std::ref(result), std::ref(percentage), std::ref(end)));
		}
		while (percentage <= 99)
		{
			while (infoWindow.pollEvent(event))
			{
				if (event.type == Event::Closed || end == true)
				{
					end = true;
					for (int i = 0; i < threads; i++)
					{
						futures[i].get();
					}
					infoWindow.close();
					result.clear();
					return result;
				}
			}
			if (percentage > oldPercentage)
			{
				auto end = std::chrono::high_resolution_clock::now();
				float _time = std::chrono::duration_cast<std::chrono::duration<long long, std::micro>>(end - start).count();
				_time = _time / percentage * 100 - _time;
				_time /= 1000000;
				_time++;
				int minutes = truncf(_time) / 60;
				int hours = minutes / 60;

				std::string info = std::to_string((int)(percentage)) + "%\t";
				if (hours > 0) info += std::to_string(hours) + "h " + std::to_string(minutes % 60) + "m ";
				else if (minutes > 0) info += std::to_string(minutes - hours * 60) + "m ";
				info += std::to_string(int(_time - minutes * 60) % 60) + "s";

				Text text;
				Font font;
				font.loadFromFile("Fonts/Roboto-Light.ttf");
				text.setFont(font);
				text.setString(info);
				text.setCharacterSize(40);
				RectangleShape progressBar;
				progressBar.setFillColor(Color::Green);
				progressBar.setSize(Vector2f(300 * (percentage / 100), 20));
				progressBar.setPosition(0, 50);
				infoWindow.clear();
				infoWindow.draw(text);
				infoWindow.draw(progressBar);
				infoWindow.display();
				oldPercentage = percentage;
			}
		}
		for (int i = 0; i < threads; i++)
		{
			futures[i].get();
		}

		infoWindow.close();
		result.resize(width, std::vector<Color>(height));
		result.erase(result.begin() + height, result.end());
		for (int i = 0; i < height; i++)
		{
			result[i].erase(result[i].begin() + width, result[i].end());
		}
		return result;
	}
};