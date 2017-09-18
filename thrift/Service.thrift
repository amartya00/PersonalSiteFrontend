namespace cpp2 Sigsegv.Personalsite

enum ErrorCode {
    EXPIRED_TOKEN,
    INCORRECT_TOKEN,
    SERVICE_UNAVAILABLE
}

exception AuthException {
    1: ErrorCode errorCode;
    2: string errorMessage;
}

exception ServiceException {
    1: ErrorCode errorCode;
    2: string errorMessage;
}

exception ClientException {
    1: ErrorCode errorCode;
    2: string errorMessage;
}


struct GetUidFromTokenRequest {
    1: string token;
}

struct GetUidFromTokenResponse {
    1: string uid;
    2: string pwd;
    3: string name;
}

service Frontend {
    GetUidFromTokenResponse getUidFromToken(
        1: GetUidFromTokenRequest request) throws (

        1: AuthException authException,
        2: ServiceException serviceException,
        3: ClientException clientException);
}
