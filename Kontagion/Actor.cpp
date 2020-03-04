#include "Actor.h"
#include "StudentWorld.h"

#define PI 4* atan(1)

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


Actor::Actor(double x, double y, int id, StudentWorld* s, bool alive = true , Direction direction = 0, int depth = 1) : GraphObject(id, x, y, alive, direction, depth)
{
	this->id = id;
	this->alive = alive;
	this->direction = direction;
	this->x = x;
	this->y = y;
	this->depth = depth;
	this->location = s;

}

StudentWorld* Actor::getLocation() const
{
	return location;
}


double Actor::getDistance(double x1, double x2, double y1, double y2) const
{
	return sqrt(pow(x1- x2,2) + pow(y1 - y2, 2));
}

bool Actor::isAlive() const
{
	return alive;
}

void Actor::setDepth(int n) 
{
	depth = n;
}

void Actor::setAlive(bool b)
{
	alive = b;
}


bool Actor::canOverlap()
{
	return false;
}


bool Actor::isMain()
{
	return false;
}

bool Actor::isDamageable()
{
	return true;
}

bool Actor::blocksAgent()
{
	return false;
}

bool Actor::blocksProjectile()
{
	return false;
}

bool Actor::isBacFood()
{
	return false;
}

bool Actor::canMove()
{
	return false;
}

bool Actor::canProduce()
{
	return false;
}





Socrates::Socrates(double x, double y, StudentWorld * s): Agent(x, y, IID_PLAYER,s) {
	
	sprayammo = 20;
	flameammo = 5;
	setDirection(0);
	setHitpoints(100);
	
	
}

bool Socrates::isMain()
{
	return true;
}



int Agent::getHitpoints() const {
	return hitpoints;
}
void Agent::setHitpoints(int x)
{
	hitpoints = x;
	if (hitpoints <= 0)
		setAlive(false);
	if (hitpoints > 100)
		hitpoints = 100;
}
void Agent::damaged(int x)
{
	int hp = getHitpoints() - x;
	if (isAlive() && hp > 0) {
		setHitpoints(hp);
		return;
	}
	setAlive(false);
	return;
}
bool Agent::canMove()
{
	return true;
}
void Socrates::doSomething()
{
	if (!getHitpoints()) {
		return;
	}
	int c;
	Direction d;
	if (getLocation()->getKey(c)) {
		double x, y;
		//std::cout << c << "Sdfsdf" << std::endl;
		switch (c) {

		case(KEY_PRESS_LEFT):
			d = getDirection() + 5;
			setDirection(d);
			x = VIEW_RADIUS * cos((d + 180) * PI / 180) + VIEW_RADIUS;
			y = VIEW_RADIUS * sin((d + 180) * PI / 180) + VIEW_RADIUS;
			moveTo(x, y);
			break;
		case(KEY_PRESS_RIGHT):
			d = getDirection() - 5;
			setDirection(d);
			x = VIEW_RADIUS * cos((d + 180) * PI / 180) + VIEW_RADIUS;
			y = VIEW_RADIUS * sin((d + 180) * PI / 180) + VIEW_RADIUS;
			moveTo(x, y);
			break;
		case(KEY_PRESS_SPACE):
			d = getDirection();
			int distance;
			if (sprayammo > 0) {
				/*x = VIEW_RADIUS * cos((d + 180) * PI / 180) + VIEW_RADIUS;
				y = VIEW_RADIUS * sin((d + 180) * PI / 180) + VIEW_RADIUS;*/
				x = SPRITE_WIDTH / 2 * cos((d)*PI / 180) + getX();
				y = SPRITE_WIDTH / 2 * sin((d)*PI / 180) + getY();
				getLocation()->inserttoField(new DisinfectantSpray(x, y, d, getLocation()));
				hasSprayed = true;
				sprayammo--;
			}
			break;

		case(KEY_PRESS_ENTER):
			
			if (flameammo > 0) {
				d = getDirection();
				for (int i = 0; i < 16; i++) {
					x = SPRITE_WIDTH  * cos((d)*PI / 180) + getX();
					y = SPRITE_WIDTH  * sin((d)*PI / 180) + getY();
					getLocation()->inserttoField(new Flame(x, y, d, getLocation()));
					d += 22;
					getLocation()->playSound(SOUND_PLAYER_FIRE);
				}
				flameammo--;
				break;
			}
			break;
		}
	}
	else{
		if (!hasSprayed && getSprayammo() < 20)
			sprayammo++;
		hasSprayed = false;
	}
	
	return;

}

