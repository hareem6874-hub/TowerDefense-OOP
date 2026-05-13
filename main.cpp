#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <fstream>

#include "BasicEnemy.h"
#include "FastEnemy.h"
#include "TankEnemy.h"
#include "FlyingEnemy.h"
#include "SplitEnemy.h"

#include "CannonTower.h"
#include "MachineGunTower.h"
#include "SniperTower.h"
#include "SlowTower.h"
#include "FireTower.h"


using namespace sf;
using namespace std;


//  ROAD 
bool isOnRoad(float y)
{
	return (y >= 300 && y <= 420);
}

//  TOWER INFO 
void setInfo(int id, string &name, int &cost, string &desc)
{
	if(id == 1) 
    {
		name="Cannon";
		cost=50;
		desc="High damage";
	}
	else if(id == 2)
     {
		name="Machine";
		cost=60;
		desc="Fast fire";
	}
	else if(id == 3) 
    {
		name="Sniper";
		cost=70;
		desc="Long range";
	}
	else if(id == 4) 
    {
		name="Slow";
		cost=80;
		desc="Slow enemies";
	}
	else if(id == 5)
     {
		name="Fire";
		cost=90;
		desc="Burn area";
	}
	else
     {
		name="None";
		cost=0;
		desc="Select tower";
	}
}

