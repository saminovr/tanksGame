#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <math.h>
#include <cmath>
#include <time.h>

const double PI = 3.1415926535;

using namespace sf;

RenderWindow window(VideoMode(800, 600), "Tanks");

class MyTank;

////////////////////////////////////
//RectangleShape line;
//Texture tex;
//Sprite beacon; 
//////////////////////////////////////

class Map
{
public:
	Map();

	Sprite mapSprite;

	Sprite longSprite;
	Sprite lowerMiddleSprite;
	Sprite lowerRightSprite;
	Sprite rightParkingSprite;
	Sprite middleParkingSprite;
	Sprite leftMiddleSprite;
	Sprite lowerRightMiddleSprite;
	Sprite cafeSprite;
	Sprite upperMiddleSprite;
	Sprite middleUpperLeftSprite;
	Sprite upperLeftSprite;
	Sprite upperRightSprite;


	bool isInside(Vector2u windowSize, Vector2f objectSize, Vector2f objectPos, float objectRotation);

private:
	Texture mapTexture;

	Texture longTexture;
	Texture lowerMiddleTexture;
	Texture lowerRightTexture;
	Texture rightParkingTexture;
	Texture middleParkingTexture;
	Texture leftMiddleTexture;
	Texture lowerRightMiddleTexture;
	Texture cafeTexture;
	Texture upperMiddleTexture;
	Texture middleUpperLeftTexture;
	Texture upperLeftTexture;
	Texture upperRightTexture;
};

class Tank : public RectangleShape
{
public:

	typedef enum {
		Forward = 1,
		Back, 
		Right,
		Left

	}direction;

	
	
	virtual Vector2f getPosition() = 0;
	virtual float getRotation() = 0;
	virtual void setPosition(Vector2f pos) = 0;
	virtual void setRotation(float rotation) = 0;

private:
	
};
class EnemyTank : public Tank
{
public:
	EnemyTank(MyTank* enemyTank);

	typedef enum {
		Blocked = 1,
		MovementToShoot
	}movingType;


	Sprite tankSprite;
	Clock clock;
	Clock pressedClock;
	Clock shootClock;
	Clock isShootableClock;

	Texture cannonballTexture;
	Sprite cannonballSprite;

	Texture timeTexture;
	Sprite timeSprite;

	Texture fireTexture;
	Sprite fireSprite;

	int hits = 0;
	bool isCrushed = false;
	bool timeNeedToMove = false;
	bool rotateUnitAIIsDone = true;

	Vector2f getPosition() override;
	float getRotation() override;
	void setPosition(Vector2f pos) override;
	void setRotation(float rotation) override;
	void moveUnit(Vector2f oldPos, int firstDirection, bool timeNeedToMove, MyTank* enemyTank);
	void moveUnitAI(int movingType, MyTank* enemyTank); /////////
	void movingUnitAI(Vector2f newPos, MyTank* enemyTank);
	void moveUnit(float oldRotation, int secondDirection, MyTank* enemyTank);
	void rotateUnitAI(float newRotation, MyTank* enemyTank); ////////
	void shoot(Vector2f tankPos, float rotation, MyTank* enemyTank);
	bool checkNewPosition(Vector2f newPos, float rotation, MyTank* enemyTank);

	void blast(MyTank* enemyTank);

	void isShootable(MyTank* enemyTank);

	Texture tankTexture;
private:
	

	const int height = 40;
	const int width = 40;

	const float linearSpeed = 0.05;
	const float linearAcceleration = 0.0125;
	const float angularSpeed = 0.03;

	const int hitsCountForDestroying = 3;

protected:


};
class MyTank : public Tank
{
public:
	MyTank();

	Sprite tankSprite;
	Clock clock;
	Clock pressedClock;
	Clock shootClock;
	Clock isShootableClock;

	Texture cannonballTexture;
	Sprite cannonballSprite;

	Texture timeTexture;
	Sprite timeSprite;

	Texture fireTexture;
	Sprite fireSprite;

	bool timeNeedToMove = false;
	int hits = 0;
	bool isCrushed = false;
	
	Vector2f getPosition() override;
	float getRotation() override;
	void setPosition(Vector2f pos) override;
	void setRotation(float rotation) override;
	void moveUnit(Vector2f oldPos, int firstDirection, bool timeNeedToMove, EnemyTank* enemyTank);
	void moveUnit(float oldRotation, int secondDirection, EnemyTank* enemyTank);
	void shoot(Vector2f tankPos, float rotation, EnemyTank* enemyTank);
	bool checkNewPosition(Vector2f newPos, float rotation, EnemyTank* enemyTank);

	void blast(EnemyTank* enemyTank);
	
	Texture tankTexture;

private:
	

	const int height = 40;
	const int width = 40;

	const float linearSpeed = 0.05;
	const float linearAcceleration = 0.0125;
	const float angularSpeed = 0.03;

	const int hitsCountForDestroying = 3;

protected:
	 

};


