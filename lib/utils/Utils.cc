#include "Utils.h"

#include <string>

#include <aws/core/utils/memory/stl/AWSString.h>

#include <aws/core/utils/logging/AWSLogging.h>
#include <aws/core/utils/logging/DefaultLogSystem.h>

void Sigsegv::Personalsite::Utils::initAwsLogging (std::string executable, std::string prefix) {
    Aws::Utils::Logging::InitializeAWSLogging (Aws::MakeShared<Aws::Utils::Logging::DefaultLogSystem> (executable.c_str (), Aws::Utils::Logging::LogLevel::Trace, prefix));
}

void Sigsegv::Personalsite::Utils::shutDownLogs () {
    Aws::Utils::Logging::ShutdownAWSLogging ();
}

Aws::String Sigsegv::Personalsite::Utils::toAwsString (const std::string& input) {
    Aws::String retval (input.c_str ());
    return retval;
}

std::string Sigsegv::Personalsite::Utils::toStdString (const Aws::String& input) {
    std::string retval (input.c_str ());
    return retval;
}

void Sigsegv::Personalsite::Utils::initAws () {
    Aws::SDKOptions options;
    Aws::InitAPI (options);
}

void Sigsegv::Personalsite::Utils::shutDownAws () {
    Aws::SDKOptions options;
    Aws::ShutdownAPI (options);
}
