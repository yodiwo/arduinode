#ifndef __YODIWO_API_DEFS_H__
#define __YODIWO_API_DEFS_H__

#include <Arduino.h>

// =============================================================================================
// NodeType definitions
// =============================================================================================

#define UnknownType         0
#define GatewayType         1
#define EndpointSingleType  2
#define TestGatewayType     3
#define TestEndpointType    4

// =============================================================================================
// Node capabilities definitions
// =============================================================================================

#define NoneCapabilities                  0
#define SupportsGraphSolvingCapabilities  1 << 0
#define ScannableCapabilities             1 << 1
#define IsWarlockCapabilities             1 << 2

// =============================================================================================
// Message flags definitions
// =============================================================================================

#define None       0
#define Request    1 << 0
#define Response   1 << 1

// =============================================================================================
// Plegma API MSGs
// =============================================================================================

#define TOPIC_XXXXXXX "XXXXXXXX"
#define TOPIC_activeportkeysmsg "activeportkeysmsg"
#define TOPIC_nodeinforeq       "nodeinforeq"
#define TOPIC_nodeinforsp       "nodeinforsp"
#define TOPIC_thingsget         "thingsget"
#define TOPIC_thingsset         "thingsset"
#define TOPIC_genericrsp        "genericrsp"
#define TOPIC_portstateget      "portstateget"
#define TOPIC_portstateset      "portstateset"
#define TOPIC_porteventmsg      "porteventmsg"

// =============================================================================================
// json member
// =============================================================================================

#define JSON_MEMBER_FLAGS    "Flags"
#define JSON_MEMBER_SYNC_ID  "SyncId"
#define JSON_MEMBER_PAYLOAD_THING_REV_NUM  "ThingsRevNum"
#define JSON_MEMBER_PAYLOAD_SEQ_NO  "SeqNo"

// =============================================================================================
// MQTT message members
// =============================================================================================

#define MQTT_MSG_FLAGS         "Flags"
#define MQTT_MSG_SYNC_ID       "SyncId"
#define MQTT_MSG_PAYLOAD       "Payload"
#define MQTT_MSG_PAYLOAD_SIZE  "PayloadSize"

// =============================================================================================
// Plegma API
// =============================================================================================

/*
 * ePortType
 */
typedef enum
{
	Yodiwo_ePortType_Undefined = 0,
	Yodiwo_ePortType_Integer = 1,
	Yodiwo_ePortType_Decimal = 2,
	Yodiwo_ePortType_DecimalHigh = 3,
	Yodiwo_ePortType_Boolean = 4,
	Yodiwo_ePortType_Color = 5,
	Yodiwo_ePortType_String = 6,
	Yodiwo_ePortType_VideoDescriptor = 7,
	Yodiwo_ePortType_AudioDescriptor = 8,
	Yodiwo_ePortType_BinaryResourceDescriptor = 9,
	Yodiwo_ePortType_I2CDescriptor = 10,
	Yodiwo_ePortType_JsonString = 11,
} Yodiwo_Plegma_ePortType;

/*
 * ioPortDirection
 */
typedef enum
{
	Yodiwo_ioPortDirection_Undefined = 0,
	Yodiwo_ioPortDirection_InputOutput = 1,
	Yodiwo_ioPortDirection_Output = 2,
	Yodiwo_ioPortDirection_Input = 3,
} Yodiwo_Plegma_ioPortDirection;

/*
 * ePortConf
 */
typedef enum
{
	Yodiwo_ePortConf_None = 0,
	Yodiwo_ePortConf_PropagateAllEvents = 1,
	Yodiwo_ePortConf_IsTrigger = 2,
	Yodiwo_ePortConf_DoNotNormalize = 4,
} Yodiwo_Plegma_ePortConf;

