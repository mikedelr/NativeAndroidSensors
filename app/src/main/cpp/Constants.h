#ifndef CONSTANTS_H
#define CONSTANTS_H

class Constants{
	public:
		static const int LPF_N;
		static const int LPF_DELAY;
		static const double LPF_COEFFICIENTS[];

		static const int DIFF_PRESS_N;
		static const int DIFF_PRESS_DELAY;
		static const int DIFF_PRESS_N_STARTUP_TRANSIENT;
		static const double DIFF_PRESS_COEFFICIENTS[];

		static const int SAMPLE_MAXVAL;
		static const int BAR_SAMPLE_MAXVAL;
		static const int BAR_SIZE_MINUS_ONE;

		static const int LPF_DIFF_N;
		static const int LPF_DIFF_DELAY;
		static const double LPF_DIFF_COEFFICIENTS[];

		static const int BPF_N;
		static const int BPF_DELAY;
		static const double BPF_COEFFICIENTS[];

		static const int WIN_SIZE;
		static const int HALF_WIN_SIZE;
		static const int ACCUM_SIZE_ACC_GYR;
		static const int ACCUM_SIZE_ACC_GYR_MINUS_ONE;
		static const int NUM_ACC_GYR_WINDOWS;
		static const int NUM_ACC_GYR_DELAY_WINDOWS;
		static const int FEAT_ACC_GYR_BUFF_SIZE;
		static const int ACC_GYR_SIZE_MINUS_ONE;
		static const int ACC_GYR_SIZE_RESET_VAL;
		static const int NUM_ACC_GYR_FEATS;

		static const int FEAT_BAR_BUFF_SIZE;
		static const int ACCUM_SIZE_BAR;
		static const int BAR_SIZE_RESET_VAL;
		static const int NUM_BAR_FEATS;

		static const int STARTUP_WINDOWS;
		static const int HALF_PERIOD;

};

#endif
