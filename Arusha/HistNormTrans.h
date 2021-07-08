
#include "opencv2/opencv.hpp"
#include "opencv2/ml.hpp"

//#include <windows.h>
#include <conio.h>
#include <time.h>

#pragma once

using namespace cv;
using namespace ml;


const int MAX_MILESTONES = 11;
const int NR_COORDS = 4;

const int INFANT_DATA = 1;
const int TUMOR_DATA = 2;

const int ALG_KNN = 1;
const int ALG_DTREE = 2;
const int ALG_RF = 3;
const int ALG_ANN = 4;
const int ALG_ADA = 5;
const int ALG_LOGREG = 6;
const int ALG_SVM = 7;


struct PatientBasicData
{
	int firstIndex;
	int lastIndex;
	int pixelCount;
};


class HistNormLinear
{
public:
	HistNormLinear(int _caseID, int _whichData, int _qBitDepth, float _l25)
	{
		CASE = _caseID;
		VOLTYPE = _whichData;
		if (VOLTYPE != INFANT_DATA && VOLTYPE != TUMOR_DATA)
			VOLTYPE = INFANT_DATA;

		lambda25 = _l25;
		if (lambda25 < 0.25f) lambda25 = 0.25;
		if (lambda25 > 0.49f) lambda25 = 0.49;
		lambda75 = 1.0f - lambda25;

		qBitDepth = _qBitDepth;
		if (qBitDepth < 2) qBitDepth = 2;
		if (qBitDepth > 10) qBitDepth = 10;

		p_lo = 0.01f;
		nrMilestones = 5;
		ALG = 1;

		if (VOLTYPE == INFANT_DATA)
		{
			MAX_PATID = 10;
			MAX_SLICES = 256;
			WIDTH = 144;
			HEIGHT = 192;
			MAX_PIXELS = 8000000;
			MILLIO = 1000000;
			OBSERVED_CHANNELS = 2;
			MAX_INTENSITY = 1000;
			NR_FEATURES = 21;
		}
		else
		{
			MAX_PATID = 50;
			MAX_SLICES = 155;
			WIDTH = 160;
			HEIGHT = 192;
			MAX_PIXELS = 0x4400000;
			MILLIO = 2000000;
			OBSERVED_CHANNELS = 4;
			MAX_INTENSITY = 0x8000;
			NR_FEATURES = 36;
		}

		VOLSIZE = WIDTH * HEIGHT * MAX_SLICES;
	}
	HistNormLinear()
	{	
		VOLTYPE = INFANT_DATA;
		lambda25 = 0.4f;
		lambda75 = 1.0f - lambda25;
		qBitDepth = 8;
		p_lo = 0.01f;
		nrMilestones = 5;
		ALG = 1;

		MAX_PATID = 10;
		MAX_SLICES = 256;
		WIDTH = 144;
		HEIGHT = 192;
		MAX_PIXELS = 8000000;
		MILLIO = 1000000;
		OBSERVED_CHANNELS = 2;
		MAX_INTENSITY = 1000;
		NR_FEATURES = 21;
		VOLSIZE = WIDTH * HEIGHT * MAX_SLICES;
	}
	~HistNormLinear() {}