/*
typedef enum
{
Yodiwo_BinaryResourceContentType_Undefined = 0,
Yodiwo_BinaryResourceContentType_Data = 1,
Yodiwo_BinaryResourceContentType_Text = 2,
Yodiwo_BinaryResourceContentType_Image = 3,
Yodiwo_BinaryResourceContentType_Audio = 4,
Yodiwo_BinaryResourceContentType_Video = 5,
} Yodiwo_Plegma_BinaryResourceContentType;

typedef enum
{
Yodiwo_BinaryResourceLocationType_Undefined = 0,
Yodiwo_BinaryResourceLocationType_Http = 1,
Yodiwo_BinaryResourceLocationType_RedisDB = 2,
} Yodiwo_Plegma_BinaryResourceLocationType;

typedef enum
{
Yodiwo_RestServiceType_Undefined = 0,
Yodiwo_RestServiceType_Dropbox = 1,
Yodiwo_RestServiceType_Pastebin = 2,
Yodiwo_RestServiceType_GoogleDrive = 3,
} Yodiwo_Plegma_RestServiceType;

typedef enum
{
Yodiwo_ImageFileFormat_Undefined = 0,
Yodiwo_ImageFileFormat_PNG = 1,
Yodiwo_ImageFileFormat_TIFF = 2,
Yodiwo_ImageFileFormat_GIF = 3,
Yodiwo_ImageFileFormat_BMP = 4,
Yodiwo_ImageFileFormat_SVG = 5,
} Yodiwo_Plegma_ImageFileFormat;

typedef enum
{
Yodiwo_ImageType_Raster = 0,
Yodiwo_ImageType_Vector = 1,
} Yodiwo_Plegma_ImageType;

typedef enum
{
Yodiwo_eConnectionFlags_None = 0,
Yodiwo_eConnectionFlags_CreateNewEndpoint = 1,
Yodiwo_eConnectionFlags_IsMasterEndpoint = 2,
} Yodiwo_Plegma_eConnectionFlags;

typedef enum
{
Yodiwo_eNodeType_Unknown = 0,
Yodiwo_eNodeType_Gateway = 1,
Yodiwo_eNodeType_EndpointSingle = 2,
Yodiwo_eNodeType_TestGateway = 3,
Yodiwo_eNodeType_TestEndpoint = 4,
Yodiwo_eNodeType_WSEndpoint = 5,
Yodiwo_eNodeType_Android = 6,
Yodiwo_eNodeType_WSSample = 200,
Yodiwo_eNodeType_RestSample = 201,
} Yodiwo_Plegma_eNodeType;

typedef enum
{
Yodiwo_eNodeCapa_None = 0,
Yodiwo_eNodeCapa_SupportsGraphSolving = 1,
Yodiwo_eNodeCapa_Scannable = 2,
} Yodiwo_Plegma_eNodeCapa;

typedef enum
{
Yodiwo_eUnpairReason_Unknown = 0,
Yodiwo_eUnpairReason_InvalidOperation = 1,
Yodiwo_eUnpairReason_UserRequested = 2,
Yodiwo_eUnpairReason_TooManyAuthFailures = 3,
} Yodiwo_Plegma_eUnpairReason;

typedef enum
{
Yodiwo_eNodeSyncOperation_GetEndpoints = 1,
Yodiwo_eNodeSyncOperation_SetEndpoint = 2,
} Yodiwo_Plegma_eNodeSyncOperation;

typedef enum
{
Yodiwo_eThingsOperation_Invalid = 0,
Yodiwo_eThingsOperation_Update = 1,
Yodiwo_eThingsOperation_Overwrite = 2,
Yodiwo_eThingsOperation_Delete = 3,
Yodiwo_eThingsOperation_Get = 4,
Yodiwo_eThingsOperation_Scan = 5,
Yodiwo_eThingsOperation_Sync = 6,
} Yodiwo_Plegma_eThingsOperation;
*/

/*
 * ePortStateOperation
 */
typedef enum
{
	Yodiwo_ePortStateOperation_Invalid = 0,
	Yodiwo_ePortStateOperation_SpecificKeys = 1,
	Yodiwo_ePortStateOperation_ActivePortStates = 2,
	Yodiwo_ePortStateOperation_AllPortStates = 3,
} Yodiwo_Plegma_ePortStateOperation;

