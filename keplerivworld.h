//
//  keplerivworld.h
//  mycarrier
//
//  Created by Rodrigo Ramele on 24/05/14.
//  Copyright (c) 2014 Baufest. All rights reserved.
//

#ifndef __mycarrier__keplerivworld__
#define __mycarrier__keplerivworld__

#include <iostream>



// *********************** ODE Controlss ********************************
/* some constants */

#define NUM 10			/* number of boxes */
#define SIDE (0.2)		/* side length of a box */
#define MASS (1.0)		/* mass of a box */
#define RADIdUS (0.1732f)	/* sphere radius */
#define RADIUS (1.0f)

#define kmf *1000.0f

void nearCallback (void *data, dGeomID o1, dGeomID o2);
void _nearCallback (void *data, dGeomID o1, dGeomID o2);

void setupWorldModelling();
void initWorldModelling();
void initWorldModelling(int testcase);
void worldStep(int value);
void endWorldModelling();

void savegame();
void loadgame();

// **********************************************************************


enum gamemodes { ACTIONGAME, STRATEGYGAME };

struct Message
{
    std::string msg;
    int faction;
};


#endif /* defined(__mycarrier__keplerivworld__) */