	void run()
	{
		char fname[100];
		unsigned short** dataBuffers = (unsigned short**)malloc(sizeof(unsigned short*) * OBSERVED_CHANNELS);
		for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
			dataBuffers[ch] = (unsigned short*)malloc(VOLSIZE * sizeof(unsigned short));
		unsigned char* bufferGT = (unsigned char*)malloc(VOLSIZE);
		unsigned short* flat = (unsigned short*)malloc(MILLIO * sizeof(unsigned short));

		for (int patID = 0; patID < MAX_PATID; ++patID)
		{
			if (VOLTYPE == 1)
				sprintf(fname, "input1/baba-%d.bab", patID);
			else
				sprintf(fname, "input2/lg19-%02d.tum", patID);
			FILE* F = fopen(fname, "rb");
			int nrBytes = 0;
			for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
				nrBytes += fread(dataBuffers[ch], sizeof(unsigned short), VOLSIZE, F);
			nrBytes += fread(bufferGT, 1, VOLSIZE, F);
			fclose(F);
			// if (nrBytes<3*VOLSIZE) printf("Data reading failed");
			int** hist = (int**)malloc(sizeof(int*) * OBSERVED_CHANNELS);
			float** coltrans = (float**)malloc(sizeof(float*) * OBSERVED_CHANNELS);
			for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
			{
				hist[ch] = (int*)calloc(MAX_INTENSITY, sizeof(int));
				coltrans[ch] = (float*)calloc(MAX_INTENSITY, sizeof(float));
			}

			int pixCount = 0;
			for (int pix = 0; pix < VOLSIZE; ++pix)
				if (bufferGT[pix] > 0)
				{
					for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
						hist[ch][dataBuffers[ch][pix] - 1]++;
					++pixCount;
				}

			int index, p25, p75;
			float a, b;

			for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
			{
				index = 0;
				for (int value = 0; value < MAX_INTENSITY; ++value)
				{
					for (int i = 0; i < hist[ch][value]; ++i)
						flat[index++] = value + 1;
				}

				p25 = flat[pixCount / 4];
				p75 = flat[pixCount * 3 / 4];

				a = (lambda75 - lambda25) / ((float)(p75 - p25));
				b = lambda25 - a * (float)p25;

				for (int value = 0; value < MAX_INTENSITY; ++value)
				{
					float newValue = a * (value + 1) + b;
					if (newValue < 0.0f) newValue = 0.0f;
					if (newValue > 1.0f) newValue = 1.0f;
					coltrans[ch][value] = newValue;
				}
			}

			float Q = 1.0f;
			for (int i = 0; i < qBitDepth; ++i) Q = 2.0f * Q;
			for (int pix = 0; pix < VOLSIZE; ++pix)
				if (bufferGT[pix] > 0)
					for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
					{
						int value = dataBuffers[ch][pix];
						int newValue = 1 + cvRound((Q - 2.0) * coltrans[ch][value]);
						dataBuffers[ch][pix] = newValue;
					}
			if (VOLTYPE == 1)
				sprintf(fname, "intermediary/baba-A1-%d-%d-%d.bab", patID, qBitDepth, cvRound(100 * lambda25));
			else
				sprintf(fname, "intermediary/lg19-A1-%02d-%d-%d.tum", patID, qBitDepth, cvRound(100 * lambda25));

			F = fopen(fname, "wb");
			for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
				fwrite(dataBuffers[ch], 2, VOLSIZE, F);
			fwrite(bufferGT, 1, VOLSIZE, F);
			fclose(F);
		}
		for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
			free(dataBuffers[ch]);
		free(bufferGT);
		free(flat);

		GenerateOutput();
	}

protected:
	void GenerateOutput()
	{
		FILE* F;
		char fname[100];
		printf("Starting feature generation. \n");

		unsigned short** dataBuffers = (unsigned short**)malloc(sizeof(unsigned short*) * OBSERVED_CHANNELS);
		for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
			dataBuffers[ch] = (unsigned short*)malloc(VOLSIZE * sizeof(unsigned short));
		unsigned char* bufferGT = (unsigned char*)malloc(VOLSIZE);

		patLimits = (PatientBasicData*)malloc(sizeof(PatientBasicData) * MAX_PATID);

		CreateBuffers();
		int totalPixels = 0;

		// adatok beolvasása és jellemzők számítása
		for (int patID = 0; patID < MAX_PATID; ++patID)
		{
			if (VOLTYPE == 1)
				if (ALG == 1)
					sprintf(fname, "intermediary/baba-A1-%d-%d-%d.bab", patID, qBitDepth, cvRound(100 * lambda25));
				else
					sprintf(fname, "intermediary/baba-A%d-%d-%d-%d-%d.bab", ALG, patID, qBitDepth, nrMilestones, cvRound(1000 * p_lo));
			else
				if (ALG == 1)
					sprintf(fname, "intermediary/lg19-A1-%02d-%d-%d.tum", patID, qBitDepth, cvRound(100 * lambda25));
				else
					sprintf(fname, "intermediary/lg19-A%d-%02d-%d-%d-%d.tum", ALG, patID, qBitDepth, nrMilestones, cvRound(1000 * p_lo));
			F = fopen(fname, "rb");
			for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
				fread(dataBuffers[ch], sizeof(unsigned short), VOLSIZE, F);
			fread(bufferGT, 1, VOLSIZE, F);
			fclose(F);
			int count = 0;

			int firstSlice = -1;
			int i = 0;
			while (i < VOLSIZE && firstSlice < 0)
			{
				if (bufferGT[i] > 0) firstSlice = i / (WIDTH * HEIGHT);
				++i;
			}
			if (firstSlice < 0) firstSlice = 0;

			for (int i = 0; i < VOLSIZE; ++i)
			{
				if (bufferGT[i] > 0)
				{
					int index = totalPixels + count;
					PosBuffer[0][index] = patID;
					PosBuffer[1][index] = i / (WIDTH * HEIGHT) - firstSlice;
					PosBuffer[2][index] = (i % (WIDTH * HEIGHT)) / WIDTH;
					PosBuffer[3][index] = i % WIDTH;
					if (VOLTYPE == INFANT_DATA)
						GTBuffer[index] = bufferGT[i] / 100; //(bufferGT[i]==10 ? 50 : bufferGT[i]);
					else
						GTBuffer[index] = bufferGT[i];
					if (qBitDepth > 8)
					{
						for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
							BigFootBuffer[ch][index] = dataBuffers[ch][i];
					}
					else
					{
						for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
							FeatBuffer[ch][index] = dataBuffers[ch][i];
					}

					++count;
				}
			}
			patLimits[patID].firstIndex = totalPixels;
			patLimits[patID].lastIndex = totalPixels + count - 1;
			patLimits[patID].pixelCount = count;
			totalPixels += count;
			printf(".");
		}

		// Mentés

		char shname[32];

		sprintf(shname, "%d", CASE);
/*		if (VOLTYPE == 1)
			if (ALG == 1)
				sprintf(shname, "infant-A1-%d-%d", qBitDepth, cvRound(100 * lambda25));
			else
				sprintf(shname, "infant-A2-%d-%d-%d", qBitDepth, nrMilestones, cvRound(1000 * p_lo));
		else
			if (ALG == 1)
				sprintf(shname, "tumor-A1-%d-%d", qBitDepth, cvRound(100 * lambda25));
			else
				sprintf(shname, "tumor-A2-%d-%d-%d", qBitDepth, nrMilestones, cvRound(1000 * p_lo));
*/
		sprintf(fname, "normhistres/%s.dat", shname);

		F = fopen(fname, "wb");
		fwrite(&totalPixels, sizeof(int), 1, F);
		fwrite(&OBSERVED_CHANNELS, sizeof(int), 1, F);
		fwrite(&qBitDepth, sizeof(int), 1, F);
		fwrite(&MAX_PATID, sizeof(int), 1, F);
		fwrite(patLimits, sizeof(PatientBasicData), MAX_PATID, F);
		fwrite(GTBuffer, sizeof(uchar), totalPixels, F);
		for (int c = 0; c < NR_COORDS; ++c)
		{
			fwrite(PosBuffer[c], sizeof(uchar), totalPixels, F);
		}
		for (int f = 0; f < OBSERVED_CHANNELS; ++f)
			if (qBitDepth > 8)
			{
				fwrite(BigFootBuffer[f], sizeof(unsigned short), totalPixels, F);
			}
			else
			{
				fwrite(FeatBuffer[f], sizeof(uchar), totalPixels, F);
			}
		fclose(F);

		F = fopen("normhistres/output.csv", "at");
		fprintf(F, "%s,%d,%d,%d,%d,%d\n", shname, VOLTYPE, ALG, 
			ALG==1 ? cvRound(lambda25*100) : cvRound(p_lo*1000), ALG == 1 ? cvRound(lambda25 * 100) : nrMilestones,
			qBitDepth);
		fclose(F);

		printf("Done. \n");

		for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
			free(dataBuffers[ch]);
		free(bufferGT);
		free(patLimits);

		ReleaseBuffers();
	}
	void CreateBuffers()
	{
		if (qBitDepth > 8)
		{
			BigFootBuffer = (unsigned short**)malloc(OBSERVED_CHANNELS * sizeof(unsigned short*));
			for (int f = 0; f < OBSERVED_CHANNELS; ++f)
				BigFootBuffer[f] = (unsigned short*)malloc(MAX_PIXELS * sizeof(unsigned short));
		}
		else
		{
			FeatBuffer = (uchar**)malloc(OBSERVED_CHANNELS * sizeof(uchar*));
			for (int f = 0; f < OBSERVED_CHANNELS; ++f)
				FeatBuffer[f] = (uchar*)malloc(MAX_PIXELS);
		}

		PosBuffer = (uchar**)malloc(NR_COORDS * sizeof(uchar*));
		for (int c = 0; c < NR_COORDS; ++c)
			PosBuffer[c] = (uchar*)malloc(MAX_PIXELS);

		GTBuffer = (uchar*)malloc(MAX_PIXELS);
	}
	void ReleaseBuffers()
	{
		if (qBitDepth > 8)
		{
			for (int f = 0; f < OBSERVED_CHANNELS; ++f)
				free(BigFootBuffer[f]);
			free(BigFootBuffer);
		}
		else
		{
			for (int f = 0; f < OBSERVED_CHANNELS; ++f)
				free(FeatBuffer[f]);
			free(FeatBuffer);
		}

		for (int c = 0; c < NR_COORDS; ++c)
			free(PosBuffer[c]);
		free(PosBuffer);

		free(GTBuffer);
	}

protected:
	unsigned short** BigFootBuffer;
	uchar** FeatBuffer;
	uchar** PosBuffer;
	uchar* GTBuffer;
	PatientBasicData* patLimits;

