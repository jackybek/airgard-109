#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
//#include "open62541.h"

#define UA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS

// function prototypes declaration
UA_StatusCode addConditionSourceObject(UA_Server *);
UA_StatusCode addCondition_1(UA_Server *);
UA_StatusCode addCondition_2(UA_Server *);
void addVariable_1_triggerAlarmOfCondition_1(UA_Server *, UA_NodeId *);
void addVariable_2_changeSeverityOfCondition_2(UA_Server *, UA_NodeId *);
void addVariable_3_returnCondition_1_toNormalState(UA_Server *, UA_NodeId*);
void afterWriteCallbackVariable_1(UA_Server *, const UA_NodeId *,
                             void *, const UA_NodeId *,
                             void *, const UA_NumericRange *,
                             const UA_DataValue *);
void afterWriteCallbackVariable_2(UA_Server *, const UA_NodeId *,
                             void *, const UA_NodeId *,
                             void *, const UA_NumericRange *,
                             const UA_DataValue *);
void afterWriteCallbackVariable_3(UA_Server *, const UA_NodeId *,
                             void *, const UA_NodeId *,
                             void *, const UA_NumericRange *,
                             const UA_DataValue *);
UA_StatusCode enteringEnabledStateCallback(UA_Server *, const UA_NodeId *);
UA_StatusCode enteringAckedStateCallback(UA_Server *, const UA_NodeId *);
UA_StatusCode enteringConfirmedStateCallback(UA_Server *, const UA_NodeId *);
UA_StatusCode setUpEvironment(UA_Server *);
void CreateServerAlarmsAndConditions(UA_Server *);

UA_NodeId conditionSource;
UA_NodeId conditionInstance_1;
UA_NodeId conditionInstance_2;
UA_NodeId conditionInstance_3;

UA_StatusCode addConditionSourceObject(UA_Server *uaServer)
{
	UA_ObjectAttributes object_attr = UA_ObjectAttributes_default;
	object_attr.eventNotifier = 1;

    	size_t namespaceIndex;
    	UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

	object_attr.displayName = UA_LOCALIZEDTEXT("en", "ConditionSourceObject");
	UA_StatusCode retval = UA_Server_addObjectNode(uaServer, UA_NODEID_NULL,
						UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
						UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
						UA_QUALIFIEDNAME(0, "ConditionSourceObject"),
						UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
						object_attr, NULL, &conditionSource);

	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
				"SV_Alarms.c : Creating Condition Source failed, statuscode %s",
				UA_StatusCode_name(retval));
		sleep(5);
	}

	/* ConditionSource should be EventNotifier of another Object (usually the
	 * Server Object).  If this Reference is not created by user then the A&C
	 * Server will create "HasEventSource" reference to the Server Object
	 * automatically when the condition is created
	*/

	//conditionSource.namespaceIndex = namespaceIndex;
	retval = UA_Server_addReference(uaServer, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER),
						UA_NODEID_NUMERIC(0, UA_NS0ID_HASNOTIFIER),
						UA_EXPANDEDNODEID_NUMERIC(conditionSource.namespaceIndex,
									conditionSource.identifier.numeric),
						UA_TRUE);
	return retval;

}

UA_StatusCode addCondition_1(UA_Server *uaServer)
{
/**
 * Create a condition instance from OffNormalAlarmType. The condition source is
 * the Object created in addConditionSourceObject(). The condition will be
 * exposed in Address Space through the HasComponent reference to the condition
 * source. */

    UA_StatusCode retval = addConditionSourceObject(uaServer);
    if(retval != UA_STATUSCODE_GOOD) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                     "creating Condition Source failed. StatusCode %s",
                     UA_StatusCode_name(retval));

	sleep(5);
    }

    size_t namespaceIndex;
    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

    //conditionInstance_1.namespaceIndex = namespaceIndex;
    retval = UA_Server_createCondition(uaServer,
                                       UA_NODEID_NULL,
                                       UA_NODEID_NUMERIC(0, UA_NS0ID_OFFNORMALALARMTYPE),
                                       UA_QUALIFIEDNAME(0, "Condition 1"),
				       conditionSource,
                                       UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
                                       &conditionInstance_1);

    return retval;
}

