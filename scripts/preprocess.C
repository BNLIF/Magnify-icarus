/*
Script to rename and combine the magnify histograms
Wenqiang Gu (wgu@bnl.gov)
*/

#include <iostream>
#include <cassert>
#include <string>
#include <regex>
#include "TFile.h"
#include "TH2F.h"
#include "TH2I.h"
#include "TCollection.h"
#include "TKey.h"
#include "TClass.h"
#include "TString.h"
#include "TROOT.h"
#include "TTree.h"
#include <numeric>      // std::accumulate

/*
 * Get File Name from a Path with or without extension
 * e.g., std::string filePath = "/home/user/sample/temp/data.csv";
 * std::string name = getFileName(filePath);
 * assert(name == "data.csv");
 */
std::string getFileName(const std::string& fullPath, const std::string extension=".root")
{
  const size_t lastSlashIndex = fullPath.find_last_of("/\\");
  return fullPath.substr(lastSlashIndex + 1, fullPath.size() - lastSlashIndex - extension.size() - 1);
}

void trim_baseline(TH2F* h){
  const int nbinsx = h->GetNbinsX();
  const int nbinsy = h->GetNbinsY();
  std::vector<float> v;
  for (int ix=0; ix<nbinsx; ix++) {
    v.clear();
    for (int iy=0; iy<nbinsy; iy++) {
      float content = h->GetBinContent(ix+1, iy+1);
      v.push_back(content);
    }// tick
    float average = std::accumulate( v.begin(), v.end(), 0.0)/v.size();
    for (int iy=0; iy<nbinsy; iy++) {
      float content = h->GetBinContent(ix+1, iy+1);
      h->SetBinContent(ix+1, iy+1,content - average);
    }// tick
  } // channel   

}

TH1F* merge_dim1(TH1F* h1, TH1F* h2, const char* name="hnew"){
    int x1 = h1->GetXaxis()->GetXmax();
    int x2 = h1->GetXaxis()->GetXmin();
    int x3 = h2->GetXaxis()->GetXmax();
    int x4 = h2->GetXaxis()->GetXmin();
    std::vector<int> xx = {x1, x2, x3, x4};
    auto minmax = std::minmax_element(xx.begin(), xx.end());
    int min = *(minmax.first);
    int max = *(minmax.second);
    // std::cout << x1 << " " << x2 << " " << x3 << " " << x4 << std::endl;
    std::cout << "min: " << min << " max: " << max << std::endl;

    TH1F* h = new TH1F(name,name,max-min, min, max);
    // fill first histogram
    for(int i=0; i<h1->GetNbinsX(); i++) {
      float center = h1->GetBinCenter(i+1);
      int bin1 = h->FindBin(center);
      h->AddBinContent(bin1, h1->GetBinContent(i+1));
    }
    // fill second histogram
    for(int i=0; i<h2->GetNbinsX(); i++) {
      float center = h2->GetBinCenter(i+1);
      int bin1 = h->FindBin(center);
      h->AddBinContent(bin1, h2->GetBinContent(i+1));
    }
    return h;
}

TH2F* merge_dim2(TH2F* h1, TH2F* h2, const char* name="h2new"){

    int ntbins1 = h1->GetNbinsY();
    int ntbins2 = h2->GetNbinsY();
    if (ntbins1 != ntbins2) {
      std::cout << "Error! Inconsistent number of ticks." << std::endl;
      return 0;
    }

    int x1 = h1->GetXaxis()->GetXmax();
    int x2 = h1->GetXaxis()->GetXmin();
    int x3 = h2->GetXaxis()->GetXmax();
    int x4 = h2->GetXaxis()->GetXmin();
    std::vector<int> xx = {x1, x2, x3, x4};
    auto minmax = std::minmax_element(xx.begin(), xx.end());
    int min = *(minmax.first);
    int max = *(minmax.second);
    // std::cout << x1 << " " << x2 << " " << x3 << " " << x4 << std::endl;
    std::cout << "min: " << min << " max: " << max << std::endl;

    TH2F* h = new TH2F(name,name, max-min, min, max, ntbins1, 0, ntbins1);
    // fill first histogram
    for(int i=0; i<h1->GetNbinsX(); i++) {
      for (int j=0; j<ntbins1; j++){
        float xcenter = h1->GetXaxis()->GetBinCenter(i+1);
        float ycenter = h1->GetYaxis()->GetBinCenter(j+1);
        int bin1 = h->FindBin(xcenter, ycenter);
        h->SetBinContent(bin1, h1->GetBinContent(i+1, j+1));
      }
    }
    // fill second histogram
    for(int i=0; i<h2->GetNbinsX(); i++) {
      for (int j=0; j<ntbins1; j++){
        float xcenter = h2->GetXaxis()->GetBinCenter(i+1);
        float ycenter = h2->GetYaxis()->GetBinCenter(j+1);
        int bin1 = h->FindBin(xcenter, ycenter);
        h->SetBinContent(bin1, h2->GetBinContent(i+1, j+1));
      }
    }
    return h;
}

