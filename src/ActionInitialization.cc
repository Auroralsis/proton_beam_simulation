#include "ActionInitialization.hh"

#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new RunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);
  auto runAction = new RunAction();
  SetUserAction(runAction);
  auto eventAction = new EventAction(runAction);
  SetUserAction(eventAction);
  auto steppingAction = new SteppingAction(eventAction);
  SetUserAction(steppingAction);
}
