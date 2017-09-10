namespace cpp2 personalsite.frontend

struct GetUidFromTokenRequest {
    1: string token;
}

struct GetUidFromTokenResponse {
    1: string uid;
    2: string pwd;
    3: string name;
}

service PersonalSiteFrontend {
    GetUidFromTokenResponse getUidFromToken(1: GetUidFromTokenRequest request);
}
