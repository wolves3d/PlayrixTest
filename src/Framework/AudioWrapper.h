#ifndef __AudioWrapper_h_included__
#define __AudioWrapper_h_included__


class AudioWrapper
{
public:

	static void SetEnabled(bool enableFlag) { ms_enableFlag = enableFlag; }
	static void Play(const string & effectID);

private:

	static bool ms_enableFlag;
};

#endif __AudioWrapper_h_included__