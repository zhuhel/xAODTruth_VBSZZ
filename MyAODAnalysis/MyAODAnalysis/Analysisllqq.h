#ifndef MyAODAnalysis_Analysisllqq_H
#define MyAODAnalysis_Analysisllqq_H

#include "TH1.h"
#include "TH2.h"

#include <EventLoop/Algorithm.h>

// Infrastructure include(s):
// EDM includes (which rootcint doesn't like)
#ifndef __MAKECINT__

#ifndef XAODMUON_ELECTRONCONTAINER_H
#include "xAODEgamma/ElectronContainer.h"
#include "xAODEgamma/ElectronAuxContainer.h"
#endif

#ifndef XAODMUON_PHOTONCONTAINER_H
#include "xAODEgamma/PhotonContainer.h"
#include "xAODEgamma/PhotonAuxContainer.h"
#endif

#ifndef XAODMUON_JETCONTAINER_H
#include "xAODJet/JetContainer.h"
#include "xAODJet/JetAuxContainer.h"
#endif

#ifndef XAODMUON_MUONCONTAINER_H
#include "xAODMuon/MuonContainer.h"
#include "xAODMuon/MuonAuxContainer.h"
#endif

#ifndef XAODMUON_TAUCONTAINER_H
#include "xAODTau/TauJetContainer.h"
#include "xAODTau/TauJetAuxContainer.h"
#endif

#ifndef XAODMUON_MISSINGETCONTAINER_H
#include "xAODMissingET/MissingETContainer.h"
#include "xAODMissingET/MissingETAuxContainer.h"
#endif

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"

#include "xAODEventInfo/EventInfo.h"

#include "xAODTruth/TruthEventContainer.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODTruth/TruthParticleContainer.h"
#include "xAODTruth/TruthVertexContainer.h"

#include "AssociationUtils/OverlapRemovalTool.h"

#endif // not __MAKECINT__

#include "CxAODTools/CommonProperties.h"
#include "CxAODTools/OverlapRemoval.h"

#include "JetSubStructureUtils/BosonTag.h"

#include "MyAODAnalysis/HistoManager.h"

//PDF uncertainty
#include "LHAPDF/LHAPDF.h"
#include "LHAPDF/Reweighting.h"

class Analysisllqq : public EL::Algorithm
{
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
public:
  // float cutValue;



  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
public:
  // Tree *myTree; //!
  // TH1 *myHist; //!
  TH1F *myHist; //!
  TH2F *myHist2; //!
  TH2F *h2d_met1; //!
  TH2F *h2d_met2; //!
  TH1D *JetUncer; //!
  xAOD::TEvent * m_event; //!
  long m_maxEvent; //!
  long m_eventCounter; //!
  long eventNumber; //!
  int  mc_channel; //!
  int  m_debug; //!
  int  m_doDressing; //!

  const xAOD::EventInfo* m_eventInfo ; //!
  const xAOD::ElectronContainer*  m_electrons; //!
  const xAOD::PhotonContainer*    m_photons;   //!
  const xAOD::MuonContainer*      m_muons;     //!
  const xAOD::TauJetContainer*    m_taus;      //!
  const xAOD::JetContainer* m_jets; //!
  const xAOD::JetContainer* m_truthSmallRJets; //!
  const xAOD::JetContainer* m_truthLargeRJets; //!
  const xAOD::TruthParticleContainer* m_truthParts ; //!
  const xAOD::TruthEventContainer * m_truthEventCont; //!
  const xAOD::TruthParticleContainer * m_truthMuonCont; //!
  const xAOD::TruthParticleContainer * m_truthElectronCont; //!
  const xAOD::TruthParticleContainer * m_truthTauCont; //!
  const xAOD::MissingETContainer*  m_truthMET; //!


  OverlapRemovalTool* m_OR; //!
  // histogram handler
  HistoManager *HM; //!

  JetSubStructureUtils::BosonTag* m_wTaggerMedium; //!
  JetSubStructureUtils::BosonTag* m_zTaggerMedium; //!

  // this is a standard constructor
  Analysisllqq ();

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode setupJob (EL::Job& job);
  virtual EL::StatusCode fileExecute ();
  virtual EL::StatusCode histInitialize ();
  virtual EL::StatusCode changeInput (bool firstFile);
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode postExecute ();
  virtual EL::StatusCode finalize ();
  virtual EL::StatusCode histFinalize ();

  // initialze events
  EL::StatusCode initializeEvent();
  // initialze tools
  EL::StatusCode initializeTools();
  // trace up truth particles
  //int TraceUp(const xAOD::TruthParticle* particle);
  const xAOD::TruthParticle* TraceUp(const xAOD::TruthParticle* particle, int mother_pdgId=24, int mother_status=22 );
  const xAOD::TruthParticle* TraceDown(const xAOD::TruthParticle* particle, int child_pdgId=24, int child_status=22 );
  int TraceUpHadron(const xAOD::TruthParticle* particle);
  TLorentzVector getDressing(const xAOD::TruthParticle* lep);
  // overlap removal 
  EL::StatusCode doOverlapRM();
  float deltaR(float eta1, float phi1, float eta2, float phi2);
  float deltaPhi(float phi1, float phi2);

  // VBSZZ truth selection
  virtual EL::StatusCode fillTest4l();
  virtual EL::StatusCode fillllvv();

  bool decorateJetsBTag(const xAOD::Jet *j);

  // PDF reweighting
  bool m_doPDFrwt; //!
  EL::StatusCode doPDFrwt (const xAOD::EventInfo* eventInfo);
  const LHAPDF::PDFSet *m_PDFset_nom; //!
  vector<LHAPDF::PDF*> m_PDFset_nom_pdfs; //!
  const LHAPDF::PDFSet *m_PDFset_new; //!
  vector<LHAPDF::PDF*> m_PDFset_new_pdfs; //!
  const LHAPDF::PDFSet *m_PDFset_new2; //!
  vector<LHAPDF::PDF*> m_PDFset_new2_pdfs; //!

  // this is needed to distribute the algorithm to the workers
  ClassDef(Analysisllqq, 1);
};

// sort particle containter by pt
static bool sort_par_pt(const xAOD::IParticle* parA, const xAOD::IParticle* parB) {
  return parA->pt() > parB->pt();
}
static bool sort_tlv_pt(TLorentzVector parA, TLorentzVector parB) {
  return parA.Pt() > parB.Pt();
}

#endif
