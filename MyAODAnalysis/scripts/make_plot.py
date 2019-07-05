#!/usr/bin/python

###############################################
## get the histograms from the root file
## and plot them
## Lailin, July, 2013
###############################################

import sys,os
sys.path.append('/afs/cern.ch/work/l/lxu/Tool/myTools/Scripts')

from plot_style import atlas_style
atlas_style()

from plot_template_multi_v3 import plot_template

#################### input format ######################################
## ['hist.root', 'histdir', 'histname', 'histlabel', 1, 'kRed', 20, 1, 5]
###  histo         dir       histname    lable     scale color  makrer, ref rebin
#################### input format ######################################

Ratio=-0.2
RatioMin,RatioMax=0.8,1.2
RatioAdj=-1

ATLAS='ATLAS Internal'
#SigCh="gg#rightarrowH#rightarrowZZ"
#SigCh="gg#rightarrowZZ(cont.)"
SigCh="H#rightarrowZZ"

yScale=2.0
DoNorm=2 ## 0: no normalization, 1: normalize to 1, 2: normalize to another number
LogPlot=-1
xLeg=0.65
FigFormat='all'
CMS=13

#MZZ for B
#input1="../out_testllqq/mc15_13TeV.100000.MadGraph_HZZ_W15.test_TruthDAOD.list/hist-xAOD.root"
input1="../out_testllqq/mc15_13TeV.100000.MadGraphPythia8_HZZ4l_W15.test_TruthDAOD.list/hist-xAOD.root"

norm_B_400=3.401
norm_B_500=3.260
norm_B_900=2.913

## MZ1
xMin,xMax=60, 120
nRebin=5
binW=nRebin*1
Histlist=[]
#Histlist.append([input1, '', 'Total_MZZ', '400',   norm_B_400, 'kBlack', 20, 1, nRebin])
Histlist.append([input1, '', 'TruthZllM1', 'm_{H}=900 GeV',   1, 'kBlack', 20, 1, nRebin])
Xt, Yt='m_{Z1} [GeV]', 'Events / %g GeV' % binW
FigName='MG_v2.3.2_HZZ_MZ1'
plot_template(histlist=Histlist, xmin=xMin, xmax=xMax, hxt=Xt, hyt=Yt, doNorm=DoNorm, log=LogPlot, ratio=Ratio, ratio_adj=RatioAdj, ratio_min=RatioMin, ratio_max=RatioMax, yscale=yScale, showATLAS=ATLAS, sigch=SigCh, legx=xLeg, outname=FigName, cms=CMS, figformat=FigFormat)

## PtZ1
xMin,xMax=0, 1000
nRebin=10
binW=nRebin*1
Histlist=[]
#Histlist.append([input1, '', 'Total_MZZ', '400',   norm_B_400, 'kBlack', 20, 1, nRebin])
Histlist.append([input1, '', 'TruthZllPt1', 'm_{H}=900 GeV',   1, 'kBlack', 20, 1, nRebin])
Xt, Yt='p_{T}^{Z1} [GeV]', 'Events / %g GeV' % binW
FigName='MG_v2.3.2_HZZ_PtZ1'
plot_template(histlist=Histlist, xmin=xMin, xmax=xMax, hxt=Xt, hyt=Yt, doNorm=DoNorm, log=LogPlot, ratio=Ratio, ratio_adj=RatioAdj, ratio_min=RatioMin, ratio_max=RatioMax, yscale=yScale, showATLAS=ATLAS, sigch=SigCh, legx=xLeg, outname=FigName, cms=CMS, figformat=FigFormat)


## MZ2
xMin,xMax=60, 120
nRebin=5
binW=nRebin*1
Histlist=[]
Histlist.append([input1, '', 'TruthZllM2', 'm_{H}=900 GeV',   1, 'kBlack', 20, 1, nRebin])
Xt, Yt='m_{Z2} [GeV]', 'Events / %g GeV' % binW
FigName='MG_v2.3.2_HZZ_MZ2'
plot_template(histlist=Histlist, xmin=xMin, xmax=xMax, hxt=Xt, hyt=Yt, doNorm=DoNorm, log=LogPlot, ratio=Ratio, ratio_adj=RatioAdj, ratio_min=RatioMin, ratio_max=RatioMax, yscale=yScale, showATLAS=ATLAS, sigch=SigCh, legx=xLeg, outname=FigName, cms=CMS, figformat=FigFormat)

## PtZ2
xMin,xMax=0, 1000
nRebin=10
binW=nRebin*1
Histlist=[]
#Histlist.append([input1, '', 'Total_MZZ', '400',   norm_B_400, 'kBlack', 20, 1, nRebin])
Histlist.append([input1, '', 'TruthZllPt2', 'm_{H}=900 GeV',   1, 'kBlack', 20, 1, nRebin])
Xt, Yt='p_{T}^{Z2} [GeV]', 'Events / %g GeV' % binW
FigName='MG_v2.3.2_HZZ_PtZ2'
plot_template(histlist=Histlist, xmin=xMin, xmax=xMax, hxt=Xt, hyt=Yt, doNorm=DoNorm, log=LogPlot, ratio=Ratio, ratio_adj=RatioAdj, ratio_min=RatioMin, ratio_max=RatioMax, yscale=yScale, showATLAS=ATLAS, sigch=SigCh, legx=xLeg, outname=FigName, cms=CMS, figformat=FigFormat)


