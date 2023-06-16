#include "generator.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
	fParticleGun = new G4ParticleGun(10);
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4String particleName="neutron";
	G4ParticleDefinition *particle = particleTable->FindParticle("neutron");
	
	G4ThreeVector pos(0.,0.,-10.0*m);
	G4ThreeVector mom(0.,0.,1.);
	
	fParticleGun->SetParticlePosition(pos);
	fParticleGun->SetParticleMomentumDirection(mom);
	fParticleGun->SetParticleMomentum(800. *MeV);
	fParticleGun->SetParticleDefinition(particle);
	
	fParticleGun->GeneratePrimaryVertex(anEvent);
}
