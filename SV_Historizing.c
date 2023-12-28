#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include "open62541.h"
#include <open62541/plugin/historydata/history_data_backend_memory.h>
#include <open62541/plugin/historydata/history_data_gathering_default.h>
#include <open62541/plugin/historydata/history_database_default.h>
#include <open62541/plugin/historydatabase.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

//#include <xml.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/xmlreader.h>
#include <mariadb/mysql.h>

//#include "myNewServer.h"
//#include "myNewMethod.h"
#define MAX_STRING_SIZE 64
//#define DEBUG_MODE
#define UA_ENABLE_HISTORIZING
#define optionPoll

void
setValue(UA_Server *server, void *hdgContext, const UA_NodeId *sessionId,
	void *sessionContext, const UA_NodeId *nodeId, UA_Boolean historizing,
	const UA_DataValue *value);
void GetHistoryDBConnection(void);
void CloseHistoryDBConnection(void);
void writeHistoryData(const UA_NodeId *nodeId, const UA_DataValue *value);
void CreateServerHistorizingItems(UA_Server *);
void readHistoryData(MYSQL *conn, const UA_NodeId *nodeId);

// to remove after test using MonitoredItems
/*
static void WriteIgramDC(UA_Server *uaServer,
                                const UA_NodeId *sessionId, void *sessionContext,
                                const UA_NodeId *nodeId, void *nodeContext,
                                const UA_NumericRange *range, const UA_DataValue*data);
static void addValueCallbackToIgramDCVariable(UA_Server *uaServer);
*/

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
extern AlarmStruct arrayOfAlarm[255];
extern NonAlarmStruct arrayOfNonAlarm[255];

extern int sockfd;
extern int command_sockfd;

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
extern UA_NodeId outSystemCounter_Id;
extern UA_NodeId outDetectorCounter_Id;
extern UA_NodeId outLaserCounter_Id;
extern UA_NodeId outFlowPumpCounter_Id;
extern UA_NodeId outDesiccantCounter_Id;

extern MYSQL *conn;
/*
void
setValue(UA_Server *server, void *hdgContext, const UA_NodeId *sessionId,
        void *sessionContext, const UA_NodeId *nodeId, UA_Boolean historizing,
        const UA_DataValue *value)
{

                        // the following creates an entry in UA_Expert History tab! FAILED
                        //HistoryDataGathering.setValue(server, HistoryDataGathering.context, sessionId,
                        //                                        sessionContext, &outIgramPP_Id, UA_TRUE, &outIgramPP_Id.value);
                        UA_Variant *h_data = UA_Variant_new();
                        UA_DataValue *myDataValue = UA_DataValue_new();
                        UA_Float raw_data;
                        UA_Server_readValue(server, outIgramPP_Id, h_data);
                        raw_data = *(UA_Float *)h_data->data;
                        myDataValue->value = h_data;
                        HistoryDataGathering.setValue(server, HistoryDataGathering.context, sessionId,
                                                                sessionContext, &outIgramPP_Id, UA_TRUE, myDataValue);

                        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "outIgramPP node value is %f", raw_data);
                        UA_Variant_delete(h_data);
                        UA_DataValue_delete(myDataValue);

}
*/

void GetHistoryDBConnection()
{
                // establish connection to mySQL server
                conn = mysql_init(NULL);
                if (conn == NULL)
                {
                        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Error initialising mySQL connection");
                        exit(-1);
                }
                else
                {
                        // writes historydata to database 'HistoryAirgard' table 'HistoryAirgardIgramDC'
                        /*mysql_real_connect(MYSQL *mysql, const char *host,
                                           const char *user,
                                           const char *passwd,
                                           const char *db,
                                           unsigned int port,
                                           const char *unix_socket,
                                           unsigned long clientflag);
                        */
                        if (mysql_real_connect(conn, "mysql1",
                                                "debian",
                                                "molekhaven24",
                                                "HistoryAirgard",
                                                3306,
                                                NULL, 0) == NULL)       // port 3306 defined in c:\Program Files\MySQL\MySQL Server 8.0\my.ini

                        {
                                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "Error connecting to mySQL server : %d %s", mysql_errno(conn), mysql_error(conn) );
                                exit (-1);
                                return;
                        }
                        else
                        {
                                //writeHistoryData(conn, &outIgramPP_Id);
                                //writeHistoryData(conn, &outIgramDC_Id);
				//writeHistoryData(conn, &outLaserPP_Id);
				//writeHistoryData(conn, &outLaserDC_Id);

                        }
                }

}

