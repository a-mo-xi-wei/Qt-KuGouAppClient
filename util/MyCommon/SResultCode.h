/*
  SResultCode - 用于定义和处理操作结果码的类
*/

#ifndef SRESULTCODE_H
#define SRESULTCODE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

/** @class SResultCode
 *  @brief 定义操作结果码及其相关信息的类
 *
 *  该类通过内部结构体 Code 定义各种操作结果码及其描述，支持转换为 JSON 格式。
 */
class SResultCode {
    /** @struct Code
     *  @brief 存储单个结果码的信息
     */
    struct Code {
        /** @brief 结果码值
         */
        int code;

        /** @brief 结果码的字符串表示
         */
        const char* codeStr;

        /** @brief 结果码的描述信息
         */
        const char* message;

        /** @brief 构造函数
         *
         *  初始化 Code 结构体。
         *
         *  @param code 结果码值
         *  @param codeStr 结果码的字符串表示
         *  @param message 结果码的描述信息
         */
        Code(int code, const char* codeStr, const char* message)
            : code(code), codeStr(codeStr), message(message) {}

        /** @brief 转换为 JSON 格式
         *
         *  将结果码及其描述信息转换为 JSON 格式的字节数组。
         *
         *  @return JSON 格式的字节数组
         */
        QByteArray toJson() const {
            QJsonObject jobj;
            jobj.insert("code", code);
            jobj.insert("message", message);
            return QJsonDocument(jobj).toJson(QJsonDocument::Compact);
        }

        /** @brief 转换为 QJsonValue
         *
         *  将结果码转换为 QJsonValue 类型。
         *
         *  @return 结果码的 QJsonValue 表示
         */
        operator QJsonValue() const {
            return code;
        }
    };

public:
    /** @def CODE
     *  @brief 定义结果码的宏
     *
     *  用于快速定义静态 Code 成员。
     *
     *  @param name 结果码名称
     *  @param value 结果码值
     *  @param message 结果码描述
     */
#define CODE(name, value, message) inline static Code name{value, #name, message}

    /** @brief 成功
     */
    CODE(Success, 0, "成功~");

    /** @brief 成功但无数据
     */
    CODE(SuccessButNotData, 1, "成功~但是好像没有数据");

    /** @brief 请求参数 JSON 无效
     */
    CODE(ParamJsonInvalid, 1000, "请求参数Json无效");

    /** @brief 缺少必要参数
     */
    CODE(ParamLoss, 1001, "缺少必要参数");

    /** @brief 参数解析错误
     */
    CODE(ParamParseError, 1002, "参数解析错误");

    /** @brief 非法访问路径
     */
    CODE(PathIllegal, 1003, "非法访问路径");

    /** @brief SQL 执行失败
     */
    CODE(ServerSqlQueryError, 2000, "Sql 执行失败");

    /** @brief 服务器内部错误
     */
    CODE(ServerInnerError, 2001, "服务器内部错误");

    /** @brief 资源未找到
     */
    CODE(ServerResourceNotFound, 2002, "资源未找到");

    /** @brief 账户被禁用
     */
    CODE(UserAccountDisable, 3000, "账户已经被禁用，请联系管理员~");

    /** @brief 账号或密码错误
     */
    CODE(UserLoginError, 3001, "账号或密码不正确");

    /** @brief 账号已存在
     */
    CODE(UserAccountExists, 3002, "账号已存在，换一个重试");

    /** @brief 未认证用户
     */
    CODE(UserUnauthorized, 3003, "未认证的用户，请先登录~");

    /** @brief 认证格式错误
     */
    CODE(UserAuthFormatError, 3004, "认证格式错误，仅支持Bearer方法认证");

    /** @brief Token 无效
     */
    CODE(UserAuthTokenInvalid, 3005, "token 无效~");

    /** @brief Token 已过期
     */
    CODE(UserAuthTokenExpired, 3006, "token 已过期，请重新登录~");

#undef CODE

    friend class SResult;
};

/** @class SResult
 *  @brief 提供生成成功或失败结果的工具类
 *
 *  该类提供静态方法，用于生成基于 SResultCode 的 JSON 格式成功或失败结果。
 */
class SResult {
public:
    /** @brief 生成成功结果
     *
     *  生成默认的成功结果（SResultCode::Success）。
     *
     *  @return JSON 格式的成功结果
     */
    static QByteArray success() {
        return SResultCode::Success.toJson();
    }

    /** @brief 生成指定成功结果
     *
     *  使用指定的结果码生成成功结果。
     *
     *  @param code 结果码
     *  @return JSON 格式的成功结果
     */
    static QByteArray success(const SResultCode::Code& code) {
        return code.toJson();
    }

    /** @brief 生成带数据的成功结果
     *
     *  生成包含数据的成功结果（SResultCode::Success）。
     *
     *  @param data 附加的数据
     *  @return JSON 格式的成功结果
     */
    static QByteArray success(const QJsonObject& data) {
        QJsonObject obj;
        obj.insert("code", SResultCode::Success);
        obj.insert("message", SResultCode::Success.message);
        obj.insert("data", data);
        return QJsonDocument(obj).toJson(QJsonDocument::Compact);
    }

    /** @brief 生成失败结果
     *
     *  使用指定的结果码生成失败结果。
     *
     *  @param code 结果码
     *  @return JSON 格式的失败结果
     */
    static QByteArray failure(const SResultCode::Code& code) {
        return code.toJson();
    }

    /** @brief 生成自定义消息的失败结果
     *
     *  使用指定的结果码和自定义消息生成失败结果。
     *
     *  @param code 结果码
     *  @param message 自定义消息
     *  @return JSON 格式的失败结果
     */
    static QByteArray failure(const SResultCode::Code& code, const QString& message) {
        QJsonObject obj;
        obj.insert("code", code.code);
        obj.insert("message", message);
        return QJsonDocument(obj).toJson(QJsonDocument::Compact);
    }
};

#endif // SRESULTCODE_H