	float lambda25, lambda75, p_lo, p_hi;
	int qBitDepth, nrMilestones;
	int ALG;
	int VOLTYPE;

	int CASE;
	int MAX_PATID;
	int MAX_SLICES;
	int WIDTH;
	int HEIGHT;
	int VOLSIZE;
	int MAX_PIXELS;
	int MILLIO;
	int OBSERVED_CHANNELS;
	int MAX_INTENSITY;
	int NR_FEATURES;
};


class HistNormNyul : public HistNormLinear
{
public:
	HistNormNyul()
	{
		p_lo = 0.01f;
		p_hi = 1.0f - p_lo;
		nrMilestones = 5;
		qBitDepth = 8;
		lambda25 = 0.4f;
		ALG = 2;

		VOLTYPE = INFANT_DATA;

		MAX_PATID = 10;
		MAX_SLICES = 256;
		WIDTH = 144;
		HEIGHT = 192;
		MAX_PIXELS = 8000000;
		MILLIO = 1000000;
		OBSERVED_CHANNELS = 2;
		MAX_INTENSITY = 1000;
		NR_FEATURES = 21;

		VOLSIZE = WIDTH * HEIGHT * MAX_SLICES;
	}
	HistNormNyul(int _caseID, int _whichData, int _qBitDepth, float _pLO, int _milestones)
	{
		CASE = _caseID;
		VOLTYPE = _whichData;
		if (VOLTYPE != INFANT_DATA && VOLTYPE != TUMOR_DATA)
			VOLTYPE = INFANT_DATA;

		p_lo = _pLO;
		if (p_lo < 0.001f) p_lo = 0.001f;
		if (p_lo > 0.05f) p_lo = 0.05f;
		p_hi = 1.0f - p_lo;

		nrMilestones = _milestones;
		if (nrMilestones < 0) nrMilestones = 0;
		if (nrMilestones > 11) nrMilestones = 11;

		qBitDepth = _qBitDepth;
		if (qBitDepth < 2) qBitDepth = 2;
		if (qBitDepth > 10) qBitDepth = 10;

		lambda25 = 0.4f;
		ALG = 2;

		if (VOLTYPE == INFANT_DATA)
		{
			MAX_PATID = 10;
			MAX_SLICES = 256;
			WIDTH = 144;
			HEIGHT = 192;
			MAX_PIXELS = 8000000;
			MILLIO = 1000000;
			OBSERVED_CHANNELS = 2;
			MAX_INTENSITY = 1000;
			NR_FEATURES = 21;
		}
		else
		{
			MAX_PATID = 50;
			MAX_SLICES = 155;
			WIDTH = 160;
			HEIGHT = 192;
			MAX_PIXELS = 0x4400000;
			MILLIO = 2000000;
			OBSERVED_CHANNELS = 4;
			MAX_INTENSITY = 0x8000;
			NR_FEATURES = 36;
		}

		VOLSIZE = WIDTH * HEIGHT * MAX_SLICES;
	}
	~HistNormNyul() {};