int main()
{
	RenderWindow window(VideoMode(1200,700), "FIXED TD FINAL");
	sf::Music bgMusic;
	sf::Music towerPlaceMusic;

	if (!bgMusic.openFromFile("background-music.ogg"))
	{
		cout << "Music load failed" << endl;
	}

	bgMusic.setLoop(true);
	bgMusic.setVolume(40);
	bgMusic.play();

	if (!towerPlaceMusic.openFromFile("tower-place-music.ogg"))
	{
		cout << "Music load failed" << endl;
	}

	towerPlaceMusic.setLoop(false);
	towerPlaceMusic.setVolume(80);

	window.setFramerateLimit(60);

	Enemy* enemies[200];
	Tower* towers[100];

	int totalEnemies = 0;
	int totalTowers = 0;

	Clock clock;
	Clock spawnClock;

	Font font;
	font.loadFromFile("arial.ttf");

	//  HIGH SCORE 
	int highScore = 0;

	ifstream readScore("highscore.txt");

	if(readScore.is_open())
	{
		readScore >> highScore;
		readScore.close();
	}

	//  GAME STATE (0 play, 1 win, 2 lose)
	int state = 0; 

	int gold = 300;
	int lives = 10;

	int wave = 1;
	const int MAX_WAVES = 5;

	int enemiesPerWave = 10;
	int spawned = 0;

	float spawnDelay = 1;

	int enemySpawn = 0;
	int selectedTower = 0;

	string name="None";
	int cost=0;
	string desc="Select tower";

	int score = 0;
	bool scoreSaved = false;

	int splitCounter = 0;

	string warningText = "";
	float warningTimer = 0;

	//  RANGE PREVIEW 
	CircleShape rangeCircle;
	rangeCircle.setFillColor(Color::Transparent);
	rangeCircle.setOutlineThickness(2);
	rangeCircle.setOutlineColor(Color(0,255,255,180));

	float towerRange = 150;

	//  COLORS 
	Color towerColors[5] =
	{
		Color::Blue,
		Color::Green,
		Color::White,
		Color(255,140,0),
		Color::Red
	};

	//  BACKGROUND 
	RectangleShape bg(Vector2f(1200,700));
	bg.setFillColor(Color(40,160,70));

	RectangleShape road(Vector2f(1200,120));
	road.setFillColor(Color(90,90,90));
	road.setPosition(0,300);

	//  UI 
	RectangleShape panel(Vector2f(350,170));
	panel.setPosition(10,10);
	panel.setFillColor(Color(0,0,0,180));

	RectangleShape btn[5];

	float size=55, spacing=10;
	float totalW=(size*5)+(spacing*4);
	float startX=1200-10-totalW;

	for(int i=0; i<5; i++)
	{
		btn[i].setSize(Vector2f(size,size));
		btn[i].setPosition(startX+i*(size+spacing),20);
		btn[i].setFillColor(towerColors[i]);

		btn[i].setOutlineThickness(3);
		btn[i].setOutlineColor(Color::Transparent);
	}

	//  LOOP 
	while(window.isOpen())
	{
		Event e;
		while(window.pollEvent(e))
		{
			if(e.type == Event::Closed)
				{
                    window.close();
                }

			if(e.mouseButton.button == Mouse::Right)
			{
				float mx = e.mouseButton.x;
				float my = e.mouseButton.y;

				for(int i=0; i<totalTowers; i++)
				{
					if(towers[i]->contains(mx,my))
					{
						int cost = towers[i]->getUpgradeCost();

						if(gold >= cost)
						{
							gold -= cost;
							towers[i]->upgrade();
							warningText = "Tower Upgraded!";
						}
						else
						{
							warningText = "Not enough gold!";
						}
						warningTimer = 1.5;
						break;
					}
				}
			}
			//  INPUT 
			if(state==0 && e.type==Event::MouseButtonPressed)
			{
				float x=e.mouseButton.x;
				float y=e.mouseButton.y;

				bool ui=false;

				if (e.mouseButton.button == Mouse::Right)
				{
					float mx = e.mouseButton.x;
					float my = e.mouseButton.y;

					for (int i = 0; i < totalTowers; i++)
					{
						if (towers[i]->getGlobalBounds().contains(mx, my))
						{
							int cost = towers[i]->getUpgradeCost();
							if (gold >= cost)
							{
								gold -= cost;
								towers[i]->upgrade();
								warningText = "Tower Upgraded!";
							}
							else
							{
								warningText = "Not enough gold!";
							}

							warningTimer = 1.5;
							break;
						}
					}
				}

				if(e.mouseButton.button==Mouse::Left)
				{
					for(int i=0; i<5; i++)
					{
						if(btn[i].getGlobalBounds().contains(x,y))
						{
							selectedTower=i+1;
							setInfo(selectedTower,name,cost,desc);
							ui=true;
						}
					}

					if(!ui)
					{
						if(selectedTower==0)
						{
							warningText="Select a tower first!";
							warningTimer=1.5;
						}
						else if(isOnRoad(y))
						{
							warningText="Cannot place on road!";
							warningTimer=1.5;
						}
						else if(gold < cost)
						{
							warningText="Not enough gold!";
							warningTimer=1.5;
						}
						else
						{
							if(selectedTower==1)
							{
								towers[totalTowers++] = new CannonTower(x,y);
								gold -= 50;
							}
							else if(selectedTower==2)
							{
								towers[totalTowers++] = new MachineGunTower(x,y);
								gold -= 60;
							}
							else if(selectedTower==3)
							{
								towers[totalTowers++] = new SniperTower(x,y);
								gold -= 70;
							}
							else if(selectedTower==4)
							{
								towers[totalTowers++] = new SlowTower(x,y);
								gold -= 80;
							}
							else if(selectedTower==5)
							{
								towers[totalTowers++] = new FireTower(x,y);
								gold -= 90;
							}
                            towerPlaceMusic.play();
						}
					}
				}
			}
		}

		float timePerFrame = clock.restart().asSeconds();

		//  KEYBOARD 
		if(state==0)
		{
			if(Keyboard::isKeyPressed(Keyboard::Num1)) selectedTower=1;
			else if(Keyboard::isKeyPressed(Keyboard::Num2)) selectedTower=2;
			else if(Keyboard::isKeyPressed(Keyboard::Num3)) selectedTower=3;
			else if(Keyboard::isKeyPressed(Keyboard::Num4)) selectedTower=4;
			else if(Keyboard::isKeyPressed(Keyboard::Num5)) selectedTower=5;

			setInfo(selectedTower,name,cost,desc);
		}

		//  RANGE UPDATE 
		if(selectedTower == 1) towerRange = 170;
		else if(selectedTower == 2) towerRange = 130;
		else if(selectedTower == 3) towerRange = 400;
		else if(selectedTower == 4) towerRange = 120;
		else if(selectedTower == 5) towerRange = 160;

		//  SPAWN 
		if(state==0 && wave<=MAX_WAVES)
		{
			if(spawnClock.getElapsedTime().asSeconds()>spawnDelay && spawned<enemiesPerWave)
			{
				if(splitCounter < 3)
				{
					enemies[totalEnemies++] = new SplitEnemy(340);
					splitCounter++;
				}
				else if(enemySpawn%5==0)
                {
                    enemies[totalEnemies++] = new FlyingEnemy(260);
                }

				else if(enemySpawn%4==0)
                {
                    enemies[totalEnemies++] = new TankEnemy(330);
                }
				else if(enemySpawn%3==0)
                {
					enemies[totalEnemies++] = new FastEnemy(340);
                }
				else
                {
                    enemies[totalEnemies++] = new BasicEnemy(340);
                }
				enemySpawn++;
				spawned++;
				spawnClock.restart();
			}
		}

		//  ENEMIES 
		if(state==0)
		{
			for(int i=0; i<totalEnemies; i++)
			{
				enemies[i]->update(timePerFrame);
				if(enemies[i]->getPos().x>1200)
				{
					delete enemies[i];
					for(int j=i; j<totalEnemies-1; j++)
                    {
                        enemies[j]=enemies[j+1];
                    }
					totalEnemies--;
					lives--;
					i--;
				}
				else if(enemies[i]->isDead())
				{
					SplitEnemy* split = dynamic_cast<SplitEnemy*>(enemies[i]);
					if(split)
					{
						Vector2f pos = enemies[i]->getPos();
						enemies[totalEnemies] = new FastEnemy(pos.y);
						enemies[totalEnemies]->setPosition(pos.x, pos.y - 20);
						totalEnemies++;
						enemies[totalEnemies] = new FastEnemy(pos.y);
						enemies[totalEnemies]->setPosition(pos.x, pos.y + 20);
						totalEnemies++;
					}
					delete enemies[i];
					for(int j=i; j<totalEnemies-1; j++)
                    {
                        enemies[j]=enemies[j+1];
                    }
					totalEnemies--;
					gold += 20;
					score += 50;
					i--;
				}
			}
		}

		//  TOWERS 
		if(state==0)
		{
			for(int i=0; i<totalTowers; i++)
			{
				towers[i]->update(timePerFrame);
				towers[i]->attack(enemies,totalEnemies);
			}
		}

		//  WAVES 
		if(state==0 && spawned>=enemiesPerWave && totalEnemies==0)
		{
			wave++;
			spawned=0;
			enemySpawn=0;
			splitCounter=0;

			enemiesPerWave += 5;
			spawnDelay *= 0.95;
		}

		//  LOSE
		if(lives <= 0)
		{
			state = 2;
			if(!scoreSaved)
			{
				score += gold;
				if(score > highScore)
				{
					highScore = score;
					ofstream save("highscore.txt");
					save << highScore;
					save.close();
				}
				scoreSaved = true;
			}
		}

		//  WIN 
		if(wave > MAX_WAVES && totalEnemies == 0)
		{
			state = 1;
			if(!scoreSaved)
			{
				score += gold;
				score += lives * 200;
				if(score > highScore)
				{
					highScore = score;
					ofstream save("highscore.txt");
					save << highScore;
					save.close();
				}
				scoreSaved = true;
			}
		}

		//  WARNING TIMER 
		if(warningTimer > 0)
        {
            warningTimer -= timePerFrame;
        }
		else
        {
            warningText = "";
        }

		//  DRAW 
		window.clear();

		window.draw(bg);
		window.draw(road);

		RectangleShape line(Vector2f(60,3));
		line.setFillColor(Color::White);

		for(int i=0; i<25; i++)
		{
			line.setPosition(i * 60, 355);
			window.draw(line);
		}

		RectangleShape border(Vector2f(1200,4));
		border.setFillColor(Color(40,40,40));

		border.setPosition(0,300);
		window.draw(border);

		border.setPosition(0,420);
		window.draw(border);

		//  ENEMIES 
		for(int i=0; i<totalEnemies; i++)
        {
            enemies[i]->draw(window);
        }
			
		//  TOWERS 
		for(int i=0; i<totalTowers; i++)
        {
            towers[i]->draw(window);
        }

		for(int i=0; i<totalTowers; i++)
		{
			Text lvl;
			lvl.setFont(font);
			lvl.setCharacterSize(14);
			lvl.setFillColor(Color::Yellow);
			lvl.setString("Lv." + to_string(towers[i]->getLevel()));
			lvl.setPosition(
			    towers[i]->getGlobalBounds().left,
			    towers[i]->getGlobalBounds().top - 18
			);
			window.draw(lvl);
		}

		//  UI PANEL 
		window.draw(panel);

		Text ui;
		ui.setFont(font);
		ui.setCharacterSize(18);
		ui.setFillColor(Color::White);
		ui.setPosition(20,20);

		ui.setString(
		    "Gold: " + to_string(gold) +
		    "\nLives: " + to_string(lives) +
		    "\nWave: " + to_string(wave) +
		    "\nScore: " + to_string(score) +
		    "\nHigh Score: " + to_string(highScore) +
		    "\nTower: " + name + " $" + to_string(cost) +
		    "\nTower: " + name + " : " + desc
		);

		window.draw(ui);

		//  WARNING 
		if(warningText != "")
		{
			Text warn;
			warn.setFont(font);
			warn.setCharacterSize(22);
			warn.setFillColor(Color::Red);
			warn.setPosition(400,650);
			warn.setString(warningText);
			window.draw(warn);
		}

		//  RANGE CIRCLE 
		Vector2i m = Mouse::getPosition(window);

		rangeCircle.setRadius(towerRange);
		rangeCircle.setOrigin(towerRange,towerRange);
		rangeCircle.setPosition((float)m.x,(float)m.y);

		if(selectedTower != 0)
        {
            window.draw(rangeCircle);
        }
			
		//  BUTTONS 
		for(int i=0; i<5; i++)
		{
			if(selectedTower == i+1)
            {
                btn[i].setOutlineColor(Color::Yellow);
            }
			else
            {
                btn[i].setOutlineColor(Color::Transparent);
            }
			window.draw(btn[i]);
		}

		//  WIN SCREEN 
		if(state == 1)
		{
			Text t;
			t.setFont(font);
			t.setCharacterSize(60);
			t.setFillColor(Color::Green);

			t.setString(
			    "YOU WIN\n\n"
			    "Score: " + to_string(score) +
			    "\nHigh Score: " + to_string(highScore)
			);
			t.setPosition(350,220);
			window.draw(t);
		}

		// LOSE SCREEN 
		if(state == 2)
		{
			Text t;
			t.setFont(font);
			t.setCharacterSize(60);
			t.setFillColor(Color::Red);

			t.setString(
			    "GAME OVER\n\n"
			    "Score: " + to_string(score) +
			    "\nHigh Score: " + to_string(highScore)
			);
			t.setPosition(300,220);
			window.draw(t);
		}

		window.display();
	}

	//  CLEANUP 
	for(int i=0; i<totalEnemies; i++)
		delete enemies[i];

	for(int i=0; i<totalTowers; i++)
		delete towers[i];

	return 0;
}