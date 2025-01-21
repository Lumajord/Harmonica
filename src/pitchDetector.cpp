

#include "pitchDetector.h"

#include <cmath>
#include <mutex>
#include <chrono>
/////////////////////////////////////////////////////


const double PI = 3.141592653589793238463;





double log_frequencies2[] = { 2.7942279, 2.8518619, 2.90962957, 2.96784707, 3.02529108, 3.08328517,
3.14069804, 3.19867312, 3.25655689, 3.314186, 3.3721118, 3.42978484,
3.48737508, 3.54529773, 3.6030492, 3.66073715, 3.71843826, 3.77620328,
3.83406146, 3.8918203, 3.94951145, 4.00733319, 4.06508738, 4.12293202,
4.18067515, 4.23844491, 4.29619638, 4.35388433, 4.41170679, 4.469465,
4.52720864, 4.58496748, 4.64275495, 4.70048037, 4.75823456, 4.81599821,
4.87374589, 4.93151993, 4.98927545, 5.04703151, 5.1047933, 5.16255492,
5.22035583, 5.27811466, 5.33585397, 5.39362755, 5.45138174, 5.50914539,
5.56693129, 5.62466711, 5.68242264, 5.74021083, 5.79797081, 5.85573073,
5.91347598, 5.97126184, 6.02900115, 6.08677473, 6.14452892, 6.20229257,
6.26005936, 6.31783233, 6.37558684, 6.43334194, 6.49110283, 6.54887791,
6.60663667, 6.66439627, 6.72216037, 6.77992191, 6.83768683, 6.89544988,
6.95320654, 7.0109705, 7.06873402, 7.12649716, 7.18425759, 7.24201793,
7.29978385, 7.35754345, 7.41530755, 7.47306909, 7.53083401, 7.588592,
7.64635372, 7.70411768, 7.7618812, 7.81964434, 7.87740477, 7.93516869,
7.99293103, 8.05069063, 8.10845473, 8.16621627, 8.22397851, 8.28174171,
8.33950329, 8.39726486, 8.45502626, 8.51278951, 8.57055195, 8.62831408,
8.68607653, 8.7438394, 8.80160191, 8.85936345, 8.91712569, 8.97488762 };

double log_sizes2[] = { 0.05770084, 0.05770084, 0.05799258, 0.05783075, 0.05771905, 0.05770348,
0.05769397, 0.05792942, 0.05775644, 0.05777745, 0.05779942, 0.05763164,
0.05775644, 0.05783706, 0.05771971, 0.05769453, 0.05773307, 0.0578116,
0.05780851, 0.05772499, 0.05775644, 0.05778797, 0.05779942, 0.05779389,
0.05775644, 0.05776061, 0.05771971, 0.05775521, 0.05779034, 0.05775093,
0.05775124, 0.05777315, 0.05775644, 0.05773981, 0.05775892, 0.05775566,
0.05776086, 0.05776478, 0.05775579, 0.05775892, 0.0577617,  0.05778126,
0.05777987, 0.05774907, 0.05775644, 0.05776389, 0.05775892, 0.05777477,
0.05776086, 0.05774567, 0.05777186, 0.05777409, 0.05775995, 0.05775258,
0.05776555, 0.05776259, 0.05775644, 0.05776389, 0.05775892, 0.05776522,
0.05776988, 0.05776374, 0.0577548,  0.05775799, 0.05776799, 0.05776692,
0.05775918, 0.05776185, 0.05776282, 0.05776323, 0.05776398, 0.05775986,
0.05776031, 0.05776374, 0.05776333, 0.05776178, 0.05776039, 0.05776313,
0.05776276, 0.05776185, 0.05776282, 0.05776323, 0.05776145, 0.05775986,
0.05776284, 0.05776374, 0.05776333, 0.05776178, 0.05776218, 0.05776313,
0.05776097, 0.05776185, 0.05776282, 0.05776189, 0.05776272, 0.05776239,
0.05776157, 0.05776148, 0.05776233, 0.05776285, 0.05776229, 0.05776229,
0.05776266, 0.05776269, 0.05776202, 0.05776189, 0.05776209, 0.05776209 };



const int numNotes = 108;
void select_note_from_frequency(const double frequency, int &note, double &error)
{

	note = numNotes;


	if (frequency == 0.0)  // invalid note, show no error bar
	{
		note = numNotes;
	}
	else // we have a valid note
	{
		double log_note = log(frequency);

		double min = 100000.0;
		for (int i = 0; i < numNotes; ++i)
		{
			double diff = fabs(log_note - log_frequencies2[i]);

			if (diff < min)
			{
				min = diff;
				note = i;
			}
		}

		error = (log_note - log_frequencies2[note]) / log_sizes2[note];

	}

	return;
}


