Double_t GetShortestDistance(Double_t* r1, Double_t* r2, Double_t* p1, Double_t* p2, Double_t* s, Double_t* t);

void analysis2(const char* filename="final.root")
{
  cout << "DAMSA Analysis v0" << endl;
  TFile fInput(filename, "read");
  if( fInput.IsOpen() )
    cout << filename << " is successfully opened." << endl;
  TTree* fTree = (TTree*)fInput.Get("tRAONSCL2");

  Double_t e, px, py, pz;
  Double_t vx, vy, vz;
  Double_t gtime, ltime;
  Double_t v[3];
  Double_t p[3];
  char volumeName[32];
  char tr[32];
  Double_t t_e, t_px, t_py, t_pz;
  Double_t t_vx, t_vy, t_vz;
  Double_t t_gtime, t_ltime;
  Double_t t_v[3];
  Double_t t_p[3];
  Double_t s, t;
  char t_volumeName[32]; // temporary variables

  Double_t invm2, dt, dist;
  ULong64_t nEntries;
  fTree->SetBranchAddress("trackPID", &tr);
  fTree->SetBranchAddress("kineticEnergy", &e);
  fTree->SetBranchAddress("px", &px);
  fTree->SetBranchAddress("py", &py);
  fTree->SetBranchAddress("pz", &pz);
  fTree->SetBranchAddress("vertex_x", &vx);
  fTree->SetBranchAddress("vertex_y", &vy);
  fTree->SetBranchAddress("vertex_z", &vz);
  fTree->SetBranchAddress("gtime", &gtime);
  fTree->SetBranchAddress("ltime", &ltime);
  fTree->SetBranchAddress("volumeName", volumeName);

  TH1D* hdt  = new TH1D("hdt", ";#Delta t (ns);Entries", 100, 0, 10000);
  TH1D* hdca   = new TH1D("hdca", ";DCA;Entries", 100, 0, 10000);
  TH2D* hdist_dt = new TH2D("hdist_dt", ";dist;#Delta t (ns)", 200, 0, 20, 100, 0, 1000);


  nEntries = fTree->GetEntries();
  for( ULong64_t i = 0; i < nEntries; i++ )
  {
    cout << "Scanning i = " << i << " / " << nEntries << " (" << (Double_t)i/(Double_t)nEntries*100. << "%)." << endl;
    fTree->GetEntry(i);
    if( strcmp(tr, "gamma") != 0 ) continue; // select only gammas
    t_e  = e;
    t_px = px;
    t_py = py;
    t_pz = pz;
    t_vx = vx;
    t_vy = vy;
    t_vz = vz;
    t_v[0] = vx;
    t_v[1] = vy;
    t_v[2] = vz;
    t_p[0] = px;
    t_p[1] = py;
    t_p[2] = pz;
    t_gtime = gtime;
    t_ltime = ltime;  // copy values to temporary variables

    if( strcmp(volumeName, "physDownstreamSensorVolume") != 0 ) continue; // select only events in sensor volume
    if( e < 5 ) continue; // 5 MeV threshold
    strncpy(t_volumeName, volumeName, sizeof(volumeName));

    // Second loop
    for( ULong64_t j = nEntries-1; j > i; j-- )
    {
      fTree->GetEntry(j);
	if( strcmp(tr, "gamma") != 0 ) continue; // select only gammas
      if( strcmp(volumeName, "physDownstreamSensorVolume") != 0 ) continue; // select only events in sensor volume
      if( e < 5 ) continue; // 5 MeV threshold

      v[0] = vx;
      v[1] = vy;
      v[2] = vz;
      p[0] = px;
      p[1] = py;
      p[2] = pz;

     
      dt = (t_gtime - gtime >= 0)?(t_gtime - gtime):(gtime - t_gtime); //the time difference
      dist = GetShortestDistance(t_v, v, t_p, p, &s, &t);//distance of shortest approach
      //cout << "dist=" << dist << ", s=" << s << ", t=" << t << ", dt=" << dt << endl;

      
      hdist_dt->Fill(dist, dt);
      hdt->Fill(dt);
      hdca->Fill(dist); //filling the histograms
   
    }
  }

  TFile* fOutput = new TFile("output_analysis_v1.root", "recreate");
  hdist_dt->Write();
  hdt->Write();
  hdca->Write();
  fOutput->Close(); //printing to the root file
}

Double_t GetShortestDistance(Double_t* r1, Double_t* r2, Double_t* p1, Double_t* p2, Double_t* s, Double_t* t)
{
  Double_t p12  = p1[0]*p2[0] + p1[1]*p2[1] + p1[2]*p2[2];
  Double_t r2p2 = r2[0]*p2[0] + r2[1]*p2[1] + r2[2]*p2[2];
  Double_t r1p2 = r1[0]*p2[0] + r1[1]*p2[1] + r1[2]*p2[2];
  Double_t r2p1 = r2[0]*p1[0] + r2[1]*p1[1] + r2[2]*p1[2];
  Double_t r1p1 = r1[0]*p1[0] + r1[1]*p1[1] + r1[2]*p1[2];
  Double_t p1p1 = p1[0]*p1[0] + p1[1]*p1[1] + p1[2]*p1[2];
  Double_t p2p2 = p2[0]*p2[0] + p2[1]*p2[1] + p2[2]*p2[2];

  // Vertex parameter
  s[0] = (  p12 * (r2p2 - r1p2) - p2p2 * ( r2p1 - r1p1 ) ) / ( p12*p12 - p1p1*p2p2 );
  t[0] = ( p1p1 * (r2p2 - r1p2) -  p12 * ( r2p1 - r1p1 ) ) / ( p12*p12 - p1p1*p2p2 );

  // Distance calculator
  Double_t determinant =  ( r2[0] - r1[0] ) * ( p1[1] * p2[2] - p1[2] * p2[1] ) +
                          ( r2[1] - r1[1] ) * ( p1[2] * p2[0] - p2[2] * p1[0] ) +
                          ( r2[2] - r1[2] ) * ( p1[0] * p2[1] - p1[1] * p2[0] );
  determinant = fabs(determinant);
  Double_t denominator =  ( p1[1] * p2[2] - p1[2] * p2[1] ) * ( p1[1] * p2[2] - p1[2] * p2[1] ) +
                          ( p1[2] * p2[0] - p1[0] * p2[2] ) * ( p1[2] * p2[0] - p1[0] * p2[2] ) +
                          ( p1[0] * p2[1] - p1[1] * p2[0] ) * ( p1[0] * p2[1] - p1[1] * p2[0] );
  denominator = sqrt( denominator );
  return determinant/denominator;
}