UA_StatusCode addCondition_2(UA_Server *uaServer)
{
/**
 * Create a condition instance from OffNormalAlarmType. The condition source is
 * the server Object. The condition won't be exposed in Address Space. */
    size_t namespaceIndex;
    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

    conditionInstance_2.namespaceIndex = namespaceIndex;
    UA_StatusCode retval =
        UA_Server_createCondition(uaServer, UA_NODEID_NULL,
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_OFFNORMALALARMTYPE),
                                  UA_QUALIFIEDNAME(0, "Condition 2"),
                                  UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER),
                                  UA_NODEID_NULL,
				  &conditionInstance_2);

    return retval;
}

void addVariable_1_triggerAlarmOfCondition_1(UA_Server *uaServer, UA_NodeId *outNodeId)
{
   size_t namespaceIndex;

    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en", "Activate Condition 1");
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    UA_Boolean tboolValue = UA_FALSE;
    UA_Variant_setScalar(&attr.value, &tboolValue, &UA_TYPES[UA_TYPES_BOOLEAN]);

    UA_QualifiedName CallbackTestVariableName = UA_QUALIFIEDNAME(namespaceIndex, "Activate Condition 1");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);
    //UA_Server_addVariableNode(uaServer, UA_NODEID_NULL, parentNodeId,
    UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 70001),
			      parentNodeId,
                              parentReferenceNodeId, CallbackTestVariableName,
                              variableTypeNodeId, attr, NULL, outNodeId);

}

void addVariable_2_changeSeverityOfCondition_2(UA_Server *uaServer, UA_NodeId *outNodeId)
{
   size_t namespaceIndex;

    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en", "Change Severity Condition 2");
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    UA_UInt16 severityValue = 0;
    UA_Variant_setScalar(&attr.value, &severityValue, &UA_TYPES[UA_TYPES_UINT16]);

    UA_QualifiedName CallbackTestVariableName = UA_QUALIFIEDNAME(namespaceIndex, "Change Severity Condition 2");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);
    //UA_Server_addVariableNode(uaServer, UA_NODEID_NULL, parentNodeId,
    UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 70002),
			      parentNodeId,
                              parentReferenceNodeId, CallbackTestVariableName,
                              variableTypeNodeId, attr, NULL, outNodeId);
}

void addVariable_3_returnCondition_1_toNormalState(UA_Server *uaServer, UA_NodeId *outNodeId)
{
   size_t namespaceIndex;

    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_VariableAttributes attr = UA_VariableAttributes_default;
    attr.displayName = UA_LOCALIZEDTEXT("en", "Return to Normal Condition 1");
    attr.accessLevel = UA_ACCESSLEVELMASK_READ | UA_ACCESSLEVELMASK_WRITE;
    UA_Boolean rtn = 0;
    UA_Variant_setScalar(&attr.value, &rtn, &UA_TYPES[UA_TYPES_BOOLEAN]);

    UA_QualifiedName CallbackTestVariableName = UA_QUALIFIEDNAME(namespaceIndex, "Return to Normal Condition 1");
    UA_NodeId parentNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER);
    UA_NodeId parentReferenceNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES);
    UA_NodeId variableTypeNodeId = UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE);
    //UA_Server_addVariableNode(uaServer, UA_NODEID_NULL, parentNodeId,
    UA_Server_addVariableNode(uaServer, UA_NODEID_NUMERIC(namespaceIndex, 70003),
			      parentNodeId,
                              parentReferenceNodeId, CallbackTestVariableName,
                              variableTypeNodeId, attr, NULL, outNodeId);
}

