//#include "open62541.h"
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#define UA_ENABLE_SUBSCRIPTIONS_EVENTS

/**
 * Generating events
 * -----------------
 * To make sense of the many things going on in a server, monitoring items can be useful. Though in many cases, data
 * change does not convey enough information to be the optimal solution. Events can be generated at any time,
 * hold a lot of information and can be filtered so the client only receives the specific attributes of interest.
 *
 * Emitting events by calling methods
 * ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 * The following example will be based on the server method tutorial. We will be
 * creating a method node which generates an event from the server node.
 *
 * The event we want to generate should be very simple. Since the `BaseEventType` is abstract,
 * we will have to create our own event type. `EventTypes` are saved internally as `ObjectTypes`,
 * so add the type as you would a new `ObjectType`. */


// function prototypes declaration
void CreateServerEvents(UA_Server *, UA_NodeId);
static UA_StatusCode addNewEventType(UA_Server *);
static UA_StatusCode setUpEvent(UA_Server *, UA_NodeId *);
static UA_StatusCode generateEventMethodCallback(UA_Server *,
                         const UA_NodeId *, void *,
                         const UA_NodeId *, void *,
                         const UA_NodeId *, void *,
                         size_t, const UA_Variant *,
                         size_t, UA_Variant *);

void addMonitoredItemToEvent(UA_Server *, UA_NodeId *);

static UA_NodeId eventType;

/**
 * Setting up an event
 * ^^^^^^^^^^^^^^^^^^^
 * In order to set up the event, we can first use ``UA_Server_createEvent`` to give us a node representation of the event.
 * All we need for this is our `EventType`. Once we have our event node, which is saved internally as an `ObjectNode`,
 * we can define the attributes the event has the same way we would define the attributes of an object node. It is not
 * necessary to define the attributes `EventId`, `ReceiveTime`, `SourceNode` or `EventType` since these are set
 * automatically by the server. In this example, we will be setting the fields 'Message' and 'Severity' in addition
 * to `Time` which is needed to make the example UaExpert compliant.
 */
static UA_StatusCode
setUpResetEvent(UA_Server *uaServer, UA_NodeId *outEventId)
{
    UA_StatusCode retval = UA_Server_createEvent(uaServer, eventType, outEventId);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                       "createResetEvent failed. StatusCode %s", UA_StatusCode_name(retval));
        return retval;
    }

    size_t namespaceIndex;
    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

    /* Set the Event Attributes */
    /* Setting the Time is required or else the event will not show up in UAExpert! */
    UA_DateTime eventTime = UA_DateTime_now();
    UA_Server_writeObjectProperty_scalar(uaServer, *outEventId, UA_QUALIFIEDNAME(0, "Time"),
                                         &eventTime, &UA_TYPES[UA_TYPES_DATETIME]);

    UA_UInt16 eventSeverity = 100;
    UA_Server_writeObjectProperty_scalar(uaServer, *outEventId, UA_QUALIFIEDNAME(0, "Severity"),
                                         &eventSeverity, &UA_TYPES[UA_TYPES_UINT16]);

    UA_LocalizedText eventMessage = UA_LOCALIZEDTEXT("en-US", "An event has been generated.");
    UA_Server_writeObjectProperty_scalar(uaServer, *outEventId, UA_QUALIFIEDNAME(0, "Message"),
                                         &eventMessage, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);

    UA_String eventSourceName = UA_STRING("Server");
    UA_Server_writeObjectProperty_scalar(uaServer, *outEventId, UA_QUALIFIEDNAME(0, "SourceName"),
                                         &eventSourceName, &UA_TYPES[UA_TYPES_STRING]);

    // need to action on the EventId i.e. outEventId so as to publish to the MQTT broker
    // TODO: addPublishedEventSet(uaServer, outEventId)
    // TODO: addEventSetField(uaServer, outEventId)
    // TODO: addEventWriterGroup(uaServer, outEventId)
    // TODO: addEventSetWriter(uaServer, outEventId)

    // create a monitoredItem
    addMonitoredItemToEvent(uaServer, outEventId);

    return UA_STATUSCODE_GOOD;
}

