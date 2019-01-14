#ifndef BALAENIDAE_H
#define BALAENIDAE_H

#include "Vehicle.h"
#include "Manta.h"

class Balaenidae : public Vehicle
{
protected:
    dGeomID geom;
    float rudder;
    int offshoring = 0;
    Vec3f ap;
public:

    static const int OFFSHORING = 1;
    static const int SAILING = 0;

    ~Balaenidae();
    Balaenidae();
    void drawModel(float yRot, float xRot, float x, float y, float z);
    void init();
    int getType();
    void embody(dWorldID world, dSpaceID space);
    void embody(dBodyID myBodySelf);

    void doControl(Controller controller);
    void doControl();

    void getViewPort(Vec3f &Up, Vec3f &position, Vec3f &forward);
    void doDynamics();


    void doDynamics(dBodyID body);

    void drawModel();

    void offshore();

    Vehicle* spawn(dWorldID  world,dSpaceID space,int type);

    void launch(Manta* m);
    void taxi(Manta *m);
};

#endif // BALAENIDAE_H
