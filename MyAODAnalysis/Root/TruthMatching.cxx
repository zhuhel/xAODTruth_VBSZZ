#ifndef TRUTHMATCHING_CXX
#define TRUTHMATCHING_CXX

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include "EventLoop/OutputStream.h"

#include <MyAODAnalysis/Analysisllqq.h>

#include "xAODEventInfo/EventInfo.h"

const xAOD::TruthParticle* Analysisllqq::TraceUp(const xAOD::TruthParticle* particle, int mother_pdgId, int mother_status) {
    // trace up the parents, until a V boson found

    const xAOD::TruthParticle* found_mother = 0;

    if(!particle) return found_mother;

    int nPar=particle->nParents();
    // in case no parent particles
    if(nPar==0) {
      return found_mother; 
    }
    for(int ich=0; ich<nPar; ich++) {
      const xAOD::TruthParticle* ch = particle->parent(ich);
      if(!ch) continue;
      int barcode = ch->barcode();
      int status = ch->status();
      int pdgId = ch->pdgId();
      //if( pdgId==24 && status==22 ) { // W, MadGraph
      if( (mother_pdgId==-1 || fabs(pdgId)==mother_pdgId) && (mother_status==-1 || status==mother_status) ) {
          found_mother = ch;
          break;
      } else {
          found_mother = TraceUp(ch, mother_pdgId, mother_status);
      }

    }

    return found_mother;

}

int Analysisllqq::TraceUpHadron(const xAOD::TruthParticle* particle) {
    // check if the lepton is from a hadron

    bool foundHadron=false;
    int nPar=particle->nParents();
    if(nPar==0) {
      return false; 
    }
    for(int ich=0; ich<nPar; ich++) {
      const xAOD::TruthParticle* ch = particle->parent(ich);
      if(!ch) continue;
      int barcode = ch->barcode();
      int status = ch->status();
      int pdgId = ch->pdgId();
      if( ch->isHadron() && status!=4 && status!=11) { // hadron
          foundHadron=true;
          break;
      } else {
          foundHadron=TraceUpHadron(ch);
      }

    }

    return foundHadron;

}

const xAOD::TruthParticle* Analysisllqq::TraceDown(const xAOD::TruthParticle* particle, int child_pdgId, int child_status ) {
    // trace down the children, until the targe is found

    const xAOD::TruthParticle* found_child = 0;

    if(!particle) return found_child;

    int nPar=particle->nChildren();
    // in case no parent particles
    if(nPar==0) {
      return found_child;
    }
    for(int ich=0; ich<nPar; ich++) {
      const xAOD::TruthParticle* ch = particle->child(ich);
      if(!ch) continue;
      int barcode = ch->barcode();
      int status = ch->status();
      int pdgId = ch->pdgId();
      //if( pdgId==24 && status==22 ) { // W, MadGraph
      if( fabs(pdgId)==fabs(child_pdgId) && (child_status==-1 || status==child_status) ) {
          found_child = ch;
          break;
      } else {
          found_child = TraceDown(ch, child_pdgId, child_status);
      }

    }

    return found_child;
}

#endif
