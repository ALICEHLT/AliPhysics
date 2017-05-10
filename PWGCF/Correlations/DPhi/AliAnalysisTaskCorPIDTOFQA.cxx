/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* AliAnaysisTaskCorPIDTOFQA
 *
 * empty task which can serve as a starting point for building an analysis
 * as an example, one histogram is filled
 */
//#include <iostream>
//#include <fstream>
#include <cmath>
//#include <iomanip>
#include "TChain.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
//#include "TGraphErrors.h"
#include "TList.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisTaskCorPIDTOFQA.h"
#include "AliPIDResponse.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TFile.h"

//#include "TFile.h"

//#include "AliMultSelection.h"


// particle identifications in ALICE
//
//    enum EParticleType
//
//    kElectron =  0;
//    kMuon     =  1;
//    kPion     =  2;
//    kKaon     =  3;
//    kProton   =  4;
//    kDeuteron =  5;
//    kTriton   =  6;
//    kHe3      =  7;
//    kAlpha    =  8;
//    kPhoton   =  9;
//    kPi0      = 10;
//    kNeutron  = 11;
//    kKaon0    = 12;
//    kEleCon   = 13;
//    kUnknown  = 14;



class AliAnalysisTaskCorPIDTOFQA;    // your analysis class

using namespace std;            // std namespace: so you can do things like 'cout'


//ofstream fout;
//Int_t NEvents            = 0;
//Int_t NTracks            = 0;
//Int_t NGoodTracks        = 0;
//Int_t NAssociatedTracks  = 0;
//Int_t DeuteronCandidates = 0;





//Double_t deut_curves[2][2][3];  // [charge][mean,sigma][par]
//TF1 *fit_deut_curve = new TF1("fit_m_mean",   "[0] + [1]*x + [2]/sqrt(x) ",       1.1, 4.4);

ClassImp(AliAnalysisTaskCorPIDTOFQA) // classimp: necessary for root

