// z_TestSDK.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SDK_SampleCode.h"
#include <iostream>
using namespace std;
using namespace PQ_SDK_MultiTouchSample;

int _tmain(int argc, _TCHAR* argv[])
{
	Sample sample;
	int err_code = sample.Init();
	if(err_code != PQMTE_SUCCESS){
		cout << "press any key to exit..." << endl;
		getchar();
		return 0;
	}
	// do other things of your application;
	cout << "hello world" << endl;
	//
	// here just wait here, not let the process exit;
	char ch  = 0;
	while(ch != 'q' && ch != 'Q'){
		cout << "press \'q\' to exit" << endl;
		ch = getchar();
	}
	return 0;
}