#ifdef asd
/* Returns length of LCS for X[0..m-1], Y[0..n-1] */
int pitchDetector::lcs(int *X, int *Y, const int m, const int n)
{

	/* Following steps build L[m+1][n+1] in bottom up fashion. Note
	that L[i][j] contains length of LCS of X[0..i-1] and Y[0..j-1] */
	for (int i = 0; i <= m; i++)
	{
		for (int j = 0; j <= n; j++)
		{
			if (i == 0 || j == 0)
				L[i*n + j] = 0;
			else if (X[i - 1] == Y[j - 1])
				L[i*n + j] = L[(i - 1)*n + j - 1] + 1;
			else
				L[i*n + j] = std::max(L[(i - 1)*n + j], L[i*n + j - 1]);
		}
	}

	// Following code is used to print LCS
	int index = L[m*n + n];

					   // Start from the right-most-bottom-most corner and
					   // one by one store characters in lcs[]
	int i = m, j = n;
	while (i > 0 && j > 0)
	{
		// If current character in X[] and Y are same, then
		// current character is part of LCS
		if (X[i - 1] == Y[j - 1])
		{
			longest_common_notes[index - 1] = X[i - 1]; // Put current character in result
			i--; j--; index--;     // reduce values of i, j and index
		}

		// If not same, then find the larger of two and
		// go in the direction of larger value
		else if (L[(i - 1)*n + j] > L[i*n + j - 1])
			i--;
		else
			j--;
	}


	// TODO return;
	return L[m*n + n];
}
#endif



const int pitchDetector::filter_order = 6;

pitchDetector::pitchDetector(int data_size) :
	m_soundGetter()
{
	m_initialized = false;
	m_multiple_note_detection = false;
	m_dataSize = data_size;

	m_dataFiltered = new double[m_dataSize];

	m_data = new float[m_dataSize];

	m_hammingAbsPaddedFFT = new double[3 * m_dataSize / 2];
	m_autocorrBuff = new double[3 * m_dataSize / 2];
	m_autocorrResult = new double[3 * m_dataSize / 2];

	m_hamming = new double[m_dataSize];
	m_hanning = new double[m_dataSize];

	m_cepstrumCpx = new kiss_fft_cpx[m_dataSize];
	m_autocorrCpx = new kiss_fft_cpx[3 * m_dataSize / 2];

	m_cepstrumData = new double[m_dataSize];
	m_tmpDouble = new double[m_dataSize];
	m_tmpInt = new int[m_dataSize];

	m_zeroCrossingsNeg = new int[m_dataSize / 2];
	m_zeroCrossingsPos = new int[m_dataSize / 2];

	m_fftData = new double[m_dataSize];



	m_cep_cfg = kiss_fftr_alloc(m_dataSize, 0, NULL, NULL);
	m_cep_icfg = kiss_fftr_alloc(m_dataSize, 1, NULL, NULL);

	m_auto_cfg = kiss_fftr_alloc(3 * m_dataSize / 2, 0, NULL, NULL);
	m_auto_icfg = kiss_fftr_alloc(3 * m_dataSize / 2, 1, NULL, NULL);




	for (int i = 0; i < m_dataSize; ++i)
	{

		// calculate hanning window
		double x = 2 * PI*double(i) / (double(m_dataSize - 1));
		m_hanning[i] = 0.5*(1.0 + cos(x));

		m_data[i] = 0.0f;

		// hamming window
		double hamming = 25.0 / 46.0 - 21.0 / 46.0 * cos(x);
		m_hamming[i] = hamming;
		m_autocorrBuff[i] = hamming;
	}



	for (int i = m_dataSize; i < 3 * m_dataSize / 2; ++i)
	{
		m_autocorrBuff[i] = 0.0; // pad with zeros
	}


	kiss_fftr(m_auto_cfg, m_autocorrBuff, m_autocorrCpx);

	for (int i = 0; i < 3 * m_dataSize / 2; ++i)
	{
		m_hammingAbsPaddedFFT[i] = sqrt(m_autocorrCpx[i].r * m_autocorrCpx[i].r + m_autocorrCpx[i].i * m_autocorrCpx[i].i);
	}



	log_note_size = 0.0;
	for (int i = 0; i < numNotes; ++i)
	{
		if (log_note_size < log_sizes2[i])
		{
			log_note_size = log_sizes2[i];
		}

	}




	L = new int[11 * (m_dataSize / 2 + 1)];
	longest_common_notes = new int[10];
	longest_common_error = new double[10];

	harmonica_top_row = new int[10];
	harmonica_bot_row = new int[10];


	m_filter = new Dsp::SmoothedFilterDesign
        <Dsp::Butterworth::Design::BandPass <filter_order>, NUM_CHANNELS, Dsp::DirectFormII>(m_dataSize);


	int no_device_found = 0;

init_soundGetter:
	
	m_soundGetter.init();
	m_last = true;
	m_soundGetter.startStream();


	// get data

	std::unique_lock<std::mutex> lock(m_soundGetter.data.m);

	while (!m_soundGetter.data.notified) {  // loop to avoid spurious wakeups
		m_soundGetter.data.flipped.wait(lock);
	}

	m_soundGetter.data.notified = false;
	float* data = m_soundGetter.data.getReadPointer();

	float sound = 0.0f;
	for (int i = 0; i < FRAMES_PER_BUFFER; ++i)
	{
		sound += fabs(data[i]);
	}


	if (sound < 5.0f)
	{
		if (no_device_found == 20)
		{
			printf("No Device found!\n");
			no_device_found = 0;
		}

		++no_device_found;
		m_soundGetter.destroy();
		goto init_soundGetter;
	}

	m_initialized = true;

	printf("Pitch Detector successfully initialized!\n");

	return;

}