AliAnalysisTaskCorPIDTOFQA::AliAnalysisTaskCorPIDTOFQA() : AliAnalysisTaskSE(), 
fAOD(0), fOutputList(0), fPIDResponse(0),
    fHistPt(0),  cent_ntracks(0), m_squared_pos(0), m_squared_pos_cut_T(0), m_squared_pos_cut_A(0), m_squared_pos_cut_B(0), m_squared_neg(0), m_squared_neg_cut_T(0), m_squared_neg_cut_A(0), m_squared_neg_cut_B(0),
    m_squared_pos_cut_V(0),  m_squared_neg_cut_V(0),
    plength_vs_mom_pos(0), plength_vs_mom_neg(0), ttof_vs_mom_pos(0), ttof_vs_mom_neg(0), beta_vs_mom_pos(0), beta_vs_mom_neg(0), deltat_vs_mom_pos(0), deltat_vs_mom_neg(0),
    deuterons_per_event(0), deuterons_per_event_pos(0), deuterons_per_event_neg(0),
    deut_dphi_T(0), deut_dphi_pos_T(0), deut_dphi_neg_T(0), deut_dphi_A(0), deut_dphi_pos_A(0), deut_dphi_neg_A(0), deut_dphi_B(0), deut_dphi_pos_B(0), deut_dphi_neg_B(0),
    deut_dphi_V(0), deut_dphi_pos_V(0), deut_dphi_neg_V(0),
    track_phi(0), track_phi_hybrid(0), track_eta(0), track_eta_hybrid(0),
    deut_dphi_deta_p0510(0), deut_dphi_deta_p1020(0), deut_dphi_deta_p2030(0), deut_dphi_deta_p3040(0), deut_dphi_deta_p4050(0)
{
    // default constructor, don't allocate memory here!
    // this is used by root for IO purposes, it needs to remain empty
}
//_____________________________________________________________________________
AliAnalysisTaskCorPIDTOFQA::AliAnalysisTaskCorPIDTOFQA(const char* name) : AliAnalysisTaskSE(name),
fAOD(0), fOutputList(0), fPIDResponse(0),
    fHistPt(0),  cent_ntracks(0), m_squared_pos(0), m_squared_pos_cut_T(0), m_squared_pos_cut_A(0), m_squared_pos_cut_B(0), m_squared_neg(0), m_squared_neg_cut_T(0), m_squared_neg_cut_A(0), m_squared_neg_cut_B(0),
    m_squared_pos_cut_V(0),  m_squared_neg_cut_V(0),
    plength_vs_mom_pos(0), plength_vs_mom_neg(0), ttof_vs_mom_pos(0), ttof_vs_mom_neg(0), beta_vs_mom_pos(0), beta_vs_mom_neg(0), deltat_vs_mom_pos(0), deltat_vs_mom_neg(0),
    deuterons_per_event(0), deuterons_per_event_pos(0), deuterons_per_event_neg(0),
    deut_dphi_T(0), deut_dphi_pos_T(0), deut_dphi_neg_T(0), deut_dphi_A(0), deut_dphi_pos_A(0), deut_dphi_neg_A(0), deut_dphi_B(0), deut_dphi_pos_B(0), deut_dphi_neg_B(0),
    deut_dphi_V(0), deut_dphi_pos_V(0), deut_dphi_neg_V(0),
    track_phi(0), track_phi_hybrid(0), track_eta(0), track_eta_hybrid(0),
    deut_dphi_deta_p0510(0), deut_dphi_deta_p1020(0), deut_dphi_deta_p2030(0), deut_dphi_deta_p3040(0), deut_dphi_deta_p4050(0)
{
    // constructor
    DefineInput(0, TChain::Class());    // define the input of the analysis: in this case we take a 'chain' of events
                                        // this chain is created by the analysis manager, so no need to worry about it, 
                                        // it does its work automatically
    DefineOutput(1, TList::Class());    // define the ouptut of the analysis: in this case it's a list of histograms 
                                        // you can add more output objects by calling DefineOutput(2, classname::Class())
                                        // if you add more output objects, make sure to call PostData for all of them, and to
                                        // make changes to your AddTask macro!
}
//_____________________________________________________________________________
AliAnalysisTaskCorPIDTOFQA::~AliAnalysisTaskCorPIDTOFQA()
{
    // destructor
    if(fOutputList) {
        delete fOutputList;     // at the end of your task, it is deleted from memory by calling this function
    }
}
//_____________________________________________________________________________
void AliAnalysisTaskCorPIDTOFQA::UserCreateOutputObjects()
{
    // create output objects
    //
    // this function is called ONCE at the start of your analysis (RUNTIME)
    // here you ceate the histograms that you want to use 
    //
    // the histograms are in this case added to a tlist, this list is in the end saved
    // to an output file
    //

//  fTrackSelection = new AliEmcalTrackSelectionAOD;
//  fTrackSelection->GenerateTrackCuts(AliEmcalTrackSelection::kHybridTracks, "lhc13c");
//    pi = TMath::Pi();
//    fout.open("output.txt");


/*
//  p-Pb 5.02 TeV
//  3.43396 0.0133493 0.0593701 -0.125677 0.0646736 0.148637 3.59796 -0.00619808 -0.107274 0.034074 0.0359503 0.0131823 
    deut_curves[0][0][0] =  3.43396;     // pos deut mean curve
    deut_curves[0][0][1] =  0.0133493;
    deut_curves[0][0][2] =  0.0593701;

    deut_curves[0][1][0] = -0.125677;  // pos deut sigma curve
    deut_curves[0][1][1] =  0.0646736;
    deut_curves[0][1][2] =  0.148637;

    deut_curves[1][0][0] =  3.59796;     // neg deut mean curve
    deut_curves[1][0][1] = -0.00619808;
    deut_curves[1][0][2] = -0.107274;

    deut_curves[1][1][0] =  0.034074;  // neg deut sigma curve
    deut_curves[1][1][1] =  0.0359503;
    deut_curves[1][1][2] =  0.0131823;
*/



//  p-p 7.0 TeV
//  3.49074 0.00633947 -0.368222 0.114274 0.414374 3.51731 -0.014165 -0.386053 0.112826 0.445191
    deut_curves[0][0][0] =  3.49074;     // pos deut mean curve
    deut_curves[0][0][1] =  0.00633947;
    deut_curves[0][0][2] =  0.00;

    deut_curves[0][1][0] = -0.368222;    // pos deut sigma curve
    deut_curves[0][1][1] =  0.114274;
    deut_curves[0][1][2] =  0.414374;

    deut_curves[1][0][0] =  3.51731;      // neg deut mean curve
    deut_curves[1][0][1] = -0.014165;
    deut_curves[1][0][2] =  0.00;

    deut_curves[1][1][0] = -0.386053;     // neg deut sigma curve
    deut_curves[1][1][1] =  0.112826;
    deut_curves[1][1][2] =  0.445191;


    
//  TF1 *fit_deut_curve = new TF1("fit_m_mean",   "[0] + [1]*x + [2]/sqrt(x) ",       1.1, 4.4);
    fit_deut_curve->SetParNames("a", "b x", "c/#sqrt(x)");
	
    fOutputList = new TList();          // this is a list which will contain all of your histograms
                                        // at the end of the analysis, the contents of this list are written
                                        // to the output file
    fOutputList->SetOwner(kTRUE);       // memory stuff: the list is owner of all objects it contains and will delete them
                                        // if requested (dont worry about this now)


    fHistPt                 = new TH1F("fHistPt",                "Pt()",                  100,     0,    10);
    track_phi               = new TH1F("track_phi",              "track_phi",             600,  -1.6708,  4.8124);
    track_phi_hybrid        = new TH1F("track_phi_hybrid",       "track_phi_hybrid",      600,  -1.6708,  4.8124);
    track_eta               = new TH1F("track_eta",              "track_eta",             600,  -2.0,   2.0);
    track_eta_hybrid        = new TH1F("track_eta_hybrid",       "track_eta_hybrid",      600,  -2.0,   2.0);
    cent_ntracks            = new TH2F("cent_vs_ntracks",        "cent_vs_ntracks",       100,     0,   100,     100,     0,   800);
    m_squared_pos           = new TH2F("m_squared_pos",          "m_squared_pos",         320,   0.0,   8.0,    2400,  -1.0,   7.0);
    m_squared_pos_cut_T     = new TH2F("m_squared_pos_cut_T",    "m_squared_pos_cut_T",   320,   0.0,   8.0,    2400,  -1.0,   7.0);
    m_squared_pos_cut_A     = new TH2F("m_squared_pos_cut_A",    "m_squared_pos_cut_A",   320,   0.0,   8.0,    2400,  -1.0,   7.0);
    m_squared_pos_cut_B     = new TH2F("m_squared_pos_cut_B",    "m_squared_pos_cut_B",   320,   0.0,   8.0,    2400,  -1.0,   7.0);
    m_squared_pos_cut_V     = new TH2F("m_squared_pos_cut_V",    "m_squared_pos_cut_V",   320,   0.0,   8.0,    2400,  -1.0,   7.0);
    m_squared_neg           = new TH2F("m_squared_neg",          "m_squared_neg",         320,   0.0,   8.0,    2400,  -1.0,   7.0);
    m_squared_neg_cut_T     = new TH2F("m_squared_neg_cut_T",    "m_squared_neg_cut_T",   320,   0.0,   8.0,    2400,  -1.0,   7.0);
    m_squared_neg_cut_A     = new TH2F("m_squared_neg_cut_A",    "m_squared_neg_cut_A",   320,   0.0,   8.0,    2400,  -1.0,   7.0);
    m_squared_neg_cut_B     = new TH2F("m_squared_neg_cut_B",    "m_squared_neg_cut_B",   320,   0.0,   8.0,    2400,  -1.0,   7.0);
    m_squared_neg_cut_V     = new TH2F("m_squared_neg_cut_V",    "m_squared_neg_cut_V",   320,   0.0,   8.0,    2400,  -1.0,   7.0);
    plength_vs_mom_pos      = new TH2F("plength_vs_mom_pos",     "plength_vs_mom_pos",    780,   0.2,   8.0,    4000, 350.0,   750);  // (GeV/c 10MeV/bin,   cm  1mm/bin)
    plength_vs_mom_neg      = new TH2F("plength_vs_mom_neg",     "plength_vs_mom_neg",    780,   0.2,   8.0,    4000, 350.0,   750);  // (GeV/c 10MeV/bin,   cm  1mm/bin)
    ttof_vs_mom_pos         = new TH2F("ttof_vs_mom_pos",        "ttof_vs_mom_pos",       780,   0.2,   8.0,    4000,  10.0,  50.0);  // (GeV/c 10MeV/bin,   ns 10ps/bin)
    ttof_vs_mom_neg         = new TH2F("ttof_vs_mom_neg",        "ttof_vs_mom_neg",       780,   0.2,   8.0,    4000,  10.0,  50.0);  // (GeV/c 10MeV/bin,   ns 10ps/bin)    
    beta_vs_mom_pos         = new TH2F("beta_vs_mom_pos",        "beta_vs_mom_pos",       780,   0.0,   8.0,    3000,   0.1,   1.1);
    beta_vs_mom_neg         = new TH2F("beta_vs_mom_neg",        "beta_vs_mom_neg",       780,   0.0,   8.0,    3000,   0.1,   1.1);
    deltat_vs_mom_pos       = new TH2F("deltat_vs_mom_pos",      "deltat_vs_mom_pos",     780,   0.2,   8.0,    2100,  -1.0,  20.0);
    deltat_vs_mom_neg       = new TH2F("deltat_vs_mom_neg",      "deltat_vs_mom_neg",     780,   0.2,   8.0,    2100,  -1.0,  20.0);
    deut_dphi_T             = new TH2F("deut_dphi_T",            "deut_dphi_T",            14,   1.0,   4.5, 300, -1.6708,  4.8124);
    deut_dphi_A             = new TH2F("deut_dphi_A",            "deut_dphi_A",            14,   1.0,   4.5, 300, -1.6708,  4.8124);
    deut_dphi_B             = new TH2F("deut_dphi_B",            "deut_dphi_B",            14,   1.0,   4.5, 300, -1.6708,  4.8124);
    deut_dphi_V             = new TH2F("deut_dphi_V",            "deut_dphi_V",            14,   1.0,   4.5, 300, -1.6708,  4.8124);
    deut_dphi_pos_T         = new TH2F("deut_dphi_pos_T",        "deut_dphi_pos_T",        14,   1.0,   4.5, 300, -1.6708,  4.8124);
    deut_dphi_pos_A         = new TH2F("deut_dphi_pos_A",        "deut_dphi_pos_A",        14,   1.0,   4.5, 300, -1.6708,  4.8124);
    deut_dphi_pos_B         = new TH2F("deut_dphi_pos_B",        "deut_dphi_pos_B",        14,   1.0,   4.5, 300, -1.6708,  4.8124);
    deut_dphi_pos_V         = new TH2F("deut_dphi_pos_V",        "deut_dphi_pos_V",        14,   1.0,   4.5, 300, -1.6708,  4.8124); 
    deut_dphi_neg_T         = new TH2F("deut_dphi_neg_T",        "deut_dphi_neg_T",        14,   1.0,   4.5, 300, -1.6708,  4.8124);
    deut_dphi_neg_A         = new TH2F("deut_dphi_neg_A",        "deut_dphi_neg_A",        14,   1.0,   4.5, 300, -1.6708,  4.8124);
    deut_dphi_neg_B         = new TH2F("deut_dphi_neg_B",        "deut_dphi_neg_B",        14,   1.0,   4.5, 300, -1.6708,  4.8124);
    deut_dphi_neg_V         = new TH2F("deut_dphi_neg_V",        "deut_dphi_neg_V",        14,   1.0,   4.5, 300, -1.6708,  4.8124);    
    deuterons_per_event     = new TH1I("deuterons_per_event",    "deuterons_per_event",     5,0,5);
    deuterons_per_event_pos = new TH1I("deuterons_per_event_pos","deuterons_per_event_pos", 5,0,5);
    deuterons_per_event_neg = new TH1I("deuterons_per_event_neg","deuterons_per_event_neg", 5,0,5);

    
    deut_dphi_deta_p0510    = new TH2F("deut_dphi_deta_p0510",   "deut_dphi_deta_p0510",           60, -1.6708,  4.8124, 46, -2.30, 2.30);
    deut_dphi_deta_p1020    = new TH2F("deut_dphi_deta_p1020",   "deut_dphi_deta_p1020",           60, -1.6708,  4.8124, 46, -2.30, 2.30);
    deut_dphi_deta_p2030    = new TH2F("deut_dphi_deta_p2030",   "deut_dphi_deta_p2030",           60, -1.6708,  4.8124, 46, -2.30, 2.30);
    deut_dphi_deta_p3040    = new TH2F("deut_dphi_deta_p3040",   "deut_dphi_deta_p3040",           60, -1.6708,  4.8124, 46, -2.30, 2.30);
    deut_dphi_deta_p4050    = new TH2F("deut_dphi_deta_p4050",   "deut_dphi_deta_p4050",           60, -1.6708,  4.8124, 46, -2.30, 2.30);
    
    
    fOutputList->Add(fHistPt);                                                                      // objects added to output file
    fOutputList->Add(cent_ntracks);
    fOutputList->Add(m_squared_pos);
    fOutputList->Add(m_squared_pos_cut_T);
    fOutputList->Add(m_squared_pos_cut_A);
    fOutputList->Add(m_squared_pos_cut_B);
    fOutputList->Add(m_squared_pos_cut_V);	
    fOutputList->Add(m_squared_neg);    
    fOutputList->Add(m_squared_neg_cut_T);
    fOutputList->Add(m_squared_neg_cut_A);
    fOutputList->Add(m_squared_neg_cut_B);
    fOutputList->Add(m_squared_neg_cut_V);
    fOutputList->Add(plength_vs_mom_pos);
    fOutputList->Add(plength_vs_mom_neg);
    fOutputList->Add(ttof_vs_mom_pos);
    fOutputList->Add(ttof_vs_mom_neg);
    fOutputList->Add(beta_vs_mom_pos);
    fOutputList->Add(beta_vs_mom_neg);
    fOutputList->Add(deltat_vs_mom_pos);
    fOutputList->Add(deltat_vs_mom_neg);
    fOutputList->Add(deuterons_per_event);
    fOutputList->Add(deuterons_per_event_pos);
    fOutputList->Add(deuterons_per_event_neg);
    fOutputList->Add(deut_dphi_T);
    fOutputList->Add(deut_dphi_A);
    fOutputList->Add(deut_dphi_B);
    fOutputList->Add(deut_dphi_V);
    fOutputList->Add(deut_dphi_pos_T);
    fOutputList->Add(deut_dphi_pos_A);
    fOutputList->Add(deut_dphi_pos_B);
    fOutputList->Add(deut_dphi_pos_V);
    fOutputList->Add(deut_dphi_neg_T);
    fOutputList->Add(deut_dphi_neg_A);
    fOutputList->Add(deut_dphi_neg_B);
    fOutputList->Add(deut_dphi_neg_V);
    fOutputList->Add(track_phi);
    fOutputList->Add(track_phi_hybrid);
    fOutputList->Add(track_eta);
    fOutputList->Add(track_eta_hybrid);
    
    fOutputList->Add(deut_dphi_deta_p0510);
    fOutputList->Add(deut_dphi_deta_p1020);
    fOutputList->Add(deut_dphi_deta_p2030);
    fOutputList->Add(deut_dphi_deta_p3040);
    fOutputList->Add(deut_dphi_deta_p4050);
    
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    AliAnalysisManager *man            = AliAnalysisManager::GetAnalysisManager();                  //// added by Brennan
    AliInputEventHandler* inputHandler = (AliInputEventHandler*) (man->GetInputEventHandler());     //// added by Brennan
    fPIDResponse                       = inputHandler->GetPIDResponse();                            //// added by Brennan

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////


    PostData(1, fOutputList);           // postdata will notify the analysis manager of changes / updates to the 
                                        // fOutputList object. the manager will in the end take care of writing your output to file
                                        // so it needs to know what's in the output
}