int Socrates::getSprayammo()
{
	return sprayammo;
}

void Socrates::setSprayammo(int x)
{
	sprayammo = x;
	if (sprayammo > 20)
		sprayammo = 20;
}

int Socrates::getFlameammo()
{
	return flameammo;
}

void Socrates::setFlameammo(int x)
{
	flameammo = x;
	if (flameammo > 5)
		flameammo = 5;
}

void Socrates::damaged(int x)
{
	setHitpoints(getHitpoints() - x);
	if (isAlive()) {
		getLocation()->playSound(SOUND_PLAYER_HURT);
	}
	getLocation()->playSound(SOUND_PLAYER_DIE);
	setAlive(false);
}






Dirt::Dirt(double x, double y,StudentWorld * s): Actor(x, y, IID_DIRT,s)
{
}

void Dirt::doSomething()
{
	return;
}

bool Dirt::canOverlap()
{
	return true;
}



void Dirt::damaged(int x)
{
	setAlive(false);
	return;
}

bool Dirt::blocksAgent()
{
	return true;
}

bool Dirt::blocksProjectile()
{
	return true;
}



Food::Food(double x, double y, StudentWorld* s) : Actor(x, y, IID_FOOD, s)
{
	setDepth(1);
	setDirection(90);
}

void Food::doSomething()
{
	return;
}

bool Food::isBacFood()
{
	return true;
}

void Food::damaged(int x)
{
	setAlive(false);
	return;
}

bool Food::isDamageable()
{
	return false;
}




Flame::Flame(double x, double y,Direction d ,StudentWorld* s) : Projectile(x,y,IID_FLAME,s)
{
	setTravel(32);
	setDirection(d);
}

void Flame::doSomething()
{
	if (!isAlive())
		return;
	Actor* temp = nullptr;
	if (getLocation()->searchOtherOverlap(this, temp)) {
		if (temp->isDamageable()) {
			temp->damaged(5);
			setAlive(false);
			return;
		}
	
	}
	int x = SPRITE_WIDTH * cos(getDirection() * PI / 180) + getX();
	int y = SPRITE_WIDTH * sin(getDirection() * PI / 180) + getY();
	moveTo(x,y);
	setTravel( getTravel()- SPRITE_WIDTH);
	if (getTravel() == 0)
		setAlive(false);
	return;

}



void Projectile::setTravel(int x)
{
	travel = x;
}

int Projectile::getTravel() const
{
	return travel;
}

bool Projectile::isDamageable()
{
	return false;
}

Projectile::Projectile(double x, double y,int id, StudentWorld* s): Actor(x,y,id,s)
{
	
}

DisinfectantSpray::DisinfectantSpray(double x, double y, Direction d, StudentWorld* s): Projectile(x, y, IID_SPRAY, s)
{
	setDirection(d);
	setTravel(112);
}

void DisinfectantSpray::doSomething()
{
	if (!isAlive())
		return;
	Actor* temp = nullptr;
	if (getLocation()->searchOtherOverlap(this, temp)) {
		if (temp->isDamageable()) {
			temp->damaged(2);
			setAlive(false);
			return;
		}
	}
	int x = SPRITE_WIDTH * cos(getDirection() * PI / 180) + getX();
	int y = SPRITE_WIDTH * sin(getDirection() * PI / 180) + getY();
	moveTo(x, y);
	setTravel(getTravel() - SPRITE_WIDTH);
	if (getTravel() == 0)
		setAlive(false);
	return;
}

