void analyze()
{
  TFile fInput("dms_full_cylinder_800_1e8_POT.root", "READ");

  Int_t     event_type;

  Double_t  kineticEnergy;
  Double_t  totalEnergy;
  Double_t  vertex_x, vertex_y, vertex_z;
  Double_t  px, py, pz,gt,lt;
  Double_t  theta, phi;
  ULong64_t nEntries;

  TH1D* hKineticEnergy = new TH1D("hKineticEnergy", ";Kinetic Energy (MeV);Entries", 8000, 0, 800);
  TH1D* hTotalEnergy  = new TH1D("hTotalEnergy",  ";Total Energy (MeV);Entries", 17400, 0, 1740); // 1740 = 800 + 940, 940 ~ 939 MeV = M_n, neutron mass
  TH1D* hvx = new TH1D("hvx", ";Vertex Position, #it{x} (cm);Entries", 1100, -550, 550);
  TH1D* hvy = new TH1D("hvy", ";Vertex Position, #it{y} (cm);Entries", 1100, -550, 550);
  TH1D* hvz = new TH1D("hvz", ";Vertex Position, #it{z} (cm);Entries", 300, 400, 700);
  TH1D* hpx = new TH1D("px", ";Vertex Momentum, #it{x} ();Entries", 800, -400, 400);
  TH1D* hpy = new TH1D("py", ";Vertex Momentum, #it{y} ();Entries", 800, -400, 400);
  TH1D* hpz = new TH1D("pz", ";Vertex Momentum, #it{z} ();Entries", 1400, -400, 1000);
  TH1D* hgt = new TH1D("gt", ";Local Time, #it{t} (ns);Entries", 12000, 0, 12000);
  TH1D* hlt = new TH1D("lt", ";Global Time, #it{t} (ns);Entries", 12000, 0, 12000);

  TH2D* hvx_vy = new TH2D("hvx_vy", ";Vertex Position, #it{x} (c);Vertex Position, #it{y} (cm)", 1100, -550, 550, 1100, -550, 550);
  TH2D* hvz_vy = new TH2D("hvy_vz", ";Vertex Position, #it{z} (c);Vertex Position, #it{y} (cm)", 1200, 400, 700, 1100, -550, 550);
  TH1D* hPhi = new TH1D("hPhi", ";Azimuthal angle w.r.t z-axi, #phi (deg.);Entries/0.1 (deg)", 3600, 0, 180);
  TH1D* hTheta  = new TH1D("hTheta", ";sAngle in x-y plane, #theta (deg.);Entries/0.1 (deg)", 3600, -180, 180);
  TH2D* h_skymap = new TH2D("h_skymap", ";z-dir;x-dir", 1000, -1.1, 1.1, 1000, -1.1, 1.1);


  TTree* fTree = (TTree*)fInput.Get("DMSNtuple");


  fTree->SetBranchAddress("kineticEnergy", &kineticEnergy);
  fTree->SetBranchAddress("totalEnergy", &totalEnergy);
  fTree->SetBranchAddress("vx", &vertex_x);
  fTree->SetBranchAddress("vy", &vertex_y);
  fTree->SetBranchAddress("vz", &vertex_z);
  fTree->SetBranchAddress("px", &px);
  fTree->SetBranchAddress("py", &py);
  fTree->SetBranchAddress("pz", &pz);
  fTree->SetBranchAddress("gTime", &gt);
  fTree->SetBranchAddress("lTime", &lt);

  nEntries = fTree->GetEntries();


  for(ULong64_t i = 0; i < nEntries; i++)
  {
    
    fTree->GetEntry(i);
    phi = acos( pz / sqrt( px*px + py*py + pz*pz ) ) * TMath::RadToDeg(); //angle wrt to z

    theta   = py / abs(py) * acos( px / sqrt(px*px + py*py) ) * TMath::RadToDeg(); //... should be uniform due to symmetry
    
      hKineticEnergy->Fill(kineticEnergy);
      hTotalEnergy->Fill(totalEnergy);
      hvx->Fill(vertex_x);
      hvy->Fill(vertex_y);
      hvz->Fill(vertex_z);
      hpx->Fill(px);
      hpy->Fill(py);
      hpz->Fill(pz);
      hgt->Fill(gt);
      hlt->Fill(lt);
      hvx_vy->Fill(vertex_x, vertex_y);
      hvz_vy->Fill(vertex_z, vertex_y);
      hTheta->Fill(theta);
      hPhi->Fill(phi);
      h_skymap->Fill(pz / abs(pz) * abs(cos(phi)) ,px / abs(px) * abs(cos(theta) * sin(phi)));
      if(i%100000==0){
      Double_t f=i/nEntries;
      cout << "Completion: " << i<<"/"<<nEntries<<" = "<<f *100<< "%"<<endl;}
    
  }
  TFile* fOutput = new TFile("output_analysis_cylidner_800_full_1e8.root", "recreate");
   
  hKineticEnergy->Write();
  hTotalEnergy->Write();
  hvx->Write();
  hvy->Write();
  hvz->Write();
  hpx->Write();
  hpy->Write();
  hgt->Write();
  hlt->Write();
  h_skymap->Write();
  hpz->Write();
  hvx_vy->Write();
  hvz_vy->Write();
  hTheta->Write();
  hPhi->Write();
  fOutput->Close();
  fInput.Close();
}
