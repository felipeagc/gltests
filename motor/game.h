#ifndef GAME_H
#define GAME_H


class Game
{
public:
	Game();
	virtual void Load();
	virtual void Update(float delta);
	virtual void Quit();
};

#endif // GAME_H