static UA_StatusCode
setUpShutdownEvent(UA_Server *uaServer, UA_NodeId *outEventId)
{
    UA_StatusCode retval = UA_Server_createEvent(uaServer, eventType, outEventId);
    if (retval != UA_STATUSCODE_GOOD) {
        UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                       "createShutdownEvent failed. StatusCode %s", UA_StatusCode_name(retval));
        return retval;
    }

   size_t namespaceIndex;

    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

    /* Set the Event Attributes */
    /* Setting the Time is required or else the event will not show up in UAExpert! */
    UA_DateTime eventTime = UA_DateTime_now();
    UA_Server_writeObjectProperty_scalar(uaServer, *outEventId, UA_QUALIFIEDNAME(0, "Time"),
                                         &eventTime, &UA_TYPES[UA_TYPES_DATETIME]);

    UA_UInt16 eventSeverity = 100;
    UA_Server_writeObjectProperty_scalar(uaServer, *outEventId, UA_QUALIFIEDNAME(0, "Severity"),
                                         &eventSeverity, &UA_TYPES[UA_TYPES_UINT16]);

    UA_LocalizedText eventMessage = UA_LOCALIZEDTEXT("en-US", "An event has been generated.");
    UA_Server_writeObjectProperty_scalar(uaServer, *outEventId, UA_QUALIFIEDNAME(0, "Message"),
                                         &eventMessage, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);

    UA_String eventSourceName = UA_STRING("Server");
    UA_Server_writeObjectProperty_scalar(uaServer, *outEventId, UA_QUALIFIEDNAME(0, "SourceName"),
                                         &eventSourceName, &UA_TYPES[UA_TYPES_STRING]);

    // need to action on the EventId i.e. outEventId so as to publish to the MQTT broker
    // TODO: addPublishedEventSet(uaServer, outEventId)
    // TODO: addEventSetField(uaServer, outEventId)
    // TODO: addEventWriterGroup(uaServer, outEventId)
    // TODO: addEventSetWriter(uaServer, outEventId)

    // create a monitoredItem
    addMonitoredItemToEvent(uaServer, outEventId);

    return UA_STATUSCODE_GOOD;
}

/**
 * Triggering an event
 * ^^^^^^^^^^^^^^^^^^^
 * First a node representing an event is generated using ``setUpEvent``. Once our event is good to go, we specify
 * a node which emits the event - in this case the server node. We can use ``UA_Server_triggerEvent`` to trigger our
 * event onto said node. Passing ``NULL`` as the second-last argument means we will not receive the `EventId`.
 * The last boolean argument states whether the node should be deleted. */
static UA_StatusCode
generateEvent_ResetMethodCallback(UA_Server *uaServer,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "About to trigger Reset event");

    /* set up event */
    UA_NodeId eventNodeId;
    UA_StatusCode retval = setUpResetEvent(uaServer, &eventNodeId);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                       "Creating Reset event failed. StatusCode %s", UA_StatusCode_name(retval));
        return retval;
    }

    /* trigger event */
    retval = UA_Server_triggerEvent(uaServer, eventNodeId,
                                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER),
                                    NULL, UA_TRUE);		// jacky changed NULL to &eventId
    if(retval != UA_STATUSCODE_GOOD)
        UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                       "Triggering Reset event failed. StatusCode %s", UA_StatusCode_name(retval));
    else
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Reset event is triggered");

    // need to action on the EventId i.e. outEventId so as to publish to the MQTT broker
	// TODO:


    return retval;
}

static UA_StatusCode
generateEvent_ShutdownMethodCallback(UA_Server *uaServer,
                         const UA_NodeId *sessionId, void *sessionHandle,
                         const UA_NodeId *methodId, void *methodContext,
                         const UA_NodeId *objectId, void *objectContext,
                         size_t inputSize, const UA_Variant *input,
                         size_t outputSize, UA_Variant *output)
{

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "About to trigger Shutdown event");

    /* set up event */
    UA_NodeId eventNodeId;
    UA_StatusCode retval = setUpShutdownEvent(uaServer, &eventNodeId);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                       "Creating Shutdown event failed. StatusCode %s", UA_StatusCode_name(retval));
        return retval;
    }

    /* trigger event */
    retval = UA_Server_triggerEvent(uaServer, eventNodeId,
                                    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER),
                                    NULL, UA_TRUE);
    if(retval != UA_STATUSCODE_GOOD)
        UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                       "Triggering Shutdown event failed. StatusCode %s", UA_StatusCode_name(retval));
    else
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "Shutdown event is triggered");

    return retval;
}