	virtual void run()
	{
		int mileCount[12] = { 3,4,5,5,6,6,7,7,8,9,10,11 };

		int mileStones[12][12] =
		{ {50,-1,0,0,0,0,0,0,0,0,0,0},
		 {25,75,-1,0,0,0,0,0,0,0,0,0},
		 {25,50,75,-1,0,0,0,0,0,0,0,0},
		 {10,50,90,-1,0,0,0,0,0,0,0,0},
		 {20,40,60,80,-1,0,0,0,0,0,0,0},
		 {10,25,75,90,-1,0,0,0,0,0,0,0},
		 {20,35,50,65,80,-1,0,0,0,0,0,0},
		 {10,25,50,75,90,-1,0,0,0,0,0,0},
		 {10,25,40,60,75,90,-1,0,0,0,0,0},
		 {10,25,40,50,60,75,90,-1,0,0,0,0},
		 {10,20,30,40,60,70,80,90,-1,0,0,0},
		{10,20,30,40,50,60,70,80,90,-1,0,0} };


		int M = mileCount[nrMilestones];
		int mileScheme = nrMilestones;

		char fname[100];
		unsigned short** dataBuffers = (unsigned short**)malloc(sizeof(unsigned short*) * OBSERVED_CHANNELS);
		for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
			dataBuffers[ch] = (unsigned short*)malloc(VOLSIZE * sizeof(unsigned short));

		unsigned char* bufferGT = (unsigned char*)malloc(VOLSIZE);

		unsigned short* flat = (unsigned short*)malloc(MILLIO * sizeof(unsigned short));

		float** a = (float**)malloc(sizeof(float*) * OBSERVED_CHANNELS);
		float** b = (float**)malloc(sizeof(float*) * OBSERVED_CHANNELS);
		float** y_k_avg = (float**)malloc(sizeof(float*) * OBSERVED_CHANNELS);
		float*** y_kh = (float***)malloc(sizeof(float**) * OBSERVED_CHANNELS);
		int*** p_kh = (int***)malloc(sizeof(int**) * OBSERVED_CHANNELS);
		for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
		{
			a[ch] = (float*)malloc(sizeof(float) * MAX_PATID);
			b[ch] = (float*)malloc(sizeof(float) * MAX_PATID);
			y_k_avg[ch] = (float*)calloc(sizeof(float), MAX_PATID);
			y_kh[ch] = (float**)malloc(sizeof(float*) * MAX_PATID);
			p_kh[ch] = (int**)malloc(sizeof(int*) * MAX_PATID);
			for (int m = 0; m < MAX_PATID; ++m)
			{
				y_kh[ch][m] = (float*)malloc(sizeof(float) * MAX_MILESTONES);
				p_kh[ch][m] = (int*)malloc(sizeof(int) * MAX_MILESTONES);
			}
		}

		for (int patID = 0; patID < MAX_PATID; ++patID)
		{
			if (VOLTYPE == 1)
				sprintf(fname, "input1/baba-%d.bab", patID);
			else
				sprintf(fname, "input2/lg19-%02d.tum", patID);

			FILE* F = fopen(fname, "rb");
			int nrBytes = 0;
			for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
				nrBytes += fread(dataBuffers[ch], 2, VOLSIZE, F);
			nrBytes += fread(bufferGT, 1, VOLSIZE, F);
			fclose(F);

			int** hist = (int**)malloc(sizeof(int*) * OBSERVED_CHANNELS);
			for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
				hist[ch] = (int*)calloc(MAX_INTENSITY, sizeof(int));

			int pixCount = 0;
			for (int pix = 0; pix < VOLSIZE; ++pix)
				if (bufferGT[pix] > 0)
				{
					for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
						hist[ch][dataBuffers[ch][pix] - 1]++;
					++pixCount;
				}

			int index, p1, p99, p_ms;

			for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
			{
				index = 0;
				for (int value = 0; value < MAX_INTENSITY; ++value)
				{
					for (int i = 0; i < hist[ch][value]; ++i)
						flat[index++] = value + 1;
				}

				p1 = flat[cvRound((float)pixCount * p_lo)];
				p99 = flat[cvRound((float)pixCount * p_hi)];

				a[ch][patID] = 1.0f / ((float)(p99 - p1));
				b[ch][patID] = -a[ch][patID] * (float)p1;

				y_kh[ch][patID][0] = 0.0f;
				y_kh[ch][patID][M - 1] = 1.0f;
				p_kh[ch][patID][0] = p1;
				p_kh[ch][patID][M - 1] = p99;
				for (int i = 0; i < M - 2; ++i)
				{
					int ms = mileStones[mileScheme][i];
					p_ms = flat[cvRound((float)pixCount * (float)ms * 0.01f)];
					y_kh[ch][patID][i + 1] = a[ch][patID] * p_ms + b[ch][patID];
					p_kh[ch][patID][i + 1] = p_ms;
				}
			}
		}

		for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
			for (int patID = 0; patID < MAX_PATID; ++patID)
			{
				for (int m = 0; m < M; ++m)
					y_k_avg[ch][m] += y_kh[ch][patID][m] / (float)MAX_PATID;
			}

		for (int patID = 0; patID < MAX_PATID; ++patID)
		{
			float** coltrans = (float**)malloc(sizeof(float*) * OBSERVED_CHANNELS);
			for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
				coltrans[ch] = (float*)calloc(MAX_INTENSITY, sizeof(float));

			for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
			{
				for (int value = 0; value < p_kh[ch][patID][0]; ++value)
					coltrans[ch][value] = 0.0f;
				for (int value = p_kh[ch][patID][0]; value < MAX_INTENSITY; ++value)
					coltrans[ch][value] = 1.0f;
				for (int m = 0; m < M - 1; ++m)
				{
					for (int value = p_kh[ch][patID][m]; value <= p_kh[ch][patID][m + 1]; ++value)
					{
						float hanyadreszettettukmegazutnak = (float)(value - p_kh[ch][patID][m]) / (float)(p_kh[ch][patID][m + 1] - p_kh[ch][patID][m]);
						coltrans[ch][value - 1] = y_k_avg[ch][m] + (y_k_avg[ch][m + 1] - y_k_avg[ch][m]) * hanyadreszettettukmegazutnak;
					}
				}
			}

			if (VOLTYPE == 1)
				sprintf(fname, "input1/baba-%d.bab", patID);
			else
				sprintf(fname, "input2/lg19-%02d.tum", patID);

			FILE* F = fopen(fname, "rb");
			int nrBytes = 0;
			for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
				nrBytes += fread(dataBuffers[ch], 2, VOLSIZE, F);
			nrBytes += fread(bufferGT, 1, VOLSIZE, F);
			fclose(F);

			float Q = 1.0f;
			for (int i = 0; i < qBitDepth; ++i) Q = 2.0f * Q;
			for (int pix = 0; pix < VOLSIZE; ++pix)
				if (bufferGT[pix] > 0)
				{
					for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
					{
						int value = dataBuffers[ch][pix];
						int newValue = 1 + cvRound((Q - 2.0) * coltrans[ch][value]);
						dataBuffers[ch][pix] = newValue;
					}
				}

			if (VOLTYPE == 1)
				sprintf(fname, "intermediary/baba-A2-%d-%d-%d-%d.bab", patID, qBitDepth, mileScheme, cvRound(1000 * p_lo));
			else
				sprintf(fname, "intermediary/lg19-A2-%02d-%d-%d-%d.tum", patID, qBitDepth, mileScheme, cvRound(1000 * p_lo));

			F = fopen(fname, "wb");
			for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
				fwrite(dataBuffers[ch], 2, VOLSIZE, F);
			fwrite(bufferGT, 1, VOLSIZE, F);
			fclose(F);
		}
		GenerateOutput();
		for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
			free(dataBuffers[ch]);
		free(bufferGT);
	}
};