void afterWriteCallbackVariable_1(UA_Server *uaServer,
                                const UA_NodeId *sessionId, void *sessionContext,
                                const UA_NodeId *nodeId, void *nodeContext,
                                const UA_NumericRange *range, const UA_DataValue *data)
{
   size_t namespaceIndex;

    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

    UA_QualifiedName activeStateField = UA_QUALIFIEDNAME(namespaceIndex, "ActiveState");
    UA_QualifiedName activeStateIdField = UA_QUALIFIEDNAME(namespaceIndex, "Id");
    UA_Variant value;

    UA_StatusCode retval =
        UA_Server_writeObjectProperty_scalar(uaServer, conditionInstance_1,
                                             UA_QUALIFIEDNAME(namespaceIndex, "Time"),
                                             &data->sourceTimestamp,
                                             &UA_TYPES[UA_TYPES_DATETIME]);

    if(*(UA_Boolean *)(data->value.data) == UA_TRUE) {
        /* By writing "UA_TRUE" in ActiveState/Id, the A&C server will set the
         * related fields automatically and then will trigger event
         * notification. */
        UA_Boolean activeStateId = UA_TRUE;
        UA_Variant_setScalar(&value, &activeStateId, &UA_TYPES[UA_TYPES_BOOLEAN]);
        retval |= UA_Server_setConditionVariableFieldProperty(uaServer, conditionInstance_1,
                                                              &value, activeStateField,
                                                              activeStateIdField);
        if(retval != UA_STATUSCODE_GOOD) {
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                         "Setting ActiveState/Id Field failed. StatusCode %s",
                         UA_StatusCode_name(retval));
	    sleep(5);
            return;
        }
    } else {
        /* By writing "UA_FALSE" in ActiveState/Id, the A&C server will set only
         * the ActiveState field automatically to the value "Inactive". The user
         * should trigger the event manually by calling
         * UA_Server_triggerConditionEvent inside the application or call
         * ConditionRefresh method with client to update the event notification. */
        UA_Boolean activeStateId = UA_FALSE;
        UA_Variant_setScalar(&value, &activeStateId, &UA_TYPES[UA_TYPES_BOOLEAN]);
        retval = UA_Server_setConditionVariableFieldProperty(uaServer, conditionInstance_1,
                                                             &value, activeStateField,
                                                             activeStateIdField);
        if(retval != UA_STATUSCODE_GOOD) {
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                         "Setting ActiveState/Id Field failed. StatusCode %s",
                         UA_StatusCode_name(retval));
	    sleep(5);
            return;
        }

        retval = UA_Server_triggerConditionEvent(uaServer, conditionInstance_1,
                                                 conditionSource, NULL);
        if(retval != UA_STATUSCODE_GOOD) {
            UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                           "Triggering condition event failed. StatusCode %s",
                           UA_StatusCode_name(retval));
	    sleep(5);
            return;
        }
    }
}



void afterWriteCallbackVariable_2(UA_Server *uaServer,
				const UA_NodeId *sessionId, void *sessionContext,
                                const UA_NodeId *nodeId, void *nodeContext,
                                const UA_NumericRange *range, const UA_DataValue *data)
{
/*
 * The callback only changes the severity field of condition 2.  The severity
 * field is of <ConditionVariableType>, so changes in it triggers an event
 * notification automatically by the server
*/

    size_t namespaceIndex;
    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

	// Another way to set fields of conditions
	UA_Server_writeObjectProperty_scalar(uaServer, conditionInstance_2,
					UA_QUALIFIEDNAME(namespaceIndex, "Severity"),
					(UA_UInt16 *)data->value.data,
					&UA_TYPES[UA_TYPES_UINT16]);

}