Map objMap;
int main()
{
	MyTank* myTank = new MyTank;
	myTank->setPosition(Vector2f(196, 379));
	EnemyTank* enemyTank = new EnemyTank(myTank);
	/*EnemyTank* enemyTank_2 = new EnemyTank(myTank);
	enemyTank_2->setPosition(Vector2f(400, 50));
	enemyTank_2->setRotation(90);*/

	
	
	myTank->clock.restart();
	myTank->shootClock.restart();
	
	enemyTank->clock.restart();
	enemyTank->shootClock.restart();

	while (window.isOpen())
	{
		Event event;
		
		Vector2i pos = Mouse::getPosition(window); 

		if (myTank->shootClock.getElapsedTime().asSeconds() > 3)
		{
			myTank->timeSprite.setPosition(Vector2f(-100, -100));
			myTank->timeNeedToMove = false;
		}
		else myTank->timeNeedToMove = true;
		///////////////////////////////////////
		if (enemyTank->shootClock.getElapsedTime().asSeconds() > 3)
		{
			enemyTank->timeSprite.setPosition(Vector2f(-100, -100));
			enemyTank->timeNeedToMove = false;
		}
		else enemyTank->timeNeedToMove = true;
		//////////////////////////////////////////
		if (!myTank->isCrushed)
		{
			if (Keyboard::isKeyPressed(Keyboard::W))
				myTank->moveUnit(Vector2f(myTank->getPosition().x, myTank->getPosition().y), MyTank::Forward, myTank->timeNeedToMove,
					enemyTank);
			if (Keyboard::isKeyPressed(Keyboard::D))
				myTank->moveUnit(myTank->getRotation(), Tank::Right, enemyTank);
			if (Keyboard::isKeyPressed(Keyboard::A))
				myTank->moveUnit(myTank->getRotation(), Tank::Left, enemyTank);
			if (Keyboard::isKeyPressed(Keyboard::S))
				myTank->moveUnit(Vector2f(myTank->getPosition().x, myTank->getPosition().y), Tank::Back, myTank->timeNeedToMove,
					enemyTank);
		}
		////////////

		/////////////
		if (!enemyTank->isCrushed)
		{
			if (Keyboard::isKeyPressed(Keyboard::Up))
				enemyTank->moveUnit(Vector2f(enemyTank->getPosition().x, enemyTank->getPosition().y), Tank::Forward,
					enemyTank->timeNeedToMove, myTank);
			if (Keyboard::isKeyPressed(Keyboard::Right))
				enemyTank->moveUnit(enemyTank->getRotation(), Tank::Right, myTank);
			if (Keyboard::isKeyPressed(Keyboard::Left))
				enemyTank->moveUnit(enemyTank->getRotation(), Tank::Left, myTank);
			if (Keyboard::isKeyPressed(Keyboard::Down))
				enemyTank->moveUnit(Vector2f(enemyTank->getPosition().x, enemyTank->getPosition().y), Tank::Back,
					enemyTank->timeNeedToMove, myTank);
		}

		while (window.pollEvent(event)) 
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left)
				{
					std::cout << "\nPOS x:" << pos.x << "\nPos y:" << pos.y << std::endl;
					std::cout << "y1 " << enemyTank->getPosition().y << " x1 " << enemyTank->getPosition().x << std::endl;
					std::cout << "alpha " << enemyTank->getRotation() << std::endl;
					std::cout << "y2 " << myTank->getPosition().y << " x2 " << myTank->getPosition().x << std::endl;
					std::cout << "betta " << myTank->getRotation() << std::endl;
				}
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Space)
				{
					if (!myTank->isCrushed)
					{
						if (myTank->shootClock.getElapsedTime().asSeconds() > 3)
						{
							myTank->timeSprite.setPosition(myTank->getPosition().x - 20, myTank->getPosition().y - 30);
							std::thread t(&MyTank::shoot, myTank, myTank->getPosition(), myTank->getRotation(),
								enemyTank);

							t.detach();
							myTank->shootClock.restart();
						}
					}
				}	
			if (myTank->isCrushed || enemyTank->isCrushed)
			{
				if (event.type == Event::KeyPressed)
				{
					if (event.key.code == Keyboard::Enter)
					{
						myTank->isCrushed = false;
						myTank->hits = 0;
						myTank->tankTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\tankk.png");
						myTank->setPosition(Vector2f(400, 500));
						myTank->setRotation(0);

						enemyTank->isCrushed = false;
						enemyTank->hits = 0;
						enemyTank->tankTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\tankk.png");	
						enemyTank->setPosition(Vector2f(730, 37));
						enemyTank->setRotation(270);
					}
				}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Numpad0)
				{
					if (!enemyTank->isCrushed)
					{
						if (enemyTank->shootClock.getElapsedTime().asSeconds() > 3)
						{
							enemyTank->timeSprite.setPosition(enemyTank->getPosition().x - 20, enemyTank->getPosition().y - 30);
							std::thread t(&EnemyTank::shoot, enemyTank, enemyTank->getPosition(), enemyTank->getRotation(),
								myTank);

							t.detach();
							enemyTank->shootClock.restart();
						}
					}
				}
			////////////////////////////////////////////////////////////////////////////////////////////////
		}
		window.draw(objMap.mapSprite);

		window.draw(objMap.longSprite);
		window.draw(objMap.lowerMiddleSprite);
		window.draw(objMap.lowerRightSprite);
		window.draw(objMap.lowerRightMiddleSprite);
		window.draw(objMap.cafeSprite);
		window.draw(objMap.upperMiddleSprite);
		window.draw(objMap.upperRightSprite);
		window.draw(objMap.upperLeftSprite);
		window.draw(objMap.middleUpperLeftSprite);
		window.draw(objMap.middleParkingSprite);
		window.draw(objMap.rightParkingSprite);
		window.draw(objMap.leftMiddleSprite);
			
		window.draw(myTank->tankSprite);
		window.draw(myTank->fireSprite);
		window.draw(myTank->timeSprite);
		window.draw(myTank->cannonballSprite);
	
		window.draw(enemyTank->tankSprite);
		window.draw(enemyTank->fireSprite);
		window.draw(enemyTank->timeSprite);
		window.draw(enemyTank->cannonballSprite);

		/*window.draw(enemyTank_2->tankSprite);
		window.draw(enemyTank_2->fireSprite);
		window.draw(enemyTank_2->timeSprite);
		window.draw(enemyTank_2->cannonballSprite);*/

		/*window.draw(line); 
		window.draw(beacon);*/
		
		
			
		window.display();
	}
	delete myTank;
	delete enemyTank;
	//delete enemyTank_2;
	return 0;
}

MyTank::MyTank()
{
	tankTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\tankk.png");
	cannonballTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\cannonball.png");
	fireTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\fire.png");
	timeTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\time.png");

	tankSprite.setTexture(tankTexture);
	cannonballSprite.setTexture(cannonballTexture);
	fireSprite.setTexture(fireTexture);
	timeSprite.setTexture(timeTexture);

	timeSprite.setPosition(Vector2f(-100,-100));
	fireSprite.setPosition(Vector2f(-100, -100));

	fireSprite.setOrigin(15, 15);

	setPosition(Vector2f(400,500));
	tankSprite.setOrigin(Vector2f(width/2, height/2));
	tankSprite.setRotation(0);
}

Vector2f MyTank::getPosition()
{
	return tankSprite.getPosition();
}


float MyTank::getRotation()
{
	return tankSprite.getRotation();
}

void MyTank::setPosition(Vector2f pos)
{
	tankSprite.setPosition(pos);
}

void MyTank::setRotation(float rotation)
{
	tankSprite.setRotation(rotation);
}