pitchDetector::~pitchDetector()
{

	assert(m_initialized);

	m_soundGetter.endStream();


	delete[] m_dataFiltered;
	delete[] m_data;
	delete[] m_hammingAbsPaddedFFT;
	delete[] m_hamming;
	delete[] m_hanning;
	delete[] m_tmpDouble;
	delete[] m_tmpInt;

	delete[] m_zeroCrossingsPos;
	delete[] m_zeroCrossingsNeg;

	delete[] m_cepstrumCpx;
	delete[] m_autocorrCpx;

	delete[] m_cepstrumData;

	delete[] m_autocorrBuff;
	delete[] m_autocorrResult;

	delete m_filter;

	delete[] m_fftData;

	delete[] harmonica_top_row;
	delete[] harmonica_bot_row;
	delete[] longest_common_notes;
	delete[] longest_common_error;
	delete[] L;


	kiss_fft_cleanup();



}



int pitchDetector::cepstrumAnalysis()
{

	// calculate the cepstrum
	kiss_fftr(m_cep_cfg, m_cepstrumData, m_cepstrumCpx);

	for (int i = 0; i < m_dataSize; ++i)
	{
		double x = log10(1.0 + m_cepstrumCpx[i].r*m_cepstrumCpx[i].r + m_cepstrumCpx[i].i*m_cepstrumCpx[i].i);

		m_cepstrumCpx[i].r = x;
		m_cepstrumCpx[i].i = 0.0;
	}

	kiss_fftri(m_cep_icfg, m_cepstrumCpx, m_cepstrumData);

	double max = -1.e100;

	// find global maxima and all local maxima 
	int numMax = 0;
    for (int i = 5; i < m_dataSize /2; ++i)
	{
		if (m_cepstrumData[i] > m_cepstrumData[i - 1] && m_cepstrumData[i] > m_cepstrumData[i + 1])
		{
			if (max < m_cepstrumData[i])
			{
				max = m_cepstrumData[i];
			}

			m_tmpDouble[numMax] = m_cepstrumData[i];
			m_tmpInt[numMax] = i;
			numMax++;

		}
	}

	if (max < 0.1)
	{
		return int(0);
	}

	// find first maxima greater than 0.85 times the global maximum
    max = 0.85*max;

	for (int i = 0; i < numMax; ++i)
	{
		if (m_tmpDouble[i] > max)
		{
			return m_tmpInt[i];
		}
	}


	return int(0);

}



