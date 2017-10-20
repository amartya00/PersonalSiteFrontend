namespace cpp2 Sigsegv.Personalsite

enum ErrorCode {
    EXPIRED_TOKEN,
    INCORRECT_TOKEN,
    SERVICE_UNAVAILABLE,
    AUTH_EXCEPTION,
    INVALID_INPUT
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

struct UpdateUserInfoRequest {
    1: string uid;
    2: string pwd;
    3: string name;
    4: bool isRsvpDay1;
    5: bool isRsvpDay2;
}

struct UpdateUserInfoResponse {
}

struct GetUserInfoRequest {
    1: string uid;
    2: string pwd;
}

struct GetUserInfoResponse {
    1: string name;
    2: bool isRsvpDay1;
    3: bool isRsvpDay2;
}

service Frontend {
    GetUidFromTokenResponse getUidFromToken(
        1: GetUidFromTokenRequest request) throws (

        1: AuthException authException,
        2: ServiceException serviceException,
        3: ClientException clientException);

    UpdateUserInfoResponse updateUserInfo(
        1: UpdateUserInfoRequest request) throws (

        1: ClientException clientException,
        2: ServiceException serverException)    ;

    GetUserInfoResponse getUserInfo(
        1: GetUserInfoRequest request) throws (

        1: ClientException clientException,
        2: ServiceException serviceException);
}
