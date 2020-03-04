#ifndef ACTOR_H_
#define ACTOR_H_
#include <vector>;
#include <map>;
#include "GraphObject.h"

class StudentWorld;
class Actor: public GraphObject{
	int x, y;
	int id;
	int depth;
	Direction direction;
	bool alive;
	StudentWorld* location;
	
public:

	Actor(double x, double y, int id, StudentWorld *s, bool alive , Direction direction , int depth );
	virtual ~Actor() {}
	virtual void doSomething() =0;
	StudentWorld* getLocation() const;
	
	double getDistance(double x1, double x2, double y1, double y2) const;
	bool isAlive() const;
	void setDepth(int n);
	void setAlive(bool b);
	virtual bool canOverlap();
	virtual bool isMain();
	virtual bool isDamageable();
	virtual void damaged(int x) {};
	virtual bool blocksAgent();
	virtual bool blocksProjectile();
	virtual bool isBacFood();
	virtual bool canMove();
	virtual bool canProduce();

};


class Agent : public Actor {
	int hitpoints;
public:
	Agent(double x, double y, int id, StudentWorld* s);
	virtual int getHitpoints() const;
	void setHitpoints(int x);
	virtual void damaged(int x);
	virtual bool canMove();
};

class Socrates : public Agent {
	int sprayammo;
	int flameammo;
	bool hasSprayed = false;
public:
	Socrates(double x, double y, StudentWorld* s);
	virtual bool isMain();
	//virtual void damaged(int x);
	virtual void doSomething();
	int getSprayammo();
	void setSprayammo(int x);
	int getFlameammo();
	void setFlameammo(int x);
	virtual void damaged(int x);

};

class Dirt : public Actor {

public:
	Dirt(double x, double y, StudentWorld * s);
	virtual void doSomething();
	virtual bool canOverlap();
	virtual void damaged(int x);
	virtual bool blocksAgent();
	virtual bool blocksProjectile();
};


class Food: public Actor {
public:
	Food(double x, double y, StudentWorld * s);
	virtual void doSomething();
	virtual bool isBacFood();
	virtual void damaged(int x);
	virtual bool isDamageable();
};

class Projectile: public Actor {
	int travel;
public:
	Projectile(double x, double y, int id, StudentWorld* s); 
	void setTravel(int x);
	int getTravel() const;
	virtual bool isDamageable();
};


class Flame :public Projectile {
public:
	Flame(double x,double y,Direction d, StudentWorld* s);
	virtual void doSomething();
};

class DisinfectantSpray :public Projectile {
public:
	DisinfectantSpray(double x, double y, Direction d, StudentWorld* s);
	virtual void doSomething();
};


class Goodie : public Actor{
	int lifetime;
public:
	Goodie(int x, int y, int id, StudentWorld* s);
	void setLifetime(int x);
	int getLifetime() const;
	virtual void damaged(int x);
};

class RestoreHealthGoodie : public Goodie {	
public:
	RestoreHealthGoodie(double x,double y, StudentWorld * s);
	virtual void doSomething();
};

class FlamethrowerGoodie : public Goodie {
public:
	FlamethrowerGoodie(double x,double y, StudentWorld * s);
	virtual void doSomething();
};

class ExtraLifeGoodie : public Goodie {

public:
	ExtraLifeGoodie(double x, double y, StudentWorld * s);
	virtual void doSomething();
};

class Fungus:public Goodie {
public:
	Fungus(double x, double y, StudentWorld* s);
	virtual void doSomething();
};

class Bacterium : public Agent {
	int movementPlan;
	int foodate = 0;
	int damage;
public:
	Bacterium(double x, double y, int id, StudentWorld* s);
	void setMovementPlan(int x);
	int getMovementPlan();
	int getdamage();
	void setdamage(int x);
	int getFood();
	void setFood(int x);
	virtual void moveBac();
	
};
class Salmonella : public Bacterium {
public:
	Salmonella(double x, double y, int id, StudentWorld* s);
	virtual void damaged(int x);
	
};

class RegularSalmonella : public Salmonella {
public:
	RegularSalmonella(double x, double y, StudentWorld * s);
	virtual void doSomething();
	
};
class AggressiveSalmonella : public Salmonella {
public:
	AggressiveSalmonella(double x, double y, StudentWorld* s);
	virtual void doSomething();
	void moveIsZero();
	//void moveAggroBac();
};

class Ecoli : public Bacterium {
public:
	Ecoli(double x, double y, StudentWorld* s);
	virtual void doSomething();
	void moveEco();
	virtual void damaged(int x);


};
class Pit : public Actor {
	enum { aggro =0 , eco= 1, reg =2};
	int storage[3];
public:
	Pit(double x, double y, StudentWorld* s);
	virtual void doSomething();
	virtual bool isDamageable();
	virtual bool canProduce();
	
};






// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

#endif // ACTOR_H_
