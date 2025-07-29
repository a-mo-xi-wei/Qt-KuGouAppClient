#include "SJwt.h"

#include <QCryptographicHash>
#include <QJsonDocument>

using namespace SJwt;

SJwtObject::SJwtObject(SAlgorithm alg, const QJsonObject &playload, QAnyStringView secret)
    : m_payloadJson(playload)
  , m_alg(alg)
  , m_secret((const char *) secret.data(), secret.size_bytes()) {
    m_headerJson.insert("alg", alg_to_str(alg));
    m_headerJson.insert("typ", "JWT");
}

SJwtObject &SJwtObject::addClaim(registered_claims claim, const QVariant &value) {
    m_payloadJson.insert(reg_claims_to_str(claim), value.toString());
    return *this;
}

SJwtObject &SJwtObject::addClaim(QAnyStringView claim, const QVariant &value) {
    m_payloadJson.insert(claim.toString(), value.toString());
    return *this;
}

SJwtObject &SJwtObject::removeClaim(registered_claims claim) {
    m_payloadJson.remove(reg_claims_to_str(claim));
    return *this;
}

SJwtObject &SJwtObject::removeClaim(QAnyStringView claim) {
    m_payloadJson.remove(claim.toString());
    return *this;
}

bool SJwtObject::hasClaim(registered_claims claim) {
    return m_payloadJson.contains(reg_claims_to_str(claim));
}

bool SJwtObject::hasClaim(QAnyStringView claim) {
    return m_payloadJson.contains(claim.toString());
}

QJsonObject &SJwtObject::header() { return m_headerJson; }

QJsonObject &SJwtObject::payload() { return m_payloadJson; }

QByteArray SJwtObject::headerBase64() const {
    return QJsonDocument(m_headerJson).toJson(QJsonDocument::JsonFormat::Compact).toBase64();
}

QByteArray SJwtObject::payloadBase64() const {
    return QJsonDocument(m_payloadJson).toJson(QJsonDocument::JsonFormat::Compact).toBase64();
}

QByteArray SJwtObject::signature() const {
    auto srcStr = headerBase64() + "." + payloadBase64() + m_secret;
    switch (m_alg) {
        case SAlgorithm::HS256:
            return QCryptographicHash::hash(srcStr, QCryptographicHash::Sha256).toHex();
        case SAlgorithm::HS384:
            return QCryptographicHash::hash(srcStr, QCryptographicHash::Sha384).toHex();
        case SAlgorithm::HS512:
            return QCryptographicHash::hash(srcStr, QCryptographicHash::Sha512).toHex();
        case SAlgorithm::RS256:
            return QCryptographicHash::hash(srcStr, QCryptographicHash::Sha3_256).toHex();
        case SAlgorithm::RS384:
            return QCryptographicHash::hash(srcStr, QCryptographicHash::Sha3_384).toHex();
        case SAlgorithm::RS512:
            return QCryptographicHash::hash(srcStr, QCryptographicHash::Sha3_512).toHex();
        case SAlgorithm::ES256:
            return QCryptographicHash::hash(srcStr, QCryptographicHash::Keccak_256).toHex();
        case SAlgorithm::ES384:
            return QCryptographicHash::hash(srcStr, QCryptographicHash::Keccak_384).toHex();
        case SAlgorithm::ES512:
            return QCryptographicHash::hash(srcStr, QCryptographicHash::Keccak_512).toHex();
        default: Q_ASSERT_X(0, __FUNCTION__, "SAlgorithm unknown");
    }
    return "algorithm not support!";
}

QByteArray SJwtObject::jwt() {
    return headerBase64() + "." + payloadBase64() + "." + signature();
}