/*** Now, all that is left to do is to create a method node which uses our callback. We do not
 * require any input and as output we will be using the `EventId` we receive from ``triggerEvent``. The `EventId` is
 * generated by the server internally and is a random unique ID which identifies that specific event.
 *
 * This method node will be added to a basic server setup.
 */
static void
addGenerateEvent_ResetMethod(UA_Server *uaServer, UA_NodeId parent)
{
    UA_StatusCode retval;

    UA_MethodAttributes generateAttr = UA_MethodAttributes_default;
    generateAttr.description = UA_LOCALIZEDTEXT("en-US","Generate an Reset event.");
    generateAttr.displayName = UA_LOCALIZEDTEXT("en-US","Reset Event");
    generateAttr.executable = UA_TRUE;
    generateAttr.userExecutable = UA_TRUE;

// 62572 is used in addClearAlarmLEDMethod() in SV_Method.c; increment by 1
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

    retval = UA_Server_addMethodNode(uaServer, UA_NODEID_NUMERIC(0, 62573),
                            parent, 				//UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(0, "Reset Event"),
                            generateAttr, &generateEvent_ResetMethodCallback,
                            0, NULL, 0, NULL, NULL, NULL);

	if(retval != UA_STATUSCODE_GOOD)
	{
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_Event:addGenerateEventResetMethod() failure : %s", UA_StatusCode_name(retval) );
		//printf("SV_Event: addGenerateEventResetMethod() failure : %d \n", retval);
		//sleep(5);
	}
	else
	{
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_Event:addGenerateEventResetMethod() success : %s", UA_StatusCode_name(retval) );
		//printf("SV_Event: addGenerateEventResetMethod() success : %d \n", retval);
	}
}

static void
addGenerateEvent_ShutdownMethod(UA_Server *uaServer, UA_NodeId parent)
{
    UA_StatusCode retval;

    UA_MethodAttributes generateAttr = UA_MethodAttributes_default;
    generateAttr.description = UA_LOCALIZEDTEXT("en-US","Generate an Shutdown event.");
    generateAttr.displayName = UA_LOCALIZEDTEXT("en-US","Shutdown Event");
    generateAttr.executable = UA_TRUE;
    generateAttr.userExecutable = UA_TRUE;

// 62572 is used in addClearAlarmLEDMethod() in SV_Method.c; increment by 1
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

    retval = UA_Server_addMethodNode(uaServer, UA_NODEID_NUMERIC(0, 62574),
                            parent, 				//UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
                            UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                            UA_QUALIFIEDNAME(0, "Shutdown Event"),
                            generateAttr, &generateEvent_ShutdownMethodCallback,
                            0, NULL, 0, NULL, NULL, NULL);
	if(retval != UA_STATUSCODE_GOOD)
	{
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_Event: addGenerateEventShutdownMethod() failure : %s", UA_StatusCode_name(retval) );
        	//printf("SV_Event: addGenerateEventShutdownMethod() failure : %d \n", retval);
		//sleep(5);
	}
	else
	{
		UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_Event: addGenerateEventShutdownMethod() success : %s", UA_StatusCode_name(retval) );
		//printf("SV_Event: addGenerateEventShutdownMethod() success : %d \n", retval);
	}
}

static UA_StatusCode
addNewEventType(UA_Server *uaServer)
{
    UA_ObjectTypeAttributes attr = UA_ObjectTypeAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en-US", "SimpleEventType");
    attr.description = UA_LOCALIZEDTEXT("en-US", "The simple event type we created");

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_Event.c : addNewEventType() ...");

    size_t namespaceIndex;
    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_StatusCode status = UA_Server_addObjectTypeNode(uaServer,UA_NODEID_NULL,
                                       UA_NODEID_NUMERIC(0, UA_NS0ID_BASEEVENTTYPE),
                                       UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
                                       UA_QUALIFIEDNAME(0, "SimpleEventType"),
                                       attr, NULL, &eventType);

    //printf("addNewEventType : status = %u \n", status);
    return status;
}

void CreateServerEvents(UA_Server *uaServer, UA_NodeId parent)
{
	UA_StatusCode retVal;

	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_Event.c : CreateServerEvents()");
	retVal = addNewEventType(uaServer);
	UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_Event.c : addNewEventType() returns %s", UA_StatusCode_name(retVal));

	// these functions should be called by Methods
	addGenerateEvent_ResetMethod(uaServer, parent);	//==> this event will be mapped to the Reset Method in OPCUA Server
	addGenerateEvent_ShutdownMethod(uaServer, parent);
}
