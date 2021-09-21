#include "savegame.h"

#include <vector>
#include <mutex>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <regex>


#include "container.h"
#include "profiling.h"

#include "units/Vehicle.h"
#include "terrain/Terrain.h"
#include "messages.h"

#include "engine.h"

#include "units/BoxVehicle.h"
#include "units/Walrus.h"
#include "units/Manta.h"
#include "units/SimplifiedDynamicManta.h"
#include "units/Buggy.h"
#include "units/MultiBodyVehicle.h"
#include "units/Balaenidae.h"
#include "units/Beluga.h"
#include "units/AdvancedWalrus.h"
#include "units/Medusa.h"
#include "units/AdvancedManta.h"
#include "units/Stingray.h"
#include "units/Otter.h"

#include "structures/Structure.h"
#include "structures/Runway.h"
#include "structures/Hangar.h"
#include "structures/Turret.h"
#include "structures/Warehouse.h"
#include "structures/Laserturret.h"
#include "structures/CommandCenter.h"
#include "structures/Launcher.h"

#include "weapons/CarrierArtillery.h"
#include "weapons/CarrierTurret.h"

#include "actions/Gunshot.h"
#include "actions/Missile.h"

#include "usercontrols.h"

extern  Controller controller;

extern int gamemode;

extern int aiplayer;

extern unsigned long timer;

extern container<Vehicle*> entities;
extern std::vector<BoxIsland*> islands;
extern std::ofstream msgboardfile;
extern std::vector<Message> messages;

extern dWorldID world;
extern dSpaceID space;


