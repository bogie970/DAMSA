#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{}

MyDetectorConstruction::~MyDetectorConstruction()
{}

G4VPhysicalVolume *MyDetectorConstruction::Construct()
{
	G4NistManager *nist = G4NistManager::Instance();
	
	G4Element *W = nist->FindOrBuildElement("W");
	G4Element *N = nist->FindOrBuildElement("N");
	G4Element *O = nist->FindOrBuildElement("O");

	G4Material *Tungesten = new G4Material("Tungesten", 19.3*g/cm3, 1);
	Tungesten->AddElement(W, 100*perCent);
	//defining rindex of light
	G4double energy[2] = {1.239841939*eV/0.667, 1.239841939*eV/200.0};
	
	G4double rindexTungesten[2] = {3.83126, 242.14161};
	G4double rindexWorld[2] = {1.0, 1.0};
	//def rindex of tungeston
	G4MaterialPropertiesTable *mptTungesten = new G4MaterialPropertiesTable();
	mptTungesten->AddProperty("RINDEX", rindexTungesten, energy, 2);
	
	Tungesten->SetMaterialPropertiesTable(mptTungesten);
	
	
	



	//vacuum info
	// vacuum
        G4Material *worldMat = new G4Material("Vacuum", 1.e-25*g/cm3, 2);
        worldMat-> AddElement(N, 70*perCent);
        worldMat-> AddElement(O, 30*perCent);
        
        
        G4double world_sizeXY = 20.0*m;
  	G4double world_sizeZ  = 25.0*m;
  	G4double concrete_thickness = 1.0*m;
        G4Material *concrete = Tungesten;
   
	//G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");
	//defining rindex of world
	
	G4MaterialPropertiesTable *mptWorld = new G4MaterialPropertiesTable();
	mptWorld->AddProperty("RINDEX",rindexWorld,energy, 2);
	
	worldMat->SetMaterialPropertiesTable(mptWorld);
	
	G4Box *solidWorld = new G4Box("solidWorld", world_sizeXY/2,world_sizeXY/2,world_sizeZ/2);
	
	G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
	
	G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, true);	
	
/*	G4Box *solidRadiator = new G4Box("solidRadiator", 0.5*m,0.5*m,0.5*m);
	
	G4LogicalVolume *logicRadiator = new G4LogicalVolume(solidRadiator, Tungesten, "logicalRadiator");
	
	G4VPhysicalVolume *physRadiator = new G4PVPlacement(0 , G4ThreeVector(0.,0.,-2.4*m), logicRadiator, "physRadiator", logicWorld, false, 0, true);*/
	
	
	//Concrete Walls
	//Roof
	G4Box *solidRoof = new G4Box("solidRoof", world_sizeXY/2,concrete_thickness/2,world_sizeZ/2);
	
	G4LogicalVolume *logicRoof = new G4LogicalVolume(solidRoof, concrete, "logicalRoof");
	
	G4VPhysicalVolume *physRoof = new G4PVPlacement(0 , G4ThreeVector(0.,world_sizeXY/2 - concrete_thickness/2,0), logicRoof, "physRoof", logicWorld, false, 0, true);
	
	//Floor
	G4Box *solidFloor = new G4Box("solidFloor", world_sizeXY/2,concrete_thickness/2,world_sizeZ/2);
	
	G4LogicalVolume *logicFloor = new G4LogicalVolume(solidFloor, concrete, "logicalFloor");
	
	G4VPhysicalVolume *physFloor = new G4PVPlacement(0 , G4ThreeVector(0.,- world_sizeXY/2 + concrete_thickness/2,0), logicFloor, "physFloor", logicWorld, false, 0, true);
	
	return physWorld; 
}
