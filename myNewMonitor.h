void MonitoredItemsInit(UA_Server *server);

void dataChangeNotificationCallback(UA_Server *server, UA_UInt32 monitoredItemId,
                               void *monitoredItemContext, const UA_NodeId *nodeId,
                               void *nodeContext, UA_UInt32 attributeId,
                               const UA_DataValue *value);


void addMonitoredItemToSoftwareVersionVariable(UA_Client *server);
void addMonitoredItemToDataBlockVersionVariable(UA_Client *server);
void addMonitoredItemToInstrumentTimeVariable(UA_Client *server);
void addMonitoredItemToMeasurementTimeVariable(UA_Client *server);

void addMonitoredItemToBootStatusVariable(UA_Client *server);
void addMonitoredItemToSnapshotStatusVariable(UA_Client *server);
void addMonitoredItemToSCPStatusVariable(UA_Client *server);
void addMonitoredItemToSFTPStatusVariable(UA_Client *server);
void addMonitoredItemToRunScriptStatusVariable(UA_Client *server);
void addMonitoredItemToArchiveStatusVariable(UA_Client *server);
void addMonitoredItemToAncillarySensorStatusVariable(UA_Client *server);

void addMonitoredItemToSensorVariable(UA_Client *server);
void addMonitoredItemToOperatingTimeVariable(UA_Client *server);
void addMonitoredItemToWarningMessageVariable(UA_Client *server);

void addMonitoredItemToIgramPPVariable(UA_Client *server);
void addMonitoredItemToIgramDCVariable(UA_Client *server);
void addMonitoredItemToLaserPPVariable(UA_Client *server);
void addMonitoredItemToLaserDCVariable(UA_Client *server);
void addMonitoredItemToSingleBeamAt900Variable(UA_Client *server);
void addMonitoredItemToSingleBeamAt2500Variable(UA_Client *server);
void addMonitoredItemToSignalToNoiseAt2500Variable(UA_Client *server);
void addMonitoredItemToCenterBurstLocationVariable(UA_Client *server);
void addMonitoredItemToDetectorTempVariable(UA_Client *server);
void addMonitoredItemToLaserFrequencyVariable(UA_Client *server);
void addMonitoredItemToHardDriveSpaceVariable(UA_Client *server);
void addMonitoredItemToFlowVariable(UA_Client *server);
void addMonitoredItemToTemperatureVariable(UA_Client *server);
void addMonitoredItemToPressureVariable(UA_Client *server);
void addMonitoredItemToTempOpticsVariable(UA_Client *server);
void addMonitoredItemToBadScanCounterVariable(UA_Client *server);
void addMonitoredItemToFreeMemorySpaceVariable(UA_Client *server);
void addMonitoredItemToLABFilenameVariable(UA_Client *server);
void addMonitoredItemToLOGFilenameVariable(UA_Client *server);
void addMonitoredItemToLgFilenameVariable(UA_Client *server);
void addMonitoredItemToSecondLgFilenameVariable(UA_Client *server);
void addMonitoredItemToSystemCounterVariable(UA_Client *server);
void addMonitoredItemToDetectorCounterVariable(UA_Client *server);
void addMonitoredItemToLaserCounterVariable(UA_Client *server);
void addMonitoredItemToFlowPumpCounterVariable(UA_Client *server);
void addMonitoredItemToDesiccantCounterVariable(UA_Client *server);
