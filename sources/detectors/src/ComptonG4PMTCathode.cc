/*
 * ComptonG4PMTCathode.cc
 *
 *  Created on: Dec 03, 2014
 *      Author: Juan Carlos Cornejo <cornejo@jlab.org>
 */

#include "ComptonG4PMTCathode.hh"
#include "ComptonG4Analysis.hh"
#include <G4OpticalPhoton.hh>
#include <G4TrackStatus.hh>
#include <G4VProcess.hh>
#include <TTree.h>

ComptonG4PMTCathode::ComptonG4PMTCathode(
    G4String name) :
  VComptonG4SensitiveDetector(name)
{
}


ComptonG4PMTCathode::~ComptonG4PMTCathode()
{
  // TODO Auto-generated destructor stub
}

void ComptonG4PMTCathode::Initialize(G4HCofThisEvent *)
{
  CleanEvent();
}

G4bool ComptonG4PMTCathode::ProcessHits(G4Step* step,
    G4TouchableHistory*)
{
  G4Track *track = step->GetTrack();
  if( !track ) return false;

  G4VPhysicalVolume *vol = track->GetVolume();
  int volIndex  = GetIndex(vol);

  // Sanity check, ensure volumes were properly registered
  if(volIndex < 0 ) {
    return false;
  }

  // Treat special for optical photons
  if( track->GetDefinition() == G4OpticalPhoton::Definition() ) {
    // We only care about particles that have stopped here (and likely
    // got absorbed)
    G4TrackStatus tStatus = track->GetTrackStatus();
    if( tStatus == fStopAndKill ) {

      // But was it a true hit?
      G4MaterialPropertyVector *efficiencyVector =
        fMaterialProperties[volIndex]->GetProperty("EFFICIENCY");
      G4double efficiency = 0.0;
      if(efficiencyVector) {
        efficiency = efficiencyVector->Value(
            track->GetKineticEnergy());
      }

      if(G4UniformRand() < efficiency) { // That's a hit!!
        fTotalOpticalPhotons[volIndex]++;
        ComptonG4OpticalHit hit;
        hit.ProcessStep(step);
        fOpticalHits[volIndex].push_back(hit);
        fOpticalData[volIndex].push_back(hit.GetData());
        fAnalysis->OpticalHit();
      }
    }
    fAnalysis->ProcessOpticalTrackID(track->GetTrackID());
  }
  return true;
}

void ComptonG4PMTCathode::EndOfEvent(G4HCofThisEvent*)
{
  // Ensure the pointers point to the correct data vectors
  for(unsigned int i = 0; i < fVolumes.size(); i++ ) {
    fOpticalDataPtr[i] = &fOpticalData[i];
  }
}

void ComptonG4PMTCathode::CleanEvent()
{
  for(unsigned int i = 0; i < fVolumes.size(); i++ ) {
    fTotalOpticalPhotons[i] = 0.0;
    fOpticalHits[i].clear();
    fOpticalData[i].clear();
  }
}


void ComptonG4PMTCathode::CreateTreeBranch(TTree* tree)
{
  // Create detector branches
  for(size_t i = 0; i < fVolumes.size(); i++ ) {
    tree->Branch(Form("%s_num_photons",fVolumes[i]->GetName().c_str()),
        &(fTotalOpticalPhotons[i]));

    tree->Branch(Form("%s_optical_hits",fVolumes[i]->GetName().c_str()),
        &(fOpticalDataPtr[i]));
  }
}


void ComptonG4PMTCathode::SetOptions(std::map<G4String,G4String>,bool)
{
}

// Finally register the class
COMPTONG4_SD_REGISTER(ComptonG4PMTCathode)