//_____________________________________________________________________________
void AliAnalysisTaskCorPIDTOFQA::UserExec(Option_t *)
{

    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());    // get an event (called fAOD) from the input file

    if(!fAOD) return;                                   // if the pointer to the event is empty (getting it failed) skip this event

    Int_t iTracks(fAOD->GetNumberOfTracks());           // see how many tracks there are in the event



//  fPIDResponse->SetTOFResponse(fAOD,AliPIDResponse::kTOF_T0);
//  fPIDResponse->SetTOFResponse(fAOD,AliPIDResponse::kT0_T0);
//  fPIDResponse->SetTOFResponse(fAOD,AliPIDResponse::kBest_T0);

    
    //////////////////////////////////////// MULTIPLICITY PART ////////////////////////////////////////

    cent_ntracks->Fill(fAOD->GetCentrality()->GetCentralityPercentile("V0M"), iTracks);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////



    int NDeuterons            = 0;
    int NDeuterons_pos        = 0;
    int NDeuterons_neg        = 0;
    int deut_flag_T           = 0;
    int deut_flag_A           = 0;
    int deut_flag_B           = 0;
    int deut_flag_V           = 0;
    int deut_track_num_T      = -9999;
    int deut_track_num_V      = -9999;
    int deut_track_num_A      = -9999;
    int deut_track_num_B      = -9999;
    
    int associated_tracks     = 0;

    struct track_node        // linked list, for efficient, dynamically allocated memory use
    {
	int track_num;
	track_node *next;
    };
    track_node *root;        // don't change this one, or you will lose the list in memory
    track_node *conductor;   // points to each node as it traverses the list
    root = new track_node;
    root->next = NULL;
    conductor = root;



    // loop over all these tracks
    for(Int_t i(0); i < iTracks; i++)
    {

        AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));         // get a track (type AliAODTrack) from the event
        if(!track) continue;                                                       // if we failed, skip this track
	if(!(track->IsHybridGlobalConstrainedGlobal())){   continue;   }


