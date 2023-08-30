#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Physics/PhysicsSystem.h"
#include "FrameWork/Framework.h"

#include "PlatformGame.h"

#include <iostream>
#include <vector>
#include <thread>
#include <array>
#include <map>
#include <cstdarg>
#include <functional>

using namespace std;
using vec2 = kiko::Vector2;

int main(int argc, char* argv[])
{
	INFO_LOG("Initalizing Engine...");

	kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("Assets/PlatformGame");

	// Initialize engine
	kiko::g_renderer.Initialize();
	kiko::g_renderer.CreateWindow("GAT150_Platform", 800, 600);

	kiko::g_inputSystem.Initialize();
	kiko::g_audioSystem.Initialize();
	kiko::PhysicsSystem::Instance().Initialize();

	// create game
	unique_ptr<PlatformGame> game = make_unique<PlatformGame>();
	game->Initialize();

	float speed = 200; // in pixels per second
	constexpr float turnRate = kiko::DegreesToRadians(180);

	// Main game loop
	bool quit = false;
	while (!quit)
	{
		// update engine
		kiko::g_time.Tick();
		kiko::g_inputSystem.Update();
		kiko::g_audioSystem.Update();
		kiko::g_particleSystem.Update(kiko::g_time.GetDeltaTime());
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}
		kiko::g_particleSystem.Update(kiko::g_time.GetDeltaTime());
		kiko::PhysicsSystem::Instance().Update(kiko::g_time.GetDeltaTime());

		// update game
		game->Update(kiko::g_time.GetDeltaTime());

		// draw game
		kiko::g_renderer.SetColor(0, 0, 0, 0);
		kiko::g_renderer.BeginFrame();
		game->Draw(kiko::g_renderer);

		kiko::g_particleSystem.Draw(kiko::g_renderer);
		kiko::g_renderer.EndFrame();
	}

	return 0;
}