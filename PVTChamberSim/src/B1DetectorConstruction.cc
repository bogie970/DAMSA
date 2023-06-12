//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Element.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fSensorPV(nullptr)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();


// Neutron Moderator
  //
  G4double r_mod = 0.5 * m;
  G4double l_mod = 0.2 * m; // thickness of the moderator is 20 cm
  // Polyurethane C3H8N2O1
  // Element definition from https://apc.u-paris.fr/~franco/g4doxy4.10/html/class_materials.html
  // See the constructor source code line 54.
  //
  //                                          Z, A
  G4Element* C = new G4Element("Carbon", "C", 6, 12.011*g/mole);
  G4Element* H = new G4Element("Hydrogen", "H", 1, 1.00794*g/mole);
  G4Element* N = new G4Element("Nitrogen", "N", 7, 14.00674*g/mole);
  G4Element* O = new G4Element("Oxygen", "O", 8, 15.9994*g/mole);
  G4Material* polyurethane = new G4Material(
      "Polyurethane",   // its name
      1100*kg/m3,       // its density
      4,                // its number of elements
      kStateSolid);     // its state
  G4int natoms=3;
  polyurethane->AddElement(C, natoms);
  natoms=8;
  polyurethane->AddElement(H, natoms);
  natoms=2;
  polyurethane->AddElement(N, natoms);
  natoms=1;
  polyurethane->AddElement(O, natoms);

  G4Material* mod_mat = polyurethane;
  //G4Material* mod_mat = nist->FindOrBuildMaterial("G4_B");



//define helium

// Define the properties of helium gas at 1 atmosphere pressure
G4double pressure = 1.0 * atmosphere;
G4double temperature = 298.15 * kelvin;
G4double density = 0.1786 * mg/cm3;  // Density of helium at 1 atm and 298.15 K

  G4Element* He = new G4Element("Helium", "He", 2, 4.0026 * g/mole);

// Create a material for helium at 1 atmosphere pressure
G4Material* heliumvol = new G4Material("HeliumVol", density, 1, kStateGas);

// Set the element composition of helium
heliumvol->AddElement(He, 2);

// Optional: Set other properties of helium, such as its mean excitation energy, etc.
// helium->GetIonisation()->SetMeanExcitationEnergy(41.8 * eV);






  // Vacuum vessel parameters
  G4double pVacVesselRmin = 499.0*cm; // 1cm thick
  G4double pVacVesselRmax = 5.00*m;
  G4double pVacVesselDz    = 5.00*m;  // half-length
  G4double pVacVesselSphi  = 0; // radian
  G4double pVacVesselDphi  = 2.0*M_PI;

  G4double pVacVesselUpStreamEndcapRmin = 0.;
  G4double pVacVesselUpStreamEndcapRmax = pVacVesselRmin;
  G4double pVacVesselUpStreamEndcapDz = 0.5*cm; //1cm thick


	//PVT
  G4Material* vacuumVessel_mat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  //G4Material* vacuumVessel_mat = nist->FindOrBuildMaterial("G4_Fe");
  //G4Material* vacuumVessel_mat = pvt;
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 12.0*m;
  G4double world_sizeZ  = 12.0*m;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  G4Box* solidWorld =
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking






  
  
  
  
  
  //
  // He Volume
  //
  /*
  
   G4Tubs* solidHeVessel = new G4Tubs(
      "He_Vol",
      0,
      499*cm,
      499*cm,   // half-length
      pVacVesselSphi,
      pVacVesselDphi);

  G4LogicalVolume* logicalHeVessel =
    new G4LogicalVolume(solidHeVessel,
        heliumvol,
        "He_Vol");

  new G4PVPlacement(0,
      G4ThreeVector(0, 0, 0),
      logicalHeVessel,
      "physHe_Vol",
      logicWorld,
      false,
      0,
      checkOverlaps);
  */
  
  
/*  
  
  //
  // Vacuum Vessel
  //

  G4Tubs* solidVacuumVessel = new G4Tubs(
      "vessel",
      pVacVesselRmin,
      pVacVesselRmax,
      pVacVesselDz,   // half-length
      pVacVesselSphi,
      pVacVesselDphi);

  G4LogicalVolume* logicalVacuumVessel =
    new G4LogicalVolume(solidVacuumVessel,
        vacuumVessel_mat,
        "vessel");

  new G4PVPlacement(0,
      G4ThreeVector(0, 0, 0),
      logicalVacuumVessel,
      "physVessel",
      logicWorld,
      false,
      0,
      checkOverlaps);

  G4Tubs* solidVacuumVessel_UpstreamEndcap = new G4Tubs(
      "vesselUpstreamEndcap",
      pVacVesselUpStreamEndcapRmin,
      pVacVesselUpStreamEndcapRmax,
      pVacVesselUpStreamEndcapDz,
      0.,
      2.0*M_PI);
  G4LogicalVolume* logicalVacuumVesselUpstreamEndcap =
    new G4LogicalVolume(solidVacuumVessel_UpstreamEndcap,
        vacuumVessel_mat,
        "vesselUpstreamEndcap");
  new G4PVPlacement(0,
      G4ThreeVector(0, 0, -499.5*cm),
      logicalVacuumVesselUpstreamEndcap,
      "physVesselUpstreamEndcap",
      logicWorld,
      false,
      0,
      checkOverlaps);

  G4Tubs* solidVacuumVessel_DownstreamEndcap = new G4Tubs(
      "vesselDownstreamEndcap",
      pVacVesselUpStreamEndcapRmin,
      pVacVesselUpStreamEndcapRmax,
      pVacVesselUpStreamEndcapDz,
      0.,
      2.0*M_PI);
  G4LogicalVolume* logicalVacuumVesselDownstreamEndcap =
    new G4LogicalVolume(solidVacuumVessel_DownstreamEndcap,
        vacuumVessel_mat,
        "vesselDownstreamEndcap");
  new G4PVPlacement(0,
      G4ThreeVector(0, 0, 499.5*cm),
      logicalVacuumVesselDownstreamEndcap,
      "physVesselDownstreamEndcap",
      logicWorld,
      false,
      0,
      checkOverlaps);
*/
  G4Tubs* solidDownstreamSensorVolume = new G4Tubs(
      "DownstreamSensorVolume",
      0,
      5.0*m,
      0.5*cm,
      0.,
      2.0*M_PI);
  G4LogicalVolume* logicalDownstreamSensorVolume =
    new G4LogicalVolume(solidDownstreamSensorVolume,
        world_mat, // vacuum
        "downstreamSensorVolume");
  fSensorPV = new G4PVPlacement(0,
              G4ThreeVector(0, 0, 500.0*cm + pVacVesselUpStreamEndcapDz),
              logicalDownstreamSensorVolume,
              "physDownstreamSensorVolume",
              logicWorld,
              false,
              0,
              checkOverlaps);
  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
