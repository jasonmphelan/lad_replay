// #include "../LAD_link_defs.h"

void replay_production_lad_mc(Int_t RunNumber = 0, Int_t MaxEvent = 0) {

  // Get RunNumber and MaxEvent if not provided.
  if (RunNumber == 0) {
    cout << "Enter a Run Number (-1 to exit): ";
    cin >> RunNumber;
    if (RunNumber <= 0)
      return;
  }
  if (MaxEvent == 0) {
    cout << "\nNumber of Events to analyze: ";
    cin >> MaxEvent;
    if (MaxEvent == 0) {
      cerr << "...Invalid entry\n";
      exit;
    }
  }



  const char *ROOTFileNamePattern = "ROOTfiles/hms_replay_sim_%d_%d.root";

  // Load Global parameters
  // Add variables to global list.
  gHcParms->Define("gen_run_number", "Run Number", RunNumber);
  gHcParms->AddString("g_ctp_database_filename", "DBASE/LAD/standard.database");
  gHcParms->Load(gHcParms->GetString("g_ctp_database_filename"), RunNumber);
  gHcParms->Load(gHcParms->GetString("g_ctp_parm_filename"));
  gHcParms->Load(gHcParms->GetString("g_ctp_kinematics_filename"), RunNumber);
  // Load params for HMS trigger configuration
  // gHcParms->Load("PARAM/TRIG/thms.param");
  // Load fadc debug parameters
  // gHcParms->Load("PARAM/HMS/GEN/h_fadc_debug.param");

  // const char* CurrentFileNamePattern = "low_curr_bcm/bcmcurrent_%d.param";
  // gHcParms->Load(Form(CurrentFileNamePattern, RunNumber));

  // Load the Hall C detector map
  gHcDetectorMap = new THcDetectorMap();
  // gHcDetectorMap->Load("MAPS/LAD/DETEC/HODO/lhodo_mc_input.map");
  gHcDetectorMap->Load("MAPS/LAD/DETEC/HODO/lhodo_mc.map");

  // // Add trigger apparatus
  // THaApparatus* TRG = new THcTrigApp("T", "TRG");
  // gHaApps->Add(TRG);
  // // Add trigger detector to trigger apparatus
  // THcTrigDet* hms = new THcTrigDet("hms", "HMS Trigger Information");
  // TRG->AddDetector(hms);

  // // Set up the equipment to be analyzed.
  // THcHallCSpectrometer* HMS = new THcHallCSpectrometer("H", "HMS");
  // gHaApps->Add(HMS);
  // // Add drift chambers to HMS apparatus
  // THcDC* dc = new THcDC("dc", "Drift Chambers");
  // HMS->AddDetector(dc);
  // // Add hodoscope to HMS apparatus
  // THcHodoscope* hod = new THcHodoscope("hod", "Hodoscope");
  // HMS->AddDetector(hod);
  // // Add Cherenkov to HMS apparatus
  // THcCherenkov* cer = new THcCherenkov("cer", "Heavy Gas Cherenkov");
  // HMS->AddDetector(cer);
  // // Add Aerogel Cherenkov to HMS apparatus
  // // THcAerogel* aero = new THcAerogel("aero", "Aerogel");
  // // HMS->AddDetector(aero);
  // // Add calorimeter to HMS apparatus
  // THcShower* cal = new THcShower("cal", "Calorimeter");
  // HMS->AddDetector(cal);

  // Add LAD detector
  THcLADSpectrometer *LAD = new THcLADSpectrometer("L", "LAD");
  gHaApps->Add(LAD);

  THcLADHodoscope *lhod = new THcLADHodoscope("hod", "LAD Hodoscope");
  LAD->AddDetector(lhod);

  // THcBCMCurrent* hbc = new THcBCMCurrent("H.bcm", "BCM current check");
  // gHaPhysics->Add(hbc);

  // Add rastered beam apparatus
  // THaApparatus* beam = new THcRasteredBeam("H.rb", "Rastered Beamline");
  // gHaApps->Add(beam);
  // Add physics modules
  // Calculate reaction point
  // THcReactionPoint* hrp = new THcReactionPoint("H.react", "HMS reaction point", "H", "H.rb");
  // gHaPhysics->Add(hrp);
  // Calculate extended target corrections
  // THcExtTarCor* hext = new THcExtTarCor("H.extcor", "HMS extended target corrections", "H", "H.react");
  // gHaPhysics->Add(hext);
  // Calculate golden track quantities
  // THaGoldenTrack* gtr = new THaGoldenTrack("H.gtr", "HMS Golden Track", "H");
  // gHaPhysics->Add(gtr);
  // Calculate primary (scattered beam - usually electrons) kinematics
  // THcPrimaryKine* hkin = new THcPrimaryKine("H.kin", "HMS Single Arm Kinematics", "H", "H.rb");
  // gHaPhysics->Add(hkin);
  // Calculate the hodoscope efficiencies
  // THcHodoEff* heff = new THcHodoEff("hhodeff", "HMS hodo efficiency", "H.hod");
  // gHaPhysics->Add(heff);

  // Add handler for prestart event 125.
  THcConfigEvtHandler *ev125 = new THcConfigEvtHandler("HC", "Config Event type 125");
  // ev125->AddEvtType(1);// Hack to set NSA,NSB,NPED for all events
  gHaEvtHandlers->Add(ev125);
  // Add handler for EPICS events
  THaEpicsEvtHandler *hcepics = new THaEpicsEvtHandler("epics", "HC EPICS event type 181");
  gHaEvtHandlers->Add(hcepics);
  // Add handler for scaler events
  // THcScalerEvtHandler *hscaler = new THcScalerEvtHandler("H", "Hall C scaler event type 2");
  // hscaler->AddEvtType(2);
  // hscaler->AddEvtType(129);
  // hscaler->SetDelayedType(129);
  // hscaler->SetUseFirstEvent(kTRUE);
  // gHaEvtHandlers->Add(hscaler);

  /*
  // Add event handler for helicity scalers
  THcHelicityScaler *hhelscaler = new THcHelicityScaler("H", "Hall C helicity scaler");
  //hhelscaler->SetDebugFile("HHelScaler.txt");
  hhelscaler->SetROC(5);
  hhelscaler->SetUseFirstEvent(kTRUE);
  gHaEvtHandlers->Add(hhelscaler);
  */

  // Add event handler for DAQ configuration event
  // THcConfigEvtHandler *hconfig = new THcConfigEvtHandler("hconfig", "Hall C configuration event handler");
  // gHaEvtHandlers->Add(hconfig);

  // Set up the analyzer - we use the standard one,
  // but this could be an experiment-specific one as well.
  // The Analyzer controls the reading of the data, executes
  // tests/cuts, loops over Acpparatus's and PhysicsModules,
  // and executes the output routines.
  THcAnalyzer *analyzer = new THcAnalyzer;

  // Set the decoder to use simulation input
  THcInterface::SetDecoder(LADSimDecoder::Class());

  // A simple event class to be output to the resulting tree.
  // Creating your own descendant of THaEvent is one way of
  // defining and controlling the output.
  THaEvent *event = new THaEvent;

  // Define the run(s) that we want to analyze.
  // We just set up one, but this could be many.
  // THcRun* run = new THcRun( pathList, Form(RunFileNamePattern, RunNumber) );

  // TString run_file = Form("%s/%s.root", "/volatile/hallc/c-lad/ehingerl",filebase.c_str());
  // TString run_file = "../libLADdig/test_scripts/lad_hodo_sim_large.root";
  TString run_file = "../libLADdig/test_scripts/lad_hodo_sim_time_walk.root";
  THaRunBase *run  = new LADSimFile(run_file.Data(), "gmn", "");

  // Set to read in Hall C run database parameters
  run->SetRunParamClass("THcRunParameters");

  // Eventually need to learn to skip over, or properly analyze the pedestal events
  run->SetEventRange(1, MaxEvent); // Physics Event number, does not include scaler or control events.
  // run->SetNscan(1);
  run->SetDataRequired(0);
  run->SetDate("2022-10-06 00:00:00");
  run->Print();

  // Define the analysis parameters
  TString ROOTFileName = Form(ROOTFileNamePattern, RunNumber, MaxEvent);
  analyzer->SetCountMode(2); // 0 = counter is # of physics triggers
                             // 1 = counter is # of all decode reads
                             // 2 = counter is event number

  analyzer->SetEvent(event);
  // Set EPICS event type
  analyzer->SetEpicsEvtType(181);
  // Define crate map
  analyzer->SetCrateMapFileName("MAPS/db_cratemap_mc.dat");
  // Define output ROOT file
  analyzer->SetOutFile(ROOTFileName.Data());
  // Define output DEF-file
  analyzer->SetOdefFile("DEF-files/LAD/PRODUCTION/lstackana_production_all.def");
  // Define cuts file
  analyzer->SetCutFile("DEF-files/HMS/PRODUCTION/CUTS/hstackana_production_cuts.def"); // optional
  // File to record cuts accounting information for cuts
  // analyzer->SetSummaryFile(Form("REPORT_OUTPUT/HMS/PRODUCTION/summary_production_%d_%d.report", RunNumber,
  // MaxEvent));    // optional Start the actual analysis.
  analyzer->Process(run);
  // Create report file from template.
  analyzer->PrintReport("TEMPLATES/LAD/PRODUCTION/lstackana_production.template",
                        Form("REPORT_OUTPUT/HMS/PRODUCTION/replay_hms_production_%d_%d.report", RunNumber, MaxEvent));
}

int main(int argc, char *argv[]) {
  gSystem->SetFlagsOpt("-g -O0");
  new THcInterface("The Hall C analyzer", &argc, argv, 0, 0, 1);
  // const
  int runnum;
  uint nev = -1;
  if (argc < 2 || argc > 3) {
    cout << "Usage: replay_gen filebase(char*) nev(uint)" << endl;
    return -1;
  }
  runnum = atoi(argv[1]);
  if (argc == 3)
    nev = atoi(argv[2]);

  replay_production_lad_mc(runnum, nev);
  return 0;
}
