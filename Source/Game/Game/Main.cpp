#include "Renderer/Renderer.h"
#include "Core/Core.h"
#include "Renderer/ModelManager.h"
#include "Renderer/Text.h"
#include "Renderer/ParticleSystem.h"
#include "Renderer/Texture.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "FrameWork/Scene.h"
#include "Player.h"
#include "Enemy.h"
#include <FrameWork/Emitter.h>
#include "FrameWork/Resource/ResourceManager.h"

#include "SpaceGame.h"

#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <cassert>
#include <array>
#include <map>

using namespace std;
using vec2 = kiko::Vector2;

class Star
{
public:
	Star(const kiko::Vector2& pos, kiko::Vector2& vel) :
		m_pos{ pos },
		m_vel{ vel }
	{}

	void Update(int width, int height)
	{
		m_pos += m_vel * kiko::g_time.GetDeltaTime();

		if (m_pos.x >= width)
			m_pos.x = 0;

		if (m_pos.y > height)
			m_pos.y = 0;
	}

	void Draw(kiko::Renderer& renderer)
	{
		renderer.DrawPoint(m_pos.x, m_pos.y);
	}

public:
	kiko::Vector2 m_pos;
	kiko::Vector2 m_vel;
};

template <typename T>
void print(const std::string& s, const T& container)
{
	std::cout << s << std::endl;
		for (auto element : container)
		{
			std::cout << element << " ";
		}
	std::cout << std::endl;
}

int main(int argc, char* argv[])
{
	/*
	// static array
	int n[4] = { 1, 2, 3, 4 };
	print("array: ", n);
	cout << n << endl;
	cout << (n + 3) << endl;

	// array class
	std::array<int, 4> na = { 1, 2, 3, 4 };
	print("array class: ", na);
	cout << na.front() << endl;
	cout << na.back() << endl;
	cout << na.max_size() << endl;

	// vector
	std::vector<int> nv = { 1, 2, 3, 4 };
	print("vector: ", nv);
	nv.insert(nv.begin(), 0);
	nv.push_back(5);
	nv.pop_back();
	std::remove(nv.begin(), nv.end(), 2);
	print("vector: ", nv);

	// list
	std::list<int> nl = { 1, 2, 3, 4 };
	print("list:", nl);
	nl.push_front(0);
	print("list:", nl);

	std::map<std::string, int> ages;
	ages["Charles"] = 17;
	ages["Zane"] = 18;
	ages["Jacob"] = 19;
	ages["Jacob"] = 20;

	cout << ages["Jacob"] << endl;
	cout << ages["Zane"] << endl;
	*/

	INFO_LOG("hello world")

	kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("Assets");

	// Init
	kiko::g_renderer.Initialize();
	kiko::g_renderer.CreateWindow("CSC196", 800, 600);

	kiko::g_inputSystem.Initialize();
	kiko::g_audioSystem.Initialize();

	unique_ptr<SpaceGame> game = make_unique<SpaceGame>();
	game->Initalize();

	kiko::vec2 v{ 5, 5 };
	v.Normalize();

	// stars
	vector <Star> stars;
	for (int i = 0; i < 1000; i++)
	{
		kiko::Vector2 pos(kiko::Vector2(kiko::random(kiko:: g_renderer.GetWidth()), kiko::random(kiko::g_renderer.GetHeight())));
		kiko::Vector2 vel(kiko::randomf(1, 4), 0.0f);

		stars.push_back(Star(pos, vel));
	}

	float speed = 200; // in pixels per second
	constexpr float turnRate = kiko::DegreesToRadians(180);

	// create texture
	//shared_ptr<kiko::Texture> texture = make_shared<kiko::Texture>();
	//texture->Load("main_ship.png", kiko::g_renderer);
	kiko::res_t<kiko::Texture> texture = kiko::g_resourceManager.Get<kiko::Texture>("main_ship.png", kiko::g_renderer);

	// Main game loop
	bool quit = false;
	while (!quit)
	{
		kiko::g_time.Tick();
		kiko::g_inputSystem.Update();
		kiko::g_audioSystem.Update();
		kiko::g_particleSystem.Update(kiko::g_time.GetDeltaTime());
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}

		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
		{
			kiko::g_audioSystem.PlayOneShot("Laser", false);
		}
		
		
		if (kiko::g_inputSystem.GetMouseButtonDown(0))
		{
			kiko::EmitterData data;
			data.burst = true;
			data.burstCount = 100;
			data.spawnRate = 200;
			data.angle = 0;
			data.angleRange = kiko::Pi;
			data.lifetimeMin = 0.5f;
			data.lifetimeMin = 1.5f;
			data.speedMin = 50;
			data.speedMax = 250;
			data.damping = 0.5f;
			data.color = kiko::Color{ 1, 0, 0, 1 };
			kiko::Transform transform{ { kiko::g_inputSystem.GetMousePosition() }, 0, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->m_lifespan = 0.1f;
			game->m_scene->Add(std::move(emitter));
		}
		

		// update game
		game->Update(kiko::g_time.GetDeltaTime());
		
		// draw game
		kiko::g_renderer.SetColor(0, 0, 0, 0);
		kiko::g_renderer.BeginFrame();
		for (auto& star : stars)
		{
			star.Update(kiko::g_renderer.GetWidth(), kiko::g_renderer.GetHeight());
			kiko::g_renderer.SetColor(kiko::random(256), kiko::random(256), kiko::random(256), 255);
			star.Draw(kiko::g_renderer);
		}

		game->Draw(kiko::g_renderer);
		kiko::g_renderer.DrawTexture(texture.get(), 200.0f, 200.0f, 0.0f);
 		kiko::g_particleSystem.Draw(kiko::g_renderer);

		//text->Draw(kiko::g_renderer, 40, 30);

		kiko::g_renderer.EndFrame();
	}

	stars.clear();

	return 0;
}
