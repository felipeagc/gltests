#ifndef MYGAME_H
#define MYGAME_H

#include "../motor/engine.h"
#include "../motor/game.h"
#include <memory>

class MyGame : public Game
{
public:
	MyGame();
	void Load() override;
	void Update(float delta) override;
	void Quit() override;

private:
	Engine* m_engine;
	Camera* m_camera;
	Model* m_model;
	Transform m_transform;
};

#endif // MYGAME_H