//	if(!(track->TestFilterBit(AliAODTrack::kTrkGlobal)) ) { continue; }

//	if(!fTrackSelection->IsTrackAccepted(track)){   continue;   }  // from markus' code
//	if(!(track->TestFilterBit(AliAODTrack::klsHybridTPCCG)) ) { continue; }
//	DCA_XY->Fill(track->XAtDCA(), track->YAtDCA());

	track_node *new_track = new track_node;
	new_track->track_num = i;
	new_track->next = NULL;
	conductor->next = new_track;
	conductor = conductor->next;

	
	short isGlobal    = 1;
	short isModGlobal = 0;  // is it suitable for deuteron candidate
	
	if(!(track->TestFilterBit(AliAODTrack::kTrkGlobal))){   isGlobal = 0;   }
	
	Double_t nsigmaTPC = 999.0;
	Double_t nsigmaTOF = 999.0;

	if(isGlobal == 1)  
	{
	    AliPIDResponse::EDetPidStatus statusTPC = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC, track, (AliPID::EParticleType) 0, nsigmaTPC);
	    AliPIDResponse::EDetPidStatus statusTOF = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTOF, track, (AliPID::EParticleType) 0, nsigmaTOF);
	    Bool_t tpcIsOk = (statusTPC == AliPIDResponse::kDetPidOk);     /* && trk->IsOn(AliESDtrack::kTPCpid)*/;
	    Bool_t tofIsOk = (statusTOF == AliPIDResponse::kDetPidOk);

	    if(tpcIsOk  && tofIsOk){   isModGlobal = 1;   }
	    else                   {   isModGlobal = 0;   }
		
	}



