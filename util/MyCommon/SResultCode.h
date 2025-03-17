#ifndef SRESULTCODE_H
#define  SRESULTCODE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

class SResultCode
{
	struct Code
	{
		int code;
		const char* codeStr;
		const char* message;

		Code(int code, const char* codeStr, const char* message)
			:code(code),codeStr(codeStr),message(message){}
		QByteArray toJson()const
		{
			QJsonObject jobj;
			jobj.insert("code", code);
			jobj.insert("message",message);
			return QJsonDocument(jobj).toJson(QJsonDocument::Compact);
		}
		operator QJsonValue()const
		{
			return code;
		}
	};

public:
#define CODE(name,value,message) inline static Code name{value,#name, message}
	/*  #name:使用预处理器的字符串化操作符 # 将宏参数 name 转换成对应的字符串字面量  */
	CODE(Success, 0, "成功~");
	CODE(SuccessButNotData, 1, "成功~但是好像没有数据");
	//参数错误[1000~1999)
	CODE(ParamJsonInvalid, 1000, "请求参数Json无效");
	CODE(ParamLoss, 1001, "缺少必要参数");
	CODE(ParamParseError, 1002, "参数解析错误");
	//服务器错误[2000~2999)
	CODE(ServerSqlQueryError, 2000, "Sql 执行失败");
	CODE(ServerInnerError, 2001, "服务器内部错误");
	CODE(ServerResourceNotFound, 2002, "资源未找到");
	//用户错误[3000~3999)
	CODE(UserAccountDisable, 3000, "账户已经被禁用，请联系管理员~");
	CODE(UserLoginError, 3001, "账号或密码不正确");
	CODE(UserAccountExists, 3002, "账号已存在，换一个重试");
	CODE(UserUnauthorized, 3003, "未认证的用户，请先登录~");
	CODE(UserAuthFormatError, 3004, "认证格式错误，仅支持Bearer方法认证");
	CODE(UserAuthTokenInvalid, 3005, "token 无效~");
	CODE(UserAuthTokenExpired, 3006, "token 已过期，请重新登录~");

#undef CODE

	friend class SResult;
};

class SResult
{
public:
	static QByteArray success()
	{
		return SResultCode::Success.toJson();
	}

	static QByteArray success(const SResultCode::Code& code)
	{
		return code.toJson();
	}

	static QByteArray success(const QJsonObject& data)
	{
		QJsonObject obj;
		obj.insert("code", SResultCode::Success);
		obj.insert("message", SResultCode::Success.message);
		obj.insert("data", data);
		return QJsonDocument(obj).toJson(QJsonDocument::Compact);
	}

	static QByteArray failure(const SResultCode::Code& code) {
		return code.toJson();
	}

	static QByteArray failure(const SResultCode::Code& code,const QString& message) {
		QJsonObject obj;
		obj.insert("code", SResultCode::Success);
		obj.insert("message", message);
		return QJsonDocument(obj).toJson(QJsonDocument::Compact);
	}


};

#endif // !SRESULTCODE_H