void pitchDetector::autoCorrelation()
{

	kiss_fftr(m_auto_cfg, m_autocorrResult, m_autocorrCpx);

	for (int i = 0; i < 3* m_dataSize /2; ++i)
	{

		m_autocorrCpx[i].r = m_autocorrCpx[i].r*m_autocorrCpx[i].r + m_autocorrCpx[i].i*m_autocorrCpx[i].i;
		m_autocorrCpx[i].i = 0.0;
	}

	kiss_fftri(m_auto_icfg, m_autocorrCpx, m_autocorrResult);

	kiss_fftr(m_auto_cfg, m_autocorrBuff, m_autocorrCpx);

	for (int i = 0; i < 3 * m_dataSize / 2; ++i)
	{
		m_autocorrCpx[i].r = m_hammingAbsPaddedFFT[i] * sqrt(m_autocorrCpx[i].r*m_autocorrCpx[i].r + m_autocorrCpx[i].i*m_autocorrCpx[i].i);
		m_autocorrCpx[i].i = 0.0;
	}

	kiss_fftri(m_auto_icfg, m_autocorrCpx, m_autocorrBuff);



	for (int i = 0; i < m_dataSize; ++i)
	{
		m_autocorrResult[i] = m_autocorrResult[i] / m_autocorrBuff[i];
	}

	return;

}




static double parabolic(double* data, int x)
{

	double xv = 0.5*(data[x - 1] - data[x + 1]) / (data[x - 1] - 2 * data[x] + data[x + 1]) + double(x);

	return xv;
}


/* Returns length of LCS for X[0..m-1], Y[0..n-1] */
int pitchDetector::lcs(const int *X, const int *Y, const int m, const int n)
{

	memset(L, 0, 11 * (m_dataSize / 2 + 1) * sizeof(int));

	int l = 0;
	int x1 = 0;
	/* Following steps build L[m+1][n+1] in bottom up fashion. Note
	that L[i][j] contains length of LCS of X[0..i-1] and Y[0..j-1] */
	for (int i = 1; i <= m; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			if (X[i - 1] == Y[j - 1])
			{
				L[i*n + j] = L[(i - 1)*n + j - 1] + 1;

				if (L[i*n + j] > l)
				{
					l = L[i*n + j];
					x1 = i;
				}
			}
			else
			{
				L[i*n + j] = 0;
			}
		}
	}

	int k = 0;
	for (int i = x1; i < x1 + l; ++i)
	{
		longest_common_notes[k] = X[i - l];
		longest_common_error[k] = m_fftData[i - 1];
		++k;
	}


	// TODO return;
	return l;
}


int pitchDetector::fftAnalysis(const int* harmonica_row)
{

	kiss_fftr(m_cep_cfg, m_cepstrumData, m_cepstrumCpx);

	for (int i = 0; i < m_dataSize / 2; ++i)
	{

		m_fftData[i] = m_cepstrumCpx[i].r*m_cepstrumCpx[i].r + m_cepstrumCpx[i].i*m_cepstrumCpx[i].i;

	}

	// get all local maxima and global maxima
	int num_max = 0;
	double max = 0.0;
	for (int i = 0; i < m_dataSize/2; ++i)
	{
		if (m_fftData[i] > m_fftData[i - 1] && m_fftData[i] > m_fftData[i + 1])
		{
			m_tmpInt[num_max] = i;		

			++num_max;

			if (m_fftData[i] > max)
			{
				max = m_fftData[i];
			}
		}
	}

	// filter out local maxima that are too small
	int new_num_max = 0;
	for (int i = 0; i < num_max; ++i)
	{
		if (m_fftData[m_tmpInt[i]] > max*0.025)
		{	
			m_tmpInt[new_num_max] = m_tmpInt[i];
			++new_num_max;
		}
	}

	// calculate frequencies
	for (int i = 0; i < new_num_max; ++i)
	{
		// reuse m_tmpDouble as frequency
		m_tmpDouble[i] = double(SAMPLE_RATE) * parabolic(m_fftData, m_tmpInt[i]) / double(m_dataSize);
	}


	// select notes and error from frequencies
	for (int i = 0; i < new_num_max; ++i)
	{
		// reuse m_tmpInt as note and m_fftData as error
		select_note_from_frequency(m_tmpDouble[i], m_tmpInt[i], m_fftData[i]);
	}

	
	double corrector = 0.0;
	for (int i = 0; i < new_num_max; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if (m_tmpInt[i] < 70)
			{
				corrector = 0.9;
			}
			if (m_tmpInt[i] < 60)
			{
				corrector = 1.1;
			}
			if (m_tmpInt[i] < 50)
			{
				corrector = 1.3;
			}
			if (m_tmpInt[i] != harmonica_row[j])
			{

				if (fabs(log(m_tmpDouble[i]) - log_frequencies2[harmonica_row[j]]) < corrector*log_sizes2[harmonica_row[j]])
				{

					m_tmpInt[i] = harmonica_row[j];
					m_fftData[i] = 0.5;
				}
			}

			if (m_tmpInt[i] > 70)
			{
				if (m_tmpInt[i] == harmonica_row[j])
				{
					if (fabs(log(m_tmpDouble[i]) - log_frequencies2[harmonica_row[j]]) > 0.3*log_sizes2[harmonica_row[j]])
					{
						m_tmpInt[i] = 108;
						m_fftData[i] = 0.5;
					}
				}
			}
		}
	}

	
	num_max = 0;
	for (int i = 1; i < new_num_max; ++i)
	{
		// if the notes are the same choose the one with smaller error
		if (m_tmpInt[num_max] == m_tmpInt[i])
		{
			if (fabs(m_fftData[num_max]) > fabs(m_fftData[i]))
			{
				m_fftData[num_max] = m_fftData[i];
				m_tmpInt[num_max] = m_tmpInt[i];
			}
		}
		else
		{
			num_max++;
			m_fftData[num_max] = m_fftData[i];
			m_tmpInt[num_max] = m_tmpInt[i];
		}
	}
	num_max++;

	return num_max;
}