void savegame()
{
    std::ofstream ss("savegame.w", std::ios_base::binary);

    // Version
    ss << 0x01 << std::endl;

    ss << aiplayer << std::endl;

    ss << controller.faction << std::endl;

    ss << gamemode << std::endl;

    ss << timer << std::endl;

    int entitiessize = 0;
    // Get flying entities.
    for(size_t i=entities.first();entities.hasMore(i);i=entities.next(i))
    {
        if (entities[i]->getType() == CARRIER || entities[i]->getType() == MANTA || entities[i]->getType() == WALRUS)
        {
            entitiessize++;
        }
    }

    ss << entitiessize << std::endl;
    for(size_t i=entities.first();entities.hasMore(i);i=entities.next(i))
    {
        if (entities[i]->getType() == CARRIER || entities[i]->getType() == MANTA || entities[i]->getType() == WALRUS)
        {
            ss << entities[i]->getFaction() << std::endl;
            ss << entities[i]->getType() << std::endl;

            int subtype = 0;


            if (AdvancedWalrus *lb = dynamic_cast<AdvancedWalrus*>(entities[i]))
                subtype = VehicleSubTypes::ADVANCEDWALRUS;
            else if (Walrus *lb = dynamic_cast<Walrus*>(entities[i]))
                subtype = VehicleSubTypes::SIMPLEWALRUS;
            else if (Medusa *lb = dynamic_cast<Medusa*>(entities[i]))
                subtype = VehicleSubTypes::MEDUSA;
            else if (AdvancedManta *lb = dynamic_cast<AdvancedManta*>(entities[i]))
                subtype = VehicleSubTypes::SIMPLEMANTA;
            else if (Stingray *lb = dynamic_cast<Stingray*>(entities[i]))
                subtype = VehicleSubTypes::STINGRAY;
            else if (Beluga *lb = dynamic_cast<Beluga*>(entities[i]))
                subtype = VehicleSubTypes::BELUGA;
            else if(Balaenidae* lb = dynamic_cast<Balaenidae*>(entities[i]))
                subtype = VehicleSubTypes::BALAENIDAE;
            else if(Otter* ot = dynamic_cast<Otter*>(entities[i]))
                subtype = VehicleSubTypes::OTTER;
            else if(Cephalopod* cp = dynamic_cast<Cephalopod*>(entities[i]))
                subtype = VehicleSubTypes::CEPHALOPOD;

            ss << subtype << std::endl;

            dout << "Subtype saving:" << subtype << std::endl;

            Vec3f p= entities[i]->getPos();
            ss << p[0] << std::endl << p[1] << std::endl << p[2] << std::endl;
            ss << entities[i]->getHealth() << std::endl;
            ss << entities[i]->getPower() << std::endl;


            float R[12];
            entities[i]->getR(R);
            for(int j=0;j<12;j++) ss << R[j] << std::endl;

            ss << entities[i]->isAuto() << std::endl;
            p = entities[i]->getDestination();
            ss << p[0] << std::endl << p[1] << std::endl << p[2] << std::endl;

            int autostatus = static_cast<int>(entities[i]->getAutoStatus());

            ss << autostatus << std::endl;

            ss << entities[i]->getStatus() << std::endl;

            ss << entities[i]->getSignal() << std::endl;

            ss << entities[i]->getOrder() << std::endl;

            // @NOTE Take care when you write string with namespaces.
            std::string name = entities[i]->getName();
            name = std::regex_replace(name, std::regex(" "), "-");

            ss << name << std::endl;


        }
    }


    ss << islands.size() << std::endl;
    for (int j=0;j<islands.size();j++)
    {
        std::string s;
        s = islands[j]->getName();

        s = std::regex_replace(s, std::regex(" "), "-");
        ss << s << std::endl;
        ss << islands[j]->getX() << std::endl;
        ss << islands[j]->getZ() << std::endl;
        ss << islands[j]->getModelName() << std::endl;
        dout << "Name:" << islands[j]->getName() << std::endl;

        Structure *c =  islands[j]->getCommandCenter();

        if (c)
        {
            std::vector<size_t> strs = islands[j]->getStructures();
            ss << 0x3f << std::endl;
            ss << strs.size() << std::endl;
            for(int i=0;i<strs.size();i++)
            {
                ss << entities[strs[i]]->getFaction() << std::endl;
                ss << entities[strs[i]]->getType() << std::endl;
                ss << entities[strs[i]]->getSubType() << std::endl;

                int typeofisland = 0x4f;

                if (entities[strs[i]]->getSubType() == VehicleSubTypes::COMMANDCENTER)
                    typeofisland = ((CommandCenter*)entities[strs[i]])->getIslandType();

                ss << typeofisland << std::endl;

                Vec3f p= entities[strs[i]]->getPos();
                ss << p[0] << std::endl << p[1] << std::endl << p[2] << std::endl;
                float orientation = getAzimuthRadians(entities[strs[i]]->getForward());
                ss << orientation << std::endl;
                ss << entities[strs[i]]->getHealth() << std::endl;
                ss << entities[strs[i]]->getPower() << std::endl;

            }
        } else {
            ss << 0x4f << std::endl;
        }


    }

    ss.flush();
    ss.close();


}