void CloseHistoryDBConnection()
{
	mysql_close(conn);
}

void writeHistoryData(const UA_NodeId *nodeId, const UA_DataValue *DataValue)//, datetime time, data)
{

	char sqlbuffer[255];
	char varbuffer[50];
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	char currenttime[19];


	//printf("node Id->identifier.numeric : %d\n", nodeId->identifier.numeric);
	sprintf(currenttime, "%4d-%d-%d %02d:%02d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min);
	//printf("SV_Historizing:writeHistoryData : currenttime is %s \n", currenttime);

	switch (nodeId->identifier.numeric)
	{
		case 10301:
			strcpy(sqlbuffer,  "insert into HistoryAirgardIgramPP values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
			break;
		case 10302:
			strcpy(sqlbuffer,  "insert into HistoryAirgardIgramDC values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
		case 10303:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardLaserPP values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
		case 10304:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardLaserDC values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10305:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardSingleBeamAt900 values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10306:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardSingleBeamAt2500 values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10307:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardSignalToNoiseAt2500 values ");
		        sprintf(varbuffer, "( %d, '%s', %d, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Int16 *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10308:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardCenterBurstLocation values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10309:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardDetectorTemp values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10310:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardLaserFrequency values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10311:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardHardDriveSpace values ");
		        sprintf(varbuffer, "( %d, '%s', %d, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Int16 *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10312:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardFlow values ");
		        sprintf(varbuffer, "( %d, '%s', %d, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Int16 *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10313:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardTemperature values ");
		        sprintf(varbuffer, "( %d, '%s', %d, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Int16 *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10314:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardPressure values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10315:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardTempOptics values ");
		        sprintf(varbuffer, "( %d, '%s', %d, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Int16 *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10316:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardBadScanCounter values ");
		        sprintf(varbuffer, "( %d, '%s', %d, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Int16 *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10317:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardFreeMemorySpace values ");
		        sprintf(varbuffer, "( %d, '%s', %d, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Int16 *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
		// 10318 - 10321 - string type, no need to historize
		/*
		case 10318:break;
		case 10319:break;
		case 10320:break;
		case 10321:break;
		*/
                case 10322:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardSystemCounter values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10323:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardDetectorCounter values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10324:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardLaserCounter values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10325:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardFlowPumpCounter values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
                case 10326:
                        strcpy(sqlbuffer,  "insert into HistoryAirgardDesiccantCounter values ");
		        sprintf(varbuffer, "( %d, '%s', %f, 0 )", nodeId->identifier.numeric, currenttime, *(UA_Float *)DataValue->value.data );
        		strcat(sqlbuffer, varbuffer);
                        break;
	}
	//printf("SV_Historizing:writeHistoryData : sqlStatement is <%s> \n", sqlbuffer);

	if (nodeId->identifier.numeric == 10001) return;	// SoftwareVersion
	if (nodeId->identifier.numeric == 10002) return;	// DataBlockVersion
	if (nodeId->identifier.numeric == 10101) return;	// InstrumentTime
	if (nodeId->identifier.numeric == 10102) return;	// MeasurementTime
	if (nodeId->identifier.numeric == 10201) return;	// Sensor
	if (nodeId->identifier.numeric == 10202) return;	// OperatingTime
	if (nodeId->identifier.numeric == 10203) return;	// WarningMessage
	if (nodeId->identifier.numeric == 10211) return;	// BootStatus
	if (nodeId->identifier.numeric == 10212) return;	// SnapshotStatus
	if (nodeId->identifier.numeric == 10213) return;	// SCPStatus
	if (nodeId->identifier.numeric == 10214) return;	// SFTPStatus
	if (nodeId->identifier.numeric == 10215) return;	// RunScriptStatus
	if (nodeId->identifier.numeric == 10216) return;	// ArchiveStatus
	if (nodeId->identifier.numeric == 10217) return;	// AncillarySensorStatus

	if (nodeId->identifier.numeric == 10318) return;	// LABFilename
	if (nodeId->identifier.numeric == 10319) return;	// LOGFilename
	if (nodeId->identifier.numeric == 10320) return;	// LgFilename
	if (nodeId->identifier.numeric == 10321) return;	// SecondLgFilename

        if (mysql_query(conn, sqlbuffer))
	{
		#ifdef DEBUG_MODE
		UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : writeHistoryData() sqlbuffer = <%s> sqlerror = <%s>", sqlbuffer, mysql_error(conn));
		#endif
	}
	else
	{
		#ifdef DEBUG_MODE
                UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : writeHistoryData() sqlbuffer = <%s> sqlerror = <%s>", sqlbuffer, mysql_error(conn));
                #endif
	}
}

/*
static void WriteIgramDC(UA_Server *uaServer,
				const UA_NodeId *sessionId, void *sessionContext,
				const UA_NodeId *nodeId, void *nodeContext,
				const UA_NumericRange *range, const UA_DataValue*data)
{
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "IgramDC is updated");
	UA_NodeId myNodeId;
	UA_NodeId_copy(&myNodeId, nodeId);
	writeHistoryData(myNodeId);
}


static void addValueCallbackToIgramDCVariable(UA_Server *uaServer)
{
	UA_ValueCallback callback;
	callback.onWrite = WriteIgramDC;
	UA_Server_setVariableNode_valueCallback(uaServer, outIgramDC_Id, callback);
}
*/

//--start block
UA_Boolean boundHistory()
{

}

UA_Boolean boundSupported(UA_Server *server,
                      void *hdbContext,
                      const UA_NodeId *sessionId,
                      void *sessionContext,
                      const UA_NodeId *nodeId)
{
	boundHistory();
}
//-- end block

UA_Boolean updateHistoryData(MYSQL *conn, UA_NodeId *nodeId)
{

}

UA_Boolean removeHistoryData(MYSQL *conn, UA_NodeId *nodeId)
{

}

UA_DateTime firstTimeStamp(MYSQL *conn, const UA_NodeId *nodeId)
{
	char sqlbuffer[255];

	switch (nodeId->identifier.numeric)
	{
		case 10301:
			sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardIgramPP where nodeid = %d order by t1 asc limit 1",
					nodeId->identifier.numeric); break;
                case 10302:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardIgramDC where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10303:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardLaserPP where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10304:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardLaserDC where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10305:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardSingleBeamAt900 where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10306:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardSingleBeamAt2500 where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10307:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardSignalToNoiseAt2500 where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10308:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardCenterBurstLocation where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10309:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardDetectorTemp where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10310:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardLaserFrequency where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10311:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardHardDriveSpace where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10312:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardFlow where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10313:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardTemperature where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10314:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardPressure where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10315:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardTempOptics where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10316:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardBadScanCounter where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10317:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardFreeMemorySpace where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                // 10318 - 10321 - string type, no need to historize
		/*
		case 10318:break;
		case 10319:break;
		case 10320:break;
		case 10321:break;
		*/
                case 10322:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardSystemCounter where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10323:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardDetectorCounter where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10324:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardLaserCounter where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10325:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardFlowPumpCounter where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10326:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardDesiccantCounter where nodeid = %d order by t1 asc limit 1",
                                        nodeId->identifier.numeric); break;

	}

        printf("firstTimeStamp SQL :%s \n", sqlbuffer);

        if (nodeId->identifier.numeric == 10318) return -1;
        if (nodeId->identifier.numeric == 10319) return -1;
        if (nodeId->identifier.numeric == 10320) return -1;
        if (nodeId->identifier.numeric == 10321) return -1;

	if (mysql_query(conn, sqlbuffer))
                fprintf(stderr, "%s\n", mysql_error(conn));

	return 0;

}

size_t firstIndex(UA_Server *server,
                  void *hdbContext,
                  const UA_NodeId *sessionId,
                  void *sessionContext,
                  const UA_NodeId *nodeId)
{
	firstTimeStamp(conn, nodeId);
}
//-- start block
UA_DateTime lastTimeStamp(MYSQL *conn, const UA_NodeId *nodeId)
{
        char sqlbuffer[255];

        switch (nodeId->identifier.numeric)
        {
                case 10301:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardIgramPP where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric);break;
                case 10302:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardIgramDC where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10303:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardLaserPP where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10304:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardLaserDC where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10305:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardSingleBeamAt900 where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10306:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardSingleBeamAt2500 where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10307:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardSignalToNoiseAt2500 where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10308:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardCenterBurstLocation where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10309:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardDetectorTemp where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10310:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardLaserFrequency where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10311:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardHardDriveSpace where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10312:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardFlow where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10313:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardTemperature where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10314:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardPressure where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10315:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardTempOptics where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10316:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardBadScanCounter where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10317:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardFreeMmemorySpace where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                // 10318 - 10321 - string type, no need to historize
                case 10322:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardSystemCounter where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10323:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardDetectorCounter where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10324:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardLaserCounter where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10325:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardFlowPumpCounter where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;
                case 10326:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardDesiccantCounter where nodeid = %d order by t1 desc limit 1",
                                        nodeId->identifier.numeric); break;

        }

	printf("lastTimeStamp SQL :%s \n", sqlbuffer);

        if (nodeId->identifier.numeric == 10318) return -1;
        if (nodeId->identifier.numeric == 10319) return -1;
        if (nodeId->identifier.numeric == 10320) return -1;
        if (nodeId->identifier.numeric == 10321) return -1;

	if (mysql_query(conn, sqlbuffer))
                fprintf(stderr, "%s\n", mysql_error(conn));

	return 0;
}

