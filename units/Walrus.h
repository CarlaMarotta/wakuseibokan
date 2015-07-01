/*
 * Walrus.h
 *
 *  Created on: Jan 16, 2011
 *      Author: faturita
 */

#ifndef WALRUS_H_
#define WALRUS_H_

#include "Vehicle.h"

class Walrus : public Vehicle
{
    dGeomID geom;
    
public:
	void virtual init();
    int  virtual getType();
	void virtual drawModel(float yRot, float xRot, float x, float y, float z);
	void virtual drawModel();
	void virtual drawDirectModel();
	void doMaterial();
	void virtual doDynamics(dBodyID);
    void doDynamics();
    void doControl(Controller controller);
    
    void embody(dWorldID world, dSpaceID space);
    void embody(dBodyID myBodySelf);
};

#endif /* WALRUS_H_ */