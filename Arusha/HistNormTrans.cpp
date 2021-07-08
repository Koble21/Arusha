#include "HistNormTrans.h"
#include "pch.h"


/*
HistNormLinear::HistNormLinear(int _whichData, int _qBitDepth, float _l25)
{
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

HistNormLinear::HistNormLinear()
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


void HistNormLinear::run()
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
			sprintf(fname, "babainput/baba-%d.bab", patID);
		else
			sprintf(fname, "tumorinput/lg19-%02d.tum", patID);
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
			sprintf(fname, "babainput/baba-A1-%d-%d-%d.bab", patID, qBitDepth, cvRound(100 * lambda25));
		else
			sprintf(fname, "tumorinput/lg19-A1-%02d-%d-%d.tum", patID, qBitDepth, cvRound(100 * lambda25));

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

	GenerateFeatures(0);
}

void HistNormLinear::GenerateFeatures(int PICSA = 0)
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
				sprintf(fname, "babainput/baba-A1-%d-%d-%d.bab", patID, qBitDepth, cvRound(100 * lambda25));
			else
				sprintf(fname, "babainput/baba-A%d-%d-%d-%d-%d.bab", ALG, patID, qBitDepth, nrMilestones, cvRound(1000 * p_lo));
		else
			if (ALG == 1)
				sprintf(fname, "tumorinput/lg19-A1-%02d-%d-%d.tum", patID, qBitDepth, cvRound(100 * lambda25));
			else
				sprintf(fname, "input-hg-2019/hg19-A%d-%03d-%d-%d-%d.tum", ALG, 7 * patID + PICSA, qBitDepth, nrMilestones, cvRound(1000 * p_lo));
		//		sprintf(fname, "tumorinput/lg19-A%d-%02d-%d-%d-%d.tum", ALG, patID, qBitDepth, nrMilestones, cvRound(1000 * p_lo));
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
				PosBuffer[0][index] = 7 * patID + PICSA;
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
					{
						BigFootBuffer[ch][index] = dataBuffers[ch][i];
						int sum = dataBuffers[ch][i];
						int db = 1;
						for (int s = 1; s <= 5; ++s)
						{
							for (int j = -s; j < s; ++j)
							{
								if (dataBuffers[ch][i - s * WIDTH + j] > 0)
								{
									sum += dataBuffers[ch][i - s * WIDTH + j];
									++db;
								}
								if (dataBuffers[ch][i + s + j * WIDTH] > 0)
								{
									sum += dataBuffers[ch][i + s + j * WIDTH];
									++db;
								}
								if (dataBuffers[ch][i + s * WIDTH - j] > 0)
								{
									sum += dataBuffers[ch][i + s * WIDTH - j];
									++db;
								}
								if (dataBuffers[ch][i - s - j * WIDTH] > 0)
								{
									sum += dataBuffers[ch][i - s - j * WIDTH];
									++db;
								}
							}
							BigFootBuffer[OBSERVED_CHANNELS * s + ch][index] = (sum + db / 2) / db;
						}
						db = 0;
						sum = 0;
						int mini = dataBuffers[ch][i];
						int maxi = dataBuffers[ch][i];
						for (int dz = -1; dz <= 1; ++dz) for (int dy = -1; dy <= 1; ++dy) for (int dx = -1; dx <= 1; ++dx)
						{
							int addr = i + (dz * HEIGHT + dy) * WIDTH + dx;
							if (addr >= 0 && addr < VOLSIZE)
							{
								int value = dataBuffers[ch][i + (dz * HEIGHT + dy) * WIDTH + dx];
								if (value > 0)
								{
									sum += value;
									if (maxi < value) maxi = value;
									if (mini > value) mini = value;
									++db;
								}
							}
						}
						BigFootBuffer[6 * OBSERVED_CHANNELS + ch][index] = (sum + db / 2) / db;
						BigFootBuffer[7 * OBSERVED_CHANNELS + ch][index] = maxi;
						BigFootBuffer[8 * OBSERVED_CHANNELS + ch][index] = mini;
					}
				}
				else
				{
					for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
					{
						FeatBuffer[ch][index] = dataBuffers[ch][i];
						int sum = dataBuffers[ch][i];
						int db = 1;
						for (int s = 1; s <= 5; ++s)
						{
							for (int j = -s; j < s; ++j)
							{
								if (dataBuffers[ch][i - s * WIDTH + j] > 0)
								{
									sum += dataBuffers[ch][i - s * WIDTH + j];
									++db;
								}
								if (dataBuffers[ch][i + s + j * WIDTH] > 0)
								{
									sum += dataBuffers[ch][i + s + j * WIDTH];
									++db;
								}
								if (dataBuffers[ch][i + s * WIDTH - j] > 0)
								{
									sum += dataBuffers[ch][i + s * WIDTH - j];
									++db;
								}
								if (dataBuffers[ch][i - s - j * WIDTH] > 0)
								{
									sum += dataBuffers[ch][i - s - j * WIDTH];
									++db;
								}
							}
							FeatBuffer[OBSERVED_CHANNELS * s + ch][index] = (sum + db / 2) / db;
						}
						db = 0;
						sum = 0;
						int mini = dataBuffers[ch][i];
						int maxi = dataBuffers[ch][i];
						for (int dz = -1; dz <= 1; ++dz) for (int dy = -1; dy <= 1; ++dy) for (int dx = -1; dx <= 1; ++dx)
						{
							int addr = i + (dz * HEIGHT + dy) * WIDTH + dx;
							if (addr >= 0 && addr < VOLSIZE)
							{
								int value = dataBuffers[ch][i + (dz * HEIGHT + dy) * WIDTH + dx];
								if (value > 0)
								{
									sum += value;
									if (maxi < value) maxi = value;
									if (mini > value) mini = value;
									++db;
								}
							}
						}
						FeatBuffer[6 * OBSERVED_CHANNELS + ch][index] = (sum + db / 2) / db;
						FeatBuffer[7 * OBSERVED_CHANNELS + ch][index] = maxi;
						FeatBuffer[8 * OBSERVED_CHANNELS + ch][index] = mini;
					}
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
	float Q = 1.0;
	for (int i = 0; i < qBitDepth; ++i) Q = 2.0f * Q;
	Q = Q - 2.0f;

	if (VOLTYPE == 1)
		for (int patID = 0; patID < MAX_PATID; ++patID)
		{
			int minX = WIDTH / 2, maxX = WIDTH / 2, minY = HEIGHT / 2, maxY = HEIGHT / 2, minZ = MAX_SLICES - 1, maxZ = 0;
			for (int i = patLimits[patID].firstIndex; i <= patLimits[patID].lastIndex; ++i)
			{
				if (PosBuffer[1][i] > maxZ) maxZ = PosBuffer[1][i];
				if (PosBuffer[1][i] < minZ) minZ = PosBuffer[1][i];
				if (PosBuffer[2][i] > maxY) maxY = PosBuffer[2][i];
				if (PosBuffer[2][i] < minY) minY = PosBuffer[2][i];
				if (PosBuffer[3][i] > maxX) maxX = PosBuffer[3][i];
				if (PosBuffer[3][i] < minX) minX = PosBuffer[3][i];
			}
			for (int i = patLimits[patID].firstIndex; i <= patLimits[patID].lastIndex; ++i)
			{
				if (qBitDepth > 8)
				{
					BigFootBuffer[18][i] = 1 + cvRound(Q * ((float)(PosBuffer[1][i] - minZ) / (float)(maxZ - minZ)));
					BigFootBuffer[19][i] = 1 + cvRound(Q * ((float)(PosBuffer[2][i] - minY) / (float)(maxY - minY)));
					BigFootBuffer[20][i] = 1 + cvRound(Q * ((float)(PosBuffer[3][i] - minX) / (float)(maxX - minX)));
				}
				else
				{
					FeatBuffer[18][i] = 1 + cvRound(Q * ((float)(PosBuffer[1][i] - minZ) / (float)(maxZ - minZ)));
					FeatBuffer[19][i] = 1 + cvRound(Q * ((float)(PosBuffer[2][i] - minY) / (float)(maxY - minY)));
					FeatBuffer[20][i] = 1 + cvRound(Q * ((float)(PosBuffer[3][i] - minX) / (float)(maxX - minX)));
				}
			}
		}
	// Mentés


	if (VOLTYPE == 1)
		if (ALG == 1)
			sprintf(fname, "bigbaba-A1-%d-%d.dat", qBitDepth, cvRound(100 * lambda25));
		else
			sprintf(fname, "bigbaba-A%d-%d-%d-%d.dat", ALG, qBitDepth, nrMilestones, cvRound(1000 * p_lo));
	else
		if (ALG == 1)
			sprintf(fname, "tumor-A1-%d-%d.dat", qBitDepth, cvRound(100 * lambda25));
		else
			sprintf(fname, "tumor-A%d-%d-%d-%d-%d.dat", ALG, qBitDepth, nrMilestones, cvRound(1000 * p_lo), PICSA);

	F = fopen(fname, "wb");
	fwrite(&totalPixels, sizeof(int), 1, F);
	fwrite(&NR_FEATURES, sizeof(int), 1, F);
	fwrite(&qBitDepth, sizeof(int), 1, F);
	fwrite(&MAX_PATID, sizeof(int), 1, F);
	fwrite(patLimits, sizeof(PatientBasicData), MAX_PATID, F);
	fwrite(GTBuffer, sizeof(uchar), totalPixels, F);
	for (int c = 0; c < NR_COORDS; ++c)
	{
		fwrite(PosBuffer[c], sizeof(uchar), totalPixels, F);
	}
	for (int f = 0; f < NR_FEATURES; ++f)
		if (qBitDepth > 8)
		{
			fwrite(BigFootBuffer[f], sizeof(unsigned short), totalPixels, F);
		}
		else
		{
			fwrite(FeatBuffer[f], sizeof(uchar), totalPixels, F);
		}
	fclose(F);
	printf("Done. \n");

	for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
		free(dataBuffers[ch]);
	free(bufferGT);
	free(patLimits);

	ReleaseBuffers();
}


void HistNormLinear::CreateBuffers()
{
	if (qBitDepth > 8)
	{
		BigFootBuffer = (unsigned short**)malloc(NR_FEATURES * sizeof(unsigned short*));
		for (int f = 0; f < NR_FEATURES; ++f)
			BigFootBuffer[f] = (unsigned short*)malloc(MAX_PIXELS * sizeof(unsigned short));
	}
	else
	{
		FeatBuffer = (uchar**)malloc(NR_FEATURES * sizeof(uchar*));
		for (int f = 0; f < NR_FEATURES; ++f)
			FeatBuffer[f] = (uchar*)malloc(MAX_PIXELS);
	}

	PosBuffer = (uchar**)malloc(NR_COORDS * sizeof(uchar*));
	for (int c = 0; c < NR_COORDS; ++c)
		PosBuffer[c] = (uchar*)malloc(MAX_PIXELS);

	GTBuffer = (uchar*)malloc(MAX_PIXELS);
}

void HistNormLinear::ReleaseBuffers()
{
	if (qBitDepth > 8)
	{
		for (int f = 0; f < NR_FEATURES; ++f)
			free(BigFootBuffer[f]);
		free(BigFootBuffer);
	}
	else
	{
		for (int f = 0; f < NR_FEATURES; ++f)
			free(FeatBuffer[f]);
		free(FeatBuffer);
	}

	for (int c = 0; c < NR_COORDS; ++c)
		free(PosBuffer[c]);
	free(PosBuffer);

	free(GTBuffer);
}*/


