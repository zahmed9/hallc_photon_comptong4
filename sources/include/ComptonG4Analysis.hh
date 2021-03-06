#ifndef _COMPTONG4ANALYSIS_H
#define _COMPTONG4ANALYSIS_H

// GEANT4 Includes
#include <G4UIcommand.hh>

// ROOT Includes
#include <Rtypes.h>

// General includes
#include <vector>
#include <string>

// Forward declarations
class TTree;
class TFile;
class ComptonG4SensitiveDetectorManager;
class ComptonG4SteppingAction;

/*!
 * @class ComptonG4Analysis
 *
 * @brief Default class to process the simulation data
 *
 * This class processes the simulation data, including writing to a ROOT file
 * when the event is finished.
 *
 * @author Juan Carlos Cornejo <cornejo@jlab.org>
 *
 !*/
class ComptonG4Analysis {
public:
  ComptonG4Analysis();
  ~ComptonG4Analysis();

  // Detector structure
  typedef struct {
    Double_t eDep;
    Double_t nOpticalPhotons;
  } DetectorStruct;

  // Public G4 Analyzer functions
  void SetRunNumber(G4int runnum) { fRunNumber = runnum; }
  void SetRunMinDigits(G4int min) { fRunMinDigits = min; }
  void SetAutoSave(G4int auto_save) { fAutoSaveEntry = auto_save; }
  void Initialize();
  void StartOfEvent();
  void EndOfEvent();
  void Finished();
  void CleanEvent();
  void SetAsym(Double_t asym) { fAsym = asym; }
  void SetGammaE(Double_t e) { fGammaE = e; }
  void SetRho(Double_t rho) { fRho = rho; }
  void SetTheta(Double_t theta) { fTheta = theta; }
  void SetPhi(Double_t phi) { fPhi = phi; }
  void AddDetector(std::string name);
  void SetOutputPath(std::string path);
  void SetRootfilePrefix(std::string prefix) {
    fRootfilePrefix = prefix;
  }
  void SetSDManager(ComptonG4SensitiveDetectorManager *manager) {
    fSDManager = manager;
  }
  void SetSteppingAction(ComptonG4SteppingAction *action) {
    fSteppingAction = action;
  }

  void SetEDep(std::string name,Double_t e);
  void SetOpticalHits(std::string name,Double_t hits);
  void SetGlobalTimes(std::string name,std::vector<Double_t> times);

  void StoreRandomSeed(G4String seed);

  void StoppedOpticalPhoton() {
    fNumberOfOpticalPhotonsStopped+=1.0;
  }

  void ProcessOpticalTrackID(G4int id);

  void OpticalHit() {
    fTotalNumberOfOpticalPhotonsAbsorbed+=1.0;
  }

  void WriteRandomSeed();

  /*
  void AddStepSize(std::string name,Double_t size) {
    fStepSize[fDetectorIndices[name]] += size;
  }
  void AddEDepAllCharges(std::string name,Double_t e) {
    fEDepAll[fDetectorIndices[name]] += e;
  }
  void AddStepSizeAllCharges(std::string name,Double_t size) {
    fStepSizeAll[fDetectorIndices[name]] += size;
  }*/

private:
  Int_t               fRunNumber;     // Run number
  UInt_t               fRunMinDigits;  // Minum number of digits for run number
  TTree               *fTree;         // Generated TTree
  TFile               *fFile;         // Output TFile
  std::string         fOutputPath;    // Path to output rootfiles
  std::string         fRootfilePrefix;// Rootfile prefix (default: ComptonG4_)
  std::vector<std::string> fDetectorNames; // Names of detectors
  std::vector<DetectorStruct> fDetectors;  // Detectors
  std::vector<std::vector<Double_t> > fDetectorTimes;  // Detector Times

  // Global tree variables per event
  Double_t            fAsym;          // Theoretical asymmetry
  Double_t            fRho;           // Normalized Gamma scattering energy
  Double_t            fGammaE;        // Absolute Gamma energy
  Double_t            fTheta;         // The scattering angle in Degrees
  Double_t            fPhi;           // The azimuthal angle
  Double_t            fNumberOfOpticalPhotonsStopped; // Number of forced stopped optical photons
  Double_t            fNumberOfOpticalPhotonsProduced; // Number of total stopped optical photons
  Double_t            fTotalNumberOfOpticalPhotonsAbsorbed; // Number of total stopped optical photons

  Int_t               fNumberOfEvents;

  G4int DetectorIndex(std::string name);
  G4String fRandomSeed;

  // Keep track of the SD Manager
  ComptonG4SensitiveDetectorManager *fSDManager;

  // Keep track of the stepping action
  ComptonG4SteppingAction *fSteppingAction;

  // Auto Save rootfile?
  unsigned int fAutoSaveEntry;        // Auto save tree every n>0 entries

  // Keep track of track IDs
  std::vector<Int_t> fOpticalTrackIDs;
};

#endif
