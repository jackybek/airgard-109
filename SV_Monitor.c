//#include "open62541.h"
#include <open62541/client_subscriptions.h>
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

//#include <mariadb/mysql.h>
//#include "myNewMonitor.h"

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

void writeHistoryData(const UA_NodeId *nodeId, const UA_DataValue *value);

//extern MYSQL *conn;

void dataChangeNotificationCallback(UA_Server *server, UA_UInt32 monitoredItemId,
                               void *monitoredItemContext, const UA_NodeId *nodeId,
                               void *nodeContext, UA_UInt32 attributeId,
                               const UA_DataValue *value)
{
//    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_Monitor.c : dataChangeNotificationCallback : Receiving Notification .. data (MonitoredItem) has changed");
	writeHistoryData(nodeId, value);
}

void addMonitoredItemToSoftwareVersionVariable(UA_Server *server)
{
    UA_UInt16 namespaceIndex = 1;
        //UA_NodeId SoftwareVersionNodeId = UA_NODEID_NUMERIC(0, 10001);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outSoftwareVersion_Id); //(SoftwareVersionNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToDataBlockVersionVariable(UA_Server *server)
{
        //UA_NodeId DataBlockVersionNodeId = UA_NODEID_NUMERIC(0, 10002);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outDataBlockVersion_Id); //(DataBlockVersionNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}


void addMonitoredItemToInstrumentTimeVariable(UA_Server *server)
{
        //UA_NodeId InstrumentTimeNodeId = UA_NODEID_NUMERIC(0, 10101);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outInstrumentTime_Id);	//(InstrumentTimeNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToMeasurementTimeVariable(UA_Server *server)
{
        //UA_NodeId MeasurementTimeNodeId = UA_NODEID_NUMERIC(0, 10102);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outMeasurementTime_Id); //(MeasurementTimeNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToBootStatusVariable(UA_Server *server)
{
        //UA_NodeId BootStatusNodeId = UA_NODEID_NUMERIC(0, 10211);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outBootStatus_Id); //(BootStatusNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToSnapshotStatusVariable(UA_Server *server)
{
        //UA_NodeId SnapshotStatusNodeId = UA_NODEID_NUMERIC(0, 10212);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outSnapshotStatus_Id);	//(SnapshotStatusNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToSCPStatusVariable(UA_Server *server)
{
        //UA_NodeId SCPStatusNodeId = UA_NODEID_NUMERIC(0, 10213);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outSCPStatus_Id); //(SCPStatusNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToSFTPStatusVariable(UA_Server *server)
{
        //UA_NodeId SFTPStatusNodeId = UA_NODEID_NUMERIC(0, 10214);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outSFTPStatus_Id); //(SFTPStatusNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void  addMonitoredItemToRunScriptStatusVariable(UA_Server *server)
{
        //UA_NodeId RunScriptStatusNodeId = UA_NODEID_NUMERIC(0, 10215);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outRunScriptStatus_Id); //(RunScriptStatusNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToArchiveStatusVariable(UA_Server *server)
{
        //UA_NodeId ArchiveStatusNodeId = UA_NODEID_NUMERIC(0, 10216);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outArchiveStatus_Id); //(ArchiveStatusNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToAncillarySensorStatusVariable(UA_Server *server)
{
        //UA_NodeId AncillarySensorStatusNodeId = UA_NODEID_NUMERIC(0, 10217);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outAncilarySensor_Id); //(AncillarySensorStatusNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToSensorVariable(UA_Server *server)
{
        //UA_NodeId SensorNodeId = UA_NODEID_NUMERIC(0, 10201);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outSensor_Id); //SensorNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToOperatingTimeVariable(UA_Server *server)
{
        //UA_NodeId OperatingTimeNodeId = UA_NODEID_NUMERIC(0, 10202);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outOperatingTime_Id); //OperatingTimeNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToWarningMessageVariable(UA_Server *server)
{
        //UA_NodeId WarningMessageNodeId = UA_NODEID_NUMERIC(0, 10203);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outWarningMessage_Id); //WarningMessageNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}


void addMonitoredItemToIgramPPVariable(UA_Server *server)
{
  	//UA_NodeId IgramPPNodeId = UA_NODEID_NUMERIC(1, 10301);
    	UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outIgramPP_Id); //(IgramPPNodeId);
    	monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
    	UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToIgramDCVariable(UA_Server *server)
{
        //UA_NodeId IgramDCNodeId = UA_NODEID_NUMERIC(1, 10302);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outIgramDC_Id);  // (IgramDCNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToLaserPPVariable(UA_Server *server)
{
        //UA_NodeId LaserPPNodeId = UA_NODEID_NUMERIC(1, 10303);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outLaserPP_Id); // (LaserPPNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToLaserDCVariable(UA_Server *server)
{
        //UA_NodeId LaserDCNodeId = UA_NODEID_NUMERIC(1, 10304);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outLaserDC_Id); // (LaserDCNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToSingleBeamAt900Variable(UA_Server *server)
{
        //UA_NodeId SingleBeamAt900NodeId = UA_NODEID_NUMERIC(1, 10305);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outSingleBeamAt900_Id);  // (SingleBeamAt900NodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToSingleBeamAt2500Variable(UA_Server *server)
{
        //UA_NodeId SingleBeamAt2500NodeId = UA_NODEID_NUMERIC(1, 10306);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outSingleBeamAt2500_Id); // (SingleBeamAt2500NodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToSignalToNoiseAt2500Variable(UA_Server *server)
{
        //UA_NodeId SignalToNoiseAt2500NodeId = UA_NODEID_NUMERIC(1, 10307);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outSignalToNoiseAt2500_Id);  // SignalToNoiseAt2500NodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToCenterBurstLocationVariable(UA_Server *server)
{
        //UA_NodeId CenterBurstLocationNodeId = UA_NODEID_NUMERIC(1, 10308);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outCenterBurstLocation_Id); // (CenterBurstLocationNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToDetectorTempVariable(UA_Server *server)
{
        //UA_NodeId DetectorTempNodeId = UA_NODEID_NUMERIC(1, 10309);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outDetectorTemp_Id); // DetectorTempNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToLaserFrequencyVariable(UA_Server *server)
{
        //UA_NodeId LaserFrequencyNodeId = UA_NODEID_NUMERIC(1, 10310);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outLaserFrequency_Id); // (LaserFrequencyNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToHardDriveSpaceVariable(UA_Server *server)
{
        //UA_NodeId HardDriveSpaceNodeId = UA_NODEID_NUMERIC(1, 10311);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outHardDriveSpace_Id); // (HardDriveSpaceNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToFlowVariable(UA_Server *server)
{
        //UA_NodeId FlowNodeId = UA_NODEID_NUMERIC(1, 10312);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outFlow_Id); // (FlowNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToTemperatureVariable(UA_Server *server)
{
        //UA_NodeId TemperatureNodeId = UA_NODEID_NUMERIC(1, 10313);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outTemperature_Id); // (TemperatureNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToPressureVariable(UA_Server *server)
{
        //UA_NodeId PressureNodeId = UA_NODEID_NUMERIC(1, 10314);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outPressure_Id);  // (PressureNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToTempOpticsVariable(UA_Server *server)
{
        //UA_NodeId TempOpticsNodeId = UA_NODEID_NUMERIC(1, 10315);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outTempOptics_Id); // (TempOpticsNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToBadScanCounterVariable(UA_Server *server)
{
        //UA_NodeId BadScanCounterNodeId = UA_NODEID_NUMERIC(1, 10316);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outBadScanCounter_Id); // (BadScanCounterNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToFreeMemorySpaceVariable(UA_Server *server)
{
        //UA_NodeId FreeMemorySpaceNodeId = UA_NODEID_NUMERIC(1, 10317);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outFreeMemorySpace_Id);  // FreeMemorySpaceNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToLABFilenameVariable(UA_Server *server)
{
        //UA_NodeId LABFilenameNodeId = UA_NODEID_NUMERIC(1, 10318);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outLABFilename_Id); //LABFilenameNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToLOGFilenameVariable(UA_Server *server)
{
        //UA_NodeId LOGFilenameNodeId = UA_NODEID_NUMERIC(1, 10319);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outLOGFilename_Id); //LOGFilenameNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToLgFilenameVariable(UA_Server *server)
{
        //UA_NodeId LgFilenameNodeId = UA_NODEID_NUMERIC(1, 10320);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outLgFilename_Id); //LgFilenameNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToSecondLgFilenameVariable(UA_Server *server)
{
        //UA_NodeId SecondLgFilenameNodeId = UA_NODEID_NUMERIC(1, 10321);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outSecondLgFilename_Id); //SecondLgFilenameNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToSystemCounterVariable(UA_Server *server)
{
        //UA_NodeId SystemCounterNodeId = UA_NODEID_NUMERIC(1, 10322);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outSystemCounter_Id); // (SystemCounterNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToDetectorCounterVariable(UA_Server *server)
{
        //UA_NodeId DetectorCounterNodeId = UA_NODEID_NUMERIC(1, 10323);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outDetectorCounter_Id); // (DetectorCounterNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToLaserCounterVariable(UA_Server *server)
{
        //UA_NodeId LaserCounterNodeId = UA_NODEID_NUMERIC(1, 10324);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outLaserCounter_Id); // (LaserCounterNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToFlowPumpCounterVariable(UA_Server *server)
{
        //UA_NodeId FlowPumpCounterNodeId = UA_NODEID_NUMERIC(1, 10325);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outFlowPumpCounter_Id); // (FlowPumpCounterNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void addMonitoredItemToDesiccantCounterVariable(UA_Server *server)
{
        //UA_NodeId DesiccantCounterNodeId = UA_NODEID_NUMERIC(1, 10326);
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outDesiccantCounter_Id); // (DesiccantCounterNodeId);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

// the following function is called in SV_Event.c
void addMonitoredItemToEvent(UA_Server *server, UA_NodeId outEvent_Id)
{
        UA_MonitoredItemCreateRequest monRequest = UA_MonitoredItemCreateRequest_default(outEvent_Id);
        monRequest.requestedParameters.samplingInterval = 100.0; /* 100 ms interval */
        UA_Server_createDataChangeMonitoredItem(server, UA_TIMESTAMPSTORETURN_SOURCE,
                                            monRequest, NULL, dataChangeNotificationCallback);
}

void CreateServerMonitoredItems(UA_Server *server)
{
        addMonitoredItemToSoftwareVersionVariable(server);	// 1
        addMonitoredItemToDataBlockVersionVariable(server);	// 2
        addMonitoredItemToInstrumentTimeVariable(server);	// 3
        addMonitoredItemToMeasurementTimeVariable(server);	// 4

        addMonitoredItemToBootStatusVariable(server);		// 5
        addMonitoredItemToSnapshotStatusVariable(server);	// 6
        addMonitoredItemToSCPStatusVariable(server);		// 7
        addMonitoredItemToSFTPStatusVariable(server);		// 8
        addMonitoredItemToRunScriptStatusVariable(server);	// 9
        addMonitoredItemToArchiveStatusVariable(server);	// 10
        addMonitoredItemToAncillarySensorStatusVariable(server);	// 11

        addMonitoredItemToSensorVariable(server);		// 12
        addMonitoredItemToOperatingTimeVariable(server);	// 13
        addMonitoredItemToWarningMessageVariable(server);	// 14

        addMonitoredItemToIgramPPVariable(server);		// 15
        addMonitoredItemToIgramDCVariable(server);		// 16
        addMonitoredItemToLaserPPVariable(server);		// 17
        addMonitoredItemToLaserDCVariable(server);		// 18
        addMonitoredItemToSingleBeamAt900Variable(server);	// 19
        addMonitoredItemToSingleBeamAt2500Variable(server);	// 20
        addMonitoredItemToSignalToNoiseAt2500Variable(server);	// 21
        addMonitoredItemToCenterBurstLocationVariable(server);	// 22
        addMonitoredItemToDetectorTempVariable(server);		// 23
        addMonitoredItemToLaserFrequencyVariable(server);	// 24
        addMonitoredItemToHardDriveSpaceVariable(server);	// 25
        addMonitoredItemToFlowVariable(server);			// 26
        addMonitoredItemToTemperatureVariable(server);		// 27
        addMonitoredItemToPressureVariable(server);		// 28
        addMonitoredItemToTempOpticsVariable(server);		// 29
        addMonitoredItemToBadScanCounterVariable(server);	// 30
        addMonitoredItemToFreeMemorySpaceVariable(server);	// 31
        addMonitoredItemToLABFilenameVariable(server);		// 32
        addMonitoredItemToLOGFilenameVariable(server);		// 33
	addMonitoredItemToLgFilenameVariable(server);		// 34
        addMonitoredItemToSecondLgFilenameVariable(server);	// 35
        addMonitoredItemToSystemCounterVariable(server);	// 36
        addMonitoredItemToDetectorCounterVariable(server);	// 37
        addMonitoredItemToLaserCounterVariable(server);		// 38
        addMonitoredItemToFlowPumpCounterVariable(server);	// 39
        addMonitoredItemToDesiccantCounterVariable(server);	// 40

      UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
        "Completed creation of monitoring nodes");

}
