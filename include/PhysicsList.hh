#ifndef SIMULATION_PhysicsList_h
#define SIMULATION_PhysicsList_h

#include "G4VModularPhysicsList.hh"

class myPhysicsList : public G4VModularPhysicsList
{
public:
  myPhysicsList();
  ~myPhysicsList() override = default;
};

#endif
