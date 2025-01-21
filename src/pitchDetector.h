




#include "DspFilters/Dsp.h"
#include <cmath>
#include <cassert>

#include "kiss_fftr.h"

#include "getSoundData.h"
#include <vector>

class pitchDetector
{

public:
	
	pitchDetector(int data_size = 4096);
	~pitchDetector();

	int detectPitch(int* playing_notes, double* playing_error);

	int * harmonica_top_row;
	int * harmonica_bot_row;

	bool m_multiple_note_detection;

private:

	int cepstrumAnalysis();

	void autoCorrelation();


	int fftAnalysis(const int* harmonica_row);
	int *L;
	int *longest_common_notes;
	double *longest_common_error;

	int lcs(const int * X, const int * Y, const int m, const int n);

	bool m_initialized;

	
	int m_dataSize;

	float* m_data;

	double* m_dataFiltered;
	std::vector<double> m_splineY;
	std::vector<double> m_splineX;


	double* m_tmpDouble;
	int* m_tmpInt;
	

	kiss_fft_cpx* m_cepstrumCpx;
	kiss_fft_cpx* m_autocorrCpx;

	double* m_cepstrumData;

	

	double* m_autocorrBuff;
	double* m_autocorrResult;
	double* m_fftData;


	double* m_hamming;
	double* m_hammingAbsPaddedFFT;
	double* m_hanning;

	int* m_zeroCrossingsPos;
	int* m_zeroCrossingsNeg;


	bool m_last;
	mySound m_soundGetter;

	Dsp::Filter* m_filter;


    int m_midBorder;
    int m_highBorder;
	static const int filter_order;
	double log_note_size;

	kiss_fftr_cfg m_cep_cfg;
	kiss_fftr_cfg m_cep_icfg;

	kiss_fftr_cfg m_auto_cfg;
	kiss_fftr_cfg m_auto_icfg;



};
