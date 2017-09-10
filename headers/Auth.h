#ifndef PERSONAL_SITE_AUTH
#define PERSONAL_SITE_AUTH

#include <fstream>
#include <string>
#include <vector>

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

namespace personalsite {
    /*
     * This class is used to interact with the datastore (ddb in our case)
     */
    class AuthUserItem {
        private:
        std::string uid;
        std::string name;
        std::string pwd;
        long int date;

        public:
        AuthUserItem () : uid{ "" }, name{ "" }, pwd{ "" }, date{ -1 } {
        }
        AuthUserItem (const AuthUserItem& other) : uid{ other.uid }, name{ other.name }, pwd{ other.pwd }, date{ other.date } {
        }
        void operator= (const AuthUserItem& other) {
            uid = other.uid;
            name = other.name;
            pwd = other.pwd;
            date = other.date;
        }
        virtual ~AuthUserItem () {
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
    class Auth {
        private:
        std::string ddbtable;
        Aws::DynamoDB::DynamoDBClient ddbClient;

        public:
        Auth (std::ifstream& configInputStream);
        virtual ~Auth () {
        }

        // Make gapi calls for user info
        static size_t writeCallback (void* contents, size_t size, size_t nmemb, void* userp);
        AuthUserItem getInfoFromOAuth (const std::string& accessToken) const;

        // Table manipulation
        std::vector<AuthUserItem> checkIfUserExists (const AuthUserItem& item) const;
        void writeUserEntry (const AuthUserItem& item) const;

        // Actual auth call fo oaut
        AuthUserItem getUserInfo (const std::string& accessToken) const;
        bool authnticateUser (AuthUserItem user) const;
    };
}
#endif
