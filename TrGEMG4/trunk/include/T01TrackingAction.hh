#ifndef T01TrackingAction_h
#define T01TrackingAction_h 1

class G4Track;
#include "G4UserTrackingAction.hh"
#include "globals.hh"

class T01TrackingAction : public G4UserTrackingAction 
{
  public:
    T01TrackingAction();
    virtual ~T01TrackingAction();
   
    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:
    G4String processName;
};

#endif