//	if(tpcIsOk  &&  tofIsOk)
//	if(tofIsOk)
//	{

	    
	Double_t mom            = track->P();
//	Double_t energy         = track->E();
//	float    mass           = track->M();
	float    pt             = track->Pt();
//	Double_t tpc_mom        = track->GetTPCmomentum();
	Short_t  charge         = track->Charge();
//	Double_t eta            = track->Eta();

	Double_t sigma_min      = -999.0;
	int      id             = 14;
	Double_t deut_mean      = 0.0;
	Double_t deut_sigma     = 0.0;
	Double_t cut_width      = 1.0;

	Double_t phi = track->Phi();
	Double_t eta = track->Eta();
	
	if(phi <  -TMath::PiOver2())    phi = phi + TMath::TwoPi();			if(phi <  -TMath::PiOver2())    phi = phi + TMath::TwoPi();
	if(phi > 3*TMath::PiOver2())    phi = phi - TMath::TwoPi();			if(phi > 3*TMath::PiOver2())    phi = phi - TMath::TwoPi();

	fHistPt->Fill(track->Pt());
	track_phi_hybrid->Fill(phi);
	track_eta_hybrid->Fill(eta);
	
	if(isModGlobal == 1)
	{
	    track_phi->Fill(phi);
	    track_eta->Fill(eta);
	    Double_t m2tof          = get_mass_squared(track);
	    Double_t start_time     = fPIDResponse->GetTOFResponse().GetStartTime(track->P());                      // in ps
	    Double_t stop_time      = track->GetTOFsignal();
	    Double_t time_of_flight = stop_time - start_time;
	    time_of_flight          = time_of_flight * 0.001;                                                       // convert ps to ns
	    Double_t c              = TMath::C()*1.E-9;                                                             // in m/ns
	    Double_t length         = 0.0;
	    length                  = fPIDResponse->GetTOFResponse().GetExpectedSignal(track,AliPID::kPion)*1E-3*c; // in meters
	    length                  = length * 100;                                                                 // convert mm to cm

	    if(charge > 0)
	    {
		m_squared_pos->Fill(mom,m2tof);
		plength_vs_mom_pos->Fill(mom, length);
		ttof_vs_mom_pos->Fill(   mom, time_of_flight);
		beta_vs_mom_pos->Fill(   mom, Beta(track));
		deltat_vs_mom_pos->Fill( mom, tof_minus_tpion(track));

		if(mom >= 1.1  &&  mom < 4.4)
		{
		    for(int w=0; w<3; w++){   fit_deut_curve->SetParameter(w, deut_curves[0][0][w]);   }
		    deut_mean = fit_deut_curve->Eval(mom);
		    for(int w=0; w<3; w++){   fit_deut_curve->SetParameter(w, deut_curves[0][1][w]);   }
		    deut_sigma = fit_deut_curve->Eval(mom);

		    if(m2tof < deut_mean + cut_width * deut_sigma  &&   m2tof > deut_mean - cut_width * deut_sigma)
		    {
			m_squared_pos_cut_T->Fill(mom,m2tof);
			deut_track_num_T = i;
		     /* deut_track_num_pos_T = i; */
			NDeuterons++;
			NDeuterons_pos++;
			deut_flag_T = 1;
		    }
		    else if(m2tof < deut_mean -1 + cut_width * deut_sigma  &&   m2tof > deut_mean -1 - cut_width * deut_sigma)
		    {
			m_squared_pos_cut_A->Fill(mom,m2tof);
			deut_flag_A = 1;
			deut_track_num_A = i;
		    }
		    else if(m2tof < deut_mean +1 + cut_width * deut_sigma  &&   m2tof > deut_mean +1 - cut_width * deut_sigma)
		    {
			m_squared_pos_cut_B->Fill(mom,m2tof);
			deut_flag_B = 1;
			deut_track_num_B = i;
		    }

////////////////////////
		    if(mom >= 2.1  &&  mom < 2.5)
		    {
			if(m2tof < deut_mean + cut_width * deut_sigma  &&   m2tof > deut_mean - cut_width * deut_sigma)
			{
			    AliPIDResponse::EDetPidStatus  statusTPC = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC,track,(AliPID::EParticleType) 2, nsigmaTPC);
			    if(fabs(nsigmaTPC) > 2.0)  // is it a pion?
			    {
				statusTPC = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC,track,(AliPID::EParticleType) 3, nsigmaTPC);
				if(fabs(nsigmaTPC) > 2.0)  // is it a kaon?
				{
				    statusTPC = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC,track,(AliPID::EParticleType) 4, nsigmaTPC);
				    if(fabs(nsigmaTPC) > 2.0)  // is it a proton?
				    {
					statusTPC=fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC,track,(AliPID::EParticleType)1,nsigmaTPC);
					if(fabs(nsigmaTPC) > 2.0)  // is it a muon?
					{
					    statusTPC = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC,track,(AliPID::EParticleType) 0, nsigmaTPC);
					    if(fabs(nsigmaTPC) > 2.0)  // is it an electron?
					    {
						m_squared_pos_cut_V->Fill(mom,m2tof);
						deut_track_num_V = i;
						deut_flag_V = 1;
					    }
					}
				    }
				}
			    }
			}		    
		    }