void afterWriteCallbackVariable_3(UA_Server *uaServer,
				const UA_NodeId *sessionId, void *sessionContext,
				const UA_NodeId *nodeId, void *nodeContext,
				const UA_NumericRange *range, const UA_DataValue *data)
{
/*
 * RTN = return to normal
 *
 * <Retain> will be set to UA_FALSE, thus no events will be generated for condition 1
 * (although EnabledState!=UA_TRUE).  To set <Retain> to UA_TRUE again, the <disable> and
 * <enable> methods should be called respectively
*/
   size_t namespaceIndex;

    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

	UA_StatusCode retval;
	UA_Variant value;
	UA_Boolean idValue = UA_FALSE;
	UA_Int16 severityValue = 100;
	UA_LocalizedText messageValue = UA_LOCALIZEDTEXT("en", "Condition returned to normal state");
	UA_LocalizedText commentValue = UA_LOCALIZEDTEXT("en", "Normal State");
	UA_Boolean retainValue = UA_FALSE;


	// UA_QualifiedName enabledStateField = UA_QUALIFIEDNAME(namespaceIndex, "EnabledState");
	UA_QualifiedName idField = UA_QUALIFIEDNAME(namespaceIndex, "Id");
	UA_QualifiedName activeStateField = UA_QUALIFIEDNAME(namespaceIndex, "ActiveState");
	UA_QualifiedName confirmedStateField = UA_QUALIFIEDNAME(namespaceIndex, "ConfirmedState");
	UA_QualifiedName ackedStateField = UA_QUALIFIEDNAME(namespaceIndex, "AckedState");
	UA_QualifiedName severityField = UA_QUALIFIEDNAME(namespaceIndex, "Severity");
	UA_QualifiedName messageField = UA_QUALIFIEDNAME(namespaceIndex, "Message");
	UA_QualifiedName commentField = UA_QUALIFIEDNAME(namespaceIndex, "Comment");
	UA_QualifiedName retainField = UA_QUALIFIEDNAME(namespaceIndex, "Retain");

	UA_Server_writeObjectProperty_scalar(uaServer, conditionInstance_1,
							UA_QUALIFIEDNAME(namespaceIndex, "Time"),
							&data->serverTimestamp,
							&UA_TYPES[UA_TYPES_DATETIME]);

	//
	UA_Variant_setScalar(&value, &idValue, &UA_TYPES[UA_TYPES_BOOLEAN]);
	retval |= UA_Server_setConditionVariableFieldProperty(uaServer, conditionInstance_1,
							&value, activeStateField,
							idField);
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
					"SV_Alarm.c: Setting <ActiveState/Id> field failed. StatusCode %s",
					UA_StatusCode_name(retval));
		sleep(5);
		return;
	}

	//
	retval = UA_Server_setConditionVariableFieldProperty(uaServer, conditionInstance_1,
							&value, confirmedStateField,
							idField);
	if (retval != UA_STATUSCODE_GOOD)
        {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                                        "SV_Alarm.c: Setting <ConfirmedState/Id> field failed. StatusCode %s",
                                        UA_StatusCode_name(retval));
		sleep(5);
                return;
        }

	//
	retval = UA_Server_setConditionVariableFieldProperty(uaServer, conditionInstance_1,
							&value, ackedStateField,
							idField);
	if (retval != UA_STATUSCODE_GOOD)
        {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                                        "SV_Alarm.c: Setting <AckedState/Id> field failed. StatusCode %s",
                                        UA_StatusCode_name(retval));
		sleep(5);
                return;
        }

	//
	UA_Variant_setScalar(&value, &severityValue, &UA_TYPES[UA_TYPES_UINT16]);
	retval = UA_Server_setConditionField(uaServer, conditionInstance_1,
						&value,severityField);
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
					"SV_Alarm.c: Setting <Severity> field failed. StatusCode %s",
					UA_StatusCode_name(retval));
		sleep(5);
		return;
	}

	//
	UA_Variant_setScalar(&value, &messageValue, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
	retval = UA_Server_setConditionField(uaServer, conditionInstance_1,
						&value, messageField);
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
					"SV_Alarm.c: Setting <Message> field failed, StatusCode %s",
					UA_StatusCode_name(retval));
		sleep(5);
		return;
	}

	//
	UA_Variant_setScalar(&value, &commentValue, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
        retval = UA_Server_setConditionField(uaServer, conditionInstance_1,
                                                &value, commentField);
        if (retval != UA_STATUSCODE_GOOD)
        {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                                        "SV_Alarm.c: Setting <Comment> field failed, StatusCode %s",
                                        UA_StatusCode_name(retval));
		sleep(5);
                return;
        }

	//
	UA_Variant_setScalar(&value, &retainValue, &UA_TYPES[UA_TYPES_BOOLEAN]);
	retval = UA_Server_setConditionField(uaServer, conditionInstance_1,
						&value, retainField);
        if (retval != UA_STATUSCODE_GOOD)
        {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                                        "SV_Alarm.c: Setting <Retain> field failed, StatusCode %s",
                                        UA_StatusCode_name(retval));
		sleep(5);
                return;
        }


	// finally, trigger the conditionEvent
	retval = UA_Server_triggerConditionEvent(uaServer, conditionInstance_1,
					conditionSource, NULL);
        if (retval != UA_STATUSCODE_GOOD)
        {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                                        "SV_Alarm.c: Triggering <Condition event> failed, StatusCode %s",
                                        UA_StatusCode_name(retval));
		sleep(5);
                return;
        }
}

UA_StatusCode enteringEnabledStateCallback(UA_Server *uaServer, const UA_NodeId *condition)
{
   size_t namespaceIndex;

    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

	UA_Boolean retain = UA_TRUE;
	return UA_Server_writeObjectProperty_scalar(uaServer, *condition,
					UA_QUALIFIEDNAME(namespaceIndex, "Retain"),
					&retain,
					&UA_TYPES[UA_TYPES_BOOLEAN]);
}