/*
typedef enum
{
Yodiwo_eA2mcuCtrlType_Reset = 0,
Yodiwo_eA2mcuCtrlType_SetValue = 1,
Yodiwo_eA2mcuCtrlType_WriteDriconf = 2,
} Yodiwo_Plegma_eA2mcuCtrlType;

typedef enum
{
Yodiwo_PairingStates_Initial = 0,
Yodiwo_PairingStates_StartRequested = 1,
Yodiwo_PairingStates_TokensRequested = 2,
Yodiwo_PairingStates_TokensSentToNode = 3,
Yodiwo_PairingStates_Token2SentToUser = 4,
Yodiwo_PairingStates_Token2PostedToServer = 5,
Yodiwo_PairingStates_UUIDEntryRedirect = 6,
Yodiwo_PairingStates_Phase1Complete = 7,
Yodiwo_PairingStates_NextRequested = 8,
Yodiwo_PairingStates_Token1PostedToServer = 9,
Yodiwo_PairingStates_KeysSentToNode = 10,
Yodiwo_PairingStates_Paired = 11,
Yodiwo_PairingStates_Failed = 12,
} Yodiwo_Plegma_NodePairing_PairingStates;

typedef enum
{
Yodiwo_Boolean_Format_TrueFalse = 0,
Yodiwo_Boolean_Format_OnOff = 1,
Yodiwo_Boolean_Format_YesNo = 2,
Yodiwo_Boolean_Format_EnabledDisabled = 3,
Yodiwo_Boolean_Format_EnableDisable = 4,
Yodiwo_Boolean_Format_ActiveInactive = 5,
Yodiwo_Boolean_Format_ActivateDeactivate = 6,
Yodiwo_Boolean_Format_ActivatedDeactivated = 7,
} Yodiwo_Plegma_PortStateSemantics_Boolean_Format;

typedef enum
{
Yodiwo_String_Case_None = 0,
Yodiwo_String_Case_Lower = 1,
Yodiwo_String_Case_Upper = 2,
} Yodiwo_Plegma_PortStateSemantics_String_Case;
*/

/*
 * eMsgFlags
 */
typedef enum
{
	Yodiwo_eMsgFlags_None = 0,
	Yodiwo_eMsgFlags_Request = 1,
	Yodiwo_eMsgFlags_Response = 2,
} Yodiwo_Plegma_WrapperMsg_eMsgFlags;

/*
typedef enum
{
Yodiwo_eWSMsgId_None = 0,
Yodiwo_eWSMsgId_Pairing = 1,
Yodiwo_eWSMsgId_Api = 2,
} Yodiwo_Plegma_WebSocketMsg_eWSMsgId;
*/

/*
 * Port
 */
typedef struct Yodiwo_Plegma_Port
{
	String PortKey;
	String Name;
	String Description;
	Yodiwo_Plegma_ioPortDirection ioDirection;
	Yodiwo_Plegma_ePortType Type;
	String Semantics;
	String State;
	uint32_t RevNum;
	Yodiwo_Plegma_ePortConf ConfFlags;
	uint64_t LastUpdatedTimestamp;
} Yodiwo_Plegma_Port_t;

/*
 * ConfigParameter
 */
typedef struct Array_Yodiwo_Plegma_ConfigParameter
{
	int num;
	struct Yodiwo_Plegma_ConfigParameter* elems;
} Array_Yodiwo_Plegma_ConfigParameter_t;

/*
 * Array port
 */
typedef struct Array_Yodiwo_Plegma_Port
{
	int num;
	struct Yodiwo_Plegma_Port* elems;
} Array_Yodiwo_Plegma_Port_t;

/*
 * ThingUIHints
 */
typedef struct Yodiwo_Plegma_ThingUIHints
{
	String IconURI;
	String Description;
} Yodiwo_Plegma_ThingUIHints_t;

/*
 * Thing
 */
typedef struct Yodiwo_Plegma_Thing
{
	String ThingKey;
	String Name;
	Array_Yodiwo_Plegma_ConfigParameter_t Config;
	Array_Yodiwo_Plegma_Port_t Ports;
	String Type;
	String BlockType;
	bool Removable;
	String RESTUri;
	Yodiwo_Plegma_ThingUIHints_t UIHints;
} Yodiwo_Plegma_Thing_t;

/*
 * ThingType
 */
typedef struct ThingTypeStruct
{

}ThingTypeStruct_t;

/*
 * nodeInfoRsp
 */
typedef struct nodeInfoRespStruct
{
	String Name;
	unsigned char Type;
	unsigned int Capabilities;
	ThingTypeStruct* ThingTypes;
	int ThingsRevNum;
	int SupportedApiRev;
	String* BlockLibraries;
	int SeqNo;
} nodeInfoRespStruct_t;

/*
 * nodeInfoReq
 */
typedef struct Yodiwo_Plegma_NodeInfoReq
{
	int SeqNo;
	int LatestApiRev;
	String AssignedEndpoint;
	int ThingsRevNum;
} Yodiwo_Plegma_NodeInfoReq_t;

/*
 * MqttMsg
 */
struct Yodiwo_Plegma_MqttMsg
{
	//  Yodiwo_Plegma_WrapperMsg_eMsgFlags Flags;
	int Flags;
	int32 SyncId;
	String Payload;
	int32 PayloadSize;
};

/*
 * Api msg
 */
struct Yodiwo_API_msg
{
	String topic;
	Yodiwo_Plegma_MqttMsg mqttMsg;
};

#endif // __YODIWO_API_DEFS_H__