/*  NYUL */

/*
HistNormNyul::HistNormNyul()
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

HistNormNyul::HistNormNyul(int _whichData, int _qBitDepth, float _pLO, int _milestones)
{
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
		MAX_PATID = 37;// 50;
		MAX_SLICES = 155;
		WIDTH = 180;// 160;
		HEIGHT = 200;// 192;
		MAX_PIXELS = 0x4400000;
		MILLIO = 2000000;
		OBSERVED_CHANNELS = 4;
		MAX_INTENSITY = 0x8000;
		NR_FEATURES = 36;
	}

	VOLSIZE = WIDTH * HEIGHT * MAX_SLICES;
}


void HistNormNyul::run()
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
			sprintf(fname, "babainput/baba-%d.bab", patID);
		else
			sprintf(fname, "input-hg-2019/hg19-%03d.tum", 7 * patID + PICSA);
		//sprintf(fname, "tumorinput/lg19-%02d.tum", patID);

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
			sprintf(fname, "babainput/baba-%d.bab", patID);
		else
			sprintf(fname, "input-hg-2019/hg19-%03d.tum", 7 * patID + PICSA);
		//sprintf(fname, "tumorinput/lg19-%02d.tum", patID);

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
			sprintf(fname, "babainput/baba-A2-%d-%d-%d-%d.bab", patID, qBitDepth, mileScheme, cvRound(1000 * p_lo));
		else
			sprintf(fname, "input-hg-2019/hg19-A2-%03d-%d-%d-%d.tum", 7 * patID + PICSA, qBitDepth, mileScheme, cvRound(1000 * p_lo));
		//sprintf(fname, "tumorinput/lg19-A2-%02d-%d-%d-%d.tum", patID, qBitDepth, mileScheme, cvRound(1000 * p_lo));

		F = fopen(fname, "wb");
		for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
			fwrite(dataBuffers[ch], 2, VOLSIZE, F);
		fwrite(bufferGT, 1, VOLSIZE, F);
		fclose(F);
	}
	GenerateFeatures(PICSA);
	for (int ch = 0; ch < OBSERVED_CHANNELS; ++ch)
		free(dataBuffers[ch]);
	free(bufferGT);
}
*/