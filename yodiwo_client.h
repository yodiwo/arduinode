#ifndef __YODIWO_CLIENT_H__
#define __YODIWO_CLIENT_H__

#include "yodiwo_api_defs.h"
#include <Arduino.h>

// =============================================================================================
// Yodiwo context
// =============================================================================================

struct Yodiwo_context
{
	Yodiwo_Plegma_Thing_t _things[THING_NUM];
	int thingsNum;
	String UserKey;
	String NodeKey;
	String NodeSecretKey;
	String LedThingKey;
	String LedThingPortKey;
	String ButtonThingKey;
	String ButtonThingPortKey;
	Yodiwo_API_msg incoming;
	Yodiwo_API_msg outgoing;
	String Payload;
};

#endif // __YODIWO_CLIENT_H__