////////////////////////
		    
		}
	    }
	    else if(charge < 0)
	    {
		m_squared_neg->Fill(mom,m2tof);
		plength_vs_mom_neg->Fill(mom, length);
		ttof_vs_mom_neg->Fill(mom, time_of_flight);
		beta_vs_mom_neg->Fill(mom, Beta(track));
		deltat_vs_mom_neg->Fill(mom, tof_minus_tpion(track));

		if(mom >= 1.1  &&  mom < 4.4)
		{
		    for(int w=0; w<3; w++){   fit_deut_curve->SetParameter(w, deut_curves[1][0][w]);   }
		    deut_mean = fit_deut_curve->Eval(mom);
		    for(int w=0; w<3; w++){   fit_deut_curve->SetParameter(w, deut_curves[1][1][w]);   }
		    deut_sigma = fit_deut_curve->Eval(mom);
		    

		    if(m2tof < deut_mean + cut_width * deut_sigma  &&   m2tof > deut_mean - cut_width * deut_sigma)
		    {
			m_squared_neg_cut_T->Fill(mom,m2tof);
			deut_track_num_T = i;
		    /*	deut_track_num_neg_T = i;*/
			NDeuterons++;
			NDeuterons_neg++;
			deut_flag_T = 1;
		    }
		    else if(m2tof < deut_mean -1 + cut_width * deut_sigma  &&   m2tof > deut_mean -1 - cut_width * deut_sigma)
		    {
			m_squared_neg_cut_A->Fill(mom,m2tof);
			deut_flag_A = 1;
			deut_track_num_A = i;
		    }
		    else if(m2tof < deut_mean +1 + cut_width * deut_sigma  &&   m2tof > deut_mean +1 - cut_width * deut_sigma)
		    {
			m_squared_neg_cut_B->Fill(mom,m2tof);
			deut_flag_B = 1;
			deut_track_num_B = i;
		    }

////////////////////////
		    if(mom >= 2.1  &&  mom < 2.5)
		    {
			if(m2tof < deut_mean + cut_width * deut_sigma  &&   m2tof > deut_mean - cut_width * deut_sigma)
			{
			    AliPIDResponse::EDetPidStatus  statusTPC = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC,track,(AliPID::EParticleType) 2, nsigmaTPC);
			    if(fabs(nsigmaTPC) > 2.0)  // is it a pion?
			    {
				statusTPC = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC,track,(AliPID::EParticleType) 3, nsigmaTPC);
				if(fabs(nsigmaTPC) > 2.0)  // is it a kaon?
				{
				    statusTPC = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC,track,(AliPID::EParticleType) 4, nsigmaTPC);
				    if(fabs(nsigmaTPC) > 2.0)  // is it a proton?
				    {
					statusTPC=fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC,track,(AliPID::EParticleType)1,nsigmaTPC);
					if(fabs(nsigmaTPC) > 2.0)  // is it a muon?
					{
					    statusTPC = fPIDResponse->NumberOfSigmas(AliPIDResponse::kTPC,track,(AliPID::EParticleType) 0, nsigmaTPC);
					    if(fabs(nsigmaTPC) > 2.0)  // is it an electron?
					    {
						m_squared_neg_cut_V->Fill(mom,m2tof);
						deut_track_num_V = i;
						deut_flag_V = 1;
					    }
					}
				    }
				}
			    }
			}		    
		    }
