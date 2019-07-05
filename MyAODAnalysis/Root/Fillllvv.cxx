#ifndef FILLSherpaVBS_CXX
#define FILLSherpaVBS_CXX

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"

#include <MyAODAnalysis/Analysisllqq.h>
#include <MyAODAnalysis/GetElicityAngles.h>

#include "xAODEventInfo/EventInfo.h"
//#include "PhysicsAnalysis/MCTruthClassifier.h"

#define EL_RETURN_CHECK( CONTEXT, EXP )                              \
  if (EXP !=  EL::StatusCode::SUCCESS) {                      \
    Error( CONTEXT, "Failed to execute: %s. Exiting.", #EXP); \
    return EL::StatusCode::FAILURE;                           \
  }

#define MZ 91.1876
#define MW 80.385

//TLorentzVector getDressing(const xAOD::TruthParticle* lep){
TLorentzVector Analysisllqq::getDressing(const xAOD::TruthParticle* lep){

  TLorentzVector dressedVec;
  dressedVec = lep->p4();

  if(lep->status() != 1) return dressedVec;
  for (const xAOD::TruthParticle* ph: *m_truthParts) {
      if(ph->pdgId() != 22) continue;
      if(ph->status() !=1) continue;
      if(lep->p4().DeltaR(ph->p4()) < 0.1){
        dressedVec += ph->p4();
      }
  }

  return dressedVec;

}

EL::StatusCode Analysisllqq::fillllvv(){

  int nel=0, nmu=0, nlEta=0;

  // truth selections
  bool pass_2lep = false; // have four signal lepton
  bool pass_lPt = false; // lepton pT
  bool pass_MZ = false; // 80GeV<mll<100GeV for Z
  bool pass_MET = false; // MET>130GeV
  bool pass_2jet = false;
  bool pass_tagjets = false;
  bool pass_VBS = false;
  
  for(;;){
     
     //======================= lepton selections =====================
     std::vector<const xAOD::TruthParticle*> GoodMu, GoodEl,GoodTau;
     std::vector<const xAOD::TruthParticle*> Goodlep;
     std::vector<const xAOD::TruthParticle*> LepAll, LepStatus1, LepStatus3, LepStatus11;
 
     // select the leptons with status==1 first
     for (const xAOD::TruthParticle* particle : *m_truthParts) {
       int status = particle->status();
       int pdgId = particle->pdgId();

       int isFromHadron = TraceUpHadron(particle);
       if(isFromHadron) continue;
       if(fabs(pdgId)!=11 && fabs(pdgId)!=13) continue;
       if(status != 1) continue;

       LepStatus1.push_back(particle);
       LepAll.push_back(particle);

     }

     for(int i=0; i<LepAll.size(); i++){
       const xAOD::TruthParticle* particle = LepAll.at(i);
       TLorentzVector tlv_par;
       if(m_doDressing) tlv_par = getDressing(particle);
       else tlv_par = particle->p4();
       float par_pt = tlv_par.Pt();
       float par_eta =  tlv_par.Eta();
       int pdgId = particle->pdgId();

       if(par_pt/1000. < 7.) continue;
       if(fabs(par_eta) > 2.5) continue;
       // For electrons
       if(fabs(pdgId)==11){
          HM->VVar["PtEl"]->push_back(par_pt/1000.);
          GoodEl.push_back(particle);
          Goodlep.push_back(particle);
       }
       // For muons
       if(fabs(pdgId)==13){
          HM->VVar["PtMu"]->push_back(par_pt/1000.);
          GoodMu.push_back(particle);
          Goodlep.push_back(particle);
       }

     }
    
     std::sort(GoodMu.begin(), GoodMu.end(), sort_par_pt);
     std::sort(GoodEl.begin(), GoodEl.end(), sort_par_pt);
     std::sort(Goodlep.begin(), Goodlep.end(), sort_par_pt);
     
     if(GoodEl.size()==2 && GoodMu.size()==2) pass_2lep=true;
     if(!pass_2lep) break;

     TLorentzVector tlv_l0 = GoodEl.at(0)->p4();
     TLorentzVector tlv_l1 = GoodEl.at(1)->p4();
     if(m_doDressing){
       tlv_l0 = getDressing(GoodEl.at(0));
       tlv_l1 = getDressing(GoodEl.at(1));
     }
     //if(tlv_l0.Pt()/1000.>30. && tlv_l1.Pt()/1000.>20. && GoodMu.at(0)->pt()/1000.<20.) pass_lPt=true;
     if(tlv_l0.Pt()/1000.>20. && tlv_l1.Pt()/1000.>20.) pass_lPt=true;
     if(!pass_lPt) break;

     // ======================== di-lepton pair selection =========================
     TLorentzVector Z1;
     Z1 = tlv_l0+tlv_l1;

     HM->Var["MZ1"] = Z1.M()/1000.;

     if(Z1.M()/1000.>80. && Z1.M()/1000.<100.) pass_MZ=true;
     if(!pass_MZ) break;

     TLorentzVector tlv_nu=GoodMu.at(0)->p4()+GoodMu.at(1)->p4();
     float mc_met = tlv_nu.E();
     //std::cout << "PT: " << tlv_nu.Pt() << "  E: " << tlv_nu.E() << std::endl;
     HM->Var["MET"]=mc_met/1000.;
     
     if(mc_met/1000.>80.) pass_MET=true;
     if(!pass_MET) break;

     // ============================== Tag jets selection ========================
     std::vector<const xAOD::Jet*> Goodjets;
     float dRlj_min=9999.;
     for( const xAOD::Jet* truthJet : *m_truthSmallRJets) {
       float truth_eta = truthJet->eta();
       float truth_pt = truthJet->pt();
       if(truth_pt > 40.e3 ) {
         // overlap removal with the leptons
         for(const xAOD::TruthParticle* lep : GoodEl){
           float dR = (lep->p4()).DeltaR( truthJet->p4() );
           if(dR < dRlj_min) dRlj_min = dR;
         }
         if(dRlj_min>0.2) Goodjets.push_back(truthJet);
       }
       dRlj_min=9999.; 
     }
     std::sort(Goodjets.begin(), Goodjets.end(), sort_par_pt);
     HM->Var["nGoodjet"]=Goodjets.size();

     if(Goodjets.size()>=2) pass_2jet=true; 
     if(!pass_2jet) break;

     // VBF tag jets:
     const xAOD::Jet *j_tag1 = nullptr, *j_tag2 = nullptr;
     TLorentzVector tag_jj;
     float m_tag = -999., deta_tag=-999.;
     j_tag1 = Goodjets.at(0);
     j_tag2 = Goodjets.at(1);

     if(j_tag1->pt()/1000.>60. && j_tag1->p4().Rapidity()*j_tag2->p4().Rapidity()<0.) pass_tagjets=true;
     if(!pass_tagjets) break;

     m_tag = (j_tag1->p4()+j_tag2->p4()).M();
     HM->Var["TagJJM"]=m_tag/1000.;
     HM->Var["TagJJdEta"]=fabs(j_tag1->eta()-j_tag2->eta());
     HM->VVar["TagJPt"]->push_back(j_tag1->pt()/1000.);
     HM->VVar["TagJPt"]->push_back(j_tag2->pt()/1000.);
     //std::cout << "m_tag=" << m_tag << std::endl;
     float dYJJ = j_tag1->p4().Rapidity()-j_tag2->p4().Rapidity();

     // Pass VBF tag jet selection
     //if(m_tag<400.e3 || fabs(dYJJ)<2.){ // for CR
     if(m_tag>400.e3 && fabs(dYJJ)>2.){ // for SR
       pass_VBS=true;
     }
     if(!pass_VBS) break;

     // Fill tree for BDT
     HM->Var["MJJ"]=m_tag/1000.;
     HM->Var["dYJJ"] = dYJJ;
     HM->Var["PtZ1"] = Z1.Pt()/1000.;
     HM->Var["PtJ1"] = j_tag1->pt()/1000.;
     HM->Var["PtJ2"] = j_tag2->pt()/1000.;
     HM->Var["EtaJ1xJ2"] = j_tag1->eta()*j_tag2->eta();
     HM->Var["YJ1xJ2"] = j_tag1->p4().Rapidity()*j_tag2->p4().Rapidity();
     HM->Var["YZ1Star"] = Z1.Rapidity()-(j_tag1->p4().Rapidity()+j_tag2->p4().Rapidity())/2.;

     HM->Var["TagJJdEta_final"]=fabs(j_tag1->eta()-j_tag2->eta());

     //TLorentzVector tlv_j1, tlv_j2;
     double etajj = (j_tag1->p4().Rapidity() + j_tag2->p4().Rapidity()) / 2.;
     double detajj = fabs(j_tag1->p4().Rapidity() - j_tag2->p4().Rapidity());

     break;

  }         

  // Save cut index
  HM->Var["pass_2lep"] = pass_2lep;
  HM->Var["pass_lPt"] = pass_lPt;
  HM->Var["pass_MZ"] = pass_MZ;
  HM->Var["pass_MET"] = pass_MET;
  HM->Var["pass_2jet"] = pass_2jet;
  HM->Var["pass_tagjets"] = pass_tagjets;
  HM->Var["pass_VBS"] = pass_VBS;

  //cutflow
  int nPassed_incl=0;

  if(pass_2lep) nPassed_incl += 1;

  if(pass_2lep && pass_lPt) nPassed_incl += 1;

  if(pass_2lep && pass_lPt && pass_MZ) nPassed_incl += 1;

  if(pass_2lep && pass_lPt && pass_MZ && pass_MET) nPassed_incl += 1;

  if(pass_2lep && pass_lPt && pass_MZ && pass_MET && pass_2jet) nPassed_incl += 1;

  if(pass_2lep && pass_lPt && pass_MZ && pass_MET && pass_2jet && pass_tagjets) nPassed_incl += 1;

  if(pass_2lep && pass_lPt && pass_MZ && pass_MET && pass_2jet && pass_tagjets && pass_VBS) nPassed_incl += 1;

  for(int icut=0; icut<=nPassed_incl; icut++) {
    HM->VVar["Cutflow_incl_Incl"]->push_back(icut);
  }

     
  return EL::StatusCode::SUCCESS;
}

#endif
