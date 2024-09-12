#include "settings.h"
#include "Include/imgui/imgui.h"
#include "Include/rapidjson/document.h"
#include "Include/rapidjson/stringbuffer.h"
#include "Include/rapidjson/writer.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace Settings {
	namespace ESP
	{
		extern bool enabled = true;
		extern bool drawLines = true;
		extern bool drawTeam = false;
		extern bool showName = true;
		extern ImColor* teamColor = new ImColor(0, 0, 255, 255);
		extern ImColor* enemyColor = new ImColor(255, 0, 0, 255);
	}
	namespace Aimbot
	{
		extern bool enabled = true;
		extern bool smoothing = false;
		extern float smoothingAmount = 1.0f;
		extern bool checkInFov = true;
		extern float fov = 180;
		extern bool drawFovCircle = true;
	}
	namespace Hacks 
	{
		extern bool RadarHack = false;
		extern bool MapHack = false;
		extern bool NoClip = false;
		extern float Speed = 0.18f;
	}
}

std::vector<std::string> Settings::getSaveFiles()
{
	std::vector<std::string> file;
	std::string path = ".";

	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".json")
			file.push_back(entry.path().filename().string());
	}
	return file;
}

std::string Settings::stripProfileName(std::string& profileName)
{
	std::string name = profileName;
	if (name.rfind(".\\", 0) == 0)
		name = name.substr(2);
	if (name.size() >= 5 && name.compare(name.size() - 5, 5, ".json"))
		name = name.substr(0, name.size() - 5);
	return name;
}

void saveColor(ImColor* color, std::string name, rapidjson::Value& parentTree, rapidjson::Document::AllocatorType& allocator)
{
	rapidjson::Value colorValue(rapidjson::kObjectType);
	colorValue.AddMember("r", color->Value.x, allocator);
	colorValue.AddMember("g", color->Value.y, allocator);
	colorValue.AddMember("b", color->Value.z, allocator);
	colorValue.AddMember("a", color->Value.w, allocator);
	parentTree.AddMember(rapidjson::Value(name.c_str(), allocator).Move(), colorValue, allocator);
}

void loadColor(ImColor* color, std::string name, rapidjson::Value& parentTree)
{
	if (!parentTree.HasMember(name.c_str()))
		return;
	if (parentTree[name.c_str()].HasMember("r") && parentTree[name.c_str()]["r"].IsFloat())
		color->Value.x = parentTree[name.c_str()]["r"].GetFloat();
	if (parentTree[name.c_str()].HasMember("g") && parentTree[name.c_str()]["g"].IsFloat())
		color->Value.y = parentTree[name.c_str()]["g"].GetFloat();
	if (parentTree[name.c_str()].HasMember("b") && parentTree[name.c_str()]["b"].IsFloat())
		color->Value.z = parentTree[name.c_str()]["b"].GetFloat();
	if (parentTree[name.c_str()].HasMember("a") && parentTree[name.c_str()]["a"].IsFloat())
		color->Value.w = parentTree[name.c_str()]["a"].GetFloat();
}

void Settings::saveSettings(const std::string& filename)
{
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value esp(rapidjson::kObjectType);
	esp.AddMember("enabled", Settings::ESP::enabled, allocator);
	esp.AddMember("drawLines", Settings::ESP::drawLines, allocator);
	esp.AddMember("drawTeam", Settings::ESP::drawTeam, allocator);
	esp.AddMember("showName", Settings::ESP::showName, allocator);
	saveColor(Settings::ESP::teamColor, "teamColor", esp, allocator);
	saveColor(Settings::ESP::enemyColor, "enemyColor", esp, allocator);
	document.AddMember("ESP", esp, allocator);

	rapidjson::Value aimbot(rapidjson::kObjectType);
	aimbot.AddMember("enabled", Settings::Aimbot::enabled, allocator);
	aimbot.AddMember("smoothing", Settings::Aimbot::smoothing, allocator);
	aimbot.AddMember("smoothingAmount", Settings::Aimbot::smoothingAmount, allocator);
	aimbot.AddMember("checkInFov", Settings::Aimbot::checkInFov, allocator);
	aimbot.AddMember("fov", Settings::Aimbot::fov, allocator);
	aimbot.AddMember("drawFovCircle", Settings::Aimbot::drawFovCircle, allocator);
	document.AddMember("Aimbot", aimbot, allocator);

	rapidjson::Value hacks(rapidjson::kObjectType);
	hacks.AddMember("RadarHack", Settings::Hacks::RadarHack, allocator);
	hacks.AddMember("MapHack", Settings::Hacks::MapHack, allocator);
	document.AddMember("Hacks", hacks, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	std::ofstream file(filename);
	//std::cout << "Saving to file: " << filename << std::endl;
	if (file.is_open())
	{
		file << buffer.GetString();
		file.close();
	}
	else
	{
		printf("Failed to save settings\n");
	}
}

template <typename T>
void GetValue(const rapidjson::Value& obj, const char* key, T& out)
{
	if (!obj.HasMember(key))
		return;
	if (obj[key].Is<T>())
		out = obj[key].Get<T>();
}

void Settings::loadSettings(const std::string& filename)
{
	std::ifstream ifs(filename);
	if (!ifs.is_open())
		return;
	std::string jsonContent((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	ifs.close();

	rapidjson::Document document;
	if (document.Parse(jsonContent.c_str()).HasParseError())
		return;
	//std::cout << "Loading settings from file: " << filename << std::endl;
	
	if (document.HasMember("ESP"))
	{
		GetValue(document["ESP"], "enabled", Settings::ESP::enabled);
		GetValue(document["ESP"], "drawLines", Settings::ESP::drawLines);
		GetValue(document["ESP"], "drawTeam", Settings::ESP::drawTeam);
		GetValue(document["ESP"], "showName", Settings::ESP::showName);
		loadColor(Settings::ESP::teamColor, "teamColor", document["ESP"]);
		loadColor(Settings::ESP::enemyColor, "enemyColor", document["ESP"]);
	}

	if (document.HasMember("Aimbot"))
	{
		GetValue(document["Aimbot"], "enabled", Settings::Aimbot::enabled);
		GetValue(document["Aimbot"], "smoothing", Settings::Aimbot::smoothing);
		GetValue(document["Aimbot"], "smoothingAmout", Settings::Aimbot::smoothingAmount);
		GetValue(document["Aimbot"], "checkInFov", Settings::Aimbot::checkInFov);
		GetValue(document["Aimbot"], "fov", Settings::Aimbot::fov);
		GetValue(document["Aimbot"], "drawFovCircle", Settings::Aimbot::drawFovCircle);
	}

	if (document.HasMember("Hacks"))
	{
		GetValue(document["Hacks"], "RadarHack", Settings::Hacks::RadarHack);
		GetValue(document["Hacks"], "MapHack", Settings::Hacks::MapHack);
	}
}
