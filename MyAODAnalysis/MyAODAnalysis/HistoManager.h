#ifndef CxAOD__HistoManager_H
#define CxAOD__HistoManager_H

#include <map>
#include <vector>

#include "TH1.h"
#include "TTree.h"

using namespace std;

//<struct for variables>
struct VarConfig{
  int NBins;
  float Xmin;
  float Xmax;
  float Option;
  //map<string, int> CutStep;
  int Vector;
  //int Dim;
};

//<Map type STRING:DOUBLE...>
typedef map<string, double> MapType_Double;
typedef map<string, vector<double> > MapType_VDouble;
typedef map<string, vector<double>* > MapType_VDoubleStar;
typedef map<string, VarConfig >  MapType_VarConfig;
typedef map<string, map<string, VarConfig > > MapType2_VarConfig;

//<Options for Variables to determine using which event weight to fill into histograms>
//<Both means both histograms and trees>
enum {InBoth,InTreeOnly,InHistoOnly};

class HistoManager {
    public:
        HistoManager();
        virtual ~HistoManager();

        void InitStrVec(vector<string>& out, string in, string de);
        void InitVar(string varlist, int nbin, double xmin, double xmax, int vec=0, int option=InBoth);
        void ClearVariables();
        void ClearVariables(MapType_Double& map);
        void ClearVariables(MapType_VDoubleStar& map);
        void CreateHistoMap();
        void FillHistograms();
        void FillTree();

        void AddVarIntoTree();

        std::vector<TH1*> getHistos();
        TTree* getTree();

        //Event weight
        float Evt_weight;//!

        //MapType2_Double VarName; 
        MapType_VarConfig VarName;//!
        //"Var1_Var2" : "Var1"{1,xbins,xmin,xmax},"Var2"{2,ybins,ymin,ymax}
        MapType_Double Var;//!
        MapType_VDoubleStar VVar;//!

        std::map<std::string, TH1*> histoMap;//!
        TTree *myTree;//!
};

#endif
