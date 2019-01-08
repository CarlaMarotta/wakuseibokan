#include "Structure.h"

Structure::Structure()
{

}

Structure::~Structure()
{
    if (_model != NULL)
            delete _model;
}

void Structure::setTexture(GLuint texture)
{
    Structure::texture = texture;
}

int Structure::getType()
{
    return COLLISIONABLE;
}

void Structure::init()
{
    //Load the model
    _model = (Model*)T3DSModel::loadModel("structures/structure.3ds",160.99f,-19.48f,76.36f,1,Structure::texture);
    if (_model != NULL)
        _model->setAnimation("run");

    Structure::height=50;
    Structure::length=8;
    Structure::width=8;

    setForward(0,0,1);
}

void Structure::drawModel()
{
    drawModel(0,0,pos[0],pos[1],pos[2]);
}

void Structure::drawModel(float yRot, float xRot, float x, float y, float z)
{
    float f[3];
    f[0] = 0; f[1] = 0; f[2] = 0;

    //Draw the saved model
    if (_model != NULL)
    {
        glPushMatrix();
        glTranslatef(x, y, z);

        glScalef(1.0f,1.0f,1.0f);

        _model->draw(Structure::texture);
        //drawRectangularBox(Structure::width, Structure::height, Structure::length);

        glPopMatrix();
    }
    else
    {
        printf ("model is null\n");
    }
}

dGeomID Structure::getGeom()
{
    return geom;
}

void Structure::embody(dWorldID world, dSpaceID space)
{
    geom = dCreateBox(space, Structure::width, Structure::height, Structure::length);
    dGeomSetPosition(geom, pos[0], pos[1], pos[2]);
}

void Structure::embody(dBodyID myBodySelf)
{

}


void Structure::doControl(Controller controller)
{
    Structure::inclination = controller.registers.pitch;
    Structure::azimuth = controller.registers.roll;
}

Vec3f Structure::getForward()
{
    Vec3f forward = toVectorInFixedSystem(0, 0, 1,Structure::azimuth,Structure::inclination);
    return forward;
}

void Structure::getViewPort(Vec3f &Up, Vec3f &position, Vec3f &forward)
{
    position = getPos();
    forward = getForward();
    Up = toVectorInFixedSystem(0.0f, 1.0f, 0.0f,0,0);

    Vec3f orig;

    forward = forward.normalize();
    orig = position;
    Up[0]=Up[2]=0;Up[1]=4;// poner en 4 si queres que este un toque arriba desde atras.
    position = position - 5*forward + Up;
    forward = orig-position;
}
