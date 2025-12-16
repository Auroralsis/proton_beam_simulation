#ifndef TrackerHit_h
#define TrackerHit_h 1

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4Threading.hh"
#include "G4ThreeVector.hh"
#include "G4VHit.hh"
#include "globals.hh"

class TrackerHit : public G4VHit
{
  public:
    TrackerHit() = default;
    TrackerHit(const TrackerHit&) = default;
    ~TrackerHit() override = default;

    // operators
    TrackerHit& operator=(const TrackerHit&) = default;
    G4bool operator==(const TrackerHit&) const;

    inline void* operator new(size_t);
    inline void operator delete(void*);

    // methods from base class
    void Draw() override;
    void Print() override;

    // Set methods
    void SetTrackID(G4int track) { fTrackID = track; };
    void SetEdep(G4double de) { fEdep = de; };
    void SetPos(G4ThreeVector xyz) { fPos = xyz; };

    // Get methods
    G4int GetTrackID() const { return fTrackID; };
    G4double GetEdep() const { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };

  private:
    G4int fTrackID = -1;
    G4double fEdep = 0.;
    G4ThreeVector fPos;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using TrackerHitsCollection = G4THitsCollection<TrackerHit>;

extern G4ThreadLocal G4Allocator<TrackerHit>* TrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* TrackerHit::operator new(size_t)
{
  if (!TrackerHitAllocator) {
    TrackerHitAllocator = new G4Allocator<TrackerHit>;
  }
  void* hit;
  hit = (void*)TrackerHitAllocator->MallocSingle();
  return hit;
}

inline void TrackerHit::operator delete(void* hit)
{
  if (!TrackerHitAllocator) {
    TrackerHitAllocator = new G4Allocator<TrackerHit>;
  }
  TrackerHitAllocator->FreeSingle((TrackerHit*)hit);
}

#endif
