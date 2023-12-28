#include "open62541.h"
#include <stdio.h>
#include <string.h>
#include "SV_NewMethod.h"
#include "SV_NewMonitor.h"
#include <mariadb/mysql.h>
//#include "AG_NewPubSub.h"
#define MAX_BUFFER_SIZE 20000
#define MAX_STRING_SIZE 64

typedef struct {
char Tag[MAX_STRING_SIZE];
char Name[MAX_STRING_SIZE];
float Probability;
char CASnumber[MAX_STRING_SIZE];
int Concentration;
} AlarmStruct;

typedef struct {
char Tag[MAX_STRING_SIZE];
char Name[MAX_STRING_SIZE];
float Probability;
char CASnumber[MAX_STRING_SIZE];
int Concentration;
} NonAlarmStruct;


extern char g_argv_ip[255];
extern int g_argv_port;

extern char SoftwareVersion[MAX_STRING_SIZE];
extern char DataBlockVersion[MAX_STRING_SIZE];
extern char InstrumentTime[MAX_STRING_SIZE];
extern char MeasurementTime[MAX_STRING_SIZE];

extern char BootStatus[MAX_STRING_SIZE];
extern char SnapshotStatus[MAX_STRING_SIZE];
extern char SCPStatus[MAX_STRING_SIZE];
extern char SFTPStatus[MAX_STRING_SIZE];
extern char RunScriptStatus[MAX_STRING_SIZE];
extern char ArchiveStatus[MAX_STRING_SIZE];
extern char AncillarySensorStatus[MAX_STRING_SIZE];

extern char Sensor[MAX_STRING_SIZE];
extern UA_Int16 OperatingTime;
extern char WarningMessage[MAX_STRING_SIZE];

extern UA_Float IgramPP;
extern UA_Float IgramDC;
extern UA_Float LaserPP;
extern UA_Float LaserDC;
extern UA_Float SingleBeamAt900;
extern UA_Float SingleBeamAt2500;
extern UA_Int16 SignalToNoiseAt2500;
extern UA_Float CenterBurstLocation;
extern UA_Float DetectorTemp;
extern UA_Float LaserFrequency;
extern UA_Int16 HardDriveSpace;
extern UA_Int16 Flow;
extern UA_Int16 Temperature;
extern UA_Float Pressure;
extern UA_Int16 TempOptics;
extern UA_Int16 BadScanCounter;
extern UA_Int16 FreeMemorySpace;

extern char LABFilename[MAX_STRING_SIZE];
extern char LOGFilename[MAX_STRING_SIZE];
extern char LgFilename[MAX_STRING_SIZE];
extern char SecondLgFilename[MAX_STRING_SIZE];

extern UA_Float SystemCounter;
extern UA_Float DetectorCounter;
extern UA_Float LaserCounter;
extern UA_Float FlowPumpCounter;
extern UA_Float DesiccantCounter;

extern UA_Int16 NoOfAlarms;
extern UA_Int16 NoOfNonAlarms;

extern UA_Boolean UA_Nodes_Setup;

// FULL Namespace, for Events, historizing and Pubsub
extern UA_NodeId outSoftwareVersion_Id;
extern UA_NodeId outDataBlockVersion_Id;
extern UA_NodeId outInstrumentTime_Id;
extern UA_NodeId outMeasurementTime_Id;
extern UA_NodeId outSensor_Id;
extern UA_NodeId outOperatingTime_Id;
extern UA_NodeId outWarningMessage_Id;
extern UA_NodeId outBootStatus_Id;
extern UA_NodeId outSnapshotStatus_Id;
extern UA_NodeId outSCPStatus_Id;
extern UA_NodeId outSFTPStatus_Id;
extern UA_NodeId outRunScriptStatus_Id;
extern UA_NodeId outArchiveStatus_Id;
extern UA_NodeId outAncilarySensor_Id;

extern UA_NodeId outIgramPP_Id;
extern UA_NodeId outIgramDC_Id;
extern UA_NodeId outLaserPP_Id;
extern UA_NodeId outLaserDC_Id;
extern UA_NodeId outSingleBeamAt900_Id;
extern UA_NodeId outSingleBeamAt2500_Id;
extern UA_NodeId outSignalToNoiseAt2500_Id;
extern UA_NodeId outCenterBurstLocation_Id;
extern UA_NodeId outDetectorTemp_Id;
extern UA_NodeId outLaserFrequency_Id;
extern UA_NodeId outHardDriveSpace_Id;
extern UA_NodeId outFlow_Id;
extern UA_NodeId outTemperature_Id;
extern UA_NodeId outPressure_Id;
extern UA_NodeId outTempOptics_Id;
extern UA_NodeId outBadScanCounter_Id;
extern UA_NodeId outFreeMemorySpace_Id;
extern UA_NodeId outLABFilename_Id;
extern UA_NodeId outLOGFilename_Id;
extern UA_NodeId outLgFilename_Id;
extern UA_NodeId outSecondLgFilename_Id;
extern UA_NodeId outSystemCounter_Id;
extern UA_NodeId outDetectorCounter_Id;
extern UA_NodeId outLaserCounter_Id;
extern UA_NodeId outFlowPumpCounter_Id;
extern UA_NodeId outDesiccantCounter_Id;

extern MYSQL *conn;

// forward declaration - to remove after testing using MonitoredItems
/*
static void WriteIgramDC(UA_Server *uaServer,
                                const UA_NodeId *sessionId, void *sessionContext,
                                const UA_NodeId *nodeId, void *nodeContext,
                                const UA_NumericRange *range, const UA_DataValue*data);
*/

//UA_NodeId CreateOPCUANodes(void* x_void_ptr);