size_t lastIndex(UA_Server *server,
                 void *hdbContext,
                 const UA_NodeId *sessionId,
                 void *sessionContext,
                 const UA_NodeId *nodeId)
{
	lastTimeStamp(conn, nodeId);
}
//--end block

UA_Boolean hasTimeStamp(MYSQL *conn, UA_NodeId *node)
{

}

UA_DateTime findTimeStamp(MYSQL *conn, UA_NodeId *nodeId)
{

}

//--start block
void numDataPointsInRange(MYSQL *conn, const UA_NodeId *nodeId)
{
	char sqlbuffer[255];

        switch (nodeId->identifier.numeric)
        {
                case 10301:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardIgramPP where t1 >= '\?\?' AND t1 <= '\?\?' and nodeid = %d order by t1 asc ",
                                        nodeId->identifier.numeric);
                        printf("numDataPointsInRange SQL :%s \n", sqlbuffer);break;
                case 10302:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardIgramDC where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10303:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardLaserPP where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10304:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardLaserDC where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10305:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardSingleBeamAt900 where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10306:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardSingleBeamAt2500 where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10307:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardSignalToNoiseAt2500 where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10308:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardCenterBurstLocation where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10309:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardDetectorTemp where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10310:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardLaserFrequency where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10311:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardHardDriveSpace where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10312:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardFlow where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10313:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardTemperature where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10314:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardPressure where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10315:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardTempOptics where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10316:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardBadScanCounter where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10317:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardFreeMmemorySpace where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                // 10318 - 10321 - string type, no need to historize
                case 10322:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardSystemCounter where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10323:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardDetectorCounter where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10324:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardLaserCounter where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10325:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardFlowPumpCounter where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
                case 10326:
                        sprintf(sqlbuffer, "select count(*) from HistoryAirgardDesiccantCounter where t1 >= '??' AND t1 <= '??' and nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric); break;
        }

        if (nodeId->identifier.numeric == 10318) return;
        if (nodeId->identifier.numeric == 10319) return;
        if (nodeId->identifier.numeric == 10320) return;
        if (nodeId->identifier.numeric == 10321) return;

        if (mysql_query(conn, sqlbuffer))
                fprintf(stderr, "%s\n", mysql_error(conn));

}