Goodie::Goodie(int x, int y, int id, StudentWorld* s): Actor(x, y, id, s)
{
	int level = getLocation()->getLevel();
	int maxLifeTime = randInt(0, 300 - 10 * level - 1);
	lifetime = maxLifeTime > 50 ? 50 : maxLifeTime;
}



void Goodie::setLifetime(int x)
{
	lifetime = x;
}

int Goodie::getLifetime() const
{
	return lifetime;
}


void Goodie::damaged(int x)
{
	setAlive(false);
}

RestoreHealthGoodie::RestoreHealthGoodie(double x, double y,StudentWorld* s): Goodie(x,y,IID_RESTORE_HEALTH_GOODIE,s)
{

}

void RestoreHealthGoodie::doSomething()
{
	setLifetime(getLifetime() - 1);
	if (!isAlive())
		return;
	if (getLocation()->overlapSocrates(this)) {
		int points = getLocation()->getPoints();
		getLocation()->setPoints(points + 250);
		setAlive(false);
		getLocation()->playSound(SOUND_GOT_GOODIE);
		getLocation()->setSocHealth(100);
		return;
	}
	if (!getLifetime())
		setAlive(false);
	return;



}

FlamethrowerGoodie::FlamethrowerGoodie(double x, double y, StudentWorld* s): Goodie(x,y,IID_FLAME_THROWER_GOODIE,s)
{
	
}

void FlamethrowerGoodie::doSomething()
{
	if (!isAlive())
		return;
	if (getLocation()->overlapSocrates(this)) {
		int points = getLocation()->getPoints();
		setAlive(false);
		getLocation()->setPoints(points + 100);
		getLocation()->setFlames(5);
		setLifetime(getLifetime() - 1);
		getLocation()->playSound(SOUND_GOT_GOODIE);
		return;
	}
	if (!getLifetime())
		setAlive(false);
	return;
}

ExtraLifeGoodie::ExtraLifeGoodie(double x, double y, StudentWorld* s) : Goodie(x, y, IID_EXTRA_LIFE_GOODIE, s)
{

}

void ExtraLifeGoodie::doSomething()
{
	if (!isAlive())
		return;
	if (getLocation()->overlapSocrates(this)) {
		int points = getLocation()->getPoints();
		getLocation()->setPoints(points + 500);
		setAlive(false);
		getLocation()->incLives();
		getLocation()->playSound(SOUND_GOT_GOODIE);
		setLifetime(getLifetime() - 1);
		return;
		
	}
	if (!getLifetime())
		setAlive(false);
	return;
}

Fungus::Fungus(double x, double y, StudentWorld* s) : Goodie(x, y, IID_FUNGUS, s)
{
}

void Fungus::doSomething()
{
	if (!isAlive())
		return;
	if (getLocation()->overlapSocrates(this)) {
		int points = getLocation()->getPoints();
		getLocation()->setPoints(points - 50);
		setAlive(false);
		getLocation()->overlapSocrates(this, -20);
		getLocation()->playSound(SOUND_GOT_GOODIE);
		return;
	}
	if (!getLifetime())
		setAlive(false);
	return;
}

Agent::Agent(double x, double y, int id, StudentWorld* s): Actor(x,y,id,s)
{
}

Bacterium::Bacterium(double x, double y, int id, StudentWorld* s): Agent(x, y, id, s)
{
	setDirection(90);

	setMovementPlan(0);
}

void Bacterium::setMovementPlan(int x)
{
	movementPlan = x;
}

int Bacterium::getMovementPlan()
{
	return movementPlan;
}

int Bacterium::getdamage()
{
	return damage;
}

