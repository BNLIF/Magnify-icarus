#ifndef DATA_H
#define DATA_H

class TH2F;
class TH1I;
class TFile;
class Waveforms;
class RawWaveforms;
class BadChannels;

#include <vector>
#include <string>

class Data {
public:
    Data();
    Data(const char* filename, const char* tpc, const char* frame, int rebin);

    virtual ~Data();

    TFile *rootFile;
    vector<Waveforms*> wfs;
    BadChannels* bad_channels;
    vector<RawWaveforms*> raw_wfs;
    vector<TH1I*> thresh_histos;
    int runNo;
    int subRunNo;
    int eventNo;
    std::string tpcNo;

private:
    void load_waveform(const char* name, const char* title="", double scale=1);
    void load_rawwaveform(const char* name, const char* baseline_name);
    void load_threshold(const char* name);
    // void load_badchannels();
    void load_runinfo();
};

#endif
