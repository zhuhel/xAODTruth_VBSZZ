#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"

#include <MyAODAnalysis/Analysisllqq.h>
#include <MyAODAnalysis/GetElicityAngles.h>
//#include "CxAODTools/ReturnCheck.h"


#define EL_RETURN_CHECK( CONTEXT, EXP )                              \
  if (EXP !=  EL::StatusCode::SUCCESS) {                      \
    Error( CONTEXT, "Failed to execute: %s. Exiting.", #EXP); \
    return EL::StatusCode::FAILURE;                           \
  }
#define TOOL_CHECK( CONTEXT, EXP )                            \
  if (!EXP.isSuccess()) {                                     \
    Error( CONTEXT, "Failed to execute: %s. Exiting.", #EXP); \
    return EL::StatusCode::FAILURE;                           \
  }

#define EL_CHECK( CONTEXT, EXP )                              \
  if (EXP !=  EL::StatusCode::SUCCESS) {                      \
    Error( CONTEXT, "Failed to execute: %s. Exiting.", #EXP); \
    return EL::StatusCode::FAILURE;                           \
  }

#define CP_CHECK( CONTEXT, EXP, DEBUG )                       \
  if (EXP ==  CP::CorrectionCode::Error) {                    \
    Error( CONTEXT, "Failed to execute: %s. Exiting.", #EXP); \
    return EL::StatusCode::FAILURE;                           \
  }

// this is needed to distribute the algorithm to the workers
ClassImp(Analysisllqq)



Analysisllqq :: Analysisllqq () :
  m_doPDFrwt(false),
  m_doDressing(1),
  m_debug(0)
{
  // Here you put any code for the base initialization of variables,
  // e.g. initialize all pointers to 0.  Note that you should only put
  // the most basic initialization here, since this method will be
  // called on both the submission and the worker node.  Most of your
  // initialization code will go into histInitialize() and
  // initialize().
  HM = new HistoManager();
}



EL::StatusCode Analysisllqq :: setupJob (EL::Job& job)
{
  // Here you put code that sets up the job on the submission object
  // so that it is ready to work with your algorithm, e.g. you can
  // request the D3PDReader service or add output files.  Any code you
  // put here could instead also go into the submission script.  The
  // sole advantage of putting it here is that it gets automatically
  // activated/deactivated when you add/remove the algorithm from your
  // job, which may or may not be of value to you.

  Info("setupJob()", "Setting up job.");

  // setup for xAOD
  job.useXAOD();
  // let's initialize the algorithm to use the xAODRootAccess package
  xAOD::Init("AnalysisBase").ignore(); // call before opening first file
  
  // tell EventLoop about our output xAOD:
  EL::OutputStream out("outputLabel","xAODNoMeta");
  job.outputAdd (out);

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Analysisllqq :: histInitialize ()
{
  // Here you do everything that needs to be done at the very
  // beginning on each worker node, e.g. create histograms and output
  // trees.  This method gets called before any input files are
  // connected.

  Info("histInitialize()", "histInitialize.");

  // book histograms
  // HM->InitVar("Var1,Var2,...",nBin,xMin,xMax);  // use comma "," to separate Var's
  HM->InitVar("mc_weight",10000,0,100); // 0: both, 1: inTreeOnly, 2: inHistOnly
  HM->InitVar("QCDscale_weights,PDFint_weights,PDFext_weights,alphaS_weights",10000,0,100,1,0); // 0: both, 1: inTreeOnly, 2: inHistOnly
  // basic kinematics
  HM->InitVar("L1Pt,L2Pt,L3Pt,L4Pt,PtL3",5000,0,5000);
  HM->InitVar("L1Eta,L2Eta,L3Eta,L4Eta",2000,-10,10);
  HM->InitVar("L1Phi,L2Phi,L3Phi,L4Phi",640,-3.2,3.2);
  HM->InitVar("PtEl,smeared_PtEl,PtMu,smeared_PtMu",5000,0,5000,1,0);
  HM->InitVar("muEff,elEff",100,0,1,1,0);
  HM->InitVar("muProb,elProb",100,0,1,1,0);
  HM->InitVar("AllLEta",2000,-10,10,1,0);
  HM->InitVar("AllLPhi",640,-3.2,3.2,1,0);
  HM->InitVar("MZAll",5000,0,5000,1,0);
  HM->InitVar("MZ1,MZ2",5000,0,5000);
  HM->InitVar("dPhiZZ",1000,-5,5);

  HM->InitVar("TagJJM,MJJ",5000,0,5000);
  HM->InitVar("YZ1Star,YZ2Star,PtZZJJOHtZZJJ",5000,-500,500);
  HM->InitVar("TagJJPt,PtJ1,PtJ2",5000,0,5000);
  HM->InitVar("TagJJdEta,TagJJdEta_final",2000,0,20);
  HM->InitVar("EtaJ1xJ2,YJ1xJ2",2000,-100,100);
  HM->InitVar("dYJJ",2000,-10,10);
  HM->InitVar("PtZZ,PtZ1,PtZ2,MET",5000,0,5000);
  HM->InitVar("TagJPt",5000,0,5000,1,0);
  HM->InitVar("hsJetPt,hsJetPt_central,hsJetPt_forward",5000,0,5000,1,0);
  HM->InitVar("puJetPt,puJetPt_central,puJetPt_forward",5000,0,5000,1,0);
  HM->InitVar("JetEta",2000,-10,10,1,0);

  HM->InitVar("MZZ",5000,0,5000);

  HM->InitVar("Cutflow_incl_Incl,Cutflow_incl_2e2mu,Cutflow_incl_4mu,Cutflow_incl_4e",50,0,50,1);

  HM->InitVar("pass_4lep,pass_lPt,pass_2e2mu,pass_4mu,pass_4e,pass_MZs,pass_Mll,pass_2jet,pass_tagjets,pass_VBS,pass_MZZ",10,0,10); 
  HM->InitVar("pass_2lep,pass_MZ,pass_MET",10,0,10); 
  HM->InitVar("type_event",10,0,10); //0: cannot regonize; 1: ee; 2: mm; 3: em.
  HM->InitVar("nVboson,nLep,nEl,nMu,nLepStatus1,nLepStatus3,nLepStatus11",10,0,10);
  HM->InitVar("njet,nGoodjet,NBjets", 10, 0, 10);

  HM->InitVar("ZZCen,Z1Cen,Z2Cen",1000,-50,50);
  HM->InitVar("dR_s13",1000,-5,5);

  // creat histograms defined in InitVar
  HM->CreateHistoMap();

  // write variables defined in InitVar into tree
  HM->AddVarIntoTree();

  // write histograms to output
  for (auto h : HM->getHistos()) {
    wk()->addOutput(h);
  }
  wk()->addOutput(HM->getTree());

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Analysisllqq :: fileExecute ()
{
  // Here you do everything that needs to be done exactly once for every
  // single file, e.g. collect a list of all lumi-blocks processed

  Info("fileExecute()", "fileExecute.");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Analysisllqq :: changeInput (bool firstFile)
{
  // Here you do everything you need to do when we change input files,
  // e.g. resetting branch addresses on trees.  If you are using
  // D3PDReader or a similar service this method is not needed.

  Info("changeInput()", "changeInput.");

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Analysisllqq :: initialize ()
{
  // Here you do everything that you need to do after the first input
  // file has been connected and before the first event is processed,
  // e.g. create additional histograms based on which variables are
  // available in the input files.  You can also create all of your
  // histograms and trees in here, but be aware that this method
  // doesn't get called if no events are processed.  So any objects
  // you create here won't be available in the output if you have no
  // input events.

  Info("initialize()", "Initialize.");

  // initialize xAOD event
  EL_RETURN_CHECK("initialize()",initializeEvent()    );
  // initialize tools
  EL_RETURN_CHECK("initialize()",initializeTools()    );

  return EL::StatusCode::SUCCESS;
}

EL::StatusCode Analysisllqq::initializeEvent() {
 
  Info("initializeEvent()", "Initialize event.");

  // get TEvent
  m_event = wk()->xaodEvent();

  // sample name
  TString sampleName = wk()->metaData()->castString("sample_name");
  Info("initialize()", "Sample name = %s", sampleName.Data());
  // as a check, let's see the number of events in our xAOD (long long int)
  Info("initialize()", "Number of events in file   = %lli", m_event->getEntries());
  if(m_maxEvent < 0) m_maxEvent = m_event->getEntries() ;
  Info("initialize()", "Number of events to run on = %li", m_maxEvent);

  // count number of events
  m_eventCounter = 0;
 
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode Analysisllqq::initializeTools() {
 
  Info("initializeTools()", "Initialize tools.");

  m_OR = new OverlapRemovalTool("OverlapRemovalTool");
  m_OR->msg().setLevel(MSG::INFO);

  // Set name of decoration which tells the OR tool which objects to consider
  // if set to "false", the object automatically passes OR, i.e. the OROutputLabel is 0 (as in "do not remove this object").
  TOOL_CHECK("OverlapRemoval::initialize()", m_OR->setProperty("InputLabel", "ORInputLabel"));
  // Set name of decoration the OR tool will add as decoration
  // if decoration is "false" the object should be removed, while if "true" the object should be kept.
  TOOL_CHECK("OverlapRemoval::initialize()", m_OR->setProperty("OverlapLabel", "OROutputLabel"));
  TOOL_CHECK("OverlapRemoval::initialize()", m_OR->setProperty("WriteSharedTrackFlag", true));

  TOOL_CHECK("OverlapRemoval::initialize()", m_OR->initialize());

  // pdf reweighting
  m_doPDFrwt=false;
  std::string pdf_nom = "NNPDF30_nnlo_as_0118";
  std::string pdf_new = "NNPDF30_nnlo_as_0117";
  std::string pdf_new2 = "NNPDF30_nnlo_as_0119";
  //m_config->getif<bool>("doPDFrwt", doPDFrwt);
  //m_config->getif<std::string>("pdf_nom", pdf_nom);
  //m_config->getif<std::string>("pdf_new", pdf_new);
  if(m_doPDFrwt && pdf_nom != "" && pdf_new != "") {
    m_PDFset_nom = new LHAPDF::PDFSet(pdf_nom);
    m_PDFset_nom_pdfs = m_PDFset_nom->mkPDFs(); // pointers to PDF set members, for internal PDF uncertainty
    m_PDFset_new = new LHAPDF::PDFSet(pdf_new);  // for PDF reweighting
    m_PDFset_new_pdfs = m_PDFset_new->mkPDFs();
    m_PDFset_new2 = new LHAPDF::PDFSet(pdf_new2);  // for PDF reweighting
    m_PDFset_new2_pdfs = m_PDFset_new2->mkPDFs();
  }

  // Configure the W/Z taggers
  std::string tagger = "smooth";
  std::string working_point = "medium";

  std::string w_config = "$ROOTCOREBIN/data/JetSubStructureUtils/config_13TeV_Wtagging_MC15_Prerecommendations_20150809.dat";
  std::string z_config = "$ROOTCOREBIN/data/JetSubStructureUtils/config_13TeV_Ztagging_MC15_Prerecommendations_20150809.dat";
  m_wTaggerMedium = new JetSubStructureUtils::BosonTag(working_point, tagger, w_config, m_debug, m_debug);
  m_zTaggerMedium = new JetSubStructureUtils::BosonTag(working_point, tagger, z_config, m_debug, m_debug);

  
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode Analysisllqq :: execute ()
{
  // Here you do everything that needs to be done on every single
  // events, e.g. read input variables, apply cuts, and fill
  // histograms and trees.  This is where most of your actual analysis
  // code will go.

  if( (m_eventCounter % 10000) == 0) {
    Info("execute()", "Event number = %i", m_eventCounter );
  }
  m_eventCounter++;
  //if(m_eventCounter>5000) return EL::StatusCode::SUCCESS;

  HM->ClearVariables();

  // retrieve event info
  EL_RETURN_CHECK( "retrieve_m_eventInfo()", m_event->retrieve(m_eventInfo, "EventInfo"));

  // retrieve truth particle container
  EL_RETURN_CHECK( "retrieve_xTruthParticleContainer()", m_event->retrieve( m_truthParts, "TruthParticles"));
  // retrieve truth thin jets
  //EL_RETURN_CHECK( "retrieve_AntiKt4TruthJets()", m_event->retrieve( m_truthSmallRJets, "AntiKt4TruthJets"));
  EL_RETURN_CHECK( "retrieve_AntiKt4TruthJets()", m_event->retrieve( m_truthSmallRJets, "AntiKt4TruthDressedWZJets"));
  //EL_RETURN_CHECK( "retrieve_AntiKt4TruthJets()", m_event->retrieve( m_truthSmallRJets, "AntiKt4TruthWZJets"));
  // retrieve truth Ewectrons
  //EL_RETURN_CHECK( "retrieve_TruthElectron()", m_event->retrieve( m_truthElectronCont, "TruthElectrons"));
  //EL_RETURN_CHECK( "retrieve_TruthElectron()", m_event->retrieve( m_truthElectronCont, "STDMTruthElectrons"));
  // retrieve truth Muons
  //EL_RETURN_CHECK( "retrieve_TruthMuon()", m_event->retrieve( m_truthMuonCont, "TruthMuons"));
  //EL_RETURN_CHECK( "retrieve_TruthMuon()", m_event->retrieve( m_truthMuonCont, "STDMTruthMuons"));
  // retrieve truth Taus
  //EL_RETURN_CHECK( "retrieve_TruthTau()", m_event->retrieve( m_truthTauCont, "TruthTaus"));
  // retrieve trimmed truth fat jets
  //EL_RETURN_CHECK( "retrieve_TrimmedAntiKt10TruthJets()", m_event->retrieve( m_truthLargeRJets, "AntiKt10TruthTrimmedPtFrac5SmallR20Jets"));
  // retrieve truth MET
  //EL_RETURN_CHECK( "retrieve_TruthMET()", m_event->retrieve( m_truthMET, "MET_Truth"));

  //EL_RETURN_CHECK( "retrieve_TruthEvent()", m_event->retrieve( m_truthEventCont, "TruthEvents"));

  // check if the MC sample is interesting to us
  mc_channel = m_eventInfo->mcChannelNumber();
  eventNumber = m_eventInfo->eventNumber();

  // MC weight
  int i_nom = 0; 
  HM->Evt_weight = m_eventInfo->mcEventWeight(i_nom);
  HM->Var["mc_weight"] = m_eventInfo->mcEventWeight(i_nom);
  //if(m_eventInfo->mcEventWeight(i_nom)>100.) return EL::StatusCode::SUCCESS; 
  //std::cout << "m_eventInfo->mcEventWeight(100): " << m_eventInfo->mcEventWeight(100) << std::endl; 

  // ========================= Save those theoretical weights on the fly ===========================
   
  for(int i=4; i<11; i++){
     HM->VVar["QCDscale_weights"]->push_back(m_eventInfo->mcEventWeight(i) );
  }  
  for(int i=11; i<111; i++){
     HM->VVar["PDFint_weights"]->push_back(m_eventInfo->mcEventWeight(i) );
  }  
  HM->VVar["PDFext_weights"]->push_back(m_eventInfo->mcEventWeight(113) );
  HM->VVar["PDFext_weights"]->push_back(m_eventInfo->mcEventWeight(114) );
  HM->VVar["alphaS_weights"]->push_back(m_eventInfo->mcEventWeight(111) );
  HM->VVar["alphaS_weights"]->push_back(m_eventInfo->mcEventWeight(112) );
  
  //fillTest4l();
  fillllvv();

  // overlap removal for reco jets (thin jets only)
  //doOverlapRM();

  // PDF reweighting
  if(m_doPDFrwt) doPDFrwt(m_eventInfo);

  // Fill histograms
  HM->FillHistograms();
  HM->FillTree();

  return EL::StatusCode::SUCCESS;

}

EL::StatusCode Analysisllqq :: postExecute ()
{
  // Here you do everything that needs to be done after the main event
  // processing.  This is typically very rare, particularly in user
  // code.  It is mainly used in implementing the NTupleSvc.
  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Analysisllqq :: finalize ()
{
  // This method is the mirror image of initialize(), meaning it gets
  // called after the last event has been processed on the worker node
  // and allows you to finish up any objects you created in
  // initialize() before they are written to disk.  This is actually
  // fairly rare, since this happens separately for each worker node.
  // Most of the time you want to do your post-processing on the
  // submission node after all your histogram outputs have been
  // merged.  This is different from histFinalize() in that it only
  // gets called on worker nodes that processed input events.

  if (m_OR) delete m_OR;
  if (m_wTaggerMedium) delete m_wTaggerMedium;
  if (m_zTaggerMedium) delete m_zTaggerMedium;

  return EL::StatusCode::SUCCESS;
}



EL::StatusCode Analysisllqq :: histFinalize ()
{
  // This method is the mirror image of histInitialize(), meaning it
  // gets called after the last event has been processed on the worker
  // node and allows you to finish up any objects you created in
  // histInitialize() before they are written to disk.  This is
  // actually fairly rare, since this happens separately for each
  // worker node.  Most of the time you want to do your
  // post-processing on the submission node after all your histogram
  // outputs have been merged.  This is different from finalize() in
  // that it gets called on all worker nodes regardless of whether
  // they processed input events.
  return EL::StatusCode::SUCCESS;
}


EL::StatusCode Analysisllqq :: doPDFrwt (const xAOD::EventInfo* eventInfo)
{

  std::vector<double> pdf_weights;

  int id1=1;
  int id2=1;
  float x1 = 0;
  float x2 = 0;
  float Q = 0;

  // get truth event
  const xAOD::TruthEvent * truthEvent = m_truthEventCont->at(0);
  if ( ! truthEvent->pdfInfoParameter(id1, xAOD::TruthEvent::PdfParam::PDGID1) ) {
    Error("Analysisllqq::doPDFrwt()","Failed to retrieve xAOD::TruthEvent::PdfParam::PDGID1");
    return EL::StatusCode::FAILURE;
  }
  if ( ! truthEvent->pdfInfoParameter(id2, xAOD::TruthEvent::PdfParam::PDGID2) ) {
    Error("Analysisllqq::doPDFrwt()","Failed to retrieve xAOD::TruthEvent::PdfParam::PDGID2");
    return EL::StatusCode::FAILURE;
  }
  if ( ! truthEvent->pdfInfoParameter(x1, xAOD::TruthEvent::PdfParam::X1) ) {
    Error("Analysisllqq::doPDFrwt()","Failed to retrieve xAOD::TruthEvent::PdfParam::X1");
    return EL::StatusCode::FAILURE;
  }
  if ( ! truthEvent->pdfInfoParameter(x2, xAOD::TruthEvent::PdfParam::X2) ) {
    Error("Analysisllqq::doPDFrwt()","Failed to retrieve xAOD::TruthEvent::PdfParam::X2");
    return EL::StatusCode::FAILURE;
  }
  if ( ! truthEvent->pdfInfoParameter(Q, xAOD::TruthEvent::PdfParam::Q) ) {
    Error("Analysisllqq::doPDFrwt()","Failed to retrieve xAOD::TruthEvent::PdfParam::Q");
    return EL::StatusCode::FAILURE;
  }

  //std::cout << "Check=> doPDFrwt(): id1= " << id1 << " id2= " << id2 << " x1= " << x1 << " x2= " << x2 << " Q= " << Q << std::endl;

  // PDF error set
  for (size_t imem = 0; imem <= m_PDFset_nom_pdfs.size()-1; imem++) {
    pdf_weights.push_back(  LHAPDF::weightxxQ( id1, id2, x1, x2, Q, m_PDFset_nom_pdfs[0], m_PDFset_nom_pdfs[imem] )  ); // one event weight for each error in the set including the nominal
  }
  // PDF reweighting
  pdf_weights.push_back(  LHAPDF::weightxxQ( id1, id2, x1, x2, Q, m_PDFset_nom_pdfs[0], m_PDFset_new_pdfs[0] )  ); // one event weight for each error in the set including the nominal
  pdf_weights.push_back(  LHAPDF::weightxxQ( id1, id2, x1, x2, Q, m_PDFset_nom_pdfs[0], m_PDFset_new2_pdfs[0] )  ); // one event weight for each error in the set including the nominal

  for(int iwt=0; iwt < pdf_weights.size(); iwt ++){
     //std::cout << "iwt = " << iwt << "  pdf_weights = " << pdf_weights[iwt] << std::endl;
     HM->VVar["pdf_weights"]->push_back(pdf_weights[iwt]);
  }
  //std::cout << "End of doPDFrwt()" << std::endl;

  return EL::StatusCode::SUCCESS;
}
