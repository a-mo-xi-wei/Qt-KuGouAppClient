#pragma once

#include<QMap>
#include<QJsonObject>

#if defined(COMMON_LIBRARY)
#define COMMON_EXPORT Q_DECL_EXPORT
#else
#define COMMON_EXPORT Q_DECL_IMPORT
#endif

using ParamInitList = QMap<QString, QVariant>;

namespace SJwt {
/**
 * JWT signing algorithm types.
 */
enum class SAlgorithm
{
    NONE = 0,
    HS256,
    HS384,
    HS512,
    RS256,
    RS384,
    RS512,
    ES256,
    ES384,
    ES512,
    UNKN,
    TERM,
};

static inline const char *alg_to_str(SAlgorithm alg) noexcept
{
    switch (alg) {
    case SAlgorithm::HS256: return "HS256";
    case SAlgorithm::HS384: return "HS384";
    case SAlgorithm::HS512: return "HS512";
    case SAlgorithm::RS256: return "RS256";
    case SAlgorithm::RS384: return "RS384";
    case SAlgorithm::RS512: return "RS512";
    case SAlgorithm::ES256: return "ES256";
    case SAlgorithm::ES384: return "ES384";
    case SAlgorithm::ES512: return "ES512";
    case SAlgorithm::TERM: return "TERM";
    case SAlgorithm::NONE: return "NONE";
    case SAlgorithm::UNKN: return "UNKN";
    default: Q_ASSERT_X(0, __FUNCTION__, "Unknown Algorithm");
    };
    return "UNKN";
}

/**
 * Convert stringified algorithm to enum class.
 * The string comparison is case insesitive.
 */
static inline SAlgorithm str_to_alg(const QAnyStringView alg) noexcept
{
    if (!alg.length())
        return SAlgorithm::UNKN;

    if (!QAnyStringView::compare(alg, "NONE"))
        return SAlgorithm::NONE;
    if (!QAnyStringView::compare(alg, "HS256"))
        return SAlgorithm::HS256;
    if (!QAnyStringView::compare(alg, "HS384"))
        return SAlgorithm::HS384;
    if (!QAnyStringView::compare(alg, "HS512"))
        return SAlgorithm::HS512;
    if (!QAnyStringView::compare(alg, "RS256"))
        return SAlgorithm::RS256;
    if (!QAnyStringView::compare(alg, "RS384"))
        return SAlgorithm::RS384;
    if (!QAnyStringView::compare(alg, "RS512"))
        return SAlgorithm::RS512;
    if (!QAnyStringView::compare(alg, "ES256"))
        return SAlgorithm::ES256;
    if (!QAnyStringView::compare(alg, "ES384"))
        return SAlgorithm::ES384;
    if (!QAnyStringView::compare(alg, "ES512"))
        return SAlgorithm::ES512;

    return SAlgorithm::UNKN;
}

/**
 * Registered claim names.
 * 已经注册了的字段名
 */
enum registered_claims
{
    expiration = 0,
    /* !>到期时间*/
    not_before,
    /* !>在此之前不可用 时间*/
    issuer,
    /* !>发行人*/
    audience,
    /* !>用户*/
    issued_at,
    /* !>发行人*/
    subject,
    /* !>主题*/
    jti,
    /* !>JWT ID 用于标识该JWT*/
};

/**
 * Converts an instance of type `enum class registered_claims`
 * to its string equivalent representation.
 * 将类型“enum类registered_claims”的实例转换为其等效的字符串表示形式。
 */
static inline const char *reg_claims_to_str(registered_claims claim) noexcept
{
    switch (claim) {
    case registered_claims::expiration: return "exp";
    case registered_claims::not_before: return "nbf";
    case registered_claims::issuer: return "iss";
    case registered_claims::audience: return "aud";
    case registered_claims::issued_at: return "iat";
    case registered_claims::subject: return "sub";
    case registered_claims::jti: return "jti";
    default: Q_ASSERT_X(0, __FUNCTION__, "Not a registered claim");
    };
    return "unkn";
}


class COMMON_EXPORT SJwtObject
{
public:
    enum class Status
    {
        Ok,
        Expired,        /*!>过期了*/
        AlgorithmError, /*!>算法错误*/
        JwrFormatError, /*!>JWT格式错误*/
        Invalid,        /*!>无效*/
    };

public:
    SJwtObject() = default;

    SJwtObject(SAlgorithm alg, const QJsonObject &playload, QAnyStringView secret);

    SJwtObject &addClaim(SJwt::registered_claims claim, const QVariant &value);

    SJwtObject &addClaim(QAnyStringView claim, const QVariant &value);

    SJwtObject &removeClaim(SJwt::registered_claims claim);

    SJwtObject &removeClaim(QAnyStringView claim);

    bool hasClaim(SJwt::registered_claims claim);

    bool hasClaim(QAnyStringView claim);

    Status status() const { return m_status; }
    bool isValid() const { return m_status == Status::Ok; }
    const QString &errorString() const { return m_errorString; }

    const QByteArray &secret() const { return m_secret; }
    void setSecret(const QByteArray &sec) { m_secret = sec; }

    SAlgorithm algorithm() const { return m_alg; }
    void setAlgorithm(SAlgorithm alg) { m_alg = alg; }

    QJsonObject &header();

    void setHeader(const QJsonObject &header) { m_headerJson = header; }

    QJsonObject &payload();

    void setPayload(const QJsonObject &payload) { m_payloadJson = payload; }

    QByteArray signature() const;

    QByteArray headerBase64() const;

    QByteArray payloadBase64() const;

    QByteArray jwt();

    static SJwtObject decode1(const QByteArray &jwt, SAlgorithm alg, QAnyStringView secret);

    static SJwtObject decode(const QByteArray &jwt, SAlgorithm alg, const QByteArray &secret);

private:
    SAlgorithm m_alg{};        //使用的算法
    QByteArray m_secret{};     //用于算法加密的额外字符串(盐)
    QJsonObject m_headerJson;  //header Json
    QJsonObject m_payloadJson; //playload Json
    QString m_errorString{};
    Status m_status{Status::Ok};
};
};
