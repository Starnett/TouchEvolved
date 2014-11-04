#ifndef LEAP_GUARD
#define LEAP_GUARD

#include "Leap.h"
using namespace Leap;

class LeapListener : public Listener 
{
	public:
		virtual void onInit(const Controller&);
		virtual void onConnect(const Controller&);
		virtual void onDisconnect(const Controller&);
		virtual void onExit(const Controller&);
		virtual void onFrame(const Controller&);
		virtual void onFocusGained(const Controller&);
		virtual void onFocusLost(const Controller&);
		virtual void onDeviceChange(const Controller&);
		virtual void onServiceConnect(const Controller&);
		virtual void onServiceDisconnect(const Controller&);

		virtual void handleCircleGesture(const Gesture&);
		virtual void handleSwipeGesture(const Gesture&);
		virtual void handleKeyTapGesture(const Gesture&);
		virtual void handleScreenTapGesture(const Gesture&);

		virtual void handleRotation(const Hand);
		virtual void handleTranslation(const Hand);
		virtual void handlePinchZoom(const Hand);
		virtual void handleTouchEmulation(const Frame&);
		virtual void handleGestures(const Frame&);
		virtual void handleInteractionBox(const Frame&);

		virtual bool checkExtendedFingersCount(int);
		virtual void checkHandRoll(const Hand);
};


#endif	/* LEAP_GUARD */