size_t
resultSize(UA_Server *server,
                  void *hdbContext,
                  const UA_NodeId *sessionId,
                  void *sessionContext,
                  const UA_NodeId *nodeId,
                  size_t startIndex,
                  size_t endIndex)
{
	numDataPointsInRange(conn, nodeId);
}
//--end block

UA_Boolean IsReturnTimeStampSupported(MYSQL *conn, const UA_NodeId *nodeId)
{

}

UA_Boolean
timestampsToReturnSupported(UA_Server *server,
                                   void *hdbContext,
                                   const UA_NodeId *sessionId,
                                   void *sessionContext,
                                   const UA_NodeId *nodeId,
                                   const UA_TimestampsToReturn timestampsToReturn)
{
	return IsReturnTimeStampSupported(conn, nodeId);
}
//--
void readHistoryData(MYSQL *conn, const UA_NodeId *nodeId)
{
        char sqlbuffer[255];

        switch (nodeId->identifier.numeric)
        {
                case 10301:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardIgramPP where nodeid = %d order by t1 asc",
                                        nodeId->identifier.numeric);
                        printf("readHistoryData SQL :%s \n", sqlbuffer);
                        break;
                case 10302:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardIgramDC where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10303:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardLaserPP where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10304:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardLaserDC where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10305:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardSingleBeamAt900 where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10306:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardSingleBeamAt2500 where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10307:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardSignalToNoiseAt2500 where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10308:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardCenterBurstLocation where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10309:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardDetectorTemp where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10310:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardLaserFrequency where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10311:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardHardDriveSpace where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10312:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardFlow where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10313:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardTemperature where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10314:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardPressure where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10315:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardTempOptics where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10316:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardBadScanCounter where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10317:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardFreeMemorySpace where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                // 10318 - 10321 - string type, no need to historize
                case 10322:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardSystemCounter where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10323:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardDetectorCounter where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10324:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardLaserCounter where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10325:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardFlowPumpCounter where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;
                case 10326:
                        sprintf(sqlbuffer, "select nodeid, t1, value, ua_status from HistoryAirgardDesiccantCounter where nodeid = %d order by t1 asc 1",
                                        nodeId->identifier.numeric); break;

        }

        if (nodeId->identifier.numeric == 10318) return;
        if (nodeId->identifier.numeric == 10319) return;
        if (nodeId->identifier.numeric == 10320) return;
        if (nodeId->identifier.numeric == 10321) return;

        if (mysql_query(conn, sqlbuffer))
                fprintf(stderr, "%s\n", mysql_error(conn));

}

