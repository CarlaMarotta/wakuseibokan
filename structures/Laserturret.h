#ifndef LASERTURRET_H
#define LASERTURRET_H

#include "Turret.h"
#include "../actions/LaserRay.h"


class LaserTurret : public Turret
{
protected:
    const int LASER_OVERHEATING = 30;
    const int LASER_RECHARGING = 1000;

    bool firing=false;

    LaserRay *ls=NULL;

public:
    LaserTurret(int faction);
    Vehicle* fire(dWorldID world, dSpaceID space);
    void drawModel(float yRot, float xRot, float x, float y, float z);
    void doControl();
    void doControl(Controller cr);
    void locateLaserRay(LaserRay *action);
    void setForward(Vec3f forw);
    void tick();
};

#endif // LASERTURRET_H