class Visualizer
{
public:
	Visualizer(int outputData) {
		o = outputData;

		char fname[100];
		sprintf(fname, "normhistres/%d.dat", outputData);
		FILE* F = fopen(fname, "rb");
		int head[4];
		fread(head, sizeof(int), 4, F); // PIXCOUNT, OBSERVED_CHANNES, DEPTH_BITS, MAX_PATID 
		PIX_COUNT = head[0];
		OBSERVED_CHANNELS = head[1];
		MAX_PATID = head[3];
		VOLTYPE = 0;
		if (head[1] == 2)
		{
			VOLTYPE = 1; WIDTH = 144; HEIGHT = 192; GAP = 32;
			imPicture = cvCreateImage(cvSize(8*WIDTH+5*GAP,5*HEIGHT),IPL_DEPTH_8U,1); 
		}
		else if (head[1] == 4)
		{
			VOLTYPE = 2; WIDTH = 160; HEIGHT = 192; GAP = 96;
			imPicture = cvCreateImage(cvSize(8 * WIDTH + GAP, 5 * HEIGHT), IPL_DEPTH_8U, 1);
		}
		cvSet(imPicture, cvScalar(0));
		patLimits = (PatientBasicData*)malloc(sizeof(PatientBasicData) * head[3]);
		fread(patLimits, sizeof(PatientBasicData), head[3], F);
		GTBuffer = (uchar*)malloc(sizeof(uchar) * head[0]);
		fread(GTBuffer, sizeof(uchar), head[0], F);
//		free(GTBuffer);
		PosBuffer = (uchar**)malloc(sizeof(uchar*) * NR_COORDS);
		for (int c = 0; c < NR_COORDS; ++c)
		{
			PosBuffer[c] = (uchar*)malloc(sizeof(uchar) * head[0]);
			fread(PosBuffer[c], sizeof(uchar), head[0], F);
		}
		if (head[2] > 8)
		{
			int x = 1;
			for (int o = 8; o < head[2]; ++o) x *= 2;
			unsigned short* BigFootBuffer = (unsigned short*)malloc(sizeof(unsigned short) * head[0]);
			FeatBuffer = (uchar**)malloc(sizeof(uchar*) * head[1]);
			for (int f = 0; f < head[1]; ++f)
			{
				fread(BigFootBuffer, sizeof(unsigned short), head[0], F);
				FeatBuffer[f] = (uchar*)malloc(sizeof(uchar) * head[0]);
				for (int o = 0; o < head[0]; ++o)
					FeatBuffer[f][o] = BigFootBuffer[o] / x;
			}
			free(BigFootBuffer);
		}
		else
		{
			int x = 1;
			for (int o = head[2]; o < 8; ++o) x *= 2;
			FeatBuffer = (uchar**)malloc(sizeof(uchar*) * head[1]);
			for (int f = 0; f < head[1]; ++f)
			{
				FeatBuffer[f] = (uchar*)malloc(sizeof(uchar) * head[0]);
				fread(FeatBuffer[f], sizeof(uchar), head[0], F);
				if (x > 1)
					for (int o = 0; o < head[0]; ++o)
						FeatBuffer[f][o] = 1 + FeatBuffer[f][o] * x;
			}
		}
		fclose(F);

		LoadInputData();
	//	dummy();
		ShowSlice(50);
	}