void loadgame()
{
    /**std::ifstream ss("savegame.w", std::ios_base::binary);

    Vec3f f(0,0,0);
    ss >> f[0] >> f[1] >> f[2] ;

    dout << f << std::endl;

    ss.close();**/

    std::ifstream ss("savegame.w", std::ios_base::binary);

    int version;
    ss >> version;

    ss >> aiplayer;

    ss >> controller.faction;

    ss >> gamemode;

    ss >> timer;

    int size;
    ss >> size;
    dout << "Size:" << size << std::endl;
    for(int i=0;i<size;i++)
    {
        Vehicle *v = NULL;
        int faction;
        ss >> faction;
        int type, subtype;
        ss >> type;
        ss >> subtype;
        dout << "Type:" << type << " subtype:" << subtype << std::endl;
        Vec3f f(0,0,0);
        if (type == CARRIER || type == MANTA || type == WALRUS)
        {
            switch (type) {
            case CARRIER:
            {
                Balaenidae *b = NULL;
                if (subtype==VehicleSubTypes::BALAENIDAE)
                {
                    b = new Balaenidae(faction);
                    v = b;
                    b->init();
                    dSpaceID carrier_space = b->embody_in_space(world, space);
                    //_b->setPos(0.0f + 0.0 kmf,20.5f,-4000.0f + 0.0 kmf);
                    ss >> f[0] >> f[1] >> f[2] ;
                    v->setPos(f);
                    b->stop();

                    entities.push_back(b, b->getGeom());


                    CarrierTurret * _bo= new CarrierTurret(GREEN_FACTION);
                    _bo->init();
                    _bo->embody(world, carrier_space);
                    _bo->attachTo(world,b, -40.0f, 20.0f + 5, -210.0f);
                    _bo->stop();

                    entities.push_back(_bo, _bo->getGeom());


                    CarrierArtillery * _w1= new CarrierArtillery(GREEN_FACTION);
                    _w1->init();
                    _w1->embody(world, carrier_space);
                    _w1->attachTo(world,b, -40.0, 27.0f, +210.0f);
                    _w1->stop();

                    entities.push_back(_w1, _w1->getGeom());
                }
                else if (subtype==VehicleSubTypes::BELUGA)
                {
                    b = new Beluga(faction);
                    v = b;
                    b->init();
                    b->embody(world,space);
                    ss >> f[0] >> f[1] >> f[2] ;
                    v->setPos(f);
                    entities.push_back(b, b->getGeom());

                }
                v = b;
                break;
            }
            case MANTA:
            {
                Manta *_manta1 = NULL;

                if (subtype == VehicleSubTypes::MEDUSA)
                    _manta1 = new Medusa(faction);
                else if (subtype == VehicleSubTypes::STINGRAY)
                    _manta1 = new Stingray(faction);
                else if (subtype == VehicleSubTypes::SIMPLEMANTA)
                    _manta1 = new AdvancedManta(faction);
                else if (subtype == VehicleSubTypes::CEPHALOPOD)
                    _manta1 = new Cephalopod(faction);

                v = _manta1;
                _manta1->init();
                _manta1->embody(world, space);
                ss >> f[0] >> f[1] >> f[2] ;
                v->setPos(f);
                _manta1->setStatus(FlyingStatus::FLYING);              // @FIXME, status should be stored.
                _manta1->inert = true;
                v = _manta1;
                entities.push_back(_manta1, _manta1->getGeom());
                break;
            }
            case WALRUS:
                Walrus *_walrus = NULL;
                if (subtype == VehicleSubTypes::ADVANCEDWALRUS)
                {
                    _walrus = new AdvancedWalrus(faction);
                    v = _walrus;
                    _walrus->init();
                    _walrus->embody(world, space);
                    ss >> f[0] >> f[1] >> f[2] ;
                    v->setPos(f);
                    entities.push_back(_walrus, _walrus->getGeom());

                }
                else if (subtype == VehicleSubTypes::SIMPLEWALRUS)
                {
                    _walrus = new Walrus(faction);
                    v = _walrus;
                    _walrus->init();
                    _walrus->embody(world, space);
                    ss >> f[0] >> f[1] >> f[2] ;
                    v->setPos(f);
                    entities.push_back(_walrus, _walrus->getGeom());

                }
                else if (subtype == VehicleSubTypes::OTTER)
                {
                    Otter* _ot = new Otter(faction);
                    v = _ot;
                    _ot->init();
                    dSpaceID car_space = _ot->embody_in_space(world, space);
                    ss >> f[0] >> f[1] >> f[2] ;
                    v->setPos(f);

                    _walrus = _ot;
                    entities.push_back(_walrus, _walrus->getGeom());

                    Wheel * _fr= new Wheel(faction, 0.001, 30.0);
                    _fr->init();
                    _fr->embody(world, car_space);
                    _fr->attachTo(world,_walrus,4.9f, -3.0, 5.8);
                    _fr->stop();

                    entities.push_back(_fr, _fr->getGeom());


                    Wheel * _fl= new Wheel(faction, 0.001, 30.0);
                    _fl->init();
                    _fl->embody(world, car_space);
                    _fl->attachTo(world,_walrus, -4.9f, -3.0, 5.8);
                    _fl->stop();

                    entities.push_back(_fl, _fl->getGeom());


                    Wheel * _br= new Wheel(faction, 0.001, 30.0);
                    _br->init();
                    _br->embody(world, car_space);
                    _br->attachTo(world,_walrus, 4.9f, -3.0, -5.8);
                    _br->stop();

                    entities.push_back(_br, _br->getGeom());


                    Wheel * _bl= new Wheel(faction, 0.001, 30.0);
                    _bl->init();
                    _bl->embody(world, car_space);
                    _bl->attachTo(world,_walrus, -4.9f, -3.0, -5.8);
                    _bl->stop();

                    entities.push_back(_bl, _bl->getGeom());

                    _ot->addWheels(_fl, _fr, _bl, _br);
                    _fl->setSteering(true);
                    _fr->setSteering(true);
                }

                //_walrus->setNumber(findNextNumber(WALRUS));
                //_walrus->inert = true;
                v = _walrus;

            }
            float health;ss >> health ;v->damage(1000-health);
            float power; ss >> power ;v->setPower(power);

            float R[12];
            for(int j=0;j<12;j++) ss >> R[j];
            v->setRotation(R);


            if (type == MANTA)
            {
                Manta *m = (Manta*) v;
                m->release(v->getForward());
            }

            // Destination and auto
            bool isauto;
            ss >> isauto;
            ( isauto ? v->enableAuto() : v->disableAuto());

            ss >> f[0] >> f[1] >> f[2] ;
            v->goTo(f);

            int autostatus;
            ss >> autostatus;

            AutoStatus autos=static_cast<AutoStatus>(autostatus);
            v->setAutoStatus(autos);

            int status;
            ss >> status;

            v->setStatus(status);

            int signal;
            ss >> signal;

            v->setSignal(signal);

            int order;
            ss >> order;

            v->setOrder(order);

            std::string name;
            ss >> name;

            v->setName(name);

            printf("Unit name: %s\n", name.c_str());

        }
    }

    ss >> size;
    dout << "Size:" << size << std::endl;
    for (int j=0;j<size;j++)
    {
        BoxIsland *is = new BoxIsland(&entities);
        std::string name,modelname;
        Vec3f loc;
        ss >> name;is->setName(name);
        ss >> loc[0];
        ss >> loc[2];
        is->setLocation(loc[0],-1,loc[2]);
        ss >> modelname;
        dout << "Reading Island:" << name << "\t" << modelname << std::endl;
        is->buildTerrainModel(space,modelname.c_str());

        islands.push_back(is);

        int moredata;

        ss >> moredata;

        if (moredata == 0x3f)
        {
            int sze;
            ss >> sze;
            for (int i=0;i<sze;i++)
            {
                Structure *v = NULL;
                int faction;
                ss >> faction;
                int type, subtype;
                ss >> type;
                ss >> subtype;
                dout << "Type:" << type << " subtype:" << subtype << std::endl;
                int typeofisland = 0x4f;
                ss >> typeofisland;

                switch (subtype) {
                case 10:
                    v = new Artillery(faction);
                    break;
                case 11:
                    v = new CommandCenter(faction,typeofisland);
                    break;
                case 12:
                    v = new Hangar(faction);
                    break;
                case 13:
                    v = new Warehouse(faction);
                    break;
                case 14:
                    v = new Runway(faction);
                    break;
                case 15:
                    v = new LaserTurret(faction);
                    break;
                case 16:
                    v = new Turret(faction);
                    break;
                case 17:
                    v = new Launcher(faction);
                    break;
                case 18:
                    v = new Factory(faction);
                    break;
                case 19:
                    v = new Dock(faction);
                    break;
                case 20:
                    v = new Antenna(faction);
                    break;
                case 21:
                    v = new Radar(faction);
                    break;
                case 22:default:
                    v = new Structure(faction);
                    break;
                }

                Vec3f f(0,0,0);
                ss >> f[0] >> f[1] >> f[2] ;
                float orientation; ss >> orientation;
                float health;ss >> health ;
                float power; ss >> power ;

                is->addStructure(v   ,       -is->getX()+f[0],    -is->getZ()+f[2],orientation,world);

            }
        }

    }


    ss.close();

}

