namespace cpp2 personalsite.frontend

struct GetUidFromTokenRequest {
    1: string token;
}

struct GetUidFromTokenResponse {
    1: i64 uid;
}

service PersonalSiteFrontend {
    GetUidFromTokenResponse getUidFromToken(1: GetUidFromTokenRequest request);
}
