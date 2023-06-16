#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
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
  G4double pVacVesselRmin = 499.9*cm; // 1mm thick
  G4double pVacVesselRmax = 5.00*m;
  G4double pVacVesselDz    = 5.00*m;  // half-length
  G4double pVacVesselSphi  = 0; // radian
  G4double pVacVesselDphi  = 2.0*M_PI;

  G4double pVacVesselUpStreamEndcapRmin = 0.;
  G4double pVacVesselUpStreamEndcapRmax = pVacVesselRmin;
  G4double pVacVesselUpStreamEndcapDz = 0.05*cm; //1mm thick


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
  G4double world_sizeXY = 20.0*m;
  G4double world_sizeZ  = 25.0*m;
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
      G4ThreeVector(0, 0, -499.95*cm),
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
      G4ThreeVector(0, 0, 499.95*cm),
      logicalVacuumVesselDownstreamEndcap,
      "physVesselDownstreamEndcap",
      logicWorld,
      false,
      0,
      checkOverlaps);

/*  G4Tubs* solidDownstreamSensorVolume = new G4Tubs(
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
              */
              
              
//Concrete Walls
G4double concrete_thickness = 2.0*m;
G4Material *concrete = vacuumVessel_mat;
	//Roof
	G4Box *solidRoof = new G4Box("solidRoof", world_sizeXY/2,concrete_thickness/2,world_sizeZ/2);
	
	G4LogicalVolume *logicRoof = new G4LogicalVolume(solidRoof, concrete, "logicalRoof");
	
	G4VPhysicalVolume *physRoof = new G4PVPlacement(0 , G4ThreeVector(0.,world_sizeXY/2 - concrete_thickness/2,0), logicRoof, "physRoof", logicWorld, false, 0, true);
	
	//Floor
	G4Box *solidFloor = new G4Box("solidFloor", world_sizeXY/2,concrete_thickness/2,world_sizeZ/2);
	
	G4LogicalVolume *logicFloor = new G4LogicalVolume(solidFloor, concrete, "logicalFloor");
	
	G4VPhysicalVolume *physFloor = new G4PVPlacement(0 , G4ThreeVector(0.,- world_sizeXY/2 + concrete_thickness/2,0), logicFloor, "physFloor", logicWorld, false, 0, true);
	
	//UpstreamWall
	G4Box *solidUpstreamWall = new G4Box("solidUpstreamWall", world_sizeXY/2,world_sizeXY/2 - concrete_thickness,concrete_thickness/2);
	
	G4LogicalVolume *logicUpstreamWall = new G4LogicalVolume(solidUpstreamWall, concrete, "logicalUpstreamWall");
	
	G4VPhysicalVolume *physUpstreamWall = new G4PVPlacement(0 , G4ThreeVector(0.,0,world_sizeZ/2 - concrete_thickness/2), logicUpstreamWall, "physUpstreamWall", logicWorld, false, 0, true);
	
//DownstreamWall
	G4Box *solidDownstreamWall = new G4Box("solidDownstreamWall", world_sizeXY/2,world_sizeXY/2 - concrete_thickness,concrete_thickness/2);
	
	G4LogicalVolume *logicDownstreamWall = new G4LogicalVolume(solidDownstreamWall, concrete, "logicalDownstreamWall");
	
	G4VPhysicalVolume *physDownstreamWall = new G4PVPlacement(0 , G4ThreeVector(0.,0,- world_sizeZ/2 + concrete_thickness/2), logicDownstreamWall, "physDownstreamWall", logicWorld, false, 0, true);
	
	
//LeftWall
	G4Box *solidLeft = new G4Box("solidLeft", concrete_thickness/2,world_sizeXY/2 - concrete_thickness ,world_sizeZ/2 - concrete_thickness);
	
	G4LogicalVolume *logicLeft = new G4LogicalVolume(solidLeft, concrete, "logicalLeft");
	
	G4VPhysicalVolume *physLeft = new G4PVPlacement(0 , G4ThreeVector(- world_sizeXY/2 + concrete_thickness/2,0,0), logicLeft, "physLeft", logicWorld, false, 0, true);
	
	
//RightWall
	G4Box *solidRight = new G4Box("solidRight", concrete_thickness/2,world_sizeXY/2 - concrete_thickness,world_sizeZ/2 - concrete_thickness);
	
	G4LogicalVolume *logicRight = new G4LogicalVolume(solidRight, concrete, "logicalRight");
	
	G4VPhysicalVolume *physRight = new G4PVPlacement(0 , G4ThreeVector(world_sizeXY/2 - concrete_thickness/2,0,0), logicRight, "physRight", logicWorld, false, 0, true);
	
	












/*
////////////////////////////////////////////////////
 //
  // W dump
  //
  G4double r_dump = 0.5 * m;
  G4double l_dump = 1.0 * m;
  G4Material* dump_mat = nist->FindOrBuildMaterial("G4_W");

  G4Tubs* solidDump =
    new G4Tubs("WDump",                    //its name
        0,               // inner radius
        r_dump,          // outer radius
        0.5 * l_dump,    // half-length
        0,
        2.* M_PI * rad);

  G4LogicalVolume* logicDump =
    new G4LogicalVolume(solidDump,            //its solid
                        dump_mat,             //its material
                        "Dump");              //its name

  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,-5.2*m),         //at (0,0,0)
                    logicDump,               //its logical volume
                    "Dump",                  //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  //
  // Neutron Moderator
  //
  

  G4Tubs* solid_mod_volume =
    new G4Tubs("mod_volume",
        0.,           // inner radius
        r_mod,        // outer raduis
        0.5 * l_mod,  // half-length of the moderator
        0.,           // starting phi
        2.* M_PI * rad ); // ending phi (full cylinder)
  G4LogicalVolume* logic_mod_volume =
    new G4LogicalVolume(solid_mod_volume,
        mod_mat,
        "mod_volume");
  new G4PVPlacement(
      0,                                              // no rotation
      G4ThreeVector(0, 0, 0.5 * ( l_dump + l_mod )-5.2*m ), // translation
      logic_mod_volume,                               // its logical volume
      "mod_volume",                                   // its name
      logicWorld,                                     // its mother volume
      false,                                          // no boolean operation
      0,                                              // a copy number
      checkOverlaps);                                 // overlaps checking

  // Neutron moderator sidewall
  G4Tubs* solid_mod_sidewall =
    new G4Tubs("mod_sidewall",
        r_mod,
        r_mod + l_mod,
        0.5*(l_dump + l_mod),
        0.,
        2.* M_PI * rad );
  G4LogicalVolume* logic_mod_sidewall =
    new G4LogicalVolume(solid_mod_sidewall,
        mod_mat,
        "mod_sidewall");
  new G4PVPlacement(
      0,                              // no rotation
      G4ThreeVector(0, 0, 0.5*l_mod-5.2*m), // translation
      logic_mod_sidewall,             // its logical volume
      "mod_sidewall",                 // its name
      logicWorld,                     // its mother volume
      false,                          // no boolean operation
      0,                              // a copy number
      checkOverlaps);                 // overlaps checking
*/
////////////////////////////////////////////////
              
              
              
  //
  //always return the physical World
  //
	return physWorld; 
}