UA_StatusCode enteringAckedStateCallback(UA_Server *uaServer, const UA_NodeId *condition)
{
/* This is user specific functino which will be called upon acknowledging an alarm condition.
 * In this example we will set the Alarm to <Inactive> state.
 * The server is responsible of seeting standard fields related to Acknowledge Method
 * and triggering the alarm notification
*/
   size_t namespaceIndex;

    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

	// deactivate Alarm when acknowledging
	UA_Boolean activeStateId = UA_FALSE;
	UA_Variant value;
	UA_QualifiedName activeStateField = UA_QUALIFIEDNAME(namespaceIndex, "ActiveState");
	UA_QualifiedName activeStateIdField = UA_QUALIFIEDNAME(namespaceIndex, "Id");

	UA_Variant_setScalar(&value, &activeStateId, &UA_TYPES[UA_TYPES_BOOLEAN]);
	UA_StatusCode retval = UA_Server_setConditionVariableFieldProperty(uaServer, *condition,
							&value, activeStateField,
							activeStateIdField);
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
				"SV_Alarm.c : Setting <ActiveState/ Id> Field failed. StatusCode %s",
				UA_StatusCode_name(retval));
		sleep(5);
	}
	return retval;
}

UA_StatusCode enteringConfirmedStateCallback(UA_Server *uaServer, const UA_NodeId *condition)
{
/* Deactivate Alarm and put it out of the interesting state (by writing
 * UA_FALSE to <Retain> field) when confirming
*/
   size_t namespaceIndex;

    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

	UA_Boolean activeStateId = UA_FALSE;
	UA_Boolean retain = UA_FALSE;
	UA_Variant value;
	UA_QualifiedName activeStateField = UA_QUALIFIEDNAME(namespaceIndex, "ActiveState");
        UA_QualifiedName activeStateIdField = UA_QUALIFIEDNAME(namespaceIndex, "Id");
	UA_QualifiedName retainField = UA_QUALIFIEDNAME(namespaceIndex, "Retain");

	UA_Variant_setScalar(&value, &activeStateId, &UA_TYPES[UA_TYPES_BOOLEAN]);
	UA_StatusCode retval = UA_Server_setConditionVariableFieldProperty(uaServer, *condition,
							&value, activeStateField,
							activeStateIdField);
        if (retval != UA_STATUSCODE_GOOD)
        {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                                "SV_Alarm.c : Setting <ActiveState/ Id> Field failed. StatusCode %s",
                                UA_StatusCode_name(retval));
		sleep(5);
        }

	UA_Variant_setScalar(&value, &retain, &UA_TYPES[UA_TYPES_BOOLEAN]);
	retval = UA_Server_setConditionField(uaServer, *condition,
					&value, retainField);
        if (retval != UA_STATUSCODE_GOOD)
        {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                                "SV_Alarm.c : Setting <retain> Field failed. StatusCode %s",
                                UA_StatusCode_name(retval));
		sleep(5);
        }
	return retval;

}

