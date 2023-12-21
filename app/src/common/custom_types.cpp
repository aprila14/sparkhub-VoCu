#include "custom_types.h"

namespace
{
constexpr uint8_t MAX_FIRMWARE_URL_LENGTH = 64;
}

bool TWorkflowData::operator==(const TWorkflowData& rhs) const
{
    if (::strcmp(workflowId, rhs.workflowId) != 0)
        return false;

    if (deviceUpdateAction != rhs.deviceUpdateAction)
        return false;

    return true;
}

bool TWorkflowData::setWorkflowId(const char* newWorkflowId)
{
    if (strlen(newWorkflowId) > MAX_WORKFLOW_ID_LENGTH)
        return false;

    ::bzero(workflowId, sizeof(workflowId));
    ::strncpy(workflowId, newWorkflowId, strlen(newWorkflowId));

    return true;
}

bool TWorkflowData::isSetWorkflowId() const
{
    return (workflowId[0] != 0);
}