void Bacterium::setdamage(int x)
{
	damage = x;
}




int Bacterium::getFood()
{
	return foodate;
}

void Bacterium::setFood(int x)
{
	foodate = x;
}

void Bacterium::moveBac()
{
	double x, y;
	Actor* c = nullptr;
	if (getMovementPlan() > 0) {

		setMovementPlan(getMovementPlan() - 1);
		x = 3 * cos(getDirection() * PI / 180) + getX();
		y = 3 * sin(getDirection() * PI / 180) + getY();
		
		Dirt d(x, y, getLocation());
		Actor* h = &d;
		if (getLocation()->searchMovementOverlap(h, c) || sqrt(pow(x - VIEW_RADIUS, 2) + pow(y - VIEW_RADIUS, 2)) >= VIEW_RADIUS) {
			int rand = randInt(0, 359);
			setDirection(rand);
			setMovementPlan(10);
			return;
		}
		else {
			moveTo(x, y);
			return;
		}
	}
	else {
		Direction angle;
		if (getLocation()->findFood(this, angle)) {
			setDirection(angle);
			x = 3 * cos(getDirection() * PI / 180) + getX();
			y = 3 * sin(getDirection() * PI / 180) + getY();
			Dirt d(x, y, getLocation());
			Actor* h = &d;
			if (getLocation()->searchMovementOverlap(h, c) || sqrt(pow(x - VIEW_RADIUS, 2) + pow(y - VIEW_RADIUS, 2)) >= VIEW_RADIUS) {
				int rand = randInt(0, 359);
				setDirection(rand);
				setMovementPlan(10);
				return;
			}

		}
		int rand = randInt(0, 359);
		setDirection(rand);
		setMovementPlan(10);
		return;


		
	}

	return;
}




RegularSalmonella::RegularSalmonella(double x, double y, StudentWorld* s) : Salmonella(x, y, IID_SALMONELLA, s)
{

	setHitpoints(4);
	setdamage(4);
}

	
void RegularSalmonella::doSomething()
{
	if (!isAlive())
		return;
	Actor* c = nullptr;
	if (getLocation()->overlapSocrates(this, getdamage())) {
		}
		
		else
		{
			moveBac();
			return;
		}

		if (getFood() == 3) {
			double x = getX() < VIEW_WIDTH / 2 ? getX() + SPRITE_WIDTH / 2 : (getX() == VIEW_WIDTH / 2 ? getX() : getX() + SPRITE_WIDTH);
			double y = getY() < VIEW_HEIGHT / 2 ? getY() + SPRITE_WIDTH / 2 : (getY() == VIEW_HEIGHT / 2 ? getY() : getY() + SPRITE_WIDTH);
			getLocation()->inserttoField(new RegularSalmonella(x, y, getLocation()));
			setFood(0);
			moveBac();
			return;
		}
		c = nullptr;
		if (getLocation()->eatFood(this)) {
			setFood(getFood() + 1);
		}
		moveBac();
		return;

	}


void Salmonella::damaged(int x)
{
	int hp = getHitpoints() - x;
	setHitpoints(hp);
	if (hp > 0) {
		getLocation()->playSound(SOUND_SALMONELLA_HURT);
		return;
	}
	setAlive(false);
	getLocation()->playSound(SOUND_SALMONELLA_DIE);
	getLocation()->increaseScore(getLocation()->getScore() + 100);
	int rand = randInt(0, 1);
	if (rand) {
		getLocation()->inserttoField(new Food(getX(), getY(), getLocation()));
	}
	return;

}



AggressiveSalmonella::AggressiveSalmonella(double x, double y, StudentWorld* s) : Salmonella(x, y, IID_SALMONELLA, s)
{
	setdamage(2);
	setHitpoints(10);
}