UA_StatusCode setUpEnvironment(UA_Server *uaServer)
{
	UA_NodeId variable_1, variable_2, variable_3;
	UA_ValueCallback callback;
	callback.onRead = NULL;
	UA_TwoStateVariableChangeCallback userSpecificCallback;

   size_t namespaceIndex;

    UA_Server_getNamespaceByName(uaServer, UA_STRING("virtualskies.com.sg/MKS/"), &namespaceIndex);

	//====================
	/* Exposed condition 1.  We will add to it user-specific callbacks
	 * 1. when entering <enabled> state
	 * 2. when acknowledging
	 * 3. when confirming
	*/
	UA_StatusCode retval = addCondition_1(uaServer);
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
				"SV_Alarm.c: adding <condition 1> failed. StatusCode %s",
				UA_StatusCode_name(retval));
		sleep(5);
		return retval;
	}

	userSpecificCallback = enteringEnabledStateCallback;
	retval = UA_Server_setConditionTwoStateVariableCallback(uaServer, conditionInstance_1,
						conditionSource, UA_FALSE,
						userSpecificCallback,
						UA_ENTERING_ENABLEDSTATE);
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
				"SV_Alarm.c: setting <ConditionTwoState: Callback UA_ENTERING_ENABLEDSTATE> failed.  StatusCode %s",
				UA_StatusCode_name(retval));
		sleep(5);
		return retval;
	}

	userSpecificCallback = enteringAckedStateCallback;
	retval = UA_Server_setConditionTwoStateVariableCallback(uaServer, conditionInstance_1,
                                                conditionSource, UA_FALSE,
                                                userSpecificCallback,
                                                UA_ENTERING_ACKEDSTATE);
        if (retval != UA_STATUSCODE_GOOD)
        {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                                "SV_Alarm.c: setting <ConditionTwoState: Callback UA_ENTERING_ACKSTATE> failed.  StatusCode %s",
                                UA_StatusCode_name(retval));
		sleep(5);
                return retval;
        }

        userSpecificCallback = enteringConfirmedStateCallback;
        retval = UA_Server_setConditionTwoStateVariableCallback(uaServer, conditionInstance_1,
                                                conditionSource, UA_FALSE,
                                                userSpecificCallback,
                                                UA_ENTERING_CONFIRMEDSTATE);
        if (retval != UA_STATUSCODE_GOOD)
        {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                                "SV_Alarm.c: setting <ConditionTwoState: Callback UA_ENTERING_CONFIRMEDSTATE> failed.  StatusCode %s",
                                UA_StatusCode_name(retval));
		sleep(5);
                return retval;
        }
	// Exposed condition 1 end
	//========================

  	//====================
	/* Unexposed condition 2. No user specific callbacks, so the server will
	 * behave in a standard manner upon
	 * 1. entering <enabled> state
	 * 2. acknowledging
	 * 3. confirming
	 * We will set <Retain> field to UA_TRUE and enable the condition
	 * so we can receive event notifications (we cannot call enable method on
	 * unexposed condition using a client like UaExpert or Softing)
	*/
	retval = addCondition_2(uaServer);
	if (retval != UA_STATUSCODE_GOOD)
	{
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                                "SV_Alarm.c: adding <condition 2> failed.  StatusCode %s",
                                UA_StatusCode_name(retval));
		sleep(5);
                return retval;
        }
	UA_Boolean retain = UA_TRUE;
	UA_Server_writeObjectProperty_scalar(uaServer, conditionInstance_2,
					UA_QUALIFIEDNAME(0, "Retain"),
					&retain, &UA_TYPES[UA_TYPES_BOOLEAN]);

	UA_Variant value;
	UA_Boolean enabledStateId = UA_TRUE;
	UA_QualifiedName enabledStateField = UA_QUALIFIEDNAME(0, "EnabledState");
	UA_QualifiedName enabledStateIdField = UA_QUALIFIEDNAME(0, "Id");
	UA_Variant_setScalar(&value, &enabledStateId, &UA_TYPES[UA_TYPES_BOOLEAN]);
	retval = UA_Server_setConditionVariableFieldProperty(uaServer, conditionInstance_2,
					&value, enabledStateField,
					enabledStateIdField);
	if (retval != UA_STATUSCODE_GOOD)
	{
		// failed here during runtime : BadNoMatch if namespaceIndex != 0
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                                "SV_Alarm.c: setting <EnabledState/ Id> field failed.  StatusCode %s",
                                UA_StatusCode_name(retval));
		sleep(5);
                return retval;
        }
	// Unexposed condition 2 end
	// =======================


	// Add 3 variables to trigger condition events
	addVariable_1_triggerAlarmOfCondition_1(uaServer, &variable_1);
	callback.onWrite = afterWriteCallbackVariable_1;
	retval = UA_Server_setVariableNode_valueCallback(uaServer, variable_1, callback);
	if (retval != UA_STATUSCODE_GOOD)
	{
		UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
				"SV_Alarm.c: Setting <variable 1> callback failed.  StatusCode %s",
				UA_StatusCode_name(retval));
		sleep(5);
		return retval;
	}

	/* Severity can change internally also when the condition is disabled and
	 * retain is UA_FALSE. However, in this case no events will be generated
	*/
	addVariable_2_changeSeverityOfCondition_2(uaServer, &variable_2);
	callback.onWrite = afterWriteCallbackVariable_2;
	retval = UA_Server_setVariableNode_valueCallback(uaServer, variable_2, callback);
	if (retval != UA_STATUSCODE_GOOD)
        {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                                "SV_Alarm.c: Setting <variable 2> callback failed.  StatusCode %s",
                                UA_StatusCode_name(retval));
		sleep(5);
                return retval;
        }

	addVariable_3_returnCondition_1_toNormalState(uaServer, &variable_3);
	callback.onWrite = afterWriteCallbackVariable_3;
        retval = UA_Server_setVariableNode_valueCallback(uaServer, variable_3, callback);
	if (retval != UA_STATUSCODE_GOOD)
        {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,
                                "SV_Alarm.c: Setting <variable 3> callback failed.  StatusCode %s",
                                UA_StatusCode_name(retval));
		sleep(5);
                return retval;
        }

	return retval;
}

void CreateServerAlarmsAndConditions(UA_Server *uaServer)
{
	UA_StatusCode retval = setUpEnvironment(uaServer);

}
