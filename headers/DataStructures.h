#ifndef SIGSEGV_PERSONALSITE_DS
#define SIGSEGV_PERSONALSITE_DS

#include <map>
#include <string>

#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>

namespace Sigsegv {
    namespace Personalsite {
        class UserItem {
            private:
            std::string uid; // Hash
            std::string name;
            std::map<std::string, bool> rsvp;

            public:
            // Constructors and destructors
            UserItem () : uid{ "" }, name{ "" }, rsvp{ { "Day1", false }, { "Day2", false } } {
            }
            UserItem (const UserItem& other) : uid{ other.uid }, name{ other.name }, rsvp{ other.rsvp } {
            }
            void operator= (const UserItem& other) {
                uid = other.uid;
                name = other.name;
                rsvp = other.rsvp;
            }
            virtual ~UserItem () {
            }

            // Setters and getters
            void setUid (const std::string&);
            void setName (const std::string&);
            void setRsvp (const std::string&, bool);

            std::string getUid () const;
            std::string getName () const;
            bool getRsvp (const std::string&) const;

            // DDB translations
            void fromAttributeMap (const Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue>& attrMap);
            Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> toAttributeMap () const;
            std::string toKeyConditionExpression () const;
            Aws::Map<Aws::String, Aws::DynamoDB::Model::AttributeValue> toKeyConditionAttributeValues () const;
        };
    }
}

#endif
