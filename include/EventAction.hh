#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"

#include "RunAction.hh"
#include "TrackerHit.hh"

#include "globals.hh"

class G4Event;
class RunAction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction);
    ~EventAction() override = default;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    // Data structure to hold a prompt gamma record per event
    struct PromptGamma {
      G4int eventID = -1;
      G4double energy = 0.;
      G4ThreeVector position{0.,0.,0.};
    };
    
    // Called from SteppingAction to add a prompt gamma produced during this event
    void AddPromptGamma(const PromptGamma& g) { fPromptGammas.push_back(g); }

  private:
    // methods
    TrackerHitsCollection* GetHitsCollection(G4int hcID, const G4Event* event) const;
    void PrintEventStatistics(G4double absoEdep, G4double absoTrackLength, G4double gapEdep,
                              G4double gapTrackLength) const;

    // data members
    G4int fScatHCID = -1;
    G4int fAbsoHCID = -1;
    std::vector<PromptGamma> fPromptGammas;
    RunAction *fRunAction = nullptr;
};

#endif