// merge histograms given in- and out-tags
void preprocess(
      std::string inPath = "data/icarus-sim-check.root",
      std::string outDir = "data/", 
      const char* intag= "orig",
      const char* outtag = "orig",
      int tpc = 0,
      bool set_baseline=false,
      const char* file_open_mode = "update"
) {

  std::string fileName = getFileName(inPath);
  std::string outPath = outDir + "/" + fileName + "-tpc" + std::to_string(tpc) + ".root";
  std::cout << "input file: " << inPath << std::endl;
  std::cout << "output file: " << outPath << std::endl;
  std::cout << "in tag: " << intag << std::endl;
  std::cout << "out tag: " << outtag << std::endl;


  TFile *f1 = TFile::Open(inPath.c_str());

  // Merge histograms
  auto h = f1->Get(Form("hu_%s", intag));
  if(h) return;

  const char* u_intag = Form("hu_%s11%d", intag, tpc);
  const char* v_intag = Form("hv_%s11%d", intag, tpc);
  const char* w_intag = Form("hw_%s11%d", intag, tpc);
  const char* u_intag2 = Form("hu_%s12%d", intag, tpc);

  if(std::string(outtag)=="threshold"){
    TH1F* hu1 = (TH1F*)f1->Get(u_intag);
    TH1F* hu2 = (TH1F*)f1->Get(u_intag2);
    TH1F* hv = (TH1F*)f1->Get(v_intag);
    TH1F* hw = (TH1F*)f1->Get(w_intag);

    TH1F* hu_new = merge_dim1(hu1,hu2, Form("hu_%s", outtag));
    hv->SetName(Form("hv_%s", outtag));
    hw->SetName(Form("hw_%s", outtag));
    hv->SetTitle(Form("hv_%s", outtag));
    hw->SetTitle(Form("hw_%s", outtag));

    TFile* fout = new TFile(outPath.c_str(), file_open_mode);
    fout->cd();
    hu_new->Write();
    hv->Write();
    hw->Write();
    fout->Close();
  }
  else{
    TH2F* hu1 = (TH2F*)f1->Get(u_intag);
    TH2F* hu2 = (TH2F*)f1->Get(u_intag2);
    TH2F* hv = (TH2F*)f1->Get(v_intag);
    TH2F* hw = (TH2F*)f1->Get(w_intag);

    TH2F* hu_new = merge_dim2(hu1,hu2, Form("hu_%s", outtag));
    hv->SetName(Form("hv_%s", outtag));
    hw->SetName(Form("hw_%s", outtag));
    hv->SetTitle(Form("hv_%s", outtag));
    hw->SetTitle(Form("hw_%s", outtag));

    if (set_baseline) {
      trim_baseline(hu_new);
      trim_baseline(hv);
      trim_baseline(hw);
    }

    TFile* fout = new TFile(outPath.c_str(), file_open_mode);
    fout->cd();
    hu_new->Write();
    hv->Write();
    hw->Write();
    fout->Close();
   }
   f1->Close();

   std::cout << "\n Now try: ./magnify.sh " + outPath + "\n"; 
}
