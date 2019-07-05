#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "MyAODAnalysis/HistoManager.h"

using namespace std;

HistoManager::HistoManager():
    myTree(nullptr),
    Evt_weight(1.)
{
    
}

void HistoManager::InitStrVec(vector<string>& out, string in, string de) {
    int pos=0, pos_pre=0;
    while(true) {
        pos=in.find(de,pos_pre);
        if(pos==-1) {out.push_back(in.substr(pos_pre,in.size()-pos_pre)); break;}
        else  out.push_back(in.substr(pos_pre,pos-pos_pre));
        pos_pre=pos+1;
    }
}

void HistoManager::InitVar(string varlist, int nbin, double xmin, double xmax, int vec, int option) {

  vector<string> variables;
  InitStrVec(variables, varlist, ",");

  for(int i=0; i<(int)variables.size(); i++) {
    VarName[variables[i]].NBins=nbin;
    VarName[variables[i]].Xmin=xmin;
    VarName[variables[i]].Xmax=xmax;
    VarName[variables[i]].Option=(float)option;
    //VarName[variables[i]].CutStep[cat]=nstep;
    VarName[variables[i]].Vector=(float)vec;
  }

}


void HistoManager::ClearVariables() {
  ClearVariables(Var);
  ClearVariables(VVar);
}

void HistoManager::ClearVariables(MapType_Double& map) {

    MapType_Double::iterator it;
    for(it=map.begin(); it!=map.end(); it++)
        map[(*it).first]=-9999.;
}

void HistoManager::ClearVariables(MapType_VDoubleStar& map) {

    MapType_VDoubleStar::iterator it;
    for(it=map.begin(); it!=map.end(); it++)
        map[(*it).first]->clear();
}

void HistoManager::CreateHistoMap() {
  MapType_VarConfig::iterator it;
  for(it=VarName.begin(); it!=VarName.end(); it++) {
      //<Prepare Variables>
      string varname=(*it).first;
      //<Create Tree Variable First>
      int nbin=0, option=0, vec=0;
      float xlow=0., xhigh=0.;
      nbin = int((*it).second.NBins);
      xlow = double((*it).second.Xmin);
      xhigh = double((*it).second.Xmax);
      option = int((*it).second.Option);
      vec = int((*it).second.Vector);
      //<Initialize variable>
      if(vec==1) {
          vector<double>* tmp = new vector<double>();
          VVar[varname] = tmp;
      }
      else Var[varname]=0.;
      if(option==InTreeOnly) continue;
      //<Create Histograms>
      string histo_name= varname;
      TH1F *histo_pointer = new TH1F(histo_name.c_str(),histo_name.c_str(),nbin,xlow,xhigh);
      histo_pointer->Sumw2();
      histoMap[varname]=histo_pointer;
  }
}

void HistoManager::AddVarIntoTree() {

    if(!myTree) myTree = new TTree("truth", "truth");

    //<loop the "treeVar" map to automatically add branches to tree>
    //<To make life easier, all variables are set to be "double">
    MapType_VarConfig::iterator it;
    for(it=VarName.begin(); it!=VarName.end(); it++) {
        string varname = (*it).first;
        if(VarName[varname].Vector)
            myTree->Branch(varname.c_str(),"vector<double>",&VVar[varname]);
        else
            myTree->Branch(varname.c_str(),&Var[varname]);
    }
}

void HistoManager::FillHistograms() {
  //<fill 1D histogram>
  MapType_VarConfig::iterator it;
  for(it=VarName.begin(); it!=VarName.end(); it++) {
      string varname = (*it).first;
      int option = int((*it).second.Option);
      if(option==InTreeOnly) continue;
      //handle if variable is single or vector
      vector<double> tmp;
      if(VarName[varname].Vector) {
          for(int k=0; k<(int)VVar[varname]->size(); k++)
              tmp.push_back(VVar[varname]->at(k));
      }
      else tmp.push_back(Var[varname]);
      //event weight
      //double wt_histo=1.0;
      //fill histogram
      if (tmp.size() == 0) continue;
      for(int k=0; k<(int)tmp.size(); k++)
      {
        // -9999. is the default value
        // if a variable is -9999, then it means it has not been assigened
        if(tmp[k]!=-9999.) histoMap[varname]->Fill(tmp[k], Evt_weight);
      }
  }
}

void HistoManager::FillTree() {

  myTree->Fill();

}

std::vector<TH1*> HistoManager::getHistos() {

  std::vector<TH1*> histos;
  std::map<std::string, TH1*>::iterator it;
  for(it=histoMap.begin(); it!=histoMap.end(); it++) {
    histos.push_back(it->second);
  } 

  return histos;

}

TTree* HistoManager::getTree() {

  return myTree;

}

HistoManager::~HistoManager() {
  std::map<std::string, TH1*>::iterator it;
  for(it=histoMap.begin(); it!=histoMap.end(); it++) {
    delete it->second;
  }
}
