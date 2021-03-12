/*
 * Manta.h
 *
 *  Created on: Jan 16, 2011
 *      Author: faturita
 */

#ifndef MANTA_H_
#define MANTA_H_

#include "Vehicle.h"

class Manta : public Vehicle
{

protected:
    float height = 10;
    float width = 10;
    float length = 10;
    int number=0;

public:
    Manta(int newfaction);
    static const int IN_CARRIER = 0;
    static const int ON_DECK = 1;
    static const int LANDED = 2;
    static const int TACKINGOFF = 3;
    static const int FLYING = 4;
    static const int HOLDING = 5;

    bool antigravity=false;

    float rudder=0;
    float elevator=0;
    float aileron=0;
    float flaps=0;
    float spoiler=0;
    float ih=0;

    float param[10];
    float alpha=0;
    float beta=0;

    float S[3] = {0.0f, 0.0f, 0.0f};
    float V[3] = {0.0f, 0.0f, 0.0f};
    float addd=0;
	void virtual init();
    int  virtual getType();
    int  virtual getSubType();
	void virtual getViewPort(Vec3f &Up, Vec3f &position, Vec3f &forward);
	void virtual drawModel(float yRot, float xRot, float x, float y, float z);
	void virtual drawModel();
	void virtual drawDirectModel();
	void virtual doDynamics(dBodyID);
    void virtual release(Vec3f orientation);
    void virtual land();
    void virtual doHold(Vec3f target, float thrust);
    void virtual attack(Vec3f target);
    void virtual dogfight(Vec3f target);
    void doDynamics();
    void doControl();


    void doControl(Controller controller);


    void doControl(struct controlregister regs);

    
    void airspeddrarestoration();
    
    void embody(dWorldID world, dSpaceID space);
    void embody(dBodyID myBodySelf);

    Vehicle* fire(dWorldID world, dSpaceID space);
    int getNumber() const;
    void setNumber(int value);
};

#endif /* MANTA_H_ */