void AggressiveSalmonella::doSomething()
{
	if (!isAlive())
		return;
	
	if (getLocation()->getSocdist(this) <= 72) {
		Direction angle = getLocation()->findSocrates(this) ;
		setDirection(angle );
		double x = 3 * cos(getDirection() * PI / 180) + getX();
		double y = 3 * sin(getDirection() * PI / 180) + getY();
		Actor* c = nullptr;
		
		RegularSalmonella d(x, y, getLocation());
		Actor * a = &d;
		if (!(getLocation()->overlapreturn(this,c) && c->canOverlap())) {
			if (sqrt(pow(x - VIEW_RADIUS, 2) + pow(y - VIEW_RADIUS, 2)) >= VIEW_RADIUS) {
				if (getLocation()->overlapSocrates(this,-2)) {
				}
				if (getFood() == 3) {
					x = getX() < VIEW_WIDTH / 2 ? getX() + SPRITE_WIDTH / 2 : (getX() == VIEW_WIDTH / 2 ? getX() : getX() + SPRITE_WIDTH);
					y = getY() < VIEW_HEIGHT / 2 ? getY() + SPRITE_WIDTH / 2 : (getY() == VIEW_HEIGHT / 2 ? getY() : getY() + SPRITE_WIDTH);
					getLocation()->inserttoField(new AggressiveSalmonella(x, y, getLocation()));
					setFood(0);
					return;
				}
				if (getLocation()->eatFood(this))
					setFood(getFood() + 1);
				return;

			}
	
		
		}
		else {
			moveTo(x, y);
			moveIsZero();
			return;
		}
	}
	moveIsZero();
	
	return;
	
	

}
//
//void AggressiveSalmonella::moveAggroBac()
//{
//	double x, y;
//	Actor* c = nullptr;
//	if (getMovementPlan() > 0) {
//
//		setMovementPlan(getMovementPlan() - 1);
//		x = 3 * cos(getDirection() * PI / 180) + getX();
//		y = 3 * sin(getDirection() * PI / 180) + getY();
//		
//		Dirt d(x, y, getLocation());
//		Actor* h = &d;
//		if (getLocation()->searchMovementOverlap(h, c) || sqrt(pow(x - VIEW_WIDTH/2, 2) + pow(y - VIEW_HEIGHT/2, 2)) >= VIEW_RADIUS) {
//			if (c->canOverlap()) {
//				int rand = randInt(0, 359);
//				setDirection(rand);
//				setMovementPlan(10);
//				return;
//			}
//		}
//		else {
//			moveTo(x, y);
//			return;
//		}
//	}
//	else {
//		Direction angle;
//		int rand = randInt(0, 359);
//		if (getLocation()->findFood(this, angle)) {
//			setDirection(angle);
//			x = 3 * cos(getDirection() * PI / 180) + getX();
//			y = 3 * sin(getDirection() * PI / 180) + getY();
//			Dirt d(x, y, getLocation());
//			Actor* h = &d;
//			if (getLocation()->searchMovementOverlap(h, c) && h->canOverlap()) {
//				setDirection(rand);
//				setMovementPlan(10);
//				return;
//			}
//			else 
//				moveTo(x, y);
//			return;
//		}
//		else {
//			
//			setDirection(rand);
//			setMovementPlan(10);
//			return;
//		}
//	}
//	
//}



void AggressiveSalmonella::moveIsZero()
{
	Actor* c = nullptr;
	if (getLocation()->overlapSocrates(this,getdamage())) {
		moveBac();
		return;
	}
	if (getFood() == 3) {
		double x = getX() < VIEW_WIDTH / 2 ? getX() + SPRITE_WIDTH / 2 : (getX() == VIEW_WIDTH / 2 ? getX() : getX() + SPRITE_WIDTH);
		double y = getY() < VIEW_HEIGHT / 2 ? getY() + SPRITE_WIDTH / 2 : (getY() == VIEW_HEIGHT / 2 ? getY() : getY() + SPRITE_WIDTH);
		getLocation()->inserttoField(new AggressiveSalmonella(x, y, getLocation()));
		setFood(0);	
		moveBac();
		return;
		
	}
	if (getLocation()->eatFood(this)) {
		setFood(getFood() + 1);
		return;
	}
	moveBac();
	return;
}

