void CreateServerMonitoredItems(UA_Server *server);

void dataChangeNotificationCallback(UA_Server *server, UA_UInt32 monitoredItemId,
                               void *monitoredItemContext, const UA_NodeId *nodeId,
                               void *nodeContext, UA_UInt32 attributeId,
                               const UA_DataValue *value);


void addMonitoredItemToSoftwareVersionVariable(UA_Server *server);
void addMonitoredItemToDataBlockVersionVariable(UA_Server *server);
void addMonitoredItemToInstrumentTimeVariable(UA_Server *server);
void addMonitoredItemToMeasurementTimeVariable(UA_Server *server);

void addMonitoredItemToBootStatusVariable(UA_Server *server);
void addMonitoredItemToSnapshotStatusVariable(UA_Server *server);
void addMonitoredItemToSCPStatusVariable(UA_Server *server);
void addMonitoredItemToSFTPStatusVariable(UA_Server *server);
void addMonitoredItemToRunScriptStatusVariable(UA_Server *server);
void addMonitoredItemToArchiveStatusVariable(UA_Server *server);
void addMonitoredItemToAncillarySensorStatusVariable(UA_Server *server);

void addMonitoredItemToSensorVariable(UA_Server *server);
void addMonitoredItemToOperatingTimeVariable(UA_Server *server);
void addMonitoredItemToWarningMessageVariable(UA_Server *server);

void addMonitoredItemToIgramPPVariable(UA_Server *server);
void addMonitoredItemToIgramDCVariable(UA_Server *server);
void addMonitoredItemToLaserPPVariable(UA_Server *server);
void addMonitoredItemToLaserDCVariable(UA_Server *server);
void addMonitoredItemToSingleBeamAt900Variable(UA_Server *server);
void addMonitoredItemToSingleBeamAt2500Variable(UA_Server *server);
void addMonitoredItemToSignalToNoiseAt2500Variable(UA_Server *server);
void addMonitoredItemToCenterBurstLocationVariable(UA_Server *server);
void addMonitoredItemToDetectorTempVariable(UA_Server *server);
void addMonitoredItemToLaserFrequencyVariable(UA_Server *server);
void addMonitoredItemToHardDriveSpaceVariable(UA_Server *server);
void addMonitoredItemToFlowVariable(UA_Server *server);
void addMonitoredItemToTemperatureVariable(UA_Server *server);
void addMonitoredItemToPressureVariable(UA_Server *server);
void addMonitoredItemToTempOpticsVariable(UA_Server *server);
void addMonitoredItemToBadScanCounterVariable(UA_Server *server);
void addMonitoredItemToFreeMemorySpaceVariable(UA_Server *server);
void addMonitoredItemToLABFilenameVariable(UA_Server *server);
void addMonitoredItemToLOGFilenameVariable(UA_Server *server);
void addMonitoredItemToLgFilenameVariable(UA_Server *server);
void addMonitoredItemToSecondLgFilenameVariable(UA_Server *server);
void addMonitoredItemToSystemCounterVariable(UA_Server *server);
void addMonitoredItemToDetectorCounterVariable(UA_Server *server);
void addMonitoredItemToLaserCounterVariable(UA_Server *server);
void addMonitoredItemToFlowPumpCounterVariable(UA_Server *server);
void addMonitoredItemToDesiccantCounterVariable(UA_Server *server);
