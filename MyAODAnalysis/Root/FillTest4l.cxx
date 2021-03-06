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
//TLorentzVector Analysisllqq::getDressing(const xAOD::TruthParticle* lep){
//
//  TLorentzVector dressedVec;
//  dressedVec = lep->p4();
//
//  if(lep->status() != 1) return dressedVec;
//  for (const xAOD::TruthParticle* ph: *m_truthParts) {
//      if(ph->pdgId() != 22) continue;
//      if(ph->status() !=1) continue;
//      if(lep->p4().DeltaR(ph->p4()) < 0.1){
//        dressedVec += ph->p4();
//      }
//  }
//
//  return dressedVec;
//
//}

EL::StatusCode Analysisllqq::fillTest4l(){

  int nel=0, nmu=0, nlEta=0;

  // truth selections
  bool pass_4lep = false; // have four signal lepton
  bool pass_lPt = false; // lepton pT
  bool pass_2e2mu = false, pass_4e = false, pass_4mu = false;
  bool pass_MZs = false; // 66GeV<mll<116GeV for Zs
  bool pass_Mll = false; // Mll>10GeV
  bool pass_2jet = false;
  bool pass_tagjets = false;
  bool pass_VBS = false;
  bool pass_MZZ = false; // MZZ>130GeV
  
  for(;;){
     
     //======================= lepton selections =====================
     std::vector<const xAOD::TruthParticle*> GoodMu, GoodEl,GoodTau;
     std::vector<const xAOD::TruthParticle*> Goodlep;
     std::vector<const xAOD::TruthParticle*> LepAll, LepStatus1, LepStatus3, LepStatus11;
 
     /*
     for (const xAOD::TruthParticle* mu: *m_truthMuonCont) {
         if (mu->status() != 1 && mu->status() != 3) continue;
         int isFromHadron = TraceUpHadron(mu);
         if(isFromHadron) continue;

         float eta_dressed = mu->auxdata< float >( "eta_dressed" );
         float pt_dressed = mu->auxdata< float >( "pt_dressed" );

         float mu_pt = m_doDressing ? pt_dressed : mu->pt();
         float mu_eta = m_doDressing ? eta_dressed : mu->eta();

         HM->VVar["PtMu"]->push_back(mu_pt/1000.);
         if(mu_pt/1000.<7.) continue;
         if(fabs(mu_eta)>2.5) continue;
         //std::cout << "mu pt: " << mu_pt << " eta: " << mu_eta << endl;
         GoodMu.push_back(mu);
         Goodlep.push_back(mu);
     }
     for (const xAOD::TruthParticle* el: *m_truthElectronCont) {
         if (el->status() != 1 && el->status() != 3) continue;
         int isFromHadron = TraceUpHadron(el);
         if(isFromHadron) continue;

         float eta_dressed = el->auxdata< float >( "eta_dressed" );
         float pt_dressed = el->auxdata< float >( "pt_dressed" );

         float el_pt = m_doDressing ? pt_dressed : el->pt();
         float el_eta = m_doDressing ? eta_dressed : el->eta();

         HM->VVar["PtEl"]->push_back(el_pt/1000.);
         if(el_pt/1000.<7.) continue;
         if(fabs(el_eta)>2.5) continue;
         //std::cout << "el pt: " << el_pt << " eta: " << el_eta << endl;
         GoodEl.push_back(el);
         Goodlep.push_back(el);
     }
     // Selections for tau, mute
     //for (const xAOD::TruthParticle* tau: *m_truthTauCont) {
     //    //if (tau->status() != 3) continue;
     //    int isFromHadron = TraceUpHadron(tau);
     //    if(isFromHadron) continue;
     //    if(tau->pt()/1000.<5.) continue;
     //    GoodTau.push_back(tau);
     //}
     */
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
     
     /* 
     // status 3 & 11
     //if(LepStatus1.size()<4){
     for (const xAOD::TruthParticle* particle : *m_truthParts) {
       float dRmin=9999.;
       int status = particle->status();
       int pdgId = particle->pdgId();

       int isFromHadron = TraceUpHadron(particle);
       if(isFromHadron) continue;
       if(fabs(pdgId)!=11 && fabs(pdgId)!=13) continue;
       if(status != 3) continue;
       LepStatus3.push_back(particle);

       for (int i=0; i<LepAll.size(); i++){
          const xAOD::TruthParticle* lep1 = LepAll.at(i);
          TLorentzVector tlv_lep1;
          if(m_doDressing) tlv_lep1 = getDressing(lep1);
          else tlv_lep1 = lep1->p4();
          float dR = particle->p4().DeltaR(tlv_lep1);
          if(dR<dRmin) dRmin=dR;
       }
       HM->Var["dR_s13"] = dRmin;
       if(dRmin<0.2) continue;
       LepAll.push_back(particle);
     }
     //}
     for (const xAOD::TruthParticle* particle : *m_truthParts) {
       float dRmin=9999.;
       int status = particle->status();
       int pdgId = particle->pdgId();

       int isFromHadron = TraceUpHadron(particle);
       if(isFromHadron) continue;
       if(fabs(pdgId)!=11 && fabs(pdgId)!=13) continue;
       if(status != 11) continue;
       LepStatus11.push_back(particle);

       for (int i=0; i<LepAll.size(); i++){
          const xAOD::TruthParticle* lep1 = LepAll.at(i);
          TLorentzVector tlv_lep1;
          if(m_doDressing) tlv_lep1 = getDressing(lep1);
          else tlv_lep1 = lep1->p4();
          float dR = particle->p4().DeltaR(tlv_lep1);
          if(dR<dRmin) dRmin=dR;
       }
       if(dRmin<0.2) continue;
       LepAll.push_back(particle);
     }
     */

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
     
     int nLep = GoodEl.size()+GoodMu.size();
     HM->Var["nLep"]=GoodEl.size()+GoodMu.size();
     //HM->Var["nLep_wTau"]=GoodEl.size()+GoodMu.size()+GoodTau.size();
     HM->Var["nEl"]=GoodEl.size();
     HM->Var["nMu"]=GoodMu.size();
     HM->Var["nStatus1"]=LepStatus1.size();
     HM->Var["nStatus3"]=LepStatus3.size();
     HM->Var["nStatus11"]=LepStatus11.size();

     for(int i=0; i<Goodlep.size(); i++) {
       const xAOD::TruthParticle* par = Goodlep.at(i);
       HM->VVar["AllLEta"]->push_back(par->eta()); 
       HM->VVar["AllLPhi"]->push_back(par->phi());
     }

     if(nLep==4) pass_4lep=true;
     if(!pass_4lep) break;

     if(GoodEl.size()==4) pass_4e = true;
     else if(GoodMu.size()==4) pass_4mu = true;
     else if(GoodMu.size()==2 && GoodEl.size()==2) pass_2e2mu = true;

     TLorentzVector tlv_l0 = Goodlep.at(0)->p4();
     TLorentzVector tlv_l1 = Goodlep.at(1)->p4();
     TLorentzVector tlv_l2 = Goodlep.at(2)->p4();
     TLorentzVector tlv_l3 = Goodlep.at(3)->p4();
     if(m_doDressing){
       tlv_l0 = getDressing(Goodlep.at(0));
       tlv_l1 = getDressing(Goodlep.at(1));
       tlv_l2 = getDressing(Goodlep.at(2));
       tlv_l3 = getDressing(Goodlep.at(3));
     }
     if(tlv_l0.Pt()/1000.>20. && tlv_l1.Pt()/1000.>20. && tlv_l2.Pt()/1000.>10.) pass_lPt=true;
     if(!pass_lPt) break;

     // ======================== di-lepton pair selection =========================
     TLorentzVector Z1, Z2;
     TLorentzVector Z1_l1, Z1_l2, Z2_l1, Z2_l2;
     TLorentzVector tmp1, tmp2;
     std::vector<TLorentzVector> pairCand;
     if(GoodMu.size()==2 && GoodEl.size()==2){
        const xAOD::TruthParticle* mu1 = GoodMu.at(0);
        const xAOD::TruthParticle* mu2 = GoodMu.at(1);
        if(m_doDressing) Z1 = getDressing(mu1)+getDressing(mu2);
        else Z1 = mu1->p4()+mu2->p4();
        if(m_doDressing) Z1_l1 = getDressing(mu1);
        else Z1_l1 = mu1->p4(); 
        if(m_doDressing) Z1_l2 = getDressing(mu2);
        else Z1_l2 = mu2->p4();
        const xAOD::TruthParticle* el1 = GoodEl.at(0);
        const xAOD::TruthParticle* el2 = GoodEl.at(1);
        Z2 = el1->p4()+el2->p4();
        Z2_l1 = el1->p4(); Z2_l2 = el2->p4();
     }else if(GoodMu.size()==4){
        float dmMin = 999.;
        const xAOD::TruthParticle* mu1 = GoodMu.at(0);
        const xAOD::TruthParticle* mu3;
        const xAOD::TruthParticle* mu4;
        for(int i=1; i<GoodMu.size(); i++){
          const xAOD::TruthParticle* mu2 = GoodMu.at(i);
          if(mu2->charge() == mu1->charge()) continue;
          if(m_doDressing) tmp1 = getDressing(mu1)+getDressing(mu2);
          else tmp1 = mu1->p4()+mu2->p4(); 
          if(i==1) {mu3 = GoodMu.at(2); mu4 = GoodMu.at(3);}
          if(i==2) {mu3 = GoodMu.at(1); mu4 = GoodMu.at(3);}
          if(i==3) {mu3 = GoodMu.at(1); mu4 = GoodMu.at(2);}
          if(m_doDressing) tmp2 = getDressing(mu3)+getDressing(mu4);
          else tmp2 = mu3->p4()+mu4->p4(); 
          pairCand.push_back(tmp1);
          pairCand.push_back(tmp2);
          float dmTmp = fabs(tmp1.M()/1000.-MZ)+fabs(tmp2.M()/1000.-MZ);
          if(dmTmp<dmMin){
            dmMin = dmTmp;
            Z1 = tmp1; Z2 = tmp2;
            if(m_doDressing){
              Z1_l1 = getDressing(mu1); Z1_l2 = getDressing(mu2);
              Z2_l1 = getDressing(mu3); Z2_l2 = getDressing(mu3);
            }
            else{
              Z1_l1 = mu1->p4(); Z1_l2 = mu2->p4();
              Z2_l1 = mu3->p4(); Z2_l2 = mu4->p4();
            }
          }
        }
     }else if(GoodEl.size()==4){
        float dmMin = 999.;
        const xAOD::TruthParticle* el1 = GoodEl.at(0);
        const xAOD::TruthParticle* el3;
        const xAOD::TruthParticle* el4;
        for(int i=1; i<GoodEl.size(); i++){
          const xAOD::TruthParticle* el2 = GoodEl.at(i);
          if(el2->charge() == el1->charge()) continue;
          if(m_doDressing) tmp1 = getDressing(el1)+getDressing(el2);
          else tmp1 = el1->p4()+el2->p4();
          if(i==1) {el3 = GoodEl.at(2); el4 = GoodEl.at(3);}
          if(i==2) {el3 = GoodEl.at(1); el4 = GoodEl.at(3);}
          if(i==3) {el3 = GoodEl.at(1); el4 = GoodEl.at(2);}
          if(m_doDressing) tmp2 = getDressing(el3)+getDressing(el4);
          else tmp2 = el3->p4()+el4->p4();
          pairCand.push_back(tmp1);
          pairCand.push_back(tmp2);
          float dmTmp = fabs(tmp1.M()/1000.-MZ)+fabs(tmp2.M()/1000.-MZ);
          if(dmTmp<dmMin){
            dmMin = dmTmp;
            Z1 = tmp1; Z2 = tmp2;
            if(m_doDressing){
              Z1_l1 = getDressing(el1); Z1_l2 = getDressing(el2);
              Z2_l1 = getDressing(el3); Z2_l2 = getDressing(el3);
            }
            else{
              Z1_l1 = el1->p4(); Z1_l2 = el2->p4();
              Z2_l1 = el3->p4(); Z2_l2 = el4->p4();
            }
          }
        }
     }
     TLorentzVector tmpZ;
     if( fabs(Z2.M()/1000.-MZ)<fabs(Z1.M()/1000.-MZ)){
         tmpZ = Z1;
         Z1 = Z2;
         Z2 = tmpZ;
     }
     HM->Var["MZ1"] = Z1.M()/1000.;
     HM->Var["MZ2"] = Z2.M()/1000.;
     for(int i=0; i<pairCand.size(); i++){
       HM->VVar["MZAll"]->push_back(pairCand.at(i).M()/1000.);
     }

     if(Z1.M()/1000.>60. && Z1.M()/1000.<120. && Z2.M()/1000.>60. && Z2.M()/1000.<120.) pass_MZs=true;
     if(!pass_MZs) break;

     pass_Mll=true;
     for(int i=0; i<pairCand.size(); i++){
        if(pairCand.at(i).M()<10.e3) pass_Mll=false;
     }
     if(!pass_Mll) break;
  
     TLorentzVector tlv_ZZ;
     tlv_ZZ = Goodlep.at(0)->p4()+Goodlep.at(1)->p4()+Goodlep.at(2)->p4()+Goodlep.at(3)->p4();
     float MZZ = tlv_ZZ.M()/1000.;

     if(MZZ>130.) pass_MZZ=true;
     if(!pass_MZZ) break;

     // ============================== Tag jets selection ========================
     std::vector<const xAOD::Jet*> Goodjets;
     float dRlj_min=9999.;
     for( const xAOD::Jet* truthJet : *m_truthSmallRJets) {
       float truth_eta = truthJet->eta();
       float truth_pt = truthJet->pt();
       if(truth_pt > 30.e3 ) {
         // overlap removal with the leptons
         for(const xAOD::TruthParticle* lep : GoodMu){
           float dR = (lep->p4()).DeltaR( truthJet->p4() );
           if(dR < dRlj_min) dRlj_min = dR;
         }
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
     //std::cout << "pass_2jet" << std::endl;

     // VBF tag jets:
     const xAOD::Jet *j_tag1 = nullptr, *j_tag2 = nullptr;
     TLorentzVector tag_jj;
     float m_tag = -999., deta_tag=-999.;
     const xAOD::Jet* j1 = Goodjets.at(0);
     for (const xAOD::Jet *j2 : Goodjets) {
       if (j1 == j2) continue;
       if (j1->eta() * j2->eta() >= 0) continue;
       tag_jj = j1->p4() + j2->p4();
       j_tag1 = j1;
       j_tag2 = j2;
       m_tag = tag_jj.M();
       break;
     }

     if(j_tag1 && j_tag2) pass_tagjets=true;
     if(!pass_tagjets) break;

     HM->Var["TagJJM"]=m_tag/1000.;
     HM->Var["TagJJdEta"]=fabs(j_tag1->eta()-j_tag2->eta());
     HM->VVar["TagJPt"]->push_back(j_tag1->pt()/1000.);
     HM->VVar["TagJPt"]->push_back(j_tag2->pt()/1000.);
     //std::cout << "m_tag=" << m_tag << std::endl;
     float dYJJ = j_tag1->p4().Rapidity()-j_tag2->p4().Rapidity();

     // Pass VBF tag jet selection
     //if(m_tag<300.e3 || fabs(dYJJ)<2.){ // for CR
     if(m_tag>300.e3 && fabs(dYJJ)>2.){ // for SR
       pass_VBS=true;
     }
     if(!pass_VBS) break;

     // Fill tree for BDT
     HM->Var["MJJ"]=m_tag/1000.;
     HM->Var["MZZ"] = tlv_ZZ.M()/1000.;
     HM->Var["dYJJ"] = dYJJ;
     HM->Var["PtZZ"] = tlv_ZZ.Pt()/1000.;
     HM->Var["PtZ1"] = Z1.Pt()/1000.;
     HM->Var["PtZ2"] = Z2.Pt()/1000.;
     HM->Var["PtJ1"] = j_tag1->pt()/1000.;
     HM->Var["PtJ2"] = j_tag2->pt()/1000.;
     HM->Var["EtaJ1xJ2"] = j_tag1->eta()*j_tag2->eta();
     HM->Var["YJ1xJ2"] = j_tag1->p4().Rapidity()*j_tag2->p4().Rapidity();
     HM->Var["PtL3"] = tlv_l2.Pt()/1000.;
     HM->Var["YZ1Star"] = Z1.Rapidity()-(j_tag1->p4().Rapidity()+j_tag2->p4().Rapidity())/2.;
     HM->Var["YZ2Star"] = Z2.Rapidity()-(j_tag1->p4().Rapidity()+j_tag2->p4().Rapidity())/2.;
     HM->Var["PtZZJJOHtZZJJ"] = (j_tag1->p4() + j_tag2->p4() + Z1 + Z2).Pt() / (j_tag1->pt() + j_tag2->pt() + Z1.Pt() + Z2.Pt());

     HM->Var["dPhiZZ"] = fabs(Z1.DeltaPhi(Z2));
     HM->Var["TagJJdEta_final"]=fabs(j_tag1->eta()-j_tag2->eta());

     //TLorentzVector tlv_j1, tlv_j2;
     double etajj = (j_tag1->p4().Rapidity() + j_tag2->p4().Rapidity()) / 2.;
     double detajj = fabs(j_tag1->p4().Rapidity() - j_tag2->p4().Rapidity());
     HM->Var["ZZCen"] = fabs(tlv_ZZ.Rapidity() - etajj) / detajj;

     break;

  }         

  // Save cut index
  HM->Var["pass_4lep"] = pass_4lep;
  HM->Var["pass_lPt"] = pass_lPt;
  HM->Var["pass_2e2mu"] = pass_2e2mu;
  HM->Var["pass_4mu"] = pass_4mu;
  HM->Var["pass_4e"] = pass_4e;
  HM->Var["pass_MZs"] = pass_MZs;
  HM->Var["pass_Mll"] = pass_Mll;
  HM->Var["pass_2jet"] = pass_2jet;
  HM->Var["pass_tagjets"] = pass_tagjets;
  HM->Var["pass_VBS"] = pass_VBS;
  HM->Var["pass_MZZ"] = pass_MZZ;

  //cutflow
  int nPassed_incl=0;

  if(pass_4lep) nPassed_incl += 1;

  if(pass_4lep && pass_lPt) nPassed_incl += 1;

  if(pass_4lep && pass_lPt && pass_MZs) nPassed_incl += 1;

  if(pass_4lep && pass_lPt && pass_MZs && pass_Mll) nPassed_incl += 1;

  if(pass_4lep && pass_lPt && pass_MZs && pass_Mll && pass_MZZ) nPassed_incl += 1;

  if(pass_4lep && pass_lPt && pass_MZs && pass_Mll && pass_MZZ && pass_2jet) nPassed_incl += 1;

  if(pass_4lep && pass_lPt && pass_MZs && pass_Mll && pass_MZZ && pass_2jet && pass_tagjets) nPassed_incl += 1;

  if(pass_4lep && pass_lPt && pass_MZs && pass_Mll && pass_MZZ && pass_2jet && pass_tagjets && pass_VBS) nPassed_incl += 1;

  for(int icut=0; icut<=nPassed_incl; icut++) {
    HM->VVar["Cutflow_incl_Incl"]->push_back(icut);
    if(pass_2e2mu) HM->VVar["Cutflow_incl_2e2mu"]->push_back(icut);
    else if(pass_4mu) HM->VVar["Cutflow_incl_4mu"]->push_back(icut);
    else if(pass_4e) HM->VVar["Cutflow_incl_4e"]->push_back(icut);
  }

     
  return EL::StatusCode::SUCCESS;
}

#endif