Ecoli::Ecoli(double x, double y, StudentWorld* s) : Bacterium(x, y, IID_ECOLI, s)
{
	setHitpoints(5);
	setDepth(0);
}

void Ecoli::doSomething()
{
	
	if (!isAlive())
		return;
	
	if (getLocation()->overlapSocrates(this)){
		moveEco();
		return;
		
	}

	if (getFood() == 3) {
		
		double x = getX() < VIEW_WIDTH / 2 ? getX() + SPRITE_WIDTH / 2 : (getX() == VIEW_WIDTH / 2 ? getX() : getX() + SPRITE_WIDTH);
		double y = getY() < VIEW_HEIGHT / 2 ? getY() + SPRITE_WIDTH / 2 : (getY() == VIEW_HEIGHT / 2 ? getY() : getY() + SPRITE_WIDTH);
		getLocation()->inserttoField(new RegularSalmonella(x, y, getLocation()));
		setFood(0);
		moveEco();
		return;
	}
	if (getLocation()->eatFood(this)) {
		setFood(getFood() + 1);	
	}
	moveEco();
	return;
}

void Ecoli::moveEco()
{
	
	if (getLocation()->getSocdist(this)<= 256) {
		Actor* c;
		Direction angle = getLocation()->findSocrates(this);
		double x, y;
		for (int i = 0; i < 10; i++) {
			setDirection(angle);
			x = 2 * cos(getDirection() * PI / 180) + getX();
			y = 2 * sin(getDirection() * PI / 180) + getY();
			Ecoli d(x, y, getLocation());
			Actor* h = &d;
			if (getLocation()->overlapreturn(h,c)) {
				if (!c->canOverlap()) {
					moveTo(x, y);
					return;
				}
			}
			else {
				angle += 10;
			
			}
		}
	}

	return;



}

void Ecoli::damaged(int x)
{
	int hp = getHitpoints() - x;
	setHitpoints(hp);
	if (hp > 0) {
		getLocation()->playSound(SOUND_ECOLI_HURT);
		return;
	}
	setAlive(false);
	getLocation()->playSound(SOUND_ECOLI_DIE);
	getLocation()->increaseScore(getLocation()->getScore() + 100);
	int rand = randInt(0, 1);
	if (rand) {
		getLocation()->inserttoField(new Food(getX(), getY(), getLocation()));
	}
	return;
}

Pit::Pit(double x, double y, StudentWorld* s) : Actor(x, y, IID_PIT, s)
{

	storage[aggro] = 3;
	storage[eco] = 2;
	storage[reg] = 5;
}






void Pit::doSomething()
{
	if (!canProduce()) {
		setAlive(false);
		return;
	}
	int chance = randInt(1, 50);
	if (chance == 1) {
		int random = randInt(0, 2);
		while (!storage[random]) {
			random = randInt(0, 2);
		}
		switch (random) {
		case(aggro):
			getLocation()->inserttoField(new AggressiveSalmonella(getX(), getY(), getLocation()));
			break;
		case(reg):
			getLocation()->inserttoField(new RegularSalmonella(getX(), getY(), getLocation()));
			break;
		case(eco):
			getLocation()->inserttoField(new Ecoli(getX(), getY(), getLocation()));
			break;
		}
		getLocation()->playSound(SOUND_BACTERIUM_BORN);
		storage[random] -= 1;
		
	}
	return;
}

bool Pit::isDamageable()
{
	return false;
}

bool Pit::canProduce()
{
	return storage[0]+ storage[1] +storage[2];
}



Salmonella::Salmonella(double x, double y, int id, StudentWorld* s): Bacterium(x,y,id,s)
{
}