SJwtObject SJwtObject::decode1(const QByteArray &jwt, SAlgorithm alg, QAnyStringView secret) {
    QByteArrayList list = jwt.split('.');
    if (list.size() != 3) {
        qWarning() << "jwt format error" << jwt;
        return SJwtObject();
    }
    //解析header
    auto jheaderObj = QJsonDocument::fromJson(QByteArray::fromBase64(list[0])).object();

    //判断是不是jwt
    if (jheaderObj.value("typ").toString() != "JWT") {
        qWarning() << "jwt type error" << jheaderObj.value("typ").toString() << "need JWT";
        return SJwtObject();
    }

    //获取hash算法
    auto realAlg = str_to_alg(jheaderObj.value("alg").toString());
    if (realAlg == SAlgorithm::UNKN || alg != realAlg) {
        qWarning() << "jwt algorithm error" << jheaderObj.value("alg").toString() << "need " << alg_to_str(alg);
        return SJwtObject();
    }


    //解析payload
    auto jpayloadObj = QJsonDocument::fromJson(QByteArray::fromBase64(list[1])).object();
    SJwtObject sobj(alg, jpayloadObj, secret);

    if (sobj.signature() == list[2])
        return sobj;

    return SJwtObject();
}

SJwtObject SJwtObject::decode(const QByteArray &jwt, SAlgorithm alg, const QByteArray &secret) {
    SJwtObject jwtObject;

    jwtObject.setSecret(secret);

    QByteArrayList list = jwt.split('.');
    if (list.size() != 3) {
        jwtObject.m_errorString = "jwt format error";
        jwtObject.m_status      = Status::JwrFormatError;
        qWarning() << "jwt format error" << jwt;
        return jwtObject;
    }
    //解析header
    QJsonParseError err;
    auto jdoc = QJsonDocument::fromJson(QByteArray::fromBase64(list[0]), &err);
    if (err.error != QJsonParseError::NoError) {
        jwtObject.m_errorString = "header json error," + err.errorString();
        jwtObject.m_status      = Status::Invalid;
        return jwtObject;
    }

    //判断是不是jwt
    auto jheaderObj = jdoc.object();
    if (jheaderObj.value("typ").toString() != "JWT") {
        jwtObject.m_errorString = "jwt type error" + jheaderObj.value("typ").toString() + "need JWT";
        jwtObject.m_status      = Status::Invalid;
        qWarning() << "jwt type error" << jheaderObj.value("typ").toString() << "need JWT";
        return jwtObject;
    }
    jwtObject.setHeader(jheaderObj);

    //获取hash算法
    auto realAlg = str_to_alg(jheaderObj.value("alg").toString());
    if (realAlg == SAlgorithm::UNKN || alg != realAlg) {
        jwtObject.m_errorString = QString("jwt algorithm error,need ") + alg_to_str(alg) + ",now is " +
                                  alg_to_str(realAlg);
        jwtObject.m_status = Status::AlgorithmError;
        qWarning() << "jwt algorithm error" << jheaderObj.value("alg").toString() << "need " << alg_to_str(alg);
        return jwtObject;
    }
    jwtObject.setAlgorithm(realAlg);


    //解析payload
    err.error = QJsonParseError::NoError;
    jdoc      = QJsonDocument::fromJson(QByteArray::fromBase64(list[1]), &err);
    if (err.error != QJsonParseError::NoError) {
        jwtObject.m_errorString = "payload json error," + err.errorString();
        jwtObject.m_status      = Status::Invalid;
        return jwtObject;
    }
    jwtObject.setPayload(jdoc.object());

    //判断是否过期
    if (jwtObject.payload().contains("exp")) {
        auto nowTime = QDateTime::currentDateTime();
        auto expTime = QDateTime::fromString(jwtObject.payload().value("exp").toString(), Qt::DateFormat::ISODate);
        if (expTime < nowTime) {
            jwtObject.m_errorString = "jwt is expiration";
            jwtObject.m_status      = Status::Expired;

            return jwtObject;
        }
    }

    //qDebug() << jheaderObj << jdoc.object();
    //qDebug() << jwtObject.signature();
    //qDebug() << list[2];

    if (jwtObject.signature() != list[2]) {
        jwtObject.m_status = Status::Invalid;
        return jwtObject;
    }

    return jwtObject;
}