	~Visualizer() {
		for (int i = 0; i < OBSERVED_CHANNELS; ++i)
		{
			free(FeatBuffer[i]);
			free(InputBuffer[i]);
		}
		free(FeatBuffer);
		free(InputBuffer);
		for (int i = 0; i < NR_COORDS; ++i)
			free(PosBuffer[i]);
		free(PosBuffer);
		free(patLimits);
		cvDestroyAllWindows();
	}
/*
	void dummy()
	{
		unsigned short** dataBuffer = (unsigned short**)malloc(sizeof(unsigned short*) * OBSERVED_CHANNELS);
		if (VOLTYPE == 2)
		{
			for (int paci = 0; paci < MAX_PATID; ++paci)
			{
				char fname[100];
				sprintf(fname, "input2/lg19-%02d.tum", paci);
				FILE* F = fopen(fname, "rb");
				for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
				{
					dataBuffer[ch] = (unsigned short*)malloc(sizeof(unsigned short) * WIDTH * HEIGHT * 155);
					fread(dataBuffer[ch], sizeof(unsigned short), WIDTH * HEIGHT * 155, F);
				}
				fclose(F);

				int o = 0;
				while (dataBuffer[0][o] == 0) ++o;
				o = o / (WIDTH * HEIGHT);

				for (int pix = patLimits[paci].firstIndex; pix <= patLimits[paci].lastIndex; ++pix)
				{
					for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
					{
						int addr = o * WIDTH * HEIGHT + PosBuffer[1][pix]*WIDTH*HEIGHT + PosBuffer[2][pix]*WIDTH + PosBuffer[3][pix];
						int value = 1 + dataBuffer[ch][addr] / 6;
						if (value > 255) value = 255;
						FeatBuffer[ch][pix] = value;
					}
				}

				for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
					free(dataBuffer[ch]);
			}

			int uff[4] = {PIX_COUNT, 4, 8, 50};
			FILE* F = fopen("input2/input.dat", "wb");
			fwrite(uff, sizeof(int), 4, F);
			fwrite(patLimits, sizeof(PatientBasicData), MAX_PATID, F);
			fwrite(GTBuffer, sizeof(uchar), PIX_COUNT, F);
			for (int ch = 0; ch < NR_COORDS; ++ch)
				fwrite(PosBuffer[ch], sizeof(uchar), PIX_COUNT, F);
			for (int f = 0; f < OBSERVED_CHANNELS; ++f)
				fwrite(FeatBuffer[f], sizeof(uchar), PIX_COUNT, F);
			fclose(F);
		}
		if (VOLTYPE == 1)
		{
			for (int paci = 0; paci < MAX_PATID; ++paci)
			{
				char fname[100];
				sprintf(fname, "input1/baba-%d.bab", paci);
				FILE* F = fopen(fname, "rb");
				for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
				{
					dataBuffer[ch] = (unsigned short*)malloc(sizeof(unsigned short) * WIDTH * HEIGHT * 256);
					fread(dataBuffer[ch], sizeof(unsigned short), WIDTH * HEIGHT * 256, F);
				}
				fclose(F);

				int o = 0;
				while (dataBuffer[0][o] == 0) ++o;
				o = o / (WIDTH * HEIGHT);

				for (int pix = patLimits[paci].firstIndex; pix <= patLimits[paci].lastIndex; ++pix)
				{
					for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
					{
						int addr = o*WIDTH*HEIGHT + PosBuffer[1][pix] * WIDTH * HEIGHT + PosBuffer[2][pix] * WIDTH + PosBuffer[3][pix];
						int value = 1 + dataBuffer[ch][addr] / 4;
						if (value > 255) value = 255;
						FeatBuffer[ch][pix] = value;
					}
				}

				for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
					free(dataBuffer[ch]);
			}

			int uff[4] = { PIX_COUNT, 2, 8, 10 };
			FILE* F = fopen("input1/input.dat", "wb");
			fwrite(uff, sizeof(int), 4, F);
			fwrite(patLimits, sizeof(PatientBasicData), MAX_PATID, F);
			fwrite(GTBuffer, sizeof(uchar), PIX_COUNT, F);
			for (int ch = 0; ch < NR_COORDS; ++ch)
				fwrite(PosBuffer[ch], sizeof(uchar), PIX_COUNT, F);
			for (int f = 0; f < OBSERVED_CHANNELS; ++f)
				fwrite(FeatBuffer[f], sizeof(uchar), PIX_COUNT, F);
			fclose(F);
		}

	}
	*/
	void LoadInputData()
	{
		unsigned short** dataBuffer = (unsigned short**)malloc(sizeof(unsigned short*) * OBSERVED_CHANNELS);
		if (VOLTYPE == 2)
		{
			int head[4];
			FILE* F = fopen("input2/input.dat", "rb");
			fread(head, sizeof(int), 4, F);

			fseek(F, 16 + head[3] * 12 + head[0] * 5, SEEK_SET);
			InputBuffer = (uchar**)malloc(sizeof(uchar*) * OBSERVED_CHANNELS);
			for (int f = 0; f < OBSERVED_CHANNELS; ++f)
			{
				InputBuffer[f] = (uchar*)malloc(head[0]);
				fread(InputBuffer[f], sizeof(uchar), head[0], F);
			}
			fclose(F);
		}
		if (VOLTYPE == 1)
		{
			int head[4];
			FILE* F = fopen("input1/input.dat", "rb");
			fread(head, sizeof(int), 4, F);

			fseek(F, 16 + head[3] * 12 + head[0] * 5, SEEK_SET);
			InputBuffer = (uchar**)malloc(sizeof(uchar*) * OBSERVED_CHANNELS);
			for (int f = 0; f < OBSERVED_CHANNELS; ++f)
			{
				InputBuffer[f] = (uchar*)malloc(head[0]);
				fread(InputBuffer[f], sizeof(uchar), head[0], F);
			}
			fclose(F);
		}
	}

