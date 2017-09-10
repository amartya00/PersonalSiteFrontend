#include "Utils.h"

#include <string>

#include <aws/core/utils/memory/stl/AWSString.h>

#include <aws/core/utils/logging/DefaultLogSystem.h>
#include <aws/core/utils/logging/AWSLogging.h>

void personalsite::utils::initAwsLogging(std::string executable, std::string prefix) {
    Aws::Utils::Logging::InitializeAWSLogging(
        Aws::MakeShared<Aws::Utils::Logging::DefaultLogSystem>(executable.c_str(), Aws::Utils::Logging::LogLevel::Trace, prefix));
}

void personalsite::utils::shutDownLogs() {
    Aws::Utils::Logging::ShutdownAWSLogging();
}

Aws::String personalsite::utils::toAwsString(const std::string& input) {
    Aws::String retval(input.c_str());
    return retval;
}

std::string personalsite::utils::toStdString(const Aws::String& input) {
    std::string retval(input.c_str());
    return retval;
}

void personalsite::utils::initAws() {
    Aws::SDKOptions options;
    Aws::InitAPI(options);
}

void personalsite::utils::shutDownAws() {
    Aws::SDKOptions options;
    Aws::ShutdownAPI(options);
}