//void* CreateAndPopulateNodes(void* x_void_ptr)
UA_NodeId* CreateOPCUANodes(void* x_void_ptr)
{
/*
    // Airgard subtree (root node)
    char XMLSoftwareVersion[MAX_STRING_SIZE];
    char XMLDataBlockVersion[MAX_STRING_SIZE];

    // TimeStamp subtree
    char XMLInstrumentTime[MAX_STRING_SIZE];
    char XMLMeasurementTime[MAX_STRING_SIZE];

    // StatusInfo subtree
    char XMLBootStatus[MAX_STRING_SIZE];
    char XMLSnapshotStatus[MAX_STRING_SIZE];
    char XMLSCPStatus[MAX_STRING_SIZE];
    char XMLSFTPStatus[MAX_STRING_SIZE];
    char XMLRunScriptStatus[MAX_STRING_SIZE];
    char XMLArchiveStatus[MAX_STRING_SIZE];
    char XMLAnciliarySensorStatus;

    // Status subtree
    char XMLSensor[MAX_STRING_SIZE];
    int XMLOperatingTime = 0;
    char XMLWarningMessage[MAX_STRING_SIZE];

    // Diagnostics subtree
        //float XMLIgramPP = 0.0;
        //float XMLIgramDC = 0.0;
    //float XMLLaserPP = 0.0;
    //float XMLLaserDC = 0.0;

    float XMLSingleBeamAt900 = 0.0;
    float XMLSingleBeamAt2500 = 0.0;
    int XMLSignalToNoiseRatioAt2500 = 0;
    float XMLCentreBurstLocation = 0.0;
    float XMLDetectorTemp = 0.0;
    float XMLLaserFrequency = 0.0;
    int XMLHardDriveSpace = 0;
    int XMLFlow = 0;
    int XMLTemperature = 0;
    float XMLPressure = 0.0;
    float XMLTempOptics = 0.0;
    int XMLBadScanCounter = 0;
    int XMLFreeMemorySpace = 0;
    char XMLLABFilename[MAX_STRING_SIZE];
    char XMLLOGFilename[MAX_STRING_SIZE];
    char XMLLgFilename[MAX_STRING_SIZE];
    char XMLSecondLgFilename[MAX_STRING_SIZE];
    float XMLSystemCounter = 0.0;
    float XMLDetectorCounter = 0.0;
    float XMLLaserCounter = 0.0;
    float XMLFlowPumpCounter = 0.0;
    float XMLDesiccantCounter = 0.0;

    // Data subtree
    int XMLAlarms = 0;
    int XMLNonAlarms = 0;
*/
    int numbytes;
    char buffer[MAX_BUFFER_SIZE];
    struct hostent* he;
    struct sockaddr_in airgard_addr;        /* connector's address information */
    int counter = 0;
    UA_StatusCode retval;

    /*
    Namespace 0: is the Standard Server addressSpace given by the OPC UA Specifications.
    Spec Text: The Server exposes a Type System with DataTypes, ReferenceTypes, ObjectTypes and
    VariableTypes including all of the OPC Foundation provided (namespace 0) types that
    are used by the Server, as defined in Part 5. Items that are defined in Namespace 0 but are
    defined in other specification parts are tested as part of the other information models.

    Namespace 1: urn:unconfigured:Application can be configured in the ua_config_default.c file

    UA_ENABLE_FULL_NS0
    Use the full NS0 instead of a minimal Namespace 0 nodeset UA_FILE_NS0 is used to specify the file for NS0 generation from namespace0 folder. Default value is Opc.Ua.NodeSet2.xml
    */

    //TODO: get the namespaceIndex from StartOPCUAServer.c; use the value 2 first to testing

    //UA_UInt16 namespaceIndex=1;		// 0 is reserved for OPC
    size_t namespaceIndex;

        //if (( he=gethostbyname(g_argv[1])) == NULL) /* get the host info */
        //if (( he=gethostbyname("192.168.2.88")) == NULL)

    //printf("In NA_CreateOPCNodes thread : ip = %s, port = %d\n", g_argv_ip, g_argv_port);

    UA_Server* uaServer = (UA_Server*)x_void_ptr;
    //UA_Client* client = (UA_Client*)x_void_ptr;


    if (!UA_Nodes_Setup) // UA_Nodes only needs to be created the first time
    {
        UA_Nodes_Setup = UA_TRUE;

        /* Create a rudimentary objectType
         *
         * BaseObjectType
         * |
         * + (V) SoftwareVersion
         * + (V) DataBlockVersion
         * +-(OT) TimestampType
         * |   + (V) InstrumentTime
         * |   + (V) MeasurementTime
         * +-(OT) StatusType
         * |   + (V) Sensor
         * |   + (V) OperatingTime
         * |   + (V) WarningMessage
         * |   +(OT) InfoType
         * |   |--+ (V) BootStatus
         * |   |  + (V) SnapshotStatus
	 * |   |  + (V) SCPStatus
         * |   |  + (V) SFTPStatus
         * |   |  + (V) RunscriptStatus
         * |   |  + (V) ArchiveStatus
         * |   |  + (V) AncillarySensorStatus
         * |-(OT) DiagnosticsType
         * |   + (V) IgramPP
         * |   + (V) ...
         * |   + (V) DesiccantCounter
         * |-(OT) DataType (10400)
         * |   + (V) Alarms (20000)
         * |   + (OT) Alarm
         * |   |---+ (V) Tag
         * |   |   + (V) Name
         * |   |   + (V) Probability
         * |   |   + (V) CASnumber
         * |   |   + (V) Concentration
         * |   + (OT) Alarm
         * |   | <repeat>
         * |   + (V) NonAlarms (30000)
        */

        printf("======================In UA Nodes setup======================= \n");

        //reference to namespace of the server : initialised in SV_StartOPCUAServer.c
        //UA_Int16 ns_MKS = UA_Server_addNamespace(uaServer, "virtualskies.com.sg/MKS/");
	UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);
        //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "New Namespace added with Nr. %d \n", ns_MKS);

        // Add new object called Airgard Sensor
        UA_NodeId r1_airgardroot_Id;  /* get the nodeid assigned by the server */
	//UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "---UA_NodeId = %d\n", r1_airgardroot_Id);

        UA_ObjectAttributes oAttr = UA_ObjectAttributes_default;
        retval = UA_Server_addObjectNode(uaServer, UA_NODEID_STRING(namespaceIndex, "AirGardSensor"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),		// parentNodeId
            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),		// referenceTypeId original UA_NS0ID_ORGANIZES
            UA_QUALIFIEDNAME(namespaceIndex, "Airgard"),		// browseName UA_QUALIFIEDNAME(1, "AirGard"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),		// typeDefinition this refers to the Object Type identifier
            oAttr, NULL, &r1_airgardroot_Id);				// attr, nodeContext, outNewNodeId assigned by the server
	if (retval != UA_STATUSCODE_GOOD)
           UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating object node : Root Node AirGardSensor : %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Root Node created : Airgard", r1_airgardroot_Id);		// OK

    //----------------------------------------------------- Subtree: Airgard->Software Version (1,10000)

        // variable
	UA_NodeId SoftwareVersionVariableType = UA_NODEID_NULL;
        UA_VariableAttributes vSoftwareVersionAttr = UA_VariableAttributes_default;
        vSoftwareVersionAttr.description = UA_LOCALIZEDTEXT("en-US", "Software Version Info");
        vSoftwareVersionAttr.displayName = UA_LOCALIZEDTEXT("en-US", "1. Software Version");
	vSoftwareVersionAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vSoftwareVersionAttr.valueRank = UA_VALUERANK_SCALAR;
        vSoftwareVersionAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vSoftwareVersionAttr.historizing = UA_FALSE;
        char SoftwareVersion[MAX_STRING_SIZE] = "open62541";
	//if (!UA_Variant_isEmpty(&vSoftwareVersionAttr.value))
	//        UA_Variant_setScalar(&vSoftwareVersionAttr.value, &SoftwareVersion, &UA_TYPES[UA_TYPES_STRING]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10001),		// NodeId
       	    r1_airgardroot_Id,				// parent NodeId
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),	// parent Reference NodeId
       	    UA_QUALIFIEDNAME(namespaceIndex, "Software Version"),	//UA_QUALIFIEDNAME(1, "Software Version"),	// Qualified Name
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),		//https://github.com/open62541/open62541(Example Server Implementation); UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
      	    vSoftwareVersionAttr, NULL, &outSoftwareVersion_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : Software Version 10001: %s", UA_StatusCode_name(retval));
	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created: Airgard->VersionType->SoftwareVersion 10001");	//, r1_airgardroot_Id);

	//=======================================================
	// add a timestamp - declaration
	//UA_DateTime currentTime;
	//UA_WriteValue wv;
	//UA_WriteValue_init(&wv);
	// add a timestamp - instantiation
	//currentTime = UA_DateTime_now();
	//wv.nodeId = outSoftwareVersion_Id;
	//wv.attributeId = UA_ATTRIBUTEID_VALUE;
	//wv.value.hasStatus = UA_FALSE;
	//wv.value.value = vSoftwareVersionAttr.value;
	//wv.value.hasValue = UA_TRUE;
	//wv.value.hasSourceTimestamp = UA_TRUE;
	//wv.value.sourceTimestamp = currentTime;
	//UA_Server_writeDataValue(uaServer, outSoftwareVersion_Id, wv.value);
	//=======================================================

        // variable
	UA_NodeId DataBlockVersionVariableType = UA_NODEID_NULL;
        UA_VariableAttributes vDataBlockVersionAttr = UA_VariableAttributes_default;
        vDataBlockVersionAttr.description = UA_LOCALIZEDTEXT("en-US", "Data Block Version Info");
        vDataBlockVersionAttr.displayName = UA_LOCALIZEDTEXT("en-US", "2. Data Block Version");
	vDataBlockVersionAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vDataBlockVersionAttr.valueRank = UA_VALUERANK_SCALAR;
        vDataBlockVersionAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vDataBlockVersionAttr.historizing = UA_FALSE;
        char DataBlockVersion[MAX_STRING_SIZE];
	//if (!UA_Variant_isEmpty(&vDataBlockVersionAttr.value))
	//        UA_Variant_setScalar(&vDataBlockVersionAttr.value, &DataBlockVersion, &UA_TYPES[UA_TYPES_STRING]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10002), r1_airgardroot_Id,
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "2. Data Block Version"),	//UA_QUALIFIEDNAME(1, "2. Data Block Version"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),	//https://github.com/open62541/open62541(Example Server Implementation);UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vDataBlockVersionAttr, NULL, &outDataBlockVersion_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : DataBlockVersion 10002: %s", UA_StatusCode_name(retval));
       	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created: Airgard->VersionType->DataBlockVersion 10002");	//, r1_airgardroot_Id);

        //***// Subtree: Airgard->Timestamp (1,10100)

            // Add objectnode to represent Airgard->Timestamp
        UA_NodeId r2_airgard_timestamp_Id;  /* get the nodeid assigned by the server */
        UA_ObjectAttributes oAttr_r2_timestamp = UA_ObjectAttributes_default;
        retval = UA_Server_addObjectNode(uaServer, UA_NODEID_STRING(namespaceIndex, "AirGardSensor_TimeStamp"),
            r1_airgardroot_Id,			// Make Airgard node <Contains> Timestamp node		//UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
            UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
            UA_QUALIFIEDNAME(namespaceIndex, "Timestamp"),	//UA_QUALIFIEDNAME(1, "Timestamp"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
            oAttr_r2_timestamp, NULL, &r2_airgard_timestamp_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating object node : AirGardSensor_TimeStamp : %s", UA_StatusCode_name(retval));
        else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Object Node created : Airgard->Timestamp Node");	//, r1_airgardroot_Id);

        // variable
        UA_VariableAttributes vInstrumentTimeAttr = UA_VariableAttributes_default;
        vInstrumentTimeAttr.description = UA_LOCALIZEDTEXT("en-US", "Instrument Time Info");
        vInstrumentTimeAttr.displayName = UA_LOCALIZEDTEXT("en-US", "1. Instrument Time");
	vInstrumentTimeAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vInstrumentTimeAttr.valueRank = UA_VALUERANK_SCALAR;
        vInstrumentTimeAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vInstrumentTimeAttr.historizing = UA_FALSE;
        char InstrumentTime[MAX_STRING_SIZE];
	//if (!UA_Variant_isEmpty(&vInstrumentTimeAttr.value))
	//        UA_Variant_setScalar(&vInstrumentTimeAttr.value, &InstrumentTime, &UA_TYPES[UA_TYPES_STRING]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10101),
       	    r2_airgard_timestamp_Id,
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Instrument Time"), 	//UA_QUALIFIEDNAME(1, "Instrument Time"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vInstrumentTimeAttr, NULL, &outInstrumentTime_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : Instrument Time 10101: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created: Airgard->TimestampType->Instrument Time 10101");	//, r2_airgard_timestamp_Id);

        // variable
        UA_VariableAttributes vMeasurementTimeAttr = UA_VariableAttributes_default;
        vMeasurementTimeAttr.description = UA_LOCALIZEDTEXT("en-US", "Measurement Time Info");
        vMeasurementTimeAttr.displayName = UA_LOCALIZEDTEXT("en-US", "2. Measurement Time");
	vMeasurementTimeAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vMeasurementTimeAttr.valueRank = UA_VALUERANK_SCALAR;
        vMeasurementTimeAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vMeasurementTimeAttr.historizing = UA_FALSE;
        char MeasurementTime[MAX_STRING_SIZE];
	//if (!UA_Variant_isEmpty(&vMeasurementTimeAttr.value))
	//        UA_Variant_setScalar(&vMeasurementTimeAttr.value, &MeasurementTime, &UA_TYPES[UA_TYPES_STRING]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10102),
       	    r2_airgard_timestamp_Id,
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Measurement Time"), 	//UA_QUALIFIEDNAME(1, "Measurement Time"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vMeasurementTimeAttr, NULL, &outMeasurementTime_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
              	"Error creating node : Measurement Time 10102: %S", UA_StatusCode_name(retval));
       	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created: Airgard->TimestampType->Measurement Time 10102");	//, r2_airgard_timestamp_Id);

        //***// Subtree: Airgard->Status (1, 10200)

            // Add objectnode to represent Airgard->Status
        UA_NodeId r2_airgard_status_Id;
        UA_ObjectAttributes oAttr_r2_status = UA_ObjectAttributes_default;
        retval = UA_Server_addObjectNode(uaServer, UA_NODEID_STRING(namespaceIndex, "AirGardSensor_Status"),
            r1_airgardroot_Id,
            UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
            UA_QUALIFIEDNAME(namespaceIndex, "Status"), 		//UA_QUALIFIEDNAME(1, "Status"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
            oAttr_r2_status, NULL, &r2_airgard_status_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating object node : AirGardSensor_Sensor : %s", UA_StatusCode_name(retval));
        else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Object Node created : Airgard->Status");	//, r2_airgard_status_Id);

        // variable
        UA_VariableAttributes vSensorAttr = UA_VariableAttributes_default;
        vSensorAttr.description = UA_LOCALIZEDTEXT("en-US", "Sensor Info");
        vSensorAttr.displayName = UA_LOCALIZEDTEXT("en-US", "1. Sensor");
	vSensorAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vSensorAttr.valueRank = UA_VALUERANK_SCALAR;
        vSensorAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vSensorAttr.historizing = UA_FALSE;
        char Sensor[MAX_STRING_SIZE];
	//if (!UA_Variant_isEmpty(&vSensorAttr.value))
	//        UA_Variant_setScalar(&vSensorAttr.value, &Sensor, &UA_TYPES[UA_TYPES_STRING]);
      	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10201),
       	    r2_airgard_status_Id,
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Sensor"),			//UA_QUALIFIEDNAME(1, "Sensor"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vSensorAttr, NULL, &outSensor_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : Sensor 10201: %s", UA_StatusCode_name(retval));
	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created: Airgard->StatusType->Sensor 10201");  //, r2_airgard_timestamp_Id);

        // variable
        UA_VariableAttributes vOperatingTimeAttr = UA_VariableAttributes_default;
        vOperatingTimeAttr.description = UA_LOCALIZEDTEXT("en-US", "Operating Time Info");
        vOperatingTimeAttr.displayName = UA_LOCALIZEDTEXT("en-US", "2. Operating Time");
	vOperatingTimeAttr.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
	vOperatingTimeAttr.valueRank = UA_VALUERANK_SCALAR;
        vOperatingTimeAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vOperatingTimeAttr.historizing = UA_FALSE;
        UA_Int16 OperatingTime;
	//if (!UA_Variant_isEmpty(&vOperatingTimeAttr.value))
 	//	UA_Variant_setScalar(&vOperatingTimeAttr.value, &OperatingTime, &UA_TYPES[UA_TYPES_INT16]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10202),
       	    r2_airgard_status_Id,
  	    UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Operating Time"),		//UA_QUALIFIEDNAME(1, "Operating Time"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vOperatingTimeAttr, NULL, &outOperatingTime_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : OperatingTime 10202: %s", UA_StatusCode_name(retval));
	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created : Airgard->StatusType->OperatingTime 10202");		//, r2_airgard_status_Id);

        // variable
        UA_VariableAttributes vWarningMessageAttr = UA_VariableAttributes_default;
        vWarningMessageAttr.description = UA_LOCALIZEDTEXT("en-US", "WarningMessageInfo");
        vWarningMessageAttr.displayName = UA_LOCALIZEDTEXT("en-US", "3. Warning Message");
	vWarningMessageAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vWarningMessageAttr.valueRank = UA_VALUERANK_SCALAR;
        vWarningMessageAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vWarningMessageAttr.historizing = UA_FALSE;
        char WarningMessage[MAX_STRING_SIZE];
	//if (!UA_Variant_isEmpty(&vWarningMessageAttr.value))
	//        UA_Variant_setScalar(&vWarningMessageAttr.value, &WarningMessage, &UA_TYPES[UA_TYPES_STRING]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10203),
       	    r2_airgard_status_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Warning Message"),	//UA_QUALIFIEDNAME(1, "Warning Message"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vWarningMessageAttr, NULL, &outWarningMessage_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : Warning Message 10203: %s", UA_StatusCode_name(retval));
	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created : Airgard->StatusType->Warning Message 10203");         //, r2_airgard_status_Id);

        // Add objectnode to represent Airgard->Status->Info
        UA_NodeId r3_airgard_status_Info_Id;			// line 1345
        UA_ObjectAttributes oAttr_r3_status_Info = UA_ObjectAttributes_default;
        oAttr_r3_status_Info.description = UA_LOCALIZEDTEXT("en-US", "Info");
        oAttr_r3_status_Info.displayName = UA_LOCALIZEDTEXT("en-US", "Info");
        retval = UA_Server_addObjectNode(uaServer, UA_NODEID_STRING(namespaceIndex, "AirGardSensor_Status_Info"),
            r2_airgard_status_Id,
            UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
            UA_QUALIFIEDNAME(namespaceIndex, "Info"),			//UA_QUALIFIEDNAME(1, "Info"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
            oAttr_r3_status_Info, NULL, &r3_airgard_status_Info_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating object node : AirGardSensor_Status_Info : %s", UA_StatusCode_name(retval));
        else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Object Node created : Airgard->Status->Info", r3_airgard_status_Info_Id);

        UA_VariableAttributes vBootStatusAttr = UA_VariableAttributes_default;
        vBootStatusAttr.description = UA_LOCALIZEDTEXT("en-US", "BootStatusInfo");
        vBootStatusAttr.displayName = UA_LOCALIZEDTEXT("en-US", "1. Boot Status");
	vBootStatusAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vBootStatusAttr.valueRank = UA_VALUERANK_SCALAR;
        vBootStatusAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vBootStatusAttr.historizing = UA_FALSE;
        char BootStatus[MAX_STRING_SIZE];
	//if (!UA_Variant_isEmpty(&vBootStatusAttr.value))
	//        UA_Variant_setScalar(&vBootStatusAttr.value, &BootStatus, &UA_TYPES[UA_TYPES_STRING]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10211),
   	    r3_airgard_status_Info_Id,
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Boot Status"),		//UA_QUALIFIEDNAME(2, "Boot Status"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vBootStatusAttr, NULL, &outBootStatus_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : Boot Status 10211 : %s", UA_StatusCode_name(retval));
       	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created : Airgard->Status->Info->Boot Status 10211");         //, r2_airgard_status_Id);

        UA_VariableAttributes vSnapshotStatusAttr = UA_VariableAttributes_default;
        vSnapshotStatusAttr.description = UA_LOCALIZEDTEXT("en-US", "SnapshotStatusInfo");
        vSnapshotStatusAttr.displayName = UA_LOCALIZEDTEXT("en-US", "2. Snapshot Status");
	vSnapshotStatusAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vSnapshotStatusAttr.valueRank = UA_VALUERANK_SCALAR;
        vSnapshotStatusAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vSnapshotStatusAttr.historizing = UA_FALSE;
        char SnapshotStatus[MAX_STRING_SIZE];
	//if (!UA_Variant_isEmpty(&vSnapshotStatusAttr.value))
	//        UA_Variant_setScalar(&vSnapshotStatusAttr.value, &SnapshotStatus, &UA_TYPES[UA_TYPES_STRING]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10212),
       	    r3_airgard_status_Info_Id,
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Snapshot Status"),	//UA_QUALIFIEDNAME(2, "Snapshot Status"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vSnapshotStatusAttr, NULL, &outSnapshotStatus_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : Snapshot Status 10212: %s", UA_StatusCode_name(retval));
	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created : Airgard->Status->Info->Snapshot Status 10212");         //, r2_airgard_status_Id);

        UA_VariableAttributes vSCPStatusAttr = UA_VariableAttributes_default;
        vSCPStatusAttr.description = UA_LOCALIZEDTEXT("en-US", "SCPStatusInfo");
        vSCPStatusAttr.displayName = UA_LOCALIZEDTEXT("en-US", "3. SCP Status");
	vSCPStatusAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vSCPStatusAttr.valueRank = UA_VALUERANK_SCALAR;
        vSCPStatusAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vSCPStatusAttr.historizing = UA_FALSE;
        char SCPStatus[MAX_STRING_SIZE];
	//if (!UA_Variant_isEmpty(&vSCPStatusAttr.value))
        //	UA_Variant_setScalar(&vSCPStatusAttr.value, &SCPStatus, &UA_TYPES[UA_TYPES_STRING]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10213),
       	    r3_airgard_status_Info_Id,
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "SCP Status"),		//UA_QUALIFIEDNAME(2, "SCP Status"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vSCPStatusAttr, NULL, &outSCPStatus_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : SCP Status 10213: %s", UA_StatusCode_name(retval));
	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created : Airgard->Status->Info->SCP Status 10213");         //, r2_airgard_status_Id);

        UA_VariableAttributes vSFTPStatusAttr = UA_VariableAttributes_default;
        vSFTPStatusAttr.description = UA_LOCALIZEDTEXT("en-US", "SFTPStatusInfo");
        vSFTPStatusAttr.displayName = UA_LOCALIZEDTEXT("en-US", "4. SFTP Status");
	vSFTPStatusAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vSFTPStatusAttr.valueRank = UA_VALUERANK_SCALAR;
        vSFTPStatusAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vSFTPStatusAttr.historizing = UA_FALSE;
        char SFTPStatus[MAX_STRING_SIZE];
	//if (!UA_Variant_isEmpty(&vSFTPStatusAttr.value))
        //	UA_Variant_setScalar(&vSFTPStatusAttr.value, &SFTPStatus, &UA_TYPES[UA_TYPES_STRING]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10214),
       	    r3_airgard_status_Info_Id,
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "SFTP Status"),		//UA_QUALIFIEDNAME(2, "SFTP Status"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vSFTPStatusAttr, NULL, &outSFTPStatus_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : SFTP Status 10214: %s", UA_StatusCode_name(retval));
	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created : Airgard->Status->Info->SFTP Status 10214");         //, r2_airgard_status_Id);

        UA_VariableAttributes vRunScriptStatusAttr = UA_VariableAttributes_default;
        vRunScriptStatusAttr.description = UA_LOCALIZEDTEXT("en-US", "RunScriptStatusInfo");
        vRunScriptStatusAttr.displayName = UA_LOCALIZEDTEXT("en-US", "5. Run Script Status");
	vRunScriptStatusAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vRunScriptStatusAttr.valueRank = UA_VALUERANK_SCALAR;
        vRunScriptStatusAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vRunScriptStatusAttr.historizing = UA_FALSE;
        char RunScriptStatus[MAX_STRING_SIZE];
	//if (!UA_Variant_isEmpty(&vRunScriptStatusAttr.value))
        //	UA_Variant_setScalar(&vRunScriptStatusAttr.value, &RunScriptStatus, &UA_TYPES[UA_TYPES_STRING]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10215),
	    r3_airgard_status_Info_Id,
 	    UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Run Script Status"),	//UA_QUALIFIEDNAME(2, "Run Script Status"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vRunScriptStatusAttr, NULL, &outRunScriptStatus_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : Run Script Status 10215: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created : Airgard->Status->Info->Run Script Status 10215");         //, r2_airgard_status_Id);

        UA_VariableAttributes vArchiveStatusAttr = UA_VariableAttributes_default;
        vArchiveStatusAttr.description = UA_LOCALIZEDTEXT("en-US", "ArchiveStatusInfo");
        vArchiveStatusAttr.displayName = UA_LOCALIZEDTEXT("en-US", "6. Archive Status");
	vArchiveStatusAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vArchiveStatusAttr.valueRank = UA_VALUERANK_SCALAR;
        vArchiveStatusAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vArchiveStatusAttr.historizing = UA_FALSE;
        char ArchiveStatus[MAX_STRING_SIZE];
	//if (!UA_Variant_isEmpty(&vArchiveStatusAttr.value))
        //	UA_Variant_setScalar(&vArchiveStatusAttr.value, &ArchiveStatus, &UA_TYPES[UA_TYPES_STRING]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10216),
       	    r3_airgard_status_Info_Id,
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Archive Status"),		//UA_QUALIFIEDNAME(2, "Archive Status"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vArchiveStatusAttr, NULL, &outArchiveStatus_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : Archive Status 10216: %s", UA_StatusCode_name(retval));
	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created : Airgard->Status->Info->Archive Status 10216");         //, r2_airgard_status_Id);

        UA_VariableAttributes vAncillarySensorStatusAttr = UA_VariableAttributes_default;
        vAncillarySensorStatusAttr.description = UA_LOCALIZEDTEXT("en-US", "AncillarySensortatusInfo");
        vAncillarySensorStatusAttr.displayName = UA_LOCALIZEDTEXT("en-US", "7. Ancillary Sensor Status");
	vAncillarySensorStatusAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vAncillarySensorStatusAttr.valueRank = UA_VALUERANK_SCALAR;
        vAncillarySensorStatusAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vAncillarySensorStatusAttr.historizing = UA_FALSE;
        char AncillarySensorStatus[MAX_STRING_SIZE];
	//if (!UA_Variant_isEmpty(&vAncillarySensorStatusAttr.value))
        //	UA_Variant_setScalar(&vAncillarySensorStatusAttr.value, &AncillarySensorStatus, &UA_TYPES[UA_TYPES_STRING]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10217),
       	    r3_airgard_status_Info_Id,
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Ancillary Sensor Status"),	//UA_QUALIFIEDNAME(2, "Ancillary Sensor Status"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vAncillarySensorStatusAttr, NULL, &outAncilarySensor_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : Ancillary Sensor Status 10217: %s", UA_StatusCode_name(retval));
	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created : Airgard->Status->Info->Ancillary Sensor Status 10217");         //, r2_airgard_status_Id);

        //***// Subtree: Airgard->Diagnostics (1,10300)

            // Add objectnode to represent Airgard->Diagnostics
        UA_NodeId r2_airgard_diagnostics_Id;
        UA_ObjectAttributes oAttr_r2_diagnostics = UA_ObjectAttributes_default;
        retval = UA_Server_addObjectNode(uaServer, UA_NODEID_STRING(namespaceIndex, "AirgardSensor_Diagnostics"),
            r1_airgardroot_Id,
            UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
            UA_QUALIFIEDNAME(namespaceIndex, "Diagnostic"),			//UA_QUALIFIEDNAME(1, "Diagnostic"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
            oAttr_r2_diagnostics, NULL, &r2_airgard_diagnostics_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating object node : AirGardSensor_Diagnostics : %s", UA_StatusCode_name(retval));
        else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
        	"Object Node created : Airgard->Diagnostics Node", r2_airgard_diagnostics_Id);

        UA_VariableAttributes vIgramPPAttr = UA_VariableAttributes_default;
        vIgramPPAttr.description = UA_LOCALIZEDTEXT("en-US", "IgramPPInfo");
        vIgramPPAttr.displayName = UA_LOCALIZEDTEXT("en-US", "01. Igram PP");
	vIgramPPAttr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vIgramPPAttr.valueRank = UA_VALUERANK_SCALAR;
        vIgramPPAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vIgramPPAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
        vIgramPPAttr.historizing = UA_TRUE;
	//if (!UA_Variant_isEmpty(&vIgramPPAttr.value))
	//        UA_Variant_setScalar(&vIgramPPAttr.value, &IgramPP, &UA_TYPES[UA_TYPES_FLOAT]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10301),	// 1 refers to namespace : NS1
       	    r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Igram PP"),		//UA_QUALIFIEDNAME(1, "Igram PP"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vIgramPPAttr, NULL, &outIgramPP_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : IgramPP 10301: %s", UA_StatusCode_name(retval));
	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created : Airgard->Diagnostics->IgramPP 10301");         //, r2_airgard_status_Id);

	#ifdef NOT_IN_OPERATION
	// add a variable callback
	addValueCallbackToIgramPPVariable(uaServer);

	UA_VariableAttributes cIgramPPAttr = UA_VariableAttributes_default;
	cIgramPPAttr.displayName = UA_LOCALIZEDTEXT("en-US", "Data Source-IgramPP");
	cIgramPPAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	UA_Node_Id IgramPPCurrentNodeId = UA_NODEID_STRING(1, "IgramPP-datasource");
	UA_QualifiedNme IgramPPcurrentName = UA_QUALIFIEDNAME(1, "IgramPP-datasource");
	UA_NodeId IgramPPparentNodeId = UA_NODE_ID_NUMERIC(0, UA_NUMERIC, UA_NS0ID_OBJECTSFOLDER);
	UA_NodeId IgramPPparentReferenceNodeId = UA_NODE_ID_NUMERIC(0, UA_NS0ID_ORGANIZES);
	UA_NodeId IgramPPvariableTypeNodeId = UA_Node_ID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);
	UA_DataSource IgramPPDataSource;
	IgramPPDataSource.write = afterWriteIgramPP;
	UA_Server_addDataSourceVariableNode(uaServer, IgramPPcurrentNodeId, IgramPPparentNodeId,
					IgramPPparentReferenceNodeId, IgramPPcurrentName,
					IgramPPvariableTypeNodeId, cIgramPPAttr,
					IgramPPDataSource, NULL, NULL);

        //vIgramPPAttr, NULL, &ds1IgramPPId);
        //UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
        //    "Attribute Variable created: Airgard->Diagnostics->IgramPP", r2_airgard_diagnostics_Id);
	#endif

        // variable
        //UA_NodeId_init(&ds1IgramPPId); // for pub sub
        UA_VariableAttributes vIgramDCAttr = UA_VariableAttributes_default;
        vIgramDCAttr.description = UA_LOCALIZEDTEXT("en-US", "IgramDCInfo");
        vIgramDCAttr.displayName = UA_LOCALIZEDTEXT("en-US", "02. Igram DC");
	vIgramDCAttr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vIgramDCAttr.valueRank = UA_VALUERANK_SCALAR;
        vIgramDCAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vIgramDCAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vIgramDCAttr.historizing = UA_TRUE;
	//if (!UA_Variant_isEmpty(&vIgramDCAttr.value))
	//        UA_Variant_setScalar(&vIgramDCAttr.value, &IgramDC, &UA_TYPES[UA_TYPES_FLOAT]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10302),
       	    r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Igram DC"),		//UA_QUALIFIEDNAME(1, "Igram DC"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vIgramDCAttr, NULL, &outIgramDC_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : IgramDC 10302: %s", UA_StatusCode_name(retval));
       	else
    	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created : Airgard->Diagnostics->IgramDC 10302");         //, r2_airgard_status_Id);

        // variable
        UA_VariableAttributes vLaserPPAttr = UA_VariableAttributes_default;
        vLaserPPAttr.description = UA_LOCALIZEDTEXT("en-US", "LaserPPInfo");
        vLaserPPAttr.displayName = UA_LOCALIZEDTEXT("en-US", "03. Laser PP");
	vLaserPPAttr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vLaserPPAttr.valueRank = UA_VALUERANK_SCALAR;
        vLaserPPAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vLaserPPAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vLaserPPAttr.historizing = UA_TRUE;
	//if (!UA_Variant_isEmpty(&vLaserPPAttr.value))
	//        UA_Variant_setScalar(&vLaserPPAttr.value, &LaserPP, &UA_TYPES[UA_TYPES_FLOAT]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10303),
       	    r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Laser PP"), 		//UA_QUALIFIEDNAME(1, "Laser PP"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vLaserPPAttr, NULL, &outLaserPP_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : LaserPP 10303: %s", UA_StatusCode_name(retval));
	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created: Airgard->Diagnostics->LaserPP 10303", r2_airgard_diagnostics_Id);

        // variable
        UA_VariableAttributes vLaserDCAttr = UA_VariableAttributes_default;
        vLaserDCAttr.description = UA_LOCALIZEDTEXT("en-US", "LaserDCInfo");
        vLaserDCAttr.displayName = UA_LOCALIZEDTEXT("en-US", "04. Laser DC");
	vLaserDCAttr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vLaserDCAttr.valueRank = UA_VALUERANK_SCALAR;
        vLaserDCAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vLaserDCAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vLaserDCAttr.historizing = UA_TRUE;
	//if (!UA_Variant_isEmpty(&vLaserDCAttr.value))
	//        UA_Variant_setScalar(&vLaserDCAttr.value, &LaserDC, &UA_TYPES[UA_TYPES_FLOAT]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10304),
       	    r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Laser DC"), 		//UA_QUALIFIEDNAME(1, "Laser DC"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vLaserDCAttr, NULL, &outLaserDC_Id);
       	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Error creating node : LaserDC 10304: %s", UA_StatusCode_name(retval));
	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created: Airgard->Diagnostics->LaserDC 10304", r2_airgard_diagnostics_Id);

        // variable
        UA_VariableAttributes vSingleBeamAt900Attr = UA_VariableAttributes_default;
        vSingleBeamAt900Attr.description = UA_LOCALIZEDTEXT("en-US", "SingleBeamAt900");
        vSingleBeamAt900Attr.displayName = UA_LOCALIZEDTEXT("en-US", "05. Single Beam At 900");
	vSingleBeamAt900Attr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vSingleBeamAt900Attr.valueRank = UA_VALUERANK_SCALAR;
        vSingleBeamAt900Attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vSingleBeamAt900Attr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vSingleBeamAt900Attr.historizing = UA_TRUE;
	//if (!UA_Variant_isEmpty(&vSingleBeamAt900Attr.value))
	//        UA_Variant_setScalar(&vSingleBeamAt900Attr.value, &SingleBeamAt900, &UA_TYPES[UA_TYPES_FLOAT]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10305),
     	    r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Single Beam At 900"), 	//UA_QUALIFIEDNAME(1, "Single Beam At 900"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
       	    vSingleBeamAt900Attr, NULL, &outSingleBeamAt900_Id);
      	if (retval != UA_STATUSCODE_GOOD)
       	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
              	"Error creating node : SingleBeamAt900 10305: %s", UA_StatusCode_name(retval));
	else
       	    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
               	"Attribute Variable created: Airgard->Diagnostics->SingleBeamAt900 10305", r2_airgard_diagnostics_Id);

        // variable
        UA_VariableAttributes vSingleBeamAt2500Attr = UA_VariableAttributes_default;
        vSingleBeamAt2500Attr.description = UA_LOCALIZEDTEXT("en-US", "SingleBeamAt2500");
        vSingleBeamAt2500Attr.displayName = UA_LOCALIZEDTEXT("en-US", "06. Single Beam At 2500");
	vSingleBeamAt2500Attr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vSingleBeamAt2500Attr.valueRank = UA_VALUERANK_SCALAR;
        vSingleBeamAt2500Attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vSingleBeamAt2500Attr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vSingleBeamAt2500Attr.historizing = UA_TRUE;
	//if (!UA_Variant_isEmpty(&vSingleBeamAt2500Attr.value))
	//        UA_Variant_setScalar(&vSingleBeamAt2500Attr.value, &SingleBeamAt2500, &UA_TYPES[UA_TYPES_FLOAT]);
       	retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10306),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
       	    UA_QUALIFIEDNAME(namespaceIndex, "Single Beam At 2500"), 	//UA_QUALIFIEDNAME(1, "Single Beam At 2500"),
       	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
	    vSingleBeamAt2500Attr, NULL, &outSingleBeamAt2500_Id);
     	if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
              	"Error creating node : SingleBeamAt2500 10306: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
              	"Attribute Variable created: Airgard->Diagnostics->SingleBeamAt2500 10306", r2_airgard_diagnostics_Id);

        // variable
        UA_VariableAttributes vSignalToNoiseAt2500Attr = UA_VariableAttributes_default;
        vSignalToNoiseAt2500Attr.description = UA_LOCALIZEDTEXT("en-US", "SignalToNoiseAt2500");
        vSignalToNoiseAt2500Attr.displayName = UA_LOCALIZEDTEXT("en-US", "07. Signal To Noise At 2500");
	vSignalToNoiseAt2500Attr.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
	vSignalToNoiseAt2500Attr.valueRank = UA_VALUERANK_SCALAR;
        vSignalToNoiseAt2500Attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vSignalToNoiseAt2500Attr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vSignalToNoiseAt2500Attr.historizing = UA_TRUE;
	//if (!UA_Variant_isEmpty(&vSignalToNoiseAt2500Attr.value))
	//        UA_Variant_setScalar(&vSignalToNoiseAt2500Attr.value, &SignalToNoiseAt2500, &UA_TYPES[UA_TYPES_INT16]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10307),
       	    r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Signal To Noise at 2500"), 	//UA_QUALIFIEDNAME(1, "Signal To Noise at 2500"),
 	    UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vSignalToNoiseAt2500Attr, NULL, &outSignalToNoiseAt2500_Id);
	if (retval != UA_STATUSCODE_GOOD)
	    UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
            	"Error creating node : SignalToNoiseAt2500 10307: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->SignalToNoiseAt2500 10307", r2_airgard_diagnostics_Id);

        // variable
        UA_VariableAttributes vCenterBurstLocationAttr = UA_VariableAttributes_default;
        vCenterBurstLocationAttr.description = UA_LOCALIZEDTEXT("en-US", "CenterBurstLocation");
        vCenterBurstLocationAttr.displayName = UA_LOCALIZEDTEXT("en-US", "08. Center Burst Location");
	vCenterBurstLocationAttr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vCenterBurstLocationAttr.valueRank = UA_VALUERANK_SCALAR;
        vCenterBurstLocationAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vCenterBurstLocationAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vCenterBurstLocationAttr.historizing = UA_TRUE;
	//if (!UA_Variant_isEmpty(&vCenterBurstLocationAttr.value))
	//        UA_Variant_setScalar(&vCenterBurstLocationAttr.value, &CenterBurstLocation, &UA_TYPES[UA_TYPES_FLOAT]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10308),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Center Burst Location"), 		//UA_QUALIFIEDNAME(1, "Center Burst Location"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vCenterBurstLocationAttr, NULL, &outCenterBurstLocation_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : CenterBurstLocation 10308: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->CenterBurstLocation 10308", r2_airgard_diagnostics_Id);

        // variable
        UA_VariableAttributes vDetectorTempAttr = UA_VariableAttributes_default;
        vDetectorTempAttr.description = UA_LOCALIZEDTEXT("en-US", "DetectorTemperature");
        vDetectorTempAttr.displayName = UA_LOCALIZEDTEXT("en-US", "09. Detector Temperature");
	vDetectorTempAttr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vDetectorTempAttr.valueRank = UA_VALUERANK_SCALAR;
        vDetectorTempAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vDetectorTempAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vDetectorTempAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vDetectorTempAttr.value))
	//        UA_Variant_setScalar(&vDetectorTempAttr.value, &DetectorTemp, &UA_TYPES[UA_TYPES_FLOAT]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10309),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Detector Temperature"), 		//UA_QUALIFIEDNAME(1, "Detector Temperature"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vDetectorTempAttr, NULL, &outDetectorTemp_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : DetectorTemp 10309: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->DetectorTemp 10309", r2_airgard_diagnostics_Id);

        // Add the variable LaserFrequencyValue to server
        UA_VariableAttributes vLaserFrequencyAttr = UA_VariableAttributes_default;
        vLaserFrequencyAttr.description = UA_LOCALIZEDTEXT("en-US", "LaserFrequency");
        vLaserFrequencyAttr.displayName = UA_LOCALIZEDTEXT("en-US", "10. Laser Frequency");
	vLaserFrequencyAttr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vLaserFrequencyAttr.valueRank = UA_VALUERANK_SCALAR;
        vLaserFrequencyAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vLaserFrequencyAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vLaserFrequencyAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vLaserFrequencyAttr.value))
	//        UA_Variant_setScalar(&vLaserFrequencyAttr.value, &LaserFrequency, &UA_TYPES[UA_TYPES_FLOAT]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10310),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Laser Frequency"),		//UA_QUALIFIEDNAME(1, "Laser Frequency"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vLaserFrequencyAttr, NULL, &outLaserFrequency_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : LaserFrequency 10310: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->LaserFrequency 10310", r2_airgard_diagnostics_Id);

        // Add the variable HardDriveSpaceValue to server
        UA_VariableAttributes vHardDriveSpaceAttr = UA_VariableAttributes_default;
        vHardDriveSpaceAttr.description = UA_LOCALIZEDTEXT("en-US", "HardDriveSpace");
        vHardDriveSpaceAttr.displayName = UA_LOCALIZEDTEXT("en-US", "11. Hard Drive Space");
	vHardDriveSpaceAttr.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
	vHardDriveSpaceAttr.valueRank = UA_VALUERANK_SCALAR;
        vHardDriveSpaceAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vHardDriveSpaceAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vHardDriveSpaceAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vHardDriveSpaceAttr.value))
	//        UA_Variant_setScalar(&vHardDriveSpaceAttr.value, &HardDriveSpace, &UA_TYPES[UA_TYPES_INT16]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10311),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Hard Disk Drive Space"), 		//UA_QUALIFIEDNAME(1, "Hard Disk Drive Space"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vHardDriveSpaceAttr, NULL, &outHardDriveSpace_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : HardDriveSpace 10311: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->HardDriveSpace 10311", r2_airgard_diagnostics_Id);

        // Add the variable FlowValue to server
        UA_VariableAttributes vFlowAttr = UA_VariableAttributes_default;
        vFlowAttr.description = UA_LOCALIZEDTEXT("en-US", "Flow");
        vFlowAttr.displayName = UA_LOCALIZEDTEXT("en-US", "12. Flow");
	vFlowAttr.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
	vFlowAttr.valueRank = UA_VALUERANK_SCALAR;
        vFlowAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vFlowAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vFlowAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vFlowAttr.value))
	//        UA_Variant_setScalar(&vFlowAttr.value, &Flow, &UA_TYPES[UA_TYPES_INT16]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10312),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Flow"), 				//UA_QUALIFIEDNAME(1, "Flow"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vFlowAttr, NULL, &outFlow_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : Flow 10312: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->Flow 10312", r2_airgard_diagnostics_Id);

        // Add the variable TemperatureValue to server
        UA_VariableAttributes vTemperatureAttr = UA_VariableAttributes_default;
        vTemperatureAttr.description = UA_LOCALIZEDTEXT("en-US", "Temperature");
        vTemperatureAttr.displayName = UA_LOCALIZEDTEXT("en-US", "13. Temperature");
	vTemperatureAttr.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
	vTemperatureAttr.valueRank = UA_VALUERANK_SCALAR;
        vTemperatureAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vTemperatureAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vTemperatureAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vTemperatureAttr.value))
	//        UA_Variant_setScalar(&vTemperatureAttr.value, &Temperature, &UA_TYPES[UA_TYPES_INT16]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10313),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Temperature"), 			//UA_QUALIFIEDNAME(1, "Temperature"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vTemperatureAttr, NULL, &outTemperature_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : Temperature 10313: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->Temperature 10313", r2_airgard_diagnostics_Id);

        // Add the variable PressureValue to server
        UA_VariableAttributes vPressureAttr = UA_VariableAttributes_default;
        vPressureAttr.description = UA_LOCALIZEDTEXT("en-US", "Pressure");
        vPressureAttr.displayName = UA_LOCALIZEDTEXT("en-US", "14. Pressure");
	vPressureAttr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vPressureAttr.valueRank = UA_VALUERANK_SCALAR;
        vPressureAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vPressureAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vPressureAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vPressureAttr.value))
	//        UA_Variant_setScalar(&vPressureAttr.value, &Pressure, &UA_TYPES[UA_TYPES_FLOAT]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10314),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Pressure"), 			//UA_QUALIFIEDNAME(1, "Pressure"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vPressureAttr, NULL, &outPressure_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : Pressure 10314: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->Pressure 10314", r2_airgard_diagnostics_Id);

        // Add the variable TempOpticsValue to server
        UA_VariableAttributes vTempOpticsAttr = UA_VariableAttributes_default;
        vTempOpticsAttr.description = UA_LOCALIZEDTEXT("en-US", "TempOptics");
        vTempOpticsAttr.displayName = UA_LOCALIZEDTEXT("en-US", "15. Temp Optics");
	vTempOpticsAttr.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
	vTempOpticsAttr.valueRank = UA_VALUERANK_SCALAR;
        vTempOpticsAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vTempOpticsAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vTempOpticsAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vTempOpticsAttr.value))
	//        UA_Variant_setScalar(&vTempOpticsAttr.value, &TempOptics, &UA_TYPES[UA_TYPES_INT16]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10315),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Temp Optics"), 			//UA_QUALIFIEDNAME(1, "Temp Optics"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vTempOpticsAttr, NULL, &outTempOptics_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : TempOptics 10315: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->TempOptics 10315", r2_airgard_diagnostics_Id);

        // Add the variable BadScanCounterValue to server
        UA_VariableAttributes vBadScanCounterAttr = UA_VariableAttributes_default;
        vBadScanCounterAttr.description = UA_LOCALIZEDTEXT("en-US", "BadScanCounter");
        vBadScanCounterAttr.displayName = UA_LOCALIZEDTEXT("en-US", "16. Bad Scan Counter");
	vBadScanCounterAttr.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
	vBadScanCounterAttr.valueRank = UA_VALUERANK_SCALAR;
        vBadScanCounterAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vBadScanCounterAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vBadScanCounterAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vBadScanCounterAttr.value))
	//        UA_Variant_setScalar(&vBadScanCounterAttr.value, &BadScanCounter, &UA_TYPES[UA_TYPES_INT16]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10316),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Bad Scan Counter"), 		//UA_QUALIFIEDNAME(1, "Bad Scan Counter"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vBadScanCounterAttr, NULL, &outBadScanCounter_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : BadScanCounter 10316: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->BadScanCounter 10316", r2_airgard_diagnostics_Id);

        // Add the variable FreeMemorySpaceValue to server
        UA_VariableAttributes vFreeMemorySpaceAttr = UA_VariableAttributes_default;
        vFreeMemorySpaceAttr.description = UA_LOCALIZEDTEXT("en-US", "FreeMemorySpace");
        vFreeMemorySpaceAttr.displayName = UA_LOCALIZEDTEXT("en-US", "17. Free Memory Space");
	vFreeMemorySpaceAttr.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
	vFreeMemorySpaceAttr.valueRank = UA_VALUERANK_SCALAR;
        vFreeMemorySpaceAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vFreeMemorySpaceAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vFreeMemorySpaceAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vFreeMemorySpaceAttr.value))
	//        UA_Variant_setScalar(&vFreeMemorySpaceAttr.value, &FreeMemorySpace, &UA_TYPES[UA_TYPES_INT16]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10317),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Free Memory Space"), 		//UA_QUALIFIEDNAME(1, "Free Memory Space"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vFreeMemorySpaceAttr, NULL, &outFreeMemorySpace_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : FreeMemorySpace 10317: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->FreeMemorySpace 10317", r2_airgard_diagnostics_Id);

        // Add the variable LABFilenameValue to server
        UA_VariableAttributes vLABFilenameAttr = UA_VariableAttributes_default;
        vLABFilenameAttr.description = UA_LOCALIZEDTEXT("en-US", "LABFilename");
        vLABFilenameAttr.displayName = UA_LOCALIZEDTEXT("en-US", "18. LAB Filename");
	vLABFilenameAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vLABFilenameAttr.valueRank = UA_VALUERANK_SCALAR;
        vLABFilenameAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vLABFilenameAttr.historizing = UA_FALSE;
        //if (!UA_Variant_isEmpty(&vLABFilenameAttr.value))
	//        UA_Variant_setScalar(&vLABFilenameAttr.value, &LABFilename, &UA_TYPES[UA_TYPES_STRING]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10318),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "LAB Filename"), 			//UA_QUALIFIEDNAME(1, "LAB Filename"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vLABFilenameAttr, NULL, &outLABFilename_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : LABFilename 10318: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->LABFilename 10318", r2_airgard_diagnostics_Id);

        // Add the variable LOGFilenameValue to server
        UA_VariableAttributes vLOGFilenameAttr = UA_VariableAttributes_default;
        vLOGFilenameAttr.description = UA_LOCALIZEDTEXT("en-US", "LOGFilename");
        vLOGFilenameAttr.displayName = UA_LOCALIZEDTEXT("en-US", "19. LOG Filename");
	vLOGFilenameAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vLOGFilenameAttr.valueRank = UA_VALUERANK_SCALAR;
        vLOGFilenameAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vLOGFilenameAttr.historizing = UA_FALSE;
        //if (!UA_Variant_isEmpty(&vLOGFilenameAttr.value))
	//        UA_Variant_setScalar(&vLOGFilenameAttr.value, &LOGFilename, &UA_TYPES[UA_TYPES_STRING]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10319),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "LOG Filename"), 			//UA_QUALIFIEDNAME(1, "LOG Filename"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vLOGFilenameAttr, NULL, &outLOGFilename_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : LOGFilename 10319: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->LOGFilename 10319", r2_airgard_diagnostics_Id);

        // Add the variable LgFilenameValue to server
        UA_VariableAttributes vLgFilenameAttr = UA_VariableAttributes_default;
        vLgFilenameAttr.description = UA_LOCALIZEDTEXT("en-US", "LgFilename");
        vLgFilenameAttr.displayName = UA_LOCALIZEDTEXT("en-US", "20. Lg Filename");
	vLgFilenameAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vLgFilenameAttr.valueRank = UA_VALUERANK_SCALAR;
        vLgFilenameAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vLgFilenameAttr.historizing = UA_FALSE;
        //if (!UA_Variant_isEmpty(&vLgFilenameAttr.value))
	//        UA_Variant_setScalar(&vLgFilenameAttr.value, &LgFilename, &UA_TYPES[UA_TYPES_STRING]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10320),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Lg Filename"),			//UA_QUALIFIEDNAME(1, "Lg Filename"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vLgFilenameAttr, NULL, &outLgFilename_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : LgFilename 10320: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->LgFilename 10320", r2_airgard_diagnostics_Id);

        // Add the variable SecondLgFilenameValue to server
        UA_VariableAttributes vSecondLgFilenameAttr = UA_VariableAttributes_default;
        vSecondLgFilenameAttr.description = UA_LOCALIZEDTEXT("en-US", "SecondLgFilename");
        vSecondLgFilenameAttr.displayName = UA_LOCALIZEDTEXT("en-US", "21. Second Lg Filename");
	vSecondLgFilenameAttr.dataType = UA_TYPES[UA_TYPES_STRING].typeId;
	vSecondLgFilenameAttr.valueRank = UA_VALUERANK_SCALAR;
        vSecondLgFilenameAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
	vSecondLgFilenameAttr.historizing = UA_FALSE;
        //if (!UA_Variant_isEmpty(&vSecondLgFilenameAttr.value))
        //	UA_Variant_setScalar(&vSecondLgFilenameAttr.value, &SecondLgFilename, &UA_TYPES[UA_TYPES_STRING]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10321),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Second Lg Filename"), 		//UA_QUALIFIEDNAME(1, "Second Lg Filename"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vSecondLgFilenameAttr, NULL, &outSecondLgFilename_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : SecondLgFilename 10321: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->SecondLgFilename 10321", r2_airgard_diagnostics_Id);

        // Add the variable SystemCounterValue to server
        UA_VariableAttributes vSystemCounterAttr = UA_VariableAttributes_default;
        vSystemCounterAttr.description = UA_LOCALIZEDTEXT("en-US", "SystemCounter");
        vSystemCounterAttr.displayName = UA_LOCALIZEDTEXT("en-US", "22. System Counter");
	vSystemCounterAttr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vSystemCounterAttr.valueRank = UA_VALUERANK_SCALAR;
        vSystemCounterAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vSystemCounterAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vSystemCounterAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vSystemCounterAttr.value))
	//        UA_Variant_setScalar(&vSystemCounterAttr.value, &SystemCounter, &UA_TYPES[UA_TYPES_FLOAT]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10322),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "System Counter"), 		//UA_QUALIFIEDNAME(1, "System Counter"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vSystemCounterAttr, NULL, &outSystemCounter_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : SystemCounter 10322: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->SystemCounter 10322", r2_airgard_diagnostics_Id);

        // Add the variable DetectorCounterValue to server
        UA_VariableAttributes vDetectorCounterAttr = UA_VariableAttributes_default;
        vDetectorCounterAttr.description = UA_LOCALIZEDTEXT("en-US", "DetectorCounter");
        vDetectorCounterAttr.displayName = UA_LOCALIZEDTEXT("en-US", "23. Detector Counter");
	vDetectorCounterAttr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vDetectorCounterAttr.valueRank = UA_VALUERANK_SCALAR;
        vDetectorCounterAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vDetectorCounterAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vDetectorCounterAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vDetectorCounterAttr.value))
	//        UA_Variant_setScalar(&vDetectorCounterAttr.value, &DetectorCounter, &UA_TYPES[UA_TYPES_FLOAT]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10323),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Detector Counter"), 		//UA_QUALIFIEDNAME(1, "Detector Counter"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vDetectorCounterAttr, NULL, &outDetectorCounter_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : DetectorCounter 10323: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->DetectorCounter 10323", r2_airgard_diagnostics_Id);

        // Add the variable LaserCounterValue to server
        UA_VariableAttributes vLaserCounterAttr = UA_VariableAttributes_default;
        vLaserCounterAttr.description = UA_LOCALIZEDTEXT("en-US", "LaserCounter");
        vLaserCounterAttr.displayName = UA_LOCALIZEDTEXT("en-US", "24. Laser Counter");
	vLaserCounterAttr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vLaserCounterAttr.valueRank = UA_VALUERANK_SCALAR;
        vLaserCounterAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vLaserCounterAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vLaserCounterAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vLaserCounterAttr.value))
	//        UA_Variant_setScalar(&vLaserCounterAttr.value, &LaserCounter, &UA_TYPES[UA_TYPES_FLOAT]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10324),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Laser Counter"), 			//UA_QUALIFIEDNAME(1, "Laser Counter"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vLaserCounterAttr, NULL, &outLaserCounter_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : LaserCounter 10324: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->LaserCounter 10324", r2_airgard_diagnostics_Id);

        // Add the variable FlowPumpCounterValue to server
        UA_VariableAttributes vFlowPumpCounterAttr = UA_VariableAttributes_default;
        vFlowPumpCounterAttr.description = UA_LOCALIZEDTEXT("en-US", "FlowPumpCounter");
        vFlowPumpCounterAttr.displayName = UA_LOCALIZEDTEXT("en-US", "25. Flow Pump Counter");
	vFlowPumpCounterAttr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vFlowPumpCounterAttr.valueRank = UA_VALUERANK_SCALAR;
        vFlowPumpCounterAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vFlowPumpCounterAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vFlowPumpCounterAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vFlowPumpCounterAttr.value))
	//        UA_Variant_setScalar(&vFlowPumpCounterAttr.value, &FlowPumpCounter, &UA_TYPES[UA_TYPES_FLOAT]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10325),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Flow Pump Counter"), 		//UA_QUALIFIEDNAME(1, "Flow Pump Counter"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vFlowPumpCounterAttr, NULL, &outFlowPumpCounter_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : FlowPumpCounter 10325: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->FlowPumpCounter 10325", r2_airgard_diagnostics_Id);

        // Add the variable DesiccantCounterValue to server
        UA_VariableAttributes vDesiccantCounterAttr = UA_VariableAttributes_default;
        vDesiccantCounterAttr.description = UA_LOCALIZEDTEXT("en-US", "DesiccantCounter");
        vDesiccantCounterAttr.displayName = UA_LOCALIZEDTEXT("en-US", "26. Desiccant Counter");
	vDesiccantCounterAttr.dataType = UA_TYPES[UA_TYPES_FLOAT].typeId;
	vDesiccantCounterAttr.valueRank = UA_VALUERANK_SCALAR;
        vDesiccantCounterAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vDesiccantCounterAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
 	vDesiccantCounterAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vDesiccantCounterAttr.value))
	//        UA_Variant_setScalar(&vDesiccantCounterAttr.value, &DesiccantCounter, &UA_TYPES[UA_TYPES_FLOAT]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10326),
            r2_airgard_diagnostics_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "Desiccant Counter"), 		//UA_QUALIFIEDNAME(1, "Desiccant Counter"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vDesiccantCounterAttr, NULL, &outDesiccantCounter_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : DesiccantCounter 10326: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Diagnostics->DesiccantCounter 10326", r2_airgard_diagnostics_Id);
         // End Subtree: Diagnostics

        //------------------------------------ Subtree: Data
                // Subtree: Airgard->Data (1,10400)
                        // Add objectnode to represent Airgard->Data
        UA_NodeId r2_airgard_data_Id;   //:: line 1866 ::  shift the definition to the front of the function()
        UA_ObjectAttributes oAttr_r2_data = UA_ObjectAttributes_default;
        retval = UA_Server_addObjectNode(uaServer, UA_NODEID_STRING(namespaceIndex, "AirgardSensor_Data"),
            r1_airgardroot_Id,
            UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
            UA_QUALIFIEDNAME(namespaceIndex, "Data"), 				//UA_QUALIFIEDNAME(1, "Data"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
            oAttr_r2_data, NULL, &r2_airgard_data_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating object node : AirGardSensor_Data : %s", UA_StatusCode_name(retval));
        else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Object Node created : Airgard->Data Node", r2_airgard_data_Id);

        // variable
        UA_VariableAttributes vAlarmsAttr = UA_VariableAttributes_default;
        vAlarmsAttr.description = UA_LOCALIZEDTEXT("en-US", "AlarmsInfo");
        vAlarmsAttr.displayName = UA_LOCALIZEDTEXT("en-US", "1. Alarms");
	vAlarmsAttr.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
        vAlarmsAttr.valueRank = UA_VALUERANK_SCALAR;
        vAlarmsAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
	vAlarmsAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
	vAlarmsAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vAlarmsAttr.value))
	//        UA_Variant_setScalar(&vAlarmsAttr.value, &NoOfAlarms, &UA_TYPES[UA_TYPES_INT16]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10401),
            r2_airgard_data_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "1. Alarms"), 			//UA_QUALIFIEDNAME(1, "1. Alarms"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vAlarmsAttr, NULL, NULL);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : Alarms 10401: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->Alarms 10401");

        // variable
        UA_VariableAttributes vNonAlarmsAttr = UA_VariableAttributes_default;
        vNonAlarmsAttr.description = UA_LOCALIZEDTEXT("en-US", "NonAlarmsInfo");
        vNonAlarmsAttr.displayName = UA_LOCALIZEDTEXT("en-US", "2. Non Alarms");
        vNonAlarmsAttr.dataType = UA_TYPES[UA_TYPES_INT16].typeId;
        vNonAlarmsAttr.valueRank = UA_VALUERANK_SCALAR;
        vNonAlarmsAttr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE | UA_ACCESSLEVELMASK_HISTORYREAD;
        vNonAlarmsAttr.userAccessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_HISTORYREAD;// needed by UAExpert
        vNonAlarmsAttr.historizing = UA_TRUE;
        //if (!UA_Variant_isEmpty(&vNonAlarmsAttr.value))
	//        UA_Variant_setScalar(&vNonAlarmsAttr.value, &NoOfNonAlarms, &UA_TYPES[UA_TYPES_INT16]);
        retval = UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 10402),
            r2_airgard_data_Id, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY),
            UA_QUALIFIEDNAME(namespaceIndex, "2. Non Alarms"),			//UA_QUALIFIEDNAME(1, "2. Non Alarms"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
            vNonAlarmsAttr, NULL, NULL);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating node : NonAlarms 10402: %s", UA_StatusCode_name(retval));
	else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Attribute Variable created: Airgard->NonAlarms 10402");

        // <(OT) Alarm> sub nodes are created dynamically in extract_element_names()
        // TBD :Non Alarm sub nodes are created dynamically in extract_element_names()

    //--
            // Add objectnode to represent Airgard->Methods
        UA_NodeId r2_airgard_method_Id;  /* get the nodeid assigned by the server */
        UA_ObjectAttributes oAttr_r2_method = UA_ObjectAttributes_default;
        retval = UA_Server_addObjectNode(uaServer, UA_NODEID_STRING(namespaceIndex, "AirGardSensor_Method"),
            r1_airgardroot_Id,
            UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),                     	// Make Airgard node <Contains> Method node
            UA_QUALIFIEDNAME(namespaceIndex, "Methods"), 			//UA_QUALIFIEDNAME(1, "Methods"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
            oAttr_r2_method, NULL, &r2_airgard_method_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating object node : AirGardSensor_Method : %s", UA_StatusCode_name(retval));
        else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Object Node created : Airgard->Methods Node");


            // Add objectnode to represent Airgard->Events
        UA_NodeId r2_airgard_event_Id;  /* get the nodeid assigned by the server */
        UA_ObjectAttributes oAttr_r2_event = UA_ObjectAttributes_default;
        retval = UA_Server_addObjectNode(uaServer, UA_NODEID_STRING(namespaceIndex, "AirGardSensor_Event"),
            r1_airgardroot_Id,
            UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),                          // Make Airgard node <Contains> Method $
            UA_QUALIFIEDNAME(namespaceIndex, "Events"),                        //UA_QUALIFIEDNAME(1, "Methods"),
            UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
            oAttr_r2_event, NULL, &r2_airgard_event_Id);
        if (retval != UA_STATUSCODE_GOOD)
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Error creating object node : AirGardSensor_Event : %s", UA_StatusCode_name(retval));
        else
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                "Object Node created : Airgard->Events Node");

	UA_NodeId *NodeId;
	NodeId = calloc (2, sizeof(UA_NodeId));
	NodeId[0] = r2_airgard_method_Id;
	NodeId[1] = r2_airgard_event_Id;
	return NodeId;

    	//return (r2_airgard_method_Id);

    } // end if


} // End CreateOPCUANodes()
