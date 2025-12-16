#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    ~RunAction() override = default;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

    G4int GetDetectionNtupleID() const {return fDetectionNtupleID;}
    G4int GetPromptNtupleID() const {return fPromptNtupleID;}

  private:
    G4int fDetectionNtupleID = -1;
    G4int fPromptNtupleID = -1;
};

#endif
