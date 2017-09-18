#ifndef PERSONAL_SITE_AUTH
#define PERSONAL_SITE_AUTH

#include <fstream>
#include <string>
#include <vector>

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

namespace Sigsegv {
    namespace Personalsite {
        namespace Auth {
            /*
             * This class is used to interact with the datastore (ddb in our case)
             */
            class UserItem {
                private:
                std::string uid;
                std::string name;
                std::string pwd;
                long int date;

                public:
                UserItem () : uid{ "" }, name{ "" }, pwd{ "" }, date{ -1 } {
                }
                UserItem (const UserItem& other) : uid{ other.uid }, name{ other.name }, pwd{ other.pwd }, date{ other.date } {
                }
                void operator= (const UserItem& other) {
                    uid = other.uid;
                    name = other.name;
                    pwd = other.pwd;
                    date = other.date;
                }
                virtual ~UserItem () {
                }

                // Setters
                void setUid (const std::string&);
                void setName (const std::string&);
                void setPwd (const std::string&);
                void setDate (const long int);

                // Getters
                std::string getUid () const;
                std::string getName () const;
                std::string getPwd () const;
                long int getDate () const;

                // DDB translations
                void fromAttributeMap (const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& attrMap);
                Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> toAttributeMap () const;
                std::string toKeyConditionExpression () const;
                Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> toKeyConditionAttributeValues () const;
            };

            /*
             * This class forms the basis for the overall auth library
             */
            class Service {
                private:
                std::string ddbtable;
                Aws::DynamoDB::DynamoDBClient ddbClient;
                bool whiteListingEnabled;
                std::vector<std::string> whitelistedIds;

                public:
                Service (std::ifstream& configInputStream, std::ifstream& whitelistInputStream);
                virtual ~Service () {
                }

                // Make gapi calls for user info
                static size_t writeCallback (void* contents, size_t size, size_t nmemb, void* userp);
                UserItem getInfoFromOAuth (const std::string& accessToken) const;

                // Table manipulation
                std::vector<UserItem> checkIfUserExists (const UserItem& item) const;
                void writeUserEntry (const UserItem& item) const;

                // Actual auth call fo oaut
                UserItem getUserInfo (const std::string& accessToken) const;
                bool authnticateUser (UserItem user) const;
            };
        }
    }
}
#endif