## MZZ
xMin,xMax=500, 1500
nRebin=10
binW=nRebin*1
Histlist=[]
Histlist.append([input1, '', 'TruthVVM', 'm_{H}=900 GeV',   1, 'kBlack', 20, 1, nRebin])
Xt, Yt='m_{ZZ} [GeV]', 'Events / %g GeV' % binW
FigName='MG_v2.3.2_HZZ_MZZ'
plot_template(histlist=Histlist, xmin=xMin, xmax=xMax, hxt=Xt, hyt=Yt, doNorm=DoNorm, log=LogPlot, ratio=Ratio, ratio_adj=RatioAdj, ratio_min=RatioMin, ratio_max=RatioMax, yscale=yScale, showATLAS=ATLAS, sigch=SigCh, legx=xLeg, outname=FigName, cms=CMS, figformat=FigFormat)

## PtZZ
xMin,xMax=0, 1000
nRebin=10
binW=nRebin*1
Histlist=[]
Histlist.append([input1, '', 'TruthVVPt', 'm_{H}=900 GeV',   1, 'kBlack', 20, 1, nRebin])
Xt, Yt='p_{T}^{ZZ} [GeV]', 'Events / %g GeV' % binW
FigName='MG_v2.3.2_HZZ_PtZZ'
plot_template(histlist=Histlist, xmin=xMin, xmax=xMax, hxt=Xt, hyt=Yt, doNorm=DoNorm, log=LogPlot, ratio=Ratio, ratio_adj=RatioAdj, ratio_min=RatioMin, ratio_max=RatioMax, yscale=yScale, showATLAS=ATLAS, sigch=SigCh, legx=xLeg, outname=FigName, cms=CMS, figformat=FigFormat)


## TruthL1Pt
xMin,xMax=0, 500
nRebin=5
binW=nRebin*1
Histlist=[]
Histlist.append([input1, '', 'TruthL1Pt', 'm_{H}=900 GeV',   1, 'kBlack', 20, 1, nRebin])
Xt, Yt='p_{T}^{1} [GeV]', 'Events / %g GeV' % binW
FigName='MG_v2.3.2_HZZ_L1Pt'
plot_template(histlist=Histlist, xmin=xMin, xmax=xMax, hxt=Xt, hyt=Yt, doNorm=DoNorm, log=LogPlot, ratio=Ratio, ratio_adj=RatioAdj, ratio_min=RatioMin, ratio_max=RatioMax, yscale=yScale, showATLAS=ATLAS, sigch=SigCh, legx=xLeg, outname=FigName, cms=CMS, figformat=FigFormat)

## TruthL2Pt
xMin,xMax=0, 500
nRebin=5
binW=nRebin*1
Histlist=[]
Histlist.append([input1, '', 'TruthL2Pt', 'm_{H}=900 GeV',   1, 'kBlack', 20, 1, nRebin])
Xt, Yt='p_{T}^{2} [GeV]', 'Events / %g GeV' % binW
FigName='MG_v2.3.2_HZZ_L2Pt'
plot_template(histlist=Histlist, xmin=xMin, xmax=xMax, hxt=Xt, hyt=Yt, doNorm=DoNorm, log=LogPlot, ratio=Ratio, ratio_adj=RatioAdj, ratio_min=RatioMin, ratio_max=RatioMax, yscale=yScale, showATLAS=ATLAS, sigch=SigCh, legx=xLeg, outname=FigName, cms=CMS, figformat=FigFormat)

## TruthL3Pt
xMin,xMax=0, 200
nRebin=5
binW=nRebin*1
Histlist=[]
Histlist.append([input1, '', 'TruthL3Pt', 'm_{H}=900 GeV',   1, 'kBlack', 20, 1, nRebin])
Xt, Yt='p_{T}^{3} [GeV]', 'Events / %g GeV' % binW
FigName='MG_v2.3.2_HZZ_L3Pt'
plot_template(histlist=Histlist, xmin=xMin, xmax=xMax, hxt=Xt, hyt=Yt, doNorm=DoNorm, log=LogPlot, ratio=Ratio, ratio_adj=RatioAdj, ratio_min=RatioMin, ratio_max=RatioMax, yscale=yScale, showATLAS=ATLAS, sigch=SigCh, legx=xLeg, outname=FigName, cms=CMS, figformat=FigFormat)

## TruthL4Pt
xMin,xMax=0, 200
nRebin=5
binW=nRebin*1
Histlist=[]
Histlist.append([input1, '', 'TruthL4Pt', 'm_{H}=900 GeV',   1, 'kBlack', 20, 1, nRebin])
Xt, Yt='p_{T}^{4} [GeV]', 'Events / %g GeV' % binW
FigName='MG_v2.3.2_HZZ_L4Pt'
plot_template(histlist=Histlist, xmin=xMin, xmax=xMax, hxt=Xt, hyt=Yt, doNorm=DoNorm, log=LogPlot, ratio=Ratio, ratio_adj=RatioAdj, ratio_min=RatioMin, ratio_max=RatioMax, yscale=yScale, showATLAS=ATLAS, sigch=SigCh, legx=xLeg, outname=FigName, cms=CMS, figformat=FigFormat)



#dir='MG_v2.3.2_HZZ'
dir='MG_v2.3.2_HZZ4l'
if not os.path.isdir(dir): os.mkdir(dir)
cmd='mv *.pdf *.png *.eps *root '+dir
os.system(cmd)
dir1='Png_'+dir
if not os.path.isdir(dir1): os.mkdir(dir1)
cmd='cp '+dir+'/*png '+dir1
os.system(cmd)