UA_StatusCode getHistoryData(UA_Server *server,
                      const UA_NodeId *sessionId,
                      void *sessionContext,
                      const UA_HistoryDataBackend *backend,
                      const UA_DateTime start,
                      const UA_DateTime end,
                      const UA_NodeId *nodeId,
                      size_t maxSizePerResponse,
                      UA_UInt32 numValuesPerNode,
                      UA_Boolean returnBounds,
                      UA_TimestampsToReturn timestampsToReturn,
                      UA_NumericRange range,
                      UA_Boolean releaseContinuationPoints,
                      const UA_ByteString *continuationPoint,
                      UA_ByteString *outContinuationPoint,
                      UA_HistoryData *result)
{
        readHistoryData(conn, sessionId);

}

void CreateServerHistorizingItems(UA_Server *server)
{
/*
    UA_HISTORIZINGUPDATESTRATEGY_USER     = 0x00, // The user of the api stores the values to the database himself.
                                                     The api will not store any value to the database.
    UA_HISTORIZINGUPDATESTRATEGY_VALUESET = 0x01, // Values will be stored when a node's value is set.
                                                     The values will be stored when a node is updated via write service.
    UA_HISTORIZINGUPDATESTRATEGY_POLL     = 0x02  // The value of the node will be read periodically.
                                                     This is mainly relevant for datasource nodes which do
                                                     not use the write service.
                                                     Values will not be stored if the value is
                                                     equal to the old value.
*/

	#ifdef optionPoll       // polling mode
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : optionPoll");
                       	size_t initialNodeIdStoreSize = 22;
			UA_StatusCode retval;

                        UA_HistoryDataGathering HistoryDataGathering = UA_HistoryDataGathering_Default(initialNodeIdStoreSize); // initial NodeStore Size = 1

                        UA_ServerConfig *config = UA_Server_getConfig(server);
                        config->historyDatabase = UA_HistoryDatabase_default(HistoryDataGathering);

                        UA_HistorizingNodeIdSettings HistorizingSetting;
                        HistorizingSetting.historizingBackend = UA_HistoryDataBackend_Memory(22, 200); // 22 nodes, 200 values each
                        HistorizingSetting.maxHistoryDataResponseSize = 100;

			// callback for HistoryDataBackend functions
			UA_HistoryDataBackend HistoryDataBackend;
			//HistoryDataBackend.serverSetHistoryData = ???
			HistoryDataBackend.getHistoryData = getHistoryData;
			HistoryDataBackend.firstIndex = firstTimeStamp;
			HistoryDataBackend.lastIndex = lastTimeStamp;
			HistoryDataBackend.resultSize = numDataPointsInRange;
			HistoryDataBackend.boundSupported = boundHistory;
			HistoryDataBackend.timestampsToReturnSupported = IsReturnTimeStampSupported;

			//==============================================================================================================================================

    			//UA_HISTORIZINGUPDATESTRATEGY_POLL     = 0x02  // The value of the node will be read periodically.
                        //                             This is mainly relevant for datasource nodes which do
                        //                             not use the write service.
                        //                             Values will not be stored if the value is
                        //                             equal to the old value.

			HistorizingSetting.pollingInterval = 100;	// in millisec
                        HistorizingSetting.historizingUpdateStrategy = UA_HISTORIZINGUPDATESTRATEGY_POLL;

	       retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outIgramPP_Id, HistorizingSetting);
			#ifdef DEBUG
			UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outIgramPP_Id : %s", UA_StatusCode_name(retval));
			#endif

			HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outIgramDC_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outLaserPP_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outLaserDC_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outSingleBeamAt900_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outSingleBeamAt2500_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outSignalToNoiseAt2500_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outCenterBurstLocation_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outDetectorTemp_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outLaserFrequency_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outHardDriveSpace_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outFlow_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outTemperature_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outPressure_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outTempOptics_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outBadScanCounter_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outFreeMemorySpace_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outSystemCounter_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outDetectorCounter_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outLaserCounter_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outFlowPumpCounter_Id, HistorizingSetting);
                        HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outDesiccantCounter_Id, HistorizingSetting);

			// the following creates an entry in UA_Expert History tab!
                        //HistoryDataGathering.setValue(server, HistoryDataGathering.context, config->sessionId,
                        //                                        config->sessionContext, &outIgramPP_Id, UA_TRUE, &outIgramPP_Id.value);

		//IgramPP = rand(); for testing purposes

                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outIgramPP_Id);
		//	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outIgramPP_Id : %s", UA_StatusCode_name(retval));
		retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outIgramDC_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outIgramDC_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outLaserPP_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outLaserPP_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outLaserDC_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outLaserPP_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outSingleBeamAt900_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outSingleBeamAt900PP_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outSingleBeamAt2500_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outSingleBeamAt2500_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outSignalToNoiseAt2500_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outSignalToNoiseAt2500_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outCenterBurstLocation_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outCenterBurstLocation_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outDetectorTemp_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outDetectorTemp_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outLaserFrequency_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outLaserFrequency_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outHardDriveSpace_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outHardDriveSpace_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outFlow_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outFlow_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outTemperature_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outTemperature_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outPressure_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outPressure_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outTempOptics_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outTempOptics_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outBadScanCounter_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outBadScanCounter_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outFreeMemorySpace_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outFreeMemorySpace_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outSystemCounter_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outSystemCounter_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outDetectorCounter_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outDetectorCounter_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outLaserCounter_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outLaserCounter_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outFlowPumpCounter_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outFlowPumpCounter_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.startPoll(server, HistoryDataGathering.context, &outDesiccantCounter_Id);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : startpoll outDesiccantCounter_Id : %s", UA_StatusCode_name(retval));


	#else // optionValueset
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : optionValueset");
       		size_t initialNodeIdStoreSize = 1;
                UA_StatusCode retval;

                UA_HistoryDataGathering HistoryDataGathering = UA_HistoryDataGathering_Default(initialNodeIdStoreSize); // initial NodeStore Size = 1

                UA_ServerConfig *config = UA_Server_getConfig(server);
                config->historyDatabase = UA_HistoryDatabase_default(HistoryDataGathering);

                UA_HistorizingNodeIdSettings HistorizingSetting;
                HistorizingSetting.historizingBackend = UA_HistoryDataBackend_Memory(3, 100); // 22 nodes, 200 values each
                HistorizingSetting.maxHistoryDataResponseSize = 100;

                HistorizingSetting.historizingUpdateStrategy = UA_HISTORIZINGUPDATESTRATEGY_VALUESET;


			/* the following creates an entry in UA_Expert History tab
                        //HistoryDataGathering.setValue(server, HistoryDataGathering.context, sessionId,
                        //                                        sessionContext, &outIgramPP_Id, UA_TRUE, &outIgramPP_Id.value);
			UA_Variant h_data;
			UA_DataValue *myDataValue = UA_DataValue_new();
			//UA_Float raw_data;
			UA_Server_readValue(server, outIgramPP_Id, &h_data);
			//raw_data = *(UA_Float *)h_data->data;
			myDataValue->value = h_data;
			HistoryDataGathering.setValue(server, HistoryDataGathering.context, config->sessionId,
								server->sessionContext, &outIgramPP_Id, UA_TRUE, &myDataValue);

			//UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "outIgramPP node value is %f", raw_data);
			//UA_Variant_delete(h_data);
			UA_DataValue_delete(myDataValue);
			*/

		retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outIgramPP_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outIgramPP_Id : %s", UA_StatusCode_name(retval));
		retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outIgramDC_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outIgramDC_Id : %s", UA_StatusCode_name(retval));
               	retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outLaserPP_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outLaserPP_Id : %s", UA_StatusCode_name(retval));
               	retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outLaserDC_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outLaserDC_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outSingleBeamAt900_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outSingleBeamAt900_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outSingleBeamAt2500_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outSingleBeamAt2500_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outSignalToNoiseAt2500_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outSignalToNoiseAt2500_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outCenterBurstLocation_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outCenterBurstLocation_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outDetectorTemp_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outDetectorTemp_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outLaserFrequency_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outLaserFrequency_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outHardDriveSpace_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outHardDriveSpace_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outFlow_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outFlow_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outTemperature_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outTemperature_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outPressure_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outPressure_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outTempOptics_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outTempOptics_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outBadScanCounter_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outBadScanCounter_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outFreeMemorySpace_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outMemorySpace_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outSystemCounter_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outSystemCounter_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outDetectorCounter_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outDetectorCounter_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outLaserCounter_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outLaserCounter_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outFlowPumpCounter_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outFlowPump_Id : %s", UA_StatusCode_name(retval));
                retval = HistoryDataGathering.registerNodeId(server, HistoryDataGathering.context, &outDesiccantCounter_Id, HistorizingSetting);
                //        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_Historizing.c : registerNodeId outDesiccantCounter_Id : %s", UA_StatusCode_name(retval));

	#endif





}