	void ShowSlice(int whichSlice, int page=0)
	{
		cvSet(imPicture, cvScalar(0));
		if (VOLTYPE == 1)
		{
			for (int pix = 0; pix < PIX_COUNT; ++pix)
				if (PosBuffer[1][pix] == whichSlice)
				{
					int paci = PosBuffer[0][pix];
					int x = PosBuffer[3][pix];
					int y = PosBuffer[2][pix];
					int t1 = FeatBuffer[0][pix];
					int t2 = FeatBuffer[1][pix];
					int _t1 = InputBuffer[0][pix];
					int _t2 = InputBuffer[1][pix];

					int X = (paci / 5) * (4 * WIDTH + 4 * GAP) + (2 * WIDTH + GAP) + x;
					int Y = (paci % 5) * HEIGHT + y;
					imPicture->imageData[Y * imPicture->widthStep + (X - GAP - 2*WIDTH)] = _t1;
					imPicture->imageData[Y * imPicture->widthStep + X] = t1;
					X += WIDTH;
					imPicture->imageData[Y * imPicture->widthStep + (X - GAP - 2 * WIDTH)] = _t2;
					imPicture->imageData[Y * imPicture->widthStep + X] = t2;
				}
		}
		else if (VOLTYPE == 2)
		{
			for (int pix = patLimits[5*page].firstIndex; pix <= patLimits[5*page+4].lastIndex; ++pix)
				if (PosBuffer[1][pix] == whichSlice)
				{
					int paci = PosBuffer[0][pix] - 5*page;
					int x = PosBuffer[3][pix];
					int y = PosBuffer[2][pix];
					int t1 = FeatBuffer[0][pix];
					int t2 = FeatBuffer[1][pix];
					int t1c = FeatBuffer[2][pix];
					int flair = FeatBuffer[3][pix];
					int _t1 = InputBuffer[0][pix];
					int _t2 = InputBuffer[1][pix];
					int _t1c = InputBuffer[2][pix];
					int _flair = InputBuffer[3][pix];

					int X = (4 * WIDTH + GAP) + x;
					int Y = paci * HEIGHT + y;
					imPicture->imageData[Y * imPicture->widthStep + X - (4 * WIDTH + GAP)] = _t1;
					imPicture->imageData[Y * imPicture->widthStep + X] = t1;
					X += WIDTH;
					imPicture->imageData[Y * imPicture->widthStep + X - (4 * WIDTH + GAP)] = _t2;
					imPicture->imageData[Y * imPicture->widthStep + X] = t2;
					X += WIDTH;
					imPicture->imageData[Y * imPicture->widthStep + X - (4 * WIDTH + GAP)] = _t1c;
					imPicture->imageData[Y * imPicture->widthStep + X] = t1c;
					X += WIDTH;
					imPicture->imageData[Y * imPicture->widthStep + X - (4 * WIDTH + GAP)] = _flair;
					imPicture->imageData[Y * imPicture->widthStep + X] = flair;
				}
		}
		cvShowImage("Histogram normalization output", imPicture);
	}
private:
	uchar** InputBuffer;
	uchar** FeatBuffer;
	uchar** PosBuffer;
	uchar* GTBuffer;
	PatientBasicData* patLimits;

	int o;
	int VOLTYPE;
	int WIDTH;
	int HEIGHT;
	int GAP;
	int MAX_PATID;
	int PIX_COUNT;
	int OBSERVED_CHANNELS;
	IplImage* imPicture;
};