int pitchDetector::detectPitch(int* playing_notes, double* playing_error)
{
#define VOCAL 0
#define TIMEIT 0
	assert(m_initialized);


#if TIMEIT
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> elapsed_seconds;

	start = std::chrono::system_clock::now();
#endif

	// get data
	
	std::unique_lock<std::mutex> lock(m_soundGetter.data.m);

	while (!m_soundGetter.data.notified) {  // loop to avoid spurious wakeups
		m_soundGetter.data.flipped.wait(lock);
	}

	m_soundGetter.data.notified = false;
	float* data = m_soundGetter.data.getReadPointer();

	for (int i = 0; i < m_dataSize - FRAMES_PER_BUFFER; ++i) // copy old data
	{

		m_data[i] = m_data[i + FRAMES_PER_BUFFER];

	}

	for (int i = 0; i < FRAMES_PER_BUFFER; ++i) // append new data
	{
		int j = i + m_dataSize - FRAMES_PER_BUFFER;
		m_data[j] = data[i];
	}






#if TIMEIT
	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;
    double time_ = 1000.0 * elapsed_seconds.count();
    //printf("time recording data %f\n", time_);


	start = std::chrono::system_clock::now();
#endif

	

	// Get average sound volume, if it's too silent return
	float average = 0.0f; // get sound volume
	for (int i = 0; i<m_dataSize; i++)
	{
		float val = m_data[i];
		if (val < 0) val = -val; // ABS

		average += val;
	}
	average = average / (float)m_dataSize;

	if (average < 0.0007f) // too silent so probably no note is played
	{
		return 0;
	}



















	for (int i = 0; i < m_dataSize; ++i)
	{
		double data = double(m_data[i]);	
		m_cepstrumData[i] = m_hanning[i] * data;

	}





	double low_freq = 15;

	double high_freq = 8200;

	int width = int(high_freq - low_freq);

	int center = int((high_freq + low_freq)*0.5);



	Dsp::Params params;
	params[0] = SAMPLE_RATE; // sample rate
	params[1] = filter_order; // order
	params[2] = center; // center frequency
	params[3] = width; // band width
	m_filter->setParams(params);


	m_filter->process(m_dataSize, &m_cepstrumData);




	if(m_multiple_note_detection)
	{
	int num_notes = fftAnalysis(harmonica_top_row);

	num_notes = lcs(m_tmpInt, harmonica_top_row, num_notes, 10);

	if (num_notes > 1)
	{

		for (int i = 0; i < num_notes; ++i)
		{
			playing_notes[i] = longest_common_notes[i];
			playing_error[i] = longest_common_error[i];
		}

	}


	int num_notes_bot = fftAnalysis(harmonica_bot_row);

	num_notes_bot = lcs(m_tmpInt, harmonica_bot_row, num_notes_bot, 10);





	if (num_notes_bot > 1)
	{

		if (num_notes > 1)
		{
			if (playing_notes[0] <= longest_common_notes[0])
			{
				return num_notes;
			}
		}

		for (int i = 0; i < num_notes_bot; ++i)
		{
			playing_notes[i] = longest_common_notes[i];
			playing_error[i] = longest_common_error[i];
		}

		return num_notes_bot;

	}
	
	if (num_notes > 1)
	{
		return num_notes;
	}

	} // detect multiple notes


	int time = cepstrumAnalysis();

	if (time == 0)
	{
		return 0;
	}

	double fundamentalFrequencyPrediciton = double(SAMPLE_RATE)/parabolic(m_cepstrumData, time);

	double log_prediction = log(fundamentalFrequencyPrediciton);

	low_freq = log_prediction - 3.0 * log_note_size;

	low_freq = std::max(exp(low_freq), 55.0);

	high_freq = log(low_freq) + 6.0 * log_note_size;

	high_freq = std::min(exp(high_freq), 7200.0);


	width = int(high_freq - low_freq);

	center = int((high_freq + low_freq)*0.5);

	

	params[0] = SAMPLE_RATE; // sample rate
	params[1] = filter_order; // order
	params[2] = center; // center frequency
	params[3] = width; // band width
	m_filter->setParams(params);
	

#if TIMEIT
    printf("%s predicted %f", filterInfo.c_str(), fundamentalFrequencyPrediciton);
#endif
	
	for (int i = 0; i < m_dataSize; ++i)
	{
		m_dataFiltered[i] = m_data[i];
	}


	
	m_filter->process(m_dataSize, &m_dataFiltered);



	for (int i = 0; i < m_dataSize; ++i)
	{
		m_autocorrResult[i] = m_hamming[i] * m_dataFiltered[i];
		m_autocorrBuff[i] = m_hamming[i] * m_dataFiltered[i] * m_dataFiltered[i];
	}


	

	for (int i = m_dataSize; i < 3*m_dataSize / 2; ++i)
	{
		m_autocorrResult[i] = 0.0;
		m_autocorrBuff[i] = 0.0;
	}


	autoCorrelation(); 
		


	// find the zero crossings
	int numCrossPos = 0; // crossings from negative to positive
	int numCrossNeg = 0; // crossings from positive to negative




	//always start with a crossing from negative to positive
	int j = 1;
	for (j; j < m_dataSize; ++j)
	{
		if (m_autocorrResult[j] >= 0.0 && m_autocorrResult[j-1] <= 0.0)
		{
			m_zeroCrossingsPos[numCrossPos] = j;
			++numCrossPos;
			++j;
			break;
		}
	}

	for (j; j < m_dataSize; ++j)
	{

		if (m_autocorrResult[j] >= 0.0 && m_autocorrResult[j - 1] <= 0.0)
		{
			m_zeroCrossingsPos[numCrossPos] = j;
			++numCrossPos;
		}

		if (m_autocorrResult[j] <= 0.0 && m_autocorrResult[j - 1] >= 0.0)
		{
			m_zeroCrossingsNeg[numCrossNeg] = j;
			++numCrossNeg;
		}

	}

	
	int numMax = std::min(numCrossNeg, numCrossPos);

	if (numMax == 0)
		return 0;


	// find all maxima between zero crossings and the global maxima
	double globalMax = 0.0;

	for (int i = 0; i < numMax; ++i)
	{

		double max = 0.0;
		int maxPos = 0;

		for (int k = m_zeroCrossingsPos[i]; k < m_zeroCrossingsNeg[i]; ++k) // find maximum between two zero crossings
		{
			if (m_autocorrResult[k] > max)
			{
				max = m_autocorrResult[k];
				maxPos = k;
			}
		}

		m_tmpDouble[i] = max;
		m_tmpInt[i] = maxPos;

		if (max > globalMax) // find global max
		{
			globalMax = max;
		}
	}


    globalMax = 0.9*globalMax; // remove all maxima smaller than c*globalMax   and use parabolic interpolation on them
	
	double frequency = 0.0;
	for (int i = 0; i < numMax; ++i)
	{
		if (m_tmpDouble[i] >= globalMax)
		{
#if VOCAL
			printf("Max = %d  %f\n", m_tmpInt[i], m_tmpDouble[i]);
#endif
			frequency = double(SAMPLE_RATE) / parabolic(m_autocorrResult, m_tmpInt[i]);
			break;

		}
	}

	
	
#if TIMEIT
	end = std::chrono::system_clock::now();
	elapsed_seconds = end - start;
    time_ = 1000.0 * elapsed_seconds.count();
    printf("note = %f auto first = %f   time processing data %f\n", note , m_cepstrumData[0], time_);
#endif



	select_note_from_frequency(frequency, playing_notes[0], playing_error[0]);

	return 1;

}

/*
int main()
{

	pitchDetector pitchD(2048, 1.0);


	for (int i = 0; i < 10; ++i)
	{
		double frequency = pitchD.detectPitch();
		printf("F = %f\n\n", frequency);
	}

	return 0;
}
*/