void MyTank::moveUnit(Vector2f oldPos, int firstDirection, bool timeNeedToMove, EnemyTank* enemyTank)
{
	if (pressedClock.getElapsedTime().asMilliseconds() > 10)  
	{
		clock.restart();
	}

	if (isShootableClock.getElapsedTime().asSeconds() > 1)
	{
		std::thread t(&EnemyTank::isShootable, enemyTank, this);
		t.detach();
		isShootableClock.restart();
	}

	float time = clock.getElapsedTime().asMicroseconds();
	float oldRotation = tankSprite.getRotation(); 
	switch (firstDirection)
	{
	case Forward:
	{
		if ((time / 1000000) < float(4))
		{
			if (checkNewPosition(Vector2f(oldPos.x + linearAcceleration *(time / 1000000)*sin((oldRotation*PI) / 180),
				(oldPos.y - linearAcceleration *(time / 1000000)*cos((oldRotation*PI) / 180))), (oldRotation*PI) / 180, 
				enemyTank))
			tankSprite.setPosition(oldPos.x + linearAcceleration *(time / 1000000)*sin((oldRotation*PI) / 180),
				(oldPos.y - linearAcceleration *(time / 1000000)*cos((oldRotation*PI) / 180)));  // ускорение 
			if (timeNeedToMove)
				timeSprite.setPosition(oldPos.x - 20 + linearAcceleration *(time / 1000000)*sin((oldRotation*PI) / 180),
					(oldPos.y - 30 - linearAcceleration *(time / 1000000)*cos((oldRotation*PI) / 180)));
		}
		else if ((time / 1000000) >= 4)
		{
			if(checkNewPosition(Vector2f(oldPos.x + linearSpeed *sin((oldRotation*PI) / 180),
				(oldPos.y - linearSpeed *cos((oldRotation*PI) / 180))), (oldRotation*PI) / 180, enemyTank))
			tankSprite.setPosition(oldPos.x + linearSpeed *sin((oldRotation*PI) / 180),
				(oldPos.y - linearSpeed*cos((oldRotation*PI) / 180))); // постоянная скорость
			if (timeNeedToMove)
				timeSprite.setPosition(oldPos.x - 20 + linearSpeed *sin((oldRotation*PI) / 180),
					(oldPos.y - 30 - linearSpeed *cos((oldRotation*PI) / 180)));
		}
		break;
	}
	case Back:
	{
		if ((time / 1000000) < float(4))
		{
			if (checkNewPosition(Vector2f(oldPos.x - linearAcceleration *(time / 1000000)*sin((oldRotation*PI) / 180),
				(oldPos.y + linearAcceleration *(time / 1000000)*cos((oldRotation*PI) / 180))), (oldRotation*PI) / 180, 
				enemyTank))
			tankSprite.setPosition(oldPos.x - linearAcceleration *(time / 1000000)*sin((oldRotation*PI) / 180),
				(oldPos.y + linearAcceleration *(time / 1000000)*cos((oldRotation*PI) / 180)));  // ускорение 
			if (timeNeedToMove)
				timeSprite.setPosition(oldPos.x - 20 - linearAcceleration *(time / 1000000)*sin((oldRotation*PI) / 180),
					(oldPos.y - 30 + linearAcceleration *(time / 1000000)*cos((oldRotation*PI) / 180)));
		}
		else if ((time / 1000000) >= 4)
		{
			if (checkNewPosition(Vector2f(oldPos.x - 0.09*sin((oldRotation*PI) / 180),
				(oldPos.y + linearSpeed *cos((oldRotation*PI) / 180))), (oldRotation*PI) / 180, enemyTank))
			tankSprite.setPosition(oldPos.x - linearSpeed *sin((oldRotation*PI) / 180),
				(oldPos.y + linearSpeed *cos((oldRotation*PI) / 180))); // постоянная скорость
			if (timeNeedToMove)
				timeSprite.setPosition(oldPos.x - 20 - linearSpeed *sin((oldRotation*PI) / 180),
					(oldPos.y - 30 + linearSpeed *cos((oldRotation*PI) / 180)));
		}
		break;
	}

	}
	pressedClock.restart();
}

void MyTank::moveUnit(float oldRotation, int SecondDirection, EnemyTank* enemyTank)
{
	switch (SecondDirection)
	{
	case Right:
	{
		if (checkNewPosition(tankSprite.getPosition(), tankSprite.getRotation() + float(angularSpeed), enemyTank))
			tankSprite.setRotation(oldRotation + float(angularSpeed));
		break;
	}
	case Left:
	{
		if (checkNewPosition(tankSprite.getPosition(), tankSprite.getRotation() - float(angularSpeed), enemyTank))
			tankSprite.setRotation(oldRotation - float(angularSpeed));
		break;
	}
	}
}

