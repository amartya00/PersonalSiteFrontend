#include "Auth.h"
#include "Exceptions.h"
#include "Utils.h"

#include <vector>

#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/QueryRequest.h>

std::vector<Sigsegv::Personalsite::Auth::UserItem> Sigsegv::Personalsite::Auth::Service::checkIfUserExists(const Sigsegv::Personalsite::Auth::UserItem& item) const {
    std::vector<Sigsegv::Personalsite::Auth::UserItem> retval;
    auto result = ddbClient.Query(
                Aws::DynamoDB::Model::QueryRequest()
                    .WithTableName(Sigsegv::Personalsite::Utils::toAwsString(ddbtable))
                    .WithKeyConditionExpression(Sigsegv::Personalsite::Utils::toAwsString(item.toKeyConditionExpression()))
                    .WithExpressionAttributeValues(item.toKeyConditionAttributeValues()));
    if (!result.IsSuccess()) {
        Sigsegv::Personalsite::Exceptions::ServiceException exc;
        exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::DDB_QUERY_FAIL);
        exc.setMessage(Sigsegv::Personalsite::Utils::toStdString(result.GetError().GetMessage()));
        throw exc;
    }
    auto items = result.GetResult().GetItems();
    for (auto returnedItem : items) {
        Sigsegv::Personalsite::Auth::UserItem returned;
        returned.fromAttributeMap(returnedItem);
        retval.push_back(returned);
    }
    return retval;
}