////////////////////////
		    
		}
	    }
	}    //   track has both TPC and TOF PID, tracks are o.k.
    }        //   end of track loop

    deuterons_per_event->Fill(NDeuterons);
    deuterons_per_event_pos->Fill(NDeuterons_pos);
    deuterons_per_event_neg->Fill(NDeuterons_neg);



 

    if(deut_flag_T > 0)
    {
//	DeuteronCandidates++;
	int j = deut_track_num_T;

	AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(j));
	if(!track) {}
	else
	{
	    Double_t deut_phi = track->Phi();
	    Double_t deut_mom = track->P();
	    Short_t  charge   = track->Charge();
	    Double_t deut_eta = track->Eta();
	    conductor = root->next;
	    if (conductor != 0)
	    {
		while(conductor->next != 0)
		{
		    int k = conductor->track_num;
		    conductor = conductor->next;
		    
		    if(k != j)  // if the k-th track is not the j-th track which is the deuteron of interest
		    {
//		    NAssociatedTracks++;
			AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(k));         // get a track (type AliAODTrack) from the event
			if(!track){}                                                               // if we failed, skip this track
			else
			{
			    Double_t mom            = track->P();
			    Double_t phi            = track->Phi();
			    

			    if(mom > 1.0  &&  mom < 5.0)
			    {
				Double_t dphi = deut_phi - phi;
				if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
				if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
				if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
				if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
				
				deut_dphi_T->Fill(deut_mom, dphi);
				
				if(charge>0) {   deut_dphi_pos_T->Fill(deut_mom, dphi);   }
				if(charge<0) {   deut_dphi_neg_T->Fill(deut_mom, dphi);   }
				
			    }
			    if(deut_mom >= 3.25  &&  deut_mom < 4.25)
			    {
				Double_t eta  = track->Eta();
				Double_t dphi = deut_phi - phi;
				if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
				if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
				if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
				if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
				
				if(     mom >= 0.5  &&  mom < 1.0){   deut_dphi_deta_p0510->Fill(dphi, deut_eta - eta);    }
				else if(mom >= 1.0  &&  mom < 2.0){   deut_dphi_deta_p1020->Fill(dphi, deut_eta - eta);    }
				else if(mom >= 2.0  &&  mom < 3.0){   deut_dphi_deta_p2030->Fill(dphi, deut_eta - eta);    }
				else if(mom >= 3.0  &&  mom < 4.0){   deut_dphi_deta_p3040->Fill(dphi, deut_eta - eta);    }
				else if(mom >= 4.0  &&  mom < 5.0){   deut_dphi_deta_p4050->Fill(dphi, deut_eta - eta);    }
			    }
			}
		    }
		}
	    
		int k = conductor->track_num;
		conductor = conductor->next;
		if(k != j)  // if the k-th track is not the j-th track which is the deuteron of interest
		{
		    AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(k));         // get a track (type AliAODTrack) from the event
		    if(!track){}                                                               // if we failed, skip this track
		    else
		    {
			Double_t mom            = track->P();
			Double_t phi            = track->Phi();
			if(mom > 1.0  &&  mom < 5.0)
			{
			    Double_t dphi = deut_phi - phi;
			    if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
			    if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
			    if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
			    if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
			    deut_dphi_T->Fill(deut_mom, dphi);
			    if(charge>0) {   deut_dphi_pos_T->Fill(deut_mom, dphi);   }
			    if(charge<0) {   deut_dphi_neg_T->Fill(deut_mom, dphi);   }
			}
			if(deut_mom >= 3.25  &&  deut_mom < 4.25)
			{
			    Double_t eta            = track->Eta();
			    Double_t dphi = deut_phi - phi;
			    if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
			    if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
			    if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
			    if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
			    if(     mom >= 0.5  &&  mom < 1.0){   deut_dphi_deta_p0510->Fill(dphi, deut_eta - eta);    }
			    else if(mom >= 1.0  &&  mom < 2.0){   deut_dphi_deta_p1020->Fill(dphi, deut_eta - eta);    }
			    else if(mom >= 2.0  &&  mom < 3.0){   deut_dphi_deta_p2030->Fill(dphi, deut_eta - eta);    }
			    else if(mom >= 3.0  &&  mom < 4.0){   deut_dphi_deta_p3040->Fill(dphi, deut_eta - eta);    }
			    else if(mom >= 4.0  &&  mom < 5.0){   deut_dphi_deta_p4050->Fill(dphi, deut_eta - eta);    }
			}
		    }
		}
	    }
	}
    }  // end of deuteron correlation PRIMARY LOOP




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    if(deut_flag_A > 0)
    {
//	DeuteronCandidates++;
	int j = deut_track_num_A;
	AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(j));
	if(!track) {}
	else
	{
	    Double_t deut_phi = track->Phi();
	    Double_t deut_mom = track->P();
	    Short_t  charge   = track->Charge();
	    Double_t deut_eta = track->Eta();
	    conductor = root->next;
	    if (conductor != 0)
	    {
		while(conductor->next != 0)
		{
		    int k = conductor->track_num;
		    conductor = conductor->next;
		    if(k != j)  // if the k-th track is not the j-th track which is the deuteron of interest
		    {
			AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(k));         // get a track (type AliAODTrack) from the event
			if(!track){}                                                               // if we failed, skip this track
			else
			{
			    Double_t mom            = track->P();
			    Double_t phi            = track->Phi();
			    if(mom > 1.0  &&  mom < 5.0)
			    {
				Double_t dphi = deut_phi - phi;
				if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
				if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
				if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
				if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
				deut_dphi_A->Fill(deut_mom, dphi);
				if(charge>0) {   deut_dphi_pos_A->Fill(deut_mom, dphi);   }
				if(charge<0) {   deut_dphi_neg_A->Fill(deut_mom, dphi);   }
			    }
			}
		    }
		}
	    
		int k = conductor->track_num;
		conductor = conductor->next;
		if(k != j)  // if the k-th track is not the j-th track which is the deuteron of interest
		{
		    AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(k));         // get a track (type AliAODTrack) from the event
		    if(!track){}                                                               // if we failed, skip this track
		    else
		    {
			Double_t mom            = track->P();
			Double_t phi            = track->Phi();
			if(mom > 1.0  &&  mom < 5.0)
			{
			    Double_t dphi = deut_phi - phi;
			    if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
			    if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
			    if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
			    if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
			    deut_dphi_A->Fill(deut_mom, dphi);
			    if(charge>0) {   deut_dphi_pos_A->Fill(deut_mom, dphi);   }
			    if(charge<0) {   deut_dphi_neg_A->Fill(deut_mom, dphi);   }
			}
		    }
		}
	    }
	}
    }  // end of deuteron correlation A LOOP



    

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    if(deut_flag_B > 0)
    {
	int j = deut_track_num_B;
	AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(j));
	if(!track) {}
	else
	{
	    Double_t deut_phi = track->Phi();
	    Double_t deut_mom = track->P();
	    Short_t  charge   = track->Charge();
	    Double_t deut_eta = track->Eta();
	    conductor = root->next;
	    if (conductor != 0)
	    {
		while(conductor->next != 0)
		{
		    int k = conductor->track_num;
		    conductor = conductor->next;
		    
		    if(k != j)  // if the k-th track is not the j-th track which is the deuteron of interest
		    {
			AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(k));         // get a track (type AliAODTrack) from the event
			if(!track){}                                                               // if we failed, skip this track
			else
			{
			    Double_t mom            = track->P();
			    Double_t phi            = track->Phi();
			    if(mom > 1.0  &&  mom < 5.0)
			    {
				Double_t dphi = deut_phi - phi;
				if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
				if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
				if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
				if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
				deut_dphi_B->Fill(deut_mom, dphi);
				if(charge>0) {   deut_dphi_pos_B->Fill(deut_mom, dphi);   }
				if(charge<0) {   deut_dphi_neg_B->Fill(deut_mom, dphi);   }
			    }
			}
		    }
		}
	    
		int k = conductor->track_num;
		conductor = conductor->next;
		if(k != j)  // if the k-th track is not the j-th track which is the deuteron of interest
		{
		    AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(k));         // get a track (type AliAODTrack) from the event
		    if(!track){}                                                               // if we failed, skip this track
		    else
		    {
			Double_t mom            = track->P();
			Double_t phi            = track->Phi();
			if(mom > 1.0  &&  mom < 5.0)
			{
			    Double_t dphi = deut_phi - phi;
			    if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
			    if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
			    if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
			    if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
			    
			    deut_dphi_B->Fill(deut_mom, dphi);
			    if(charge>0) {   deut_dphi_pos_B->Fill(deut_mom, dphi);   }
			    if(charge<0) {   deut_dphi_neg_B->Fill(deut_mom, dphi);   }
			}
		    }
		}
	    }
	}
    }  // end of deuteron correlation B LOOP

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    
    if(deut_flag_V > 0)
    {
	int j = deut_track_num_V;
	AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(j));
	if(!track) {}
	else
	{
	    Double_t deut_phi = track->Phi();
	    Double_t deut_mom = track->P();
	    Short_t  charge   = track->Charge();
	    Double_t deut_eta = track->Eta();
	    conductor = root->next;
	    if (conductor != 0)
	    {
		while(conductor->next != 0)
		{
		    int k = conductor->track_num;
		    conductor = conductor->next;
		    if(k != j)  // if the k-th track is not the j-th track which is the deuteron of interest
		    {
			AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(k));         // get a track (type AliAODTrack) from the event
			if(!track){}                                                               // if we failed, skip this track
			else
			{
			    Double_t mom            = track->P();
			    Double_t phi            = track->Phi();
			    if(mom > 1.0  &&  mom < 5.0)
			    {
				Double_t dphi = deut_phi - phi;
				if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
				if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
				if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
				if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
				deut_dphi_V->Fill(deut_mom, dphi);
				if(charge>0) {   deut_dphi_pos_V->Fill(deut_mom, dphi);   }
				if(charge<0) {   deut_dphi_neg_V->Fill(deut_mom, dphi);   }
			    }
			}
		    }
		}
	    
		int k = conductor->track_num;
		conductor = conductor->next;
		if(k != j)  // if the k-th track is not the j-th track which is the deuteron of interest
		{
		    AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(k));         // get a track (type AliAODTrack) from the event
		    if(!track){}                                                               // if we failed, skip this track
		    else
		    {
			Double_t mom            = track->P();
			Double_t phi            = track->Phi();
			if(mom > 1.0  &&  mom < 5.0)
			{
			    Double_t dphi = deut_phi - phi;
			    if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
			    if(dphi <  -TMath::PiOver2())    dphi = dphi + TMath::TwoPi();
			    if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
			    if(dphi > 3*TMath::PiOver2())    dphi = dphi - TMath::TwoPi();
			    deut_dphi_V->Fill(deut_mom, dphi);
			    if(charge>0) {   deut_dphi_pos_V->Fill(deut_mom, dphi);   }
			    if(charge<0) {   deut_dphi_neg_V->Fill(deut_mom, dphi);   }
			}
		    }
		}
	    }
	}
    }  // end of deuteron correlation B LOOP


    
                                                     // continue until all the tracks are processed
    PostData(1, fOutputList);                           // stream the results the analysis of this event to
                                                        // the output manager which will take care of writing
                                                        // it to a file

}




//_____________________________________________________________________________
void AliAnalysisTaskCorPIDTOFQA::Terminate(Option_t *)
{
    // terminate
    // called at the END of the analysis (when all events are processed)
//    cout<<endl<<endl;
//    cout<<"total number deuteron candidates:  "<<DeuteronCandidates<<endl;
//    cout<<"total number of events analyzed:   "<<NEvents<<endl;
//    cout<<"total number of tracks analyzed:   "<<NTracks<<endl;
//    cout<<"total number of good tracks:       "<<NGoodTracks<<endl;
//    cout<<"tracks from events with deuterons: "<<NAssociatedTracks<<endl;
//    cout<<endl<<endl;
}
//_____________________________________________________________________________



//_____________________________________________________________________________
Double_t AliAnalysisTaskCorPIDTOFQA::Beta(AliAODTrack *track)
{
    Double_t startTime     = fPIDResponse->GetTOFResponse().GetStartTime(((AliVTrack*)track)->P());         //in ps
    Double_t stoptime      = track->GetTOFsignal();
    Double_t c             = TMath::C()*1.E-9;                                                              // m/ns
    Double_t length        = fPIDResponse->GetTOFResponse().GetExpectedSignal(track,AliPID::kElectron)*1E-3*c;
    stoptime -= startTime;      
    Double_t scaleStopTime = stoptime*1E-3;          
    scaleStopTime          = scaleStopTime*c;
    return length/scaleStopTime;
}



//_____________________________________________________________________________
Double_t AliAnalysisTaskCorPIDTOFQA::tof_minus_tpion(AliAODTrack *track)
{
    Double_t start_time     = fPIDResponse->GetTOFResponse().GetStartTime(track->P());                      // in ps
//  Double_t start_time     = fPIDResponse->GetTOFResponse().GetTimeZero() * 1e-12;
    Double_t stop_time      = track->GetTOFsignal();
    Double_t c              = TMath::C()*1.E-9;                                                             // in m/ns
    Double_t time_of_flight = stop_time - start_time;
    time_of_flight          = time_of_flight * 0.001;                                                       // convert from ps to ns
    Double_t length         = fPIDResponse->GetTOFResponse().GetExpectedSignal(track,AliPID::kPion)*1E-3*c; // in meters
    length = length * 100;                                                                                  // convert to cm
    Double_t pion_time      = fPIDResponse->GetTOFResponse().GetExpectedSignal(track,AliPID::kPion);        // // take a close look at this function -BRENNAN SCHAEFER-
    pion_time               = pion_time * 0.001;
    return time_of_flight - pion_time;
}



//_____________________________________________________________________________
Double_t AliAnalysisTaskCorPIDTOFQA::get_mass_squared(AliAODTrack *track)
{
    Double_t start_time     = fPIDResponse->GetTOFResponse().GetStartTime(track->P());                      // in ps
//  Double_t start_time     = fPIDResponse->GetTOFResponse().GetTimeZero() * 1e-12;                         // lower resolution than previous line
    Double_t stop_time      = track->GetTOFsignal();
    Double_t c              = TMath::C()*1.E-9;                                                             // in m/ns
    Double_t tof            = stop_time - start_time;
    tof                     = tof * 0.001;                                                                  // convert from ps to ns
    Double_t length         = fPIDResponse->GetTOFResponse().GetExpectedSignal(track,AliPID::kPion)*1E-3*c; // in meters
    length                  = length * 100;                                                                 // convert to cm
    Double_t mom            = track->P();
    Double_t m_squared      = 0.0;

    Double_t c2 = 29.9792458;
    
    m_squared = mom*mom * (tof*tof * c2 * c2 / (length * length) - 1);
    return m_squared;

}