bool MyTank::checkNewPosition(Vector2f newPos, float rotation, EnemyTank* enemyTank)
{
	bool answer = true;
	RectangleShape tempRect;
	tempRect.setSize(Vector2f(width, height));
	tempRect.setOrigin(Vector2f(20.0, 25.0));
	tempRect.setRotation(getRotation()+rotation);
	tempRect.setPosition(newPos);

	float alpha = (((getRotation()+rotation)*PI) / 180);
	float arccos = acos((height / 2) / (sqrt(pow((height / 2), 2) + pow((width / 2), 2))));
	float cot = 1 / tan(arccos);

	Vector2f points[4];
	
	float dxa = -(width / 2)*(cos(alpha) + cot * sin(alpha));								//  
	float dya = ((height / 2)*sin((PI / 2) - arccos - alpha)) / (sin((PI / 2) + arccos));	//  B _______ C
	points[3] = Vector2f(newPos.x + dxa, newPos.y + dya);									//    |     |
																							//    |  .O |  - tank rect
	float dxb = -(width / 2)*(cos(alpha) - cot * sin(alpha));								//    |_____|  O - center
	float dyb = (-(height / 2)*sin((PI / 2) + arccos - alpha)) / (sin((PI / 2) + arccos));	//  A         D
	points[1] = Vector2f(newPos.x + dxb, newPos.y + dyb);									// dx and dy regarding O

	float dxc = (width / 2)*(cos(alpha) + cot * sin(alpha));
	float dyc = (-(height / 2)*sin((PI / 2) - arccos - alpha)) / (sin((PI / 2) + arccos));
	points[0] = Vector2f(newPos.x + dxc, newPos.y + dyc);

	float dxd = (width / 2)*(cos(alpha) - cot * sin(alpha));
	float dyd = ((height / 2)*sin((PI / 2) + arccos - alpha)) / (sin((PI / 2) + arccos));
	points[2] = Vector2f(newPos.x + dxd, newPos.y + dyd);
	
	for (int i = 0; i < 4; i++)
	{
		if (objMap.longSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.lowerMiddleSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.lowerRightSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.cafeSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.lowerRightMiddleSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.upperRightSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.upperMiddleSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.upperLeftSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.middleUpperLeftSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.middleParkingSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.rightParkingSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.leftMiddleSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (enemyTank->tankSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;

			break;
		}
	}

	if (!objMap.isInside(window.getSize(), tempRect.getSize(), tempRect.getPosition(), tempRect.getRotation()))
		answer = false;

	return answer;
}

void MyTank::shoot(Vector2f tankPos, float rotation, EnemyTank* enemyTank)
{
	cannonballSprite.setPosition(tankPos.x - 1 + 25 * sin((rotation*PI) / 180), tankPos.y - 25 * cos((rotation*PI) / 180));
	cannonballSprite.setRotation(rotation);
	bool answer = true;
	while (answer)
	{
		cannonballSprite.move(0.15 * sin((rotation*PI) / 180), -0.15 * cos((rotation*PI) / 180));
		std::this_thread::sleep_for(std::chrono::microseconds(1));
		
		if (objMap.longSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.lowerMiddleSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.lowerRightSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.lowerRightMiddleSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.cafeSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.upperRightSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.upperMiddleSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.middleUpperLeftSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.upperLeftSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.middleParkingSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.rightParkingSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.leftMiddleSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (enemyTank->tankSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			enemyTank->hits++;
			if (enemyTank->hits == hitsCountForDestroying)
			{
				enemyTank->tankTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\crushedTank.png");
				enemyTank->isCrushed = true;
				std::thread t(&MyTank::blast, this, enemyTank);
				t.detach();
			}
			break;
		}
			
		if (!objMap.isInside(window.getSize(), Vector2f(2, 2), cannonballSprite.getPosition(), cannonballSprite.getRotation()))
		{
			answer = false;
			break;
		}

		
	}
	fireSprite.setPosition(cannonballSprite.getPosition());
	for (int i = 1; i <= 15; i++)
	{
		fireSprite.setScale(Vector2f(0.1*i, 0.1*i));
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	
	for (int i = 15; i >= 5; i--)
	{
		fireSprite.setScale(Vector2f(0.1*i, 0.1*i));
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	fireSprite.setPosition(Vector2f(-100, -100));
	cannonballSprite.setPosition(Vector2f(-100, -100));
	
}

void MyTank::blast(EnemyTank* enemyTank)
{
	enemyTank->fireSprite.setPosition(enemyTank->getPosition());
	for (int i = 1; i <= 20; i++)
	{
		enemyTank->fireSprite.setScale(Vector2f(0.1*i, 0.1*i));
		std::this_thread::sleep_for(std::chrono::milliseconds(15));
	}

	for (int i = 20; i >= 5; i--)
	{
		enemyTank->fireSprite.setScale(Vector2f(0.1*i, 0.1*i));
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	enemyTank->fireSprite.setPosition(Vector2f(-100, -100));
}

EnemyTank::EnemyTank(MyTank* enemyTank)
{
	tankTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\tankk.png");
	cannonballTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\cannonball.png");
	fireTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\fire.png");
	timeTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\time.png");

	tankSprite.setTexture(tankTexture);
	cannonballSprite.setTexture(cannonballTexture);
	fireSprite.setTexture(fireTexture);
	timeSprite.setTexture(timeTexture);

	timeSprite.setPosition(Vector2f(-100, -100));
	fireSprite.setPosition(Vector2f(-100, -100));

	fireSprite.setOrigin(15, 15);

	setPosition(Vector2f(730, 37));
	setRotation(270);
	tankSprite.setOrigin(Vector2f(width/2, height/2));

	isShootable(enemyTank);
}

Vector2f EnemyTank::getPosition()
{
	return tankSprite.getPosition();
}


float EnemyTank::getRotation()
{
	return tankSprite.getRotation();
}

void EnemyTank::setPosition(Vector2f pos)
{
	tankSprite.setPosition(pos);
}

void EnemyTank::setRotation(float rotation)
{
	tankSprite.setRotation(rotation);
}

void EnemyTank::moveUnit(Vector2f oldPos, int firstDirection, bool timeNeedToMove, MyTank* enemyTank)
{
	if (pressedClock.getElapsedTime().asMilliseconds() > 10)
	{
		clock.restart();
	}
	

	float time = clock.getElapsedTime().asMicroseconds();
	float oldRotation = tankSprite.getRotation();
	switch (firstDirection)
	{
	case Forward:
	{

		if ((time / 1000000) < float(4))
		{
			if (checkNewPosition(Vector2f(oldPos.x + linearAcceleration * (time / 1000000)*sin((oldRotation*PI) / 180),
				(oldPos.y - linearAcceleration * (time / 1000000)*cos((oldRotation*PI) / 180))), (oldRotation*PI) / 180,
				enemyTank))
				tankSprite.setPosition(oldPos.x + linearAcceleration * (time / 1000000)*sin((oldRotation*PI) / 180),
				(oldPos.y - linearAcceleration * (time / 1000000)*cos((oldRotation*PI) / 180)));  // ускорение 
			if (timeNeedToMove)
				timeSprite.setPosition(oldPos.x - 20 + linearAcceleration * (time / 1000000)*sin((oldRotation*PI) / 180),
				(oldPos.y - 30 - linearAcceleration * (time / 1000000)*cos((oldRotation*PI) / 180)));
		}
		else if ((time / 1000000) >= 4)
		{
			if (checkNewPosition(Vector2f(oldPos.x + linearSpeed * sin((oldRotation*PI) / 180),
				(oldPos.y - linearSpeed * cos((oldRotation*PI) / 180))), (oldRotation*PI) / 180, enemyTank))
				tankSprite.setPosition(oldPos.x + linearSpeed * sin((oldRotation*PI) / 180),
				(oldPos.y - linearSpeed * cos((oldRotation*PI) / 180))); // постоянная скорость
			if (timeNeedToMove)
				timeSprite.setPosition(oldPos.x - 20 + linearSpeed * sin((oldRotation*PI) / 180),
				(oldPos.y - 30 - linearSpeed * cos((oldRotation*PI) / 180)));
		}
		break;
	}
	case Back:
	{
		if ((time / 1000000) < float(4))
		{
			if (checkNewPosition(Vector2f(oldPos.x - linearAcceleration * (time / 1000000)*sin((oldRotation*PI) / 180),
				(oldPos.y + linearAcceleration * (time / 1000000)*cos((oldRotation*PI) / 180))), (oldRotation*PI) / 180,
				enemyTank))
				tankSprite.setPosition(oldPos.x - linearAcceleration * (time / 1000000)*sin((oldRotation*PI) / 180),
				(oldPos.y + linearAcceleration * (time / 1000000)*cos((oldRotation*PI) / 180)));  // ускорение 
			if (timeNeedToMove)
				timeSprite.setPosition(oldPos.x - 20 - linearAcceleration * (time / 1000000)*sin((oldRotation*PI) / 180),
				(oldPos.y - 30 + linearAcceleration * (time / 1000000)*cos((oldRotation*PI) / 180)));
		}
		else if ((time / 1000000) >= 4)
		{
			if (checkNewPosition(Vector2f(oldPos.x - 0.09*sin((oldRotation*PI) / 180),
				(oldPos.y + linearSpeed * cos((oldRotation*PI) / 180))), (oldRotation*PI) / 180, enemyTank))
				tankSprite.setPosition(oldPos.x - linearSpeed * sin((oldRotation*PI) / 180),
				(oldPos.y + linearSpeed * cos((oldRotation*PI) / 180))); // постоянная скорость
			if (timeNeedToMove)
				timeSprite.setPosition(oldPos.x - 20 - linearSpeed * sin((oldRotation*PI) / 180),
				(oldPos.y - 30 + linearSpeed * cos((oldRotation*PI) / 180)));
		}
		break;
	}

	}
	pressedClock.restart();
}

void EnemyTank::moveUnit(float oldRotation, int SecondDirection, MyTank* enemyTank)
{
	switch (SecondDirection)
	{
	case Right:
	{
		if (checkNewPosition(tankSprite.getPosition(), tankSprite.getRotation() + float(angularSpeed), enemyTank))
			tankSprite.setRotation(oldRotation + float(angularSpeed));
		break;
	}
	case Left:
	{
		if (checkNewPosition(tankSprite.getPosition(), tankSprite.getRotation() - float(angularSpeed), enemyTank))
			tankSprite.setRotation(oldRotation - float(angularSpeed));
		break;
	}
	}
}

bool EnemyTank::checkNewPosition(Vector2f newPos, float rotation, MyTank* enemyTank)
{
	bool answer = true;
	RectangleShape tempRect;
	tempRect.setSize(Vector2f(width, height));
	tempRect.setOrigin(Vector2f(20.0, 25.0));
	tempRect.setRotation(getRotation() + rotation);
	tempRect.setPosition(newPos);

	float alpha = (((getRotation() + rotation)*PI) / 180);
	float arccos = acos((height / 2) / (sqrt(pow((height / 2), 2) + pow((width / 2), 2))));
	float cot = 1 / tan(arccos);

	Vector2f points[4];

	float dxa = -(width / 2)*(cos(alpha) + cot * sin(alpha));								//  
	float dya = ((height / 2)*sin((PI / 2) - arccos - alpha)) / (sin((PI / 2) + arccos));	//  B _______ C
	points[3] = Vector2f(newPos.x + dxa, newPos.y + dya);									//    |     |
																							//    |  .O |  - tank rect
	float dxb = -(width / 2)*(cos(alpha) - cot * sin(alpha));								//    |_____|  O - center
	float dyb = (-(height / 2)*sin((PI / 2) + arccos - alpha)) / (sin((PI / 2) + arccos));	//  A         D
	points[1] = Vector2f(newPos.x + dxb, newPos.y + dyb);									// dx and dy regarding O

	float dxc = (width / 2)*(cos(alpha) + cot * sin(alpha));
	float dyc = (-(height / 2)*sin((PI / 2) - arccos - alpha)) / (sin((PI / 2) + arccos));
	points[0] = Vector2f(newPos.x + dxc, newPos.y + dyc);

	float dxd = (width / 2)*(cos(alpha) - cot * sin(alpha));
	float dyd = ((height / 2)*sin((PI / 2) + arccos - alpha)) / (sin((PI / 2) + arccos));
	points[2] = Vector2f(newPos.x + dxd, newPos.y + dyd);

	for (int i = 0; i < 4; i++)
	{
		if (objMap.longSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.lowerMiddleSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.lowerRightSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.cafeSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.lowerRightMiddleSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.upperRightSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.upperMiddleSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.upperLeftSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.middleUpperLeftSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.middleParkingSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.rightParkingSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (objMap.leftMiddleSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
		if (enemyTank->tankSprite.getGlobalBounds().contains(points[i]))
		{
			answer = false;
			break;
		}
	}

	if (!objMap.isInside(window.getSize(), tempRect.getSize(), tempRect.getPosition(), tempRect.getRotation()))
		answer = false;
	return answer;
	
}

void EnemyTank::shoot(Vector2f tankPos, float rotation, MyTank* enemyTank)
{
	cannonballSprite.setPosition(tankPos.x - 1 + 25 * sin((rotation*PI) / 180), tankPos.y - 25 * cos((rotation*PI) / 180));
	cannonballSprite.setRotation(rotation);
	bool answer = true;
	while (answer)
	{
		cannonballSprite.move(0.15 * sin((rotation*PI) / 180), -0.15 * cos((rotation*PI) / 180));
		std::this_thread::sleep_for(std::chrono::microseconds(1));

		if (objMap.longSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.lowerMiddleSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.lowerRightSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.lowerRightMiddleSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.cafeSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.upperRightSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.upperMiddleSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.middleUpperLeftSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.upperLeftSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.middleParkingSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.rightParkingSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (objMap.leftMiddleSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			break;
		}
		if (enemyTank->tankSprite.getGlobalBounds().intersects(cannonballSprite.getGlobalBounds()))
		{
			answer = false;
			enemyTank->hits++;
			if (enemyTank->hits == hitsCountForDestroying)
			{
				enemyTank->tankTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\crushedTank.png");
				enemyTank->isCrushed = true;
				std::thread t(&EnemyTank::blast, this, enemyTank);
				t.detach();
			}

			break;
		}

		if (!objMap.isInside(window.getSize(), Vector2f(2, 2), cannonballSprite.getPosition(), cannonballSprite.getRotation()))
		{
			answer = false;
			break;
		}


	}
	fireSprite.setPosition(cannonballSprite.getPosition());
	for (int i = 1; i <= 15; i++)
	{
		fireSprite.setScale(Vector2f(0.1*i, 0.1*i));
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	for (int i = 15; i >= 5; i--)
	{
		fireSprite.setScale(Vector2f(0.1*i, 0.1*i));
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	fireSprite.setPosition(Vector2f(-100, -100));
	cannonballSprite.setPosition(Vector2f(-100, -100));

}

void EnemyTank::blast(MyTank* enemyTank)
{
	enemyTank->fireSprite.setPosition(enemyTank->getPosition());
	for (int i = 1; i <= 20; i++)
	{
		enemyTank->fireSprite.setScale(Vector2f(0.1*i, 0.1*i));
		std::this_thread::sleep_for(std::chrono::milliseconds(15));
	}

	for (int i = 20; i >= 5; i--)
	{
		enemyTank->fireSprite.setScale(Vector2f(0.1*i, 0.1*i));
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
	enemyTank->fireSprite.setPosition(Vector2f(-100, -100));
}

void EnemyTank::isShootable(MyTank* enemyTank)
{
	std::cout << "XAXAXAXA" << std::endl;
	Vector2f newPoint(enemyTank->getPosition().x - getPosition().x, enemyTank->getPosition().y - getPosition().y);

	RectangleShape beacon(Vector2f(1,1));
	beacon.setPosition(getPosition());
	
	if (newPoint.x <= 0)
		beacon.setRotation((atan(newPoint.y / newPoint.x) * 180 / PI) + 180);
	else if (newPoint.x > 0)
		beacon.setRotation((atan(newPoint.y / newPoint.x) * 180 / PI));
	
	bool answer = true;
	bool isShootable = false;
	while(answer)
	{
		beacon.move(0.5 * sin(((beacon.getRotation()+90)*PI) / 180), -0.5 * cos(((beacon.getRotation()+90)*PI) / 180));

		if (objMap.longSprite.getGlobalBounds().contains(Vector2f(beacon.getPosition().x, beacon.getPosition().y)))
		{
			answer = false;
			break;
		}
		if (objMap.lowerMiddleSprite.getGlobalBounds().contains(Vector2f(beacon.getPosition().x, beacon.getPosition().y)))
		{
			answer = false;
			break;
		}
		if (objMap.lowerRightSprite.getGlobalBounds().contains(Vector2f(beacon.getPosition().x, beacon.getPosition().y)))
		{
			answer = false;
			break;
		}
		if (objMap.lowerRightMiddleSprite.getGlobalBounds().contains(Vector2f(beacon.getPosition().x, beacon.getPosition().y)))
		{
			answer = false;
			break;
		}
		if (objMap.cafeSprite.getGlobalBounds().contains(Vector2f(beacon.getPosition().x, beacon.getPosition().y)))
		{
			answer = false;
			break;
		}
		if (objMap.upperRightSprite.getGlobalBounds().contains(Vector2f(beacon.getPosition().x, beacon.getPosition().y)))
		{
			answer = false;
			break;
		}
		if (objMap.upperMiddleSprite.getGlobalBounds().contains(Vector2f(beacon.getPosition().x, beacon.getPosition().y)))
		{
			answer = false;
			break;
		}
		if (objMap.middleUpperLeftSprite.getGlobalBounds().contains(Vector2f(beacon.getPosition().x, beacon.getPosition().y)))
		{
			answer = false;
			break;
		}
		if (objMap.upperLeftSprite.getGlobalBounds().contains(Vector2f(beacon.getPosition().x, beacon.getPosition().y)))
		{
			answer = false;
			break;
		}
		if (objMap.middleParkingSprite.getGlobalBounds().contains(Vector2f(beacon.getPosition().x, beacon.getPosition().y)))
		{
			answer = false;
			break;
		}
		if (objMap.rightParkingSprite.getGlobalBounds().contains(Vector2f(beacon.getPosition().x, beacon.getPosition().y)))
		{
			answer = false;
			break;
		}
		if (objMap.leftMiddleSprite.getGlobalBounds().contains(Vector2f(beacon.getPosition().x, beacon.getPosition().y)))
		{
			answer = false;
			break;
		}
		if (enemyTank->tankSprite.getGlobalBounds().contains(Vector2f(beacon.getPosition().x, beacon.getPosition().y)))
		{
			isShootable = true;
			answer = false;
			break;
		}
	}
	if (isShootable && rotateUnitAIIsDone)
	{
		std::thread t(&EnemyTank::rotateUnitAI, this, (beacon.getRotation() + 90), enemyTank);
		t.detach();
	}
	if (!isShootable)
	{
		moveUnitAI(EnemyTank::MovementToShoot, enemyTank);
		std::cout << "is not shootable " << std::endl;
	}

}

void EnemyTank::rotateUnitAI(float newRotation, MyTank* enemyTank)
{
	rotateUnitAIIsDone = false;

	float delta = round(newRotation - getRotation());
	bool right = false;
	bool left = false;
	if (delta != 0)
	{
		if (newRotation < getRotation())
		{
			left = true;
			right = false;
		}
		if (newRotation > getRotation())
		{
			right = true;
			left = false;
		}
		if (delta > 180)
		{
			delta = 360 - delta;
			left = true;
			right = false;
		}
		if (delta < -180)
		{
			delta = 360 + delta;
			right = true;
			left = false;
		}
		bool isBreaked = false;
		for (int i = 0; i < abs(delta * 2) - 1; i++)
		{
			if (checkNewPosition(getPosition(), (getRotation() + 0.5*right - 0.5*left), enemyTank))
			{
				tankSprite.setRotation(getRotation() + 0.5*right - 0.5*left);
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			else
			{
				moveUnitAI(EnemyTank::Blocked, enemyTank);
				isBreaked = true;
				break;	
			}
		}
		if (!isBreaked)
		{
			rotateUnitAIIsDone = true;
			isShootable(enemyTank);
		}
	}
	else shoot(getPosition(), getRotation(), enemyTank);
	rotateUnitAIIsDone = true;

	
}
void EnemyTank::moveUnitAI(int movingType, MyTank* enemyTank)
{
	if (movingType == Blocked)
	{
		std::cout << "Blocked Move " << std::endl;
	}
	if (movingType == MovementToShoot)
	{
		float beta = 0;
		for (int i = 0; i < 36; i++)
		{
			float rotation = i * 10;
			RectangleShape testBeacon(Vector2f(1,1));
			testBeacon.setPosition(enemyTank->getPosition().x - 1 + 25 * sin((rotation*PI) / 180), 
				enemyTank->getPosition().y - 25 * cos((rotation*PI) / 180));
			
			testBeacon.setRotation(rotation);
			bool answer = true;
			bool quit = false;
			while (answer)
			{
				testBeacon.move(0.5 * sin((rotation*PI) / 180), -0.5 * cos((rotation*PI) / 180));

				if (objMap.longSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
				{
					answer = false;
					break;
				}
				if (objMap.lowerMiddleSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
				{
					answer = false;
					break;
				}
				if (objMap.lowerRightSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
				{
					answer = false;
					break;
				}
				if (objMap.lowerRightMiddleSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
				{
					answer = false;
					break;
				}
				if (objMap.cafeSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
				{
					answer = false;
					break;
				}
				if (objMap.upperRightSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
				{
					answer = false;
					break;
				}
				if (objMap.upperMiddleSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
				{
					answer = false;
					break;
				}
				if (objMap.middleUpperLeftSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
				{
					answer = false;
					break;
				}
				if (objMap.upperLeftSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
				{
					answer = false;
					break;
				}
				if (objMap.middleParkingSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
				{
					answer = false;
					break;
				}
				if (objMap.rightParkingSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
				{
					answer = false;
					break;
				}
				if (objMap.leftMiddleSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
				{
					answer = false;
					break;
				}
				if (!objMap.isInside(window.getSize(), Vector2f(1, 1), testBeacon.getPosition(), testBeacon.getRotation()))
				{
					beta = i;
					quit = true;
					answer = false;
					break;
				}
			}
			if (quit) break;
		}

		std::cout << "beta = " << beta << std::endl;

		float alpha = 0;
		bool alphaIsReal = false;

		float rotation = getRotation();
		RectangleShape testBeacon(Vector2f(1, 1));
		testBeacon.setPosition(getPosition().x - 1 + 25 * sin((rotation*PI) / 180),
			getPosition().y - 25 * cos((rotation*PI) / 180));
		
		testBeacon.setRotation(rotation);
		bool answer = true;
		while (answer)
		{
			testBeacon.move(0.5 * sin((rotation*PI) / 180), -0.5 * cos((rotation*PI) / 180));

			if (objMap.longSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
			{
				answer = false;
				break;
			}
			if (objMap.lowerMiddleSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
			{
				answer = false;
				break;
			}
			if (objMap.lowerRightSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
			{
				answer = false;
				break;
			}
			if (objMap.lowerRightMiddleSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
			{
				answer = false;
				break;
			}
			if (objMap.cafeSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
			{
				answer = false;
				break;
			}
			if (objMap.upperRightSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
			{
				answer = false;
				break;
			}
			if (objMap.upperMiddleSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
			{
				answer = false;
				break;
			}
			if (objMap.middleUpperLeftSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
			{
				answer = false;
				break;
			}
			if (objMap.upperLeftSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
			{
				answer = false;
				break;
			}
			if (objMap.middleParkingSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
			{
				answer = false;
				break;
			}
			if (objMap.rightParkingSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
			{
				answer = false;
				break;
			}
			if (objMap.leftMiddleSprite.getGlobalBounds().intersects(testBeacon.getGlobalBounds()))
			{
				answer = false;
				break;
			}
			if (!objMap.isInside(window.getSize(), Vector2f(1, 1), testBeacon.getPosition(), testBeacon.getRotation()))
			{
				alpha = round(getRotation());
				alphaIsReal = true;
				answer = false;
				break;
			}
		}
		if (alphaIsReal)
			std::cout << "alpha " << alpha << std::endl;
		else std::cout << "alpha is not real " << std::endl;

		if (alphaIsReal)
		{
			float x_0 = 0;
			float y_0 = 0;
			
			if (((abs(alpha - 90) == 90) || (abs(alpha - 90) == 270)) && ((abs(beta - 90) == 90) || (abs(beta - 90) == 270)))
			{
				// ДВИЖЕНИЕ ПО УМОЛЧАНИЮ ИЛИ СТОЯТЬ
			}
			else if ((((alpha - 90) == 0) || (abs(alpha - 90) == 180) || (abs(alpha - 90) == 360)) &&
				(((beta - 90) == 0) || (abs(beta - 90) == 180) || (abs(beta - 90) == 360)))
			{
				// ДВИЖЕНИЕ ПО УМОЛЧАНИЮ ИЛИ СТОЯТЬ
			}
			else if (((abs(alpha - 90) == 90) || (abs(alpha - 90) == 270)) && ((beta - 90) != 0) && (abs(beta - 90) != 180)
				&& (abs(beta - 90) != 90) && (abs(beta - 90) != 270) && (abs(beta - 90) != 360))
			{
				x_0 = getPosition().x;
				y_0 = tan(((beta - 90)*PI) / 180)*(x_0 - enemyTank->getPosition().x) + enemyTank->getPosition().y;
			}
			else if (((abs(alpha - 90) == 180) || (abs(alpha - 90) == 360) || ((alpha - 90) == 0)) && ((beta - 90) != 0) && (abs(beta - 90) != 180)
				&& (abs(beta - 90) != 90) && (abs(beta - 90) != 270) && (abs(beta - 90) != 360))
			{
				y_0 = getPosition().y;
				x_0 = (getPosition().y - enemyTank->getPosition().y + tan(((beta - 90)*PI) / 180)*enemyTank->getPosition().x) / (tan(((beta - 90)*PI) / 180));
			}
			else if (((abs(beta - 90) == 90) || (abs(beta - 90) == 270)) && ((alpha - 90) != 0) && (abs(alpha - 90) != 180)
				&& (abs(alpha - 90) != 90) && (abs(alpha - 90) != 270) && (abs(alpha - 90) != 360))
			{
				x_0 = enemyTank->getPosition().x;
				y_0 = tan(((alpha - 90)*PI) / 180)*(x_0 - getPosition().x) + getPosition().y;
			}
			else if (((abs(beta - 90) == 180) || (abs(beta - 90) == 360) || ((beta - 90) == 0)) && ((alpha - 90) != 0) && (abs(alpha - 90) != 180)
				&& (abs(alpha - 90) != 90) && (abs(alpha - 90) != 270) && (abs(alpha - 90) != 360))
			{
				y_0 = enemyTank->getPosition().y;
				x_0 = (enemyTank->getPosition().y - getPosition().y + tan(((alpha - 90)*PI) / 180)*getPosition().x) / (tan(((alpha - 90)*PI) / 180));
			}
			else if (tan(((alpha - 90)*PI) / 180) == tan(((beta - 90)*PI) / 180))
			{
				// ДВИЖЕНИЕ ПО УМОЛЧАНИЮ ИЛИ СТОЯТЬ
			}
			else
			{
				x_0 = round((enemyTank->getPosition().y - getPosition().y + tan(((alpha - 90)*PI) / 180)*getPosition().x - tan(((beta - 90)*PI) / 180)*enemyTank->getPosition().x) / (tan(((alpha - 90)*PI) / 180) - tan(((beta - 90)*PI) / 180)));
				y_0 = round(tan(((alpha - 90)*PI) / 180)*(x_0 - getPosition().x) + getPosition().y);
			}

			Vector2f commonPoint(x_0, y_0);

			std::cout << "x_0 " << x_0 << " y_0 " << y_0 << std::endl;
















			std::thread t(&EnemyTank::movingUnitAI, this, commonPoint, enemyTank);
			t.detach();
		}
		else 
		{
			// ПОВЕРНУТЬСЯ ЧТОБЫ АЛЬФА СТАЛ РЕАЛЬНЫМ
		}
		std::cout << "movement to shoot " << std::endl;
	}
}

void EnemyTank::movingUnitAI(Vector2f newPos, MyTank* enemyTank)
{
	std::cout << "newPos.x " << newPos.x << " newPos.y " << newPos.y << std::endl;
	std::cout << "get x " << getPosition().x << " get y " << getPosition().y << std::endl;

	/////// НАПИСАТЬ ФУНКИЦИЮ ДВИЖЕНИЯ ПРАВИЛЬНУЮ С ЗАДАННЫМ ЧИСЛОМ ПОВТОРОРЕНИЙ ЦИКЛА

	
}

Map::Map()
{
	mapTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\map.png");

	longTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\long.png");
	lowerMiddleTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\lowerMiddle.png");
	lowerRightTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\lowerRight.png"); 
	rightParkingTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\rightParking.png");
	middleParkingTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\middleParking.png");
	leftMiddleTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\leftMiddle.png");
	lowerRightMiddleTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\lowerRightMiddle.png");
	cafeTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\cafe.png");
	upperMiddleTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\upperMiddle.png");
	middleUpperLeftTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\middleUpperLeft.png");
	upperLeftTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\upperLeft.png");
	upperRightTexture.loadFromFile("C:\\Users\\111\\source\\repos\\images\\upperRight.png");

	

	mapSprite.setTexture(mapTexture);
	mapSprite.setPosition(Vector2f(0, 0));

	longSprite.setTexture(longTexture);
	longSprite.setPosition(Vector2f(0,459));

	lowerMiddleSprite.setTexture(lowerMiddleTexture);
	lowerMiddleSprite.setPosition(Vector2f(435,485));

	lowerRightSprite.setTexture(lowerRightTexture);
	lowerRightSprite.setPosition(Vector2f(666, 485));

	rightParkingSprite.setTexture(rightParkingTexture);
	rightParkingSprite.setPosition(Vector2f(104, 325));

	middleParkingSprite.setTexture(middleParkingTexture);
	middleParkingSprite.setPosition(Vector2f(0, 250));

	leftMiddleSprite.setTexture(leftMiddleTexture);
	leftMiddleSprite.setPosition(Vector2f(239, 353));

	lowerRightMiddleSprite.setTexture(lowerRightMiddleTexture);
	lowerRightMiddleSprite.setPosition(Vector2f(435, 354));
	
	cafeSprite.setTexture(cafeTexture);
	cafeSprite.setPosition(Vector2f(590, 270));
	
	upperMiddleSprite.setTexture(upperMiddleTexture);
	upperMiddleSprite.setPosition(Vector2f(435, 163));

	middleUpperLeftSprite.setTexture(middleUpperLeftTexture);
	middleUpperLeftSprite.setPosition(Vector2f(241, 187));

	upperLeftSprite.setTexture(upperLeftTexture);
	upperLeftSprite.setPosition(Vector2f(6, 75));

	upperRightSprite.setTexture(upperRightTexture);
	upperRightSprite.setPosition(Vector2f(677, 76));
	
}
bool Map::isInside(Vector2u windowSize, Vector2f objectSize, Vector2f objectPos, float objectRotation)
{
	bool isInside = true;

	RectangleShape object;
	object.setSize(objectSize);
	object.setPosition(objectPos);
	object.setRotation(objectRotation);
	if (objectSize == Vector2f(40,40))
		object.setOrigin(Vector2f(20, 20));
	else if (objectSize == Vector2f(2, 2))
		object.setOrigin(Vector2f(1, 1));

	RectangleShape upperBorder;
	upperBorder.setSize(Vector2f(windowSize.x, 1));
	upperBorder.setPosition(Vector2f(0, -1));

	RectangleShape leftBorder;
	leftBorder.setSize(Vector2f(1, float(windowSize.y)));
	leftBorder.setPosition(Vector2f(-1, 0));

	RectangleShape rightBorder;
	rightBorder.setSize(Vector2f(10, float(windowSize.y)));
	rightBorder.setPosition(Vector2f(800, 0));

	RectangleShape lowerBorder;
	lowerBorder.setSize(Vector2f(float(windowSize.x), 1));
	lowerBorder.setPosition(Vector2f(0, 600));

	if (upperBorder.getGlobalBounds().intersects(object.getGlobalBounds()))
		isInside = false;

	if (leftBorder.getGlobalBounds().intersects(object.getGlobalBounds()))
		isInside = false;

	if (rightBorder.getGlobalBounds().intersects(object.getGlobalBounds()))
		isInside = false;

	if (lowerBorder.getGlobalBounds().intersects(object.getGlobalBounds()))
		isInside = false;


	return isInside;
}
Vector2f Tank::getPosition() { return Vector2f(0, 0); }
float Tank::getRotation() { return 0.0; }
void Tank::setPosition(Vector2f pos) {}
void Tank::setRotation(float rotation) {}
//void Tank::moveUnit(Vector2f oldPos, int firstDirection, bool timeNeedToMove) {}
//void Tank::moveUnit(float oldRotation, int SecondDirection) {}
//bool Tank::checkNewPosition(Vector2f newPos, float rotation) { return 0; }
//void Tank::shoot(Vector2f tankPos, float rotation) {}

