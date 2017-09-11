#include "Auth.h"
#include "Exceptions.h"
#include "Utils.h"

#include <vector>

#include <aws/core/utils/Outcome.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/PutItemRequest.h>

void Sigsegv::Personalsite::Auth::Service::writeUserEntry(const Sigsegv::Personalsite::Auth::UserItem& item) const {
    auto response= ddbClient.PutItem(
                Aws::DynamoDB::Model::PutItemRequest()
                    .WithTableName(Sigsegv::Personalsite::Utils::toAwsString(ddbtable))
                    .WithItem(item.toAttributeMap()));
    if (!response.IsSuccess()) {
        Sigsegv::Personalsite::Exceptions::ServiceException exc;
        exc.setErrorCode(Sigsegv::Personalsite::Exceptions::ExceptionType::DDB_PUT_ITEM_FAIL);
        exc.setMessage(Sigsegv::Personalsite::Utils::toStdString(response.GetError().GetMessage()));
        throw exc;
